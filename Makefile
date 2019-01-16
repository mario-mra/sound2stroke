CC := clang
# CC := gcc
CFLAGS := -Wall -Wextra -Wundef -std=c17
CLIBS  := -lm

BUILD_DIR := ./build
SRC_DIR   := ./src

MAIN_OUT_NAME := sound2stroke.run
TEST_OUT_NAME := test.run

SRC_NAMES     = base64 wave mrrb
MAIN_SRC_NAME = main
TEST_SRC_NAME = test

D_TEST := 0


###
ifeq ($(D_TEST), 0)
	OUT_NAME := $(MAIN_OUT_NAME)
	SRC_MAIN := $(MAIN_SRC_NAME)
else ifeq ($(D_TEST), 1)
	OUT_NAME := $(TEST_OUT_NAME)
	SRC_MAIN := $(TEST_SRC_NAME)
else
	OUT_NAME := $(MAIN_OUT_NAME)
	SRC_MAIN := $(MAIN_SRC_NAME)
endif
###

###
SRC_ROUTE := $(addprefix $(SRC_DIR)/, $(SRC_NAMES) $(SRC_MAIN))
SRC_OBJTS := $(addsuffix .o, $(SRC_ROUTE))
###


all: build

run: build
	@$(BUILD_DIR)/$(OUT_NAME)


$(SRC_OBJTS): %.o: %.c
	@echo Building $@
	@$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_DIR)/$(OUT_NAME): $(SRC_OBJTS)
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(CLIBS) $^ -o $@

build: $(BUILD_DIR)/$(OUT_NAME)


clean:
	rm -fdr $(BUILD_DIR)/
	rm -f $(SRC_DIR)/*.o

###

.PHONY: all build clean run
# .SILENT: