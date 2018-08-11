OUT_DIR=build
EXE_NAME=interp

$(OUT_DIR)/$(EXE_NAME): $(OUT_DIR)/Makefile
	@make -C $(OUT_DIR) --no-print-directory

$(OUT_DIR)/Makefile: CMakeLists.txt $(OUT_DIR)
	@cd $(OUT_DIR) && cmake ../

$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

.PHONY: run
run: $(OUT_DIR)/$(EXE_NAME)
	@$(OUT_DIR)/$(EXE_NAME)
