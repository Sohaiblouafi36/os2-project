# Compiler and assembler
CC = gcc
ASM = nasm

# Directories
SRC_DIR = c
ASM_DIR = asm
TEST_DIR = tests
BUILD_DIR = build

# Flags
CFLAGS = -Wall -Wextra -Iinclude -D_POSIX_C_SOURCE=199309L -O3
ASMFLAGS = -f elf64
LDFLAGS = -no-pie -lm

# Targets
TARGETS = benchmarks test_numbers test_strings test_arrays main

# Source files
C_SRCS = $(wildcard $(SRC_DIR)/*.c)
ASM_SRCS = $(wildcard $(ASM_DIR)/*.asm)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Objects
C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))
ASM_OBJS = $(patsubst $(ASM_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))

.PHONY: all clean test

all: $(TARGETS)

# Main programs
main: $(BUILD_DIR)/main.o $(ASM_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

benchmarks: $(BUILD_DIR)/benchmarks.o $(ASM_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Test programs
test_%: $(BUILD_DIR)/test_%.o $(BUILD_DIR)/%.o
	$(CC) $(LDFLAGS) -o $@ $^

# Build rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(ASM_DIR)/%.asm | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: test_numbers test_strings test_arrays
	@echo "\n=== Running Tests ==="
	@./test_numbers && echo "Numbers tests passed!"
	@./test_strings && echo "Strings tests passed!"
	@./test_arrays && echo "Arrays tests passed!"
	@echo "\n✅ All tests passed!"

clean:
	rm -rf $(BUILD_DIR) $(TARGETS)
