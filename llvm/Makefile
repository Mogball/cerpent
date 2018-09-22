BUILD_DIR=bin
EXE_NAME=cerp

$(BUILD_DIR)/$(EXE_NAME): $(BUILD_DIR)/Makefile
	@make -C $(BUILD_DIR) -j8 --no-print-directory

$(BUILD_DIR)/Makefile: CMakeLists.txt $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ../

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

.PHONY: run

run:
	@$(BUILD_DIR)/$(EXE_NAME)
