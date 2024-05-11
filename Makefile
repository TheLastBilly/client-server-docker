CC			:= gcc
CFLAGS			:= -Wall -Wpedantic

BUILD_DIR		:= obj
TARGET_DIR			:= bin
SRC_DIR			:= .
ASSETS_DIR		:= assets

SRCS			:= $(shell find $(SRC_DIR) -maxdepth 1 -name '*.c')
OBJS			:= $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
CFLAGS			+= -MMD -MP
DEPS			:= $(OBJS:%.o=%.d)
DEPS			+= $(TEST_OBJS:%.o=%.d)

CFLAGS			+= -DGIT_VERSION=\"$(shell git rev-parse HEAD)\"

SERVER_TARGET		:= $(TARGET_DIR)/server
CLIENT_TARGET		:= $(TARGET_DIR)/client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_TARGET): $(BUILD_DIR)/server.o
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

$(CLIENT_TARGET): $(BUILD_DIR)/client.o
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET_DIR)

.PHONY: all clean

-include: $(DEPS)
