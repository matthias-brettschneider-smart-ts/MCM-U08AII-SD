################################################################################
#                                                                              #
#                         |\    /|   /- - -  |\    /|                          #
#                         | \  / |  |        | \  / |                          #
#                         |  \/  |  |        |  \/  |                          #
#                         |      |   \_ _ _  |      |                          #
#                                                                              #
#                                                                              #
#         T H E   S M A R T   M E A S U R E M E N T   S O L U T I O N          #
#                                                                              #
################################################################################
#             (c) SMART TESTSOLUTIONS GmbH - all rights reserved               #
################################################################################

TOOLS_IHEX_MODIFIER := ihex_modifier

# Individual artifacts have to be checked manually because:
# - Names are not fix (and therefore can not be known before building)
# - Dependencies are unknown to this makefile (and therefore changes can not be tracked)

.PHONY: all clean individuals

.SECONDARY:

all: individuals build/temp/dummy.dep

clean:
	@echo "removing build folder"
	@rm -rf build
	@make -C $(MAKE_PATH_TO_FLASHLOADER) -f $(MAKE_FLASHLOADER_PROJECT_NAME).mk clean
	@make -C $(MAKE_PATH_TO_APPLICATION) -f $(MAKE_APPLICATION_PROJECT_NAME).mk clean

individuals:
	@echo "--- updating flashloader ---"
	@make -C $(MAKE_PATH_TO_FLASHLOADER) -f $(MAKE_FLASHLOADER_PROJECT_NAME).mk -j
	@echo "--- updating application ---"
	@make -C $(MAKE_PATH_TO_APPLICATION) -f $(MAKE_APPLICATION_PROJECT_NAME).mk -j

build/temp/dummy.dep: $(MAKE_PATH_TO_FLASHLOADER)/build $(MAKE_PATH_TO_APPLICATION)/build
	$(eval FILENAME_FLASHLOADER := $(shell ls $(MAKE_PATH_TO_FLASHLOADER)/build | grep $(MAKE_FLASHLOADER_PROJECT_NAME)_[0-9]\*.[0-9]\*.[0-9]\*.hex))
	$(eval FILENAME_APPLICATION := $(shell ls $(MAKE_PATH_TO_APPLICATION)/build | grep $(MAKE_APPLICATION_PROJECT_NAME)_[0-9]\*.[0-9]\*.[0-9]\*.hex))
	$(eval VERSION_FLASHLOADER := $(FILENAME_FLASHLOADER:$(MAKE_FLASHLOADER_PROJECT_NAME)_%.hex=%))
	$(eval VERSION_APPLICATION := $(FILENAME_APPLICATION:$(MAKE_APPLICATION_PROJECT_NAME)_%.hex=%))
	$(eval FILE_HEX_MERGED := $(MAKE_FLASHLOADER_PROJECT_NAME)_$(VERSION_FLASHLOADER)_$(MAKE_APPLICATION_PROJECT_NAME)_$(VERSION_APPLICATION).hex)
	@echo "--- joining flashloader & application ---"
	@rm -rf build/temp
	@mkdir -p build/temp
	@cp $(MAKE_PATH_TO_FLASHLOADER)/build/$(FILENAME_FLASHLOADER) build/temp/flashloader_$(FILENAME_FLASHLOADER)
	@cp $(MAKE_PATH_TO_APPLICATION)/build/$(FILENAME_APPLICATION) build/temp/application_$(FILENAME_APPLICATION)
	@$(TOOLS_IHEX_MODIFIER) --exclude $(MERGE_BLOCK_APPLICATION_START) $(MERGE_BLOCK_APPLICATION_LENGTH) build/temp/flashloader_$(FILENAME_FLASHLOADER)
	@$(TOOLS_IHEX_MODIFIER) --crop $(MERGE_BLOCK_APPLICATION_START) $(MERGE_BLOCK_APPLICATION_LENGTH) build/temp/application_$(FILENAME_APPLICATION)
	@touch build/temp/$(FILE_HEX_MERGED)
	@$(TOOLS_IHEX_MODIFIER) --merge build/temp/flashloader_$(FILENAME_FLASHLOADER) build/temp/$(FILE_HEX_MERGED)
	@$(TOOLS_IHEX_MODIFIER) --merge build/temp/application_$(FILENAME_APPLICATION) build/temp/$(FILE_HEX_MERGED)
	@echo "creating/replacing build/$(FILE_HEX_MERGED)"
	@cp build/temp/$(FILE_HEX_MERGED) build/$(FILE_HEX_MERGED)
	@touch build/temp/dummy.dep

################################################################################
#  E N D   O F   F I L E                                                       #
################################################################################
