CXX			:= g++
CFLAGS			:= -Wall -Wpedantic

BUILD_DIR		:= obj
TARGET_DIR		:= bin
SRC_DIR			:= .
ASSETS_DIR		:= assets

SRCS			:= $(shell find $(SRC_DIR) -maxdepth 1 -name '*.cpp')
OBJS			:= $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
CFLAGS			+= -MMD -MP
DEPS			:= $(OBJS:%.o=%.d)
DEPS			+= $(TEST_OBJS:%.o=%.d)

CFLAGS			+= -DGIT_VERSION=\"$(shell git rev-parse HEAD)\"

SERVER_TARGET		:= $(TARGET_DIR)/server
CLIENT_TARGET		:= $(TARGET_DIR)/client
TEST_SOCKET		:= $(TARGET_DIR)/test.sock

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(SERVER_TARGET): $(BUILD_DIR)/server.o $(BUILD_DIR)/socket.o
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(CLIENT_TARGET): $(BUILD_DIR)/client.o $(BUILD_DIR)/socket.o
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TARGET_DIR)

test_client: $(CLIENT_TARGET)
	$^ $(TEST_SOCKET) VERSION
	$^ $(TEST_SOCKET) TEST

test_server: $(SERVER_TARGET)
	$^ $(TEST_SOCKET)

.PHONY: all clean test_client test_server

-include: $(DEPS)
