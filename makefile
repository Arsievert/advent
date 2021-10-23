# Makefile

# Author:  Austin Sievert (arsievert1@gmail.com)
# URL:     https://github.com/arsievert1/advent

# License: MIT

YEAR    := 2020
DAY     := day8

CC      := gcc
CFLAGS  := -Wall
BUILD   := build
OUT     := $(BUILD)/out
OBJ     := $(BUILD)/obj
TARGET  := $(DAY).out
INCLUDE := -Iutils/

SRC     :=                      \
	$(wildcard utils/*.c)   \
	$(wildcard $(YEAR)/$(DAY)/*.c)  \

OBJECTS := $(SRC:%.c=$(OBJ)/%.o)

all: $(OUT)/$(TARGET)
	mv $(OUT)/$(TARGET) $(YEAR)/$(DAY)/

$(OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OUT)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CLFAGS) -o $(OUT)/$(TARGET) $^

.PHONY: all clean run info

clean:
	-@rm -rvf $(BUILD)
	-@rm -rvf $(YEAR)/$(DAY)/$(DAY).out

run:
	./$(YEAR)/$(DAY)/$(TARGET) $(YEAR)/$(DAY)/input.txt

info:
	@echo "[*] Src:  ${SRC}            "
	@echo "[*] Obj:  ${OBJECTS}        "
	@echo "[*] Out:  ${DAY}/${TARGET}  "
