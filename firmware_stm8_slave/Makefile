# STM8 Makefile for SmartPlants Firmware
# Target MCU: STM8S003F3P6
TARGET = main
MCU = stm8s003f3
SDCC = sdcc
OBJCOPY = objcopy
STFLASH = st-flash

# Source files
SRC_DIR = src
INC_DIR = inc
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/hal_gpio.c $(SRC_DIR)/led.c
HEADERS = $(INC_DIR)/hal_gpio.h $(INC_DIR)/led.h
OBJS = $(SRC_DIR)/main.rel $(SRC_DIR)/hal_gpio.rel $(SRC_DIR)/led.rel

# Compiler flags
CFLAGS = -mstm8 --std-c99 -I$(INC_DIR) --opt-code-size
LDFLAGS = -mstm8 -lstm8

# Build targets
all: $(TARGET).ihx

$(TARGET).ihx: $(OBJS)
	$(SDCC) $(LDFLAGS) -o $(TARGET).ihx $(OBJS)

$(SRC_DIR)/%.rel: $(SRC_DIR)/%.c $(HEADERS)
	$(SDCC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET).ihx $(TARGET).lk $(TARGET).lst $(TARGET).map $(TARGET).rel $(TARGET).rst $(TARGET).sym $(SRC_DIR)/*.rel

flash: $(TARGET).ihx
	$(STFLASH) --format ihex write $(TARGET).ihx

lint:
	cppcheck --enable=all $(SRC_DIR)/ $(INC_DIR)/

.PHONY: all clean flash lint