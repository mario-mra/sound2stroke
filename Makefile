CC := clang
# CC := gcc
CFLAGS := -Wall -Wextra -Wundef -std=c17
CLIBS  := -lm

BUILD_DIR := ./build
SRC_DIR   := ./src


MAIN_SRC_OBJTS = main.o base64.o wave.o mrrb.o
MAIN_OUT_NAME := sound2stroke.run

D_TEST := 0
TEST_SRC_OBJTS = test.o mrrb.o base64.o
TEST_OUT_NAME := test.run

###
MAIN_SRC_OBJTS := $(addprefix $(SRC_DIR)/, $(MAIN_SRC_OBJTS))
TEST_SRC_OBJTS := $(addprefix $(SRC_DIR)/, $(TEST_SRC_OBJTS))
###

###
ifeq ($(D_TEST), 0)
	SRC_OBJTS := $(MAIN_SRC_OBJTS)
	OUT_NAME  := $(MAIN_OUT_NAME)
else ifeq ($(D_TEST), 1)
	SRC_OBJTS := $(TEST_SRC_OBJTS)
	OUT_NAME  := $(TEST_OUT_NAME)
else
	SRC_OBJTS := $(MAIN_SRC_OBJTS)
	OUT_NAME  := $(MAIN_OUT_NAME)
endif
###

all: build

run: build
	$(BUILD_DIR)/$(OUT_NAME)


$(SRC_OBJTS): %.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_DIR)/$(OUT_NAME): $(SRC_OBJTS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CLIBS) $^ -o $@

build: $(BUILD_DIR)/$(OUT_NAME)



clean:
	rm -fdr $(BUILD_DIR)/
	rm -f $(SRC_DIR)/*.o

###

.PHONY: all build clean run
# .SILENT: