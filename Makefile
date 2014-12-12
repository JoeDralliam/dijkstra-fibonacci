CC=gcc

WARNINGS=-W -Wall -Wextra -Werror
OTHER_FLAGS=-std=c99 -I. -g
FLAGS=$(OTHER_FLAGS) $(WARNINGS)
BONUS_FLAGS=$(FLAGS) -I/usr/include/libxml2
COMPILE=-c
OUTPUT=-o
LINK_FLAGS=-lm
BONUS_LINK_FLAGS=$(LINK_FLAGS) -lxml2

BUILD_DIR=_build
TEST_DIR=tests
SOURCES=${wildcard *.c}
OBJECTS=$(patsubst %.c,_build/%.o,$(SOURCES))

DIJKSTRA_DIR=dijkstra
BUILD_DIJKSTRA_DIR=$(BUILD_DIR)/$(DIJKSTRA_DIR)
DIJKSTRA_SOURCES=$(DIJKSTRA_DIR)/main.c
DIJKSTRA_OBJECTS=$(BUILD_DIJKSTRA_DIR)/main.o

BONUS_DIR=bonus
BUILD_BONUS_DIR=$(BUILD_DIR)/$(BONUS_DIR)
BONUS_SOURCES=$(wildcard $(BONUS_DIR)/*.c)
BONUS_OBJECTS=$(patsubst $(BONUS_DIR)/%.c,$(BUILD_BONUS_DIR)/%.o,$(BONUS_SOURCES))

TARGETS=exec_dijkstra exec_bonus

#gen_graph_dijkstra

BUILD_TEST_DIR=$(BUILD_DIR)/$(TEST_DIR)
TESTS_SOURCES=$(wildcard $(TEST_DIR)/*.c)
TESTS=$(patsubst $(TEST_DIR)/%.c,%.test,$(TESTS_SOURCES))

#//*.c/'_build/%.o')"


all:$(TARGETS)

exec_dijkstra:$(OBJECTS) $(DIJKSTRA_OBJECTS)
	$(CC) $(FLAGS) $(LINK_FLAGS) $^ $(OUTPUT) $@

exec_bonus:$(BONUS_OBJECTS) $(OBJECTS)
	$(CC) $(BONUS_FLAGS) $(BONUS_LINK_FLAGS) $^ $(OUTPUT) $@

gen_graph_dijkstra:$(OBJECTS)
	$(CC) $(FLAGS) $(LINK_FLAGS) $^ gen_tests/graph_dijkstra.c $(OUTPUT) $@

tests:$(TESTS)

%.test:$(OBJECTS) $(BUILD_TEST_DIR)/%.o
	$(CC) $(FLAGS) $(LINK_FLAGS) $^ $(OUTPUT) $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_TEST_DIR): $(BUILD_DIR)
	mkdir -p $(BUILD_TEST_DIR)

$(BUILD_DIJKSTRA_DIR):
	mkdir -p $(BUILD_DIJKSTRA_DIR)

$(BUILD_BONUS_DIR):
	mkdir -p $(BUILD_BONUS_DIR)


$(BUILD_DIR)/%.o:%.c $(BUILD_DIR)
	$(CC) $(FLAGS) $(COMPILE) $< $(OUTPUT) $@

$(BUILD_TEST_DIR)/%.o:$(TEST_DIR)/%.c $(BUILD_TEST_DIR)
	$(CC) $(FLAGS) $(COMPILE) $< $(OUTPUT) $@

$(BUILD_DIJKSTRA_DIR)/%.o:$(DIJKSTRA_DIR)/%.c $(BUILD_DIJKSTRA_DIR)
	$(CC) $(FLAGS) $(COMPILE) $< $(OUTPUT) $@

$(BUILD_BONUS_DIR)/%.o:$(BONUS_DIR)/%.c $(BUILD_BONUS_DIR)
	$(CC) $(BONUS_FLAGS) $(COMPILE) $< $(OUTPUT) $@


clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TESTS)
	rm -f $(TARGETS)
