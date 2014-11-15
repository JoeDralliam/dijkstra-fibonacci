CC=gcc
WARNINGS=-W -Wall -Wextra -Werror
OTHER_FLAGS=-std=c99
FLAGS= $(OTHER_FLAGS) $(WARNINGS)
COMPILE=-c
OUTPUT=-o
BUILD_DIR=_build

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o:%.c $(BUILD_DIR)
	$(CC) $(FLAGS) $(COMPILE) $< $(OUTPUT) $@

clean:
	rm -r $(BUILD_DIR)
