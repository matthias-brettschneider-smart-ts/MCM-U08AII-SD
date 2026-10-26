/*-----------------------------------------------------------------------------+
|                                                                              |
|                         |\    /|   /- - -  |\    /|                          |
|                         | \  / |  |        | \  / |                          |
|                         |  \/  |  |        |  \/  |                          |
|                         |      |   \_ _ _  |      |                          |
|                                                                              |
|                                                                              |
|         T H E   S M A R T   M E A S U R E M E N T   S O L U T I O N          |
|                                                                              |
+------------------------------------------------------------------------------+
|             (c) SMART TESTSOLUTIONS GmbH - all rights reserved               |
+-----------------------------------------------------------------------------*/

/*============================================================================*/
/* I N C L U D E S                                                            */
/*============================================================================*/

#include "middleware/IErrorInternal.h"
#include "middleware/IGeneric.h"
#include "middleware/IOperation.h"
#include "middleware/IWait.h"
#include "os/IBlink.h"
#include "os/ILed.h"

#ifndef BLINK_FLASHLOADER
#error "error compile flags - BLINK_FLASHLOADER undefined"
#elif ((BLINK_FLASHLOADER != false) && (BLINK_FLASHLOADER != true))
#error "error compile flags - BLINK_FLASHLOADER invalid"
#endif

/*============================================================================*/
/* D E F I N E S                                                              */
/*============================================================================*/

typedef enum {
  BLINK_SEQUENCE_CONFIG_MODE,
  BLINK_SEQUENCE_OPERATION_MODE,
  BLINK_SEQUENCE_WARNING,
  BLINK_SEQUENCE_ERROR,
  BLINK_SEQUENCE_FATAL_ERROR
} e_BLINK_SEQUENCE;

typedef struct {
  e_BLINK_SEQUENCE sequence;
  e_LED_COLOR color;
  uint16_t on_time_in_ms;
  uint16_t off_time_in_ms;
  uint16_t pause_time_in_ms;
} t_BLINK_TIMING;

typedef struct {
  bool initialized;
  bool led_active;
  uint32_t ticks_last;
  uint16_t cycle_last;
  uint16_t count_ms;
  bool consecutive_pulse_series;
  uint16_t fatal_error_pulses_done;
  bool fatal_error_break_active;
} t_BLINK;

/*============================================================================*/
/* V A R I A B L E S  /  C O N S T A N T S  -  I N T E R N A L                */
/*============================================================================*/

static t_BLINK blink;

// frequencies have to be multiple of 1 Hz because there is no longer time base than 1 s
static const t_BLINK_TIMING blink_timings[5] = {
#if (BLINK_FLASHLOADER)
    { .sequence = BLINK_SEQUENCE_CONFIG_MODE,    .color = LED_COLOR_GREEN, .on_time_in_ms =  10, .off_time_in_ms =  90, .pause_time_in_ms =    0 }, // 10,00 Hz, 10 % dc
    { .sequence = BLINK_SEQUENCE_OPERATION_MODE, .color = LED_COLOR_GREEN, .on_time_in_ms =  10, .off_time_in_ms =  90, .pause_time_in_ms =    0 }, // 10,00 Hz, 10 % dc
#else
    { .sequence = BLINK_SEQUENCE_CONFIG_MODE,    .color = LED_COLOR_GREEN, .on_time_in_ms =  50, .off_time_in_ms = 950, .pause_time_in_ms =    0 }, //  1,00 Hz,  5 % dc
    { .sequence = BLINK_SEQUENCE_OPERATION_MODE, .color = LED_COLOR_GREEN, .on_time_in_ms = 500, .off_time_in_ms = 500, .pause_time_in_ms =    0 }, //  1,00 Hz, 50 % dc
#endif
    { .sequence = BLINK_SEQUENCE_WARNING,        .color = LED_COLOR_RED,   .on_time_in_ms =  10, .off_time_in_ms = 190, .pause_time_in_ms =    0 }, //  5,00 Hz,  5 % dc
    { .sequence = BLINK_SEQUENCE_ERROR,          .color = LED_COLOR_RED,   .on_time_in_ms = 150, .off_time_in_ms =  50, .pause_time_in_ms =    0 }, //  5,00 Hz, 75 % dc
    { .sequence = BLINK_SEQUENCE_FATAL_ERROR,    .color = LED_COLOR_RED,   .on_time_in_ms = 250, .off_time_in_ms = 250, .pause_time_in_ms = 1750 }  //  2,00 Hz, 50 % dc
};

/*============================================================================*/
/* V A R I A B L E S  / C O N S T A N T S   -  E X P O R T E D                */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   P R O T O T Y P E S                                      */
/*============================================================================*/

/*============================================================================*/
/* F U N C T I O N   D E F I N I T I O N S                                    */
/*============================================================================*/

/*------------------------------------------------------------------------------
 *               F U N C T I O N   I N F O R M A T I O N
 *
 * Update the LED statemachine and activate or deactivate the LED if necessary.
 *
 -----------------------------------------------------------------------------*/
void BlinkUpdate(const uint16_t cycle, const bool fatal_error_active, const uint16_t fatal_error_pulses)
{
  // initialize timer and activate LED at the very first call
  if (!blink.initialized) {
    blink.initialized = true;
    blink.ticks_last = WAIT_GET_TIMER_VALUE_32();
    blink.cycle_last = cycle;
    LED_SET(LED_COLOR_GREEN, LED_TODO_ACTIVATE);
  }

  // count milliseconds since last call
  while ((WAIT_GET_TIMER_VALUE_32() - blink.ticks_last) >= WAIT_GET_TICKS_FOR_MS_32(1)) {
    blink.ticks_last += WAIT_GET_TICKS_FOR_MS_32(1);
    blink.count_ms++;
  }

  // determine blink sequence depending on mode and errors
  const e_ERROR_CATEGORY highest_error_category = ErrorGetHighestCategory();
  const e_BLINK_SEQUENCE blink_sequence =
      fatal_error_active ? BLINK_SEQUENCE_FATAL_ERROR :
          (highest_error_category == ERROR_CATEGORY_ERROR) ? BLINK_SEQUENCE_ERROR :
              (highest_error_category == ERROR_CATEGORY_WARNING) ? BLINK_SEQUENCE_WARNING :
                  OperationIsRunning() ? BLINK_SEQUENCE_OPERATION_MODE : BLINK_SEQUENCE_CONFIG_MODE;

  e_LED_TODO todo = LED_TODO_NOTHING;

  // deactivate LED and do break to ensure optical separation of very first pulse series
  if ((blink_sequence == BLINK_SEQUENCE_FATAL_ERROR) && !blink.consecutive_pulse_series) {
    blink.consecutive_pulse_series = true;
    todo = LED_TODO_DEACTIVATE;
    blink.fatal_error_break_active = true;
  }

  // do break
  if (blink.fatal_error_break_active) {
    if (blink.count_ms >= blink_timings[blink_sequence].pause_time_in_ms) {
      blink.count_ms -= blink_timings[blink_sequence].pause_time_in_ms;
      blink.fatal_error_pulses_done = 0;
      blink.fatal_error_break_active = false;
    }
  }
  else {
    // enable LED after off time
    if (!blink.led_active && (blink.count_ms >= blink_timings[blink_sequence].off_time_in_ms)) {
      blink.count_ms -= blink_timings[blink_sequence].off_time_in_ms;
      todo = LED_TODO_ACTIVATE;
      // count activations in case of fatal errors
      if (fatal_error_active)
        blink.fatal_error_pulses_done++;
    }
    // disable LED after on time
    if (blink.led_active && (blink.count_ms >= blink_timings[blink_sequence].on_time_in_ms)) {
      blink.count_ms -= blink_timings[blink_sequence].on_time_in_ms;
      todo = LED_TODO_DEACTIVATE;
      // force break after package of fatal error pulses
      if (fatal_error_active && (blink.fatal_error_pulses_done >= fatal_error_pulses))
        blink.fatal_error_break_active = true;
    }
  }

  // synchronize things once a second
  if ((cycle < blink.cycle_last) && !blink.fatal_error_break_active) {
    todo = LED_TODO_ACTIVATE;
    blink.ticks_last = WAIT_GET_TIMER_VALUE_32();
    blink.count_ms = 0;
  }

  // update LED and remind changes
  LED_SET(blink_timings[blink_sequence].color, todo);
  if (todo == LED_TODO_DEACTIVATE)
    blink.led_active = false;
  if (todo == LED_TODO_ACTIVATE)
    blink.led_active = true;

  // remember cycle for the next call
  blink.cycle_last = cycle;
}

/*============================================================================*/
/* E N D   O F   F I L E                                                      */
/*============================================================================*/
