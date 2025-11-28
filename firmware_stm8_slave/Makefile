# ===============================
# STM8 Makefile (Updated)
# Target: STM8S003F3
# ===============================

# Tools
SDCC    = sdcc
STFLASH = st-flash

# Directories
SRC_DIR   = src
INC_DIR   = inc
BUILD_DIR = Builds

# Target output
TARGET = $(BUILD_DIR)/main

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/gpio.c \
       $(SRC_DIR)/clock.c \
       $(SRC_DIR)/tim4.c \
	   $(SRC_DIR)/uart.c

# Header files
HEADERS = $(INC_DIR)/gpio.h \
          $(INC_DIR)/clock.h \
          $(INC_DIR)/tim4.h \
		  $(INC_DIR)/uart.h
		  
# Object files → Builds/*.rel
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.rel)

# Compiler flags
CFLAGS = -mstm8 --std-c99 -I$(INC_DIR) --opt-code-size
LDFLAGS = -mstm8 -lstm8

# ===============================
# Build Rules
# ===============================

# Default target
all: $(TARGET).ihx

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Linking into .ihx
$(TARGET).ihx: $(OBJS)
	$(SDCC) $(LDFLAGS) -o $(TARGET).ihx $(OBJS)

# Compile .c → .rel inside Builds/
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	$(SDCC) $(CFLAGS) -c $< -o $@

# ===============================
# Utility Targets
# ===============================

clean:
	rm -rf $(BUILD_DIR)

flash: $(TARGET).ihx
	$(STFLASH) --format ihex write $(TARGET).ihx

.PHONY: all clean flash
