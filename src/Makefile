CXX			= g++
CFLAGS			:= -Wall -Wpedantic

BUILD_DIR		:= build
OBJ_DIR			:= $(BUILD_DIR)/obj
TARGET_DIR		:= $(BUILD_DIR)/bin
SRC_DIR			:= .
ASSETS_DIR		:= assets

# I'm using git rev-parse instead to increase compatibility with older
# versions of git.
GIT_VERSION		= $(shell git rev-parse HEAD)

SRCS			:= $(shell find $(SRC_DIR) -maxdepth 1 -name '*.cpp')
OBJS			:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CFLAGS			+= -MMD -MP
DEPS			:= $(OBJS:%.o=%.d)

CFLAGS			+= -DGIT_VERSION=\"$(GIT_VERSION)\"

SERVER_TARGET		:= $(TARGET_DIR)/server
CLIENT_TARGET		:= $(TARGET_DIR)/client
TEST_SOCKET		:= $(TARGET_DIR)/test.sock

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(SERVER_TARGET): $(OBJ_DIR)/server.o $(OBJ_DIR)/socket.o
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(CLIENT_TARGET): $(OBJ_DIR)/client.o $(OBJ_DIR)/socket.o
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

test: $(SERVER_TARGET) $(CLIENT_TARGET)
	$(SERVER_TARGET) $(TEST_SOCKET) &

	@# Not necessary per say, but the socket may not
	@# be ready by the time the client comes online
	@while [ ! -S $(TEST_SOCKET) ]; do sleep 0.1; done

	$(CLIENT_TARGET) $(TEST_SOCKET) VERSION
	$(CLIENT_TARGET) $(TEST_SOCKET) TEST
	$(CLIENT_TARGET) $(TEST_SOCKET) QUIT

.PHONY: all clean test_client test_server rebuild test

-include: $(DEPS)
