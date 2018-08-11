CC=gcc
AR=ar
CFLAGS=
ARFLAGS=

OUT_DIR=bin
LIB_DIR=lib
SRC_DIR=src
I_DIR=include

LIB_NAME=libsym.a

$(OUT_DIR)/$(LIB_NAME): $(patsubst %.c, $(OUT_DIR)/$(LIB_NAME)/%.o, $(wildcard *.c))
	$(AR) -r $(ARFLAGS) -o $@ $^

$(OUT_DIR)/$(LIB_NAME)/%.o: $(LIB_DIR)/%.c dirmake
	$(CC) -c -I $(I_DIR) $(CFLAGS) -o $@ $<

dirmake:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OUT_DIR)/$(LIB_NAME)
