
# Directories
BIN_DIR := bin
OBJ_DIR := obj
SRC_DIR := src
LOG_DIR := log
DATA_DIR := data
INC_DIR := include  # Add this for header files

# Server files
SERVER_SRCS := $(wildcard $(SRC_DIR)/server/*.c) $(SRC_DIR)/server/encrypt_decrypt.c $(SRC_DIR)/server/server_logger.c
SERVER_OBJS := $(patsubst $(SRC_DIR)/server/%.c, $(OBJ_DIR)/server/%.o, $(SERVER_SRCS))
SERVER_EXEC := $(BIN_DIR)/server

# Client files
CLIENT_SRCS := $(wildcard $(SRC_DIR)/client/*.c) $(SRC_DIR)/client/encrypt_decrypt.c $(SRC_DIR)/client/client_logger.c
CLIENT_OBJS := $(patsubst $(SRC_DIR)/client/%.c, $(OBJ_DIR)/client/%.o, $(CLIENT_SRCS))
CLIENT_EXEC := $(BIN_DIR)/client

# Compiler and flags
CC := gcc
CFLAGS := -Wall -g -I$(INC_DIR)  # Add -I to include the directory
LDFLAGS := -lpthread

# Targets
all: $(SERVER_EXEC) $(CLIENT_EXEC)

# Server executable
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Client executable
$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Object files for server
$(OBJ_DIR)/server/%.o: $(SRC_DIR)/server/%.c
	@mkdir -p $(OBJ_DIR)/server
	$(CC) $(CFLAGS) -c $< -o $@

# Object files for client
$(OBJ_DIR)/client/%.o: $(SRC_DIR)/client/%.c
	@mkdir -p $(OBJ_DIR)/client
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

