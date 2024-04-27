# Makefile

# Author:  Austin Sievert (arsievert1@gmail.com)
# URL:     https://github.com/arsievert1/advent

# License: MIT

YEAR    := 2020
DAY     := day8

CC      := clang
CFLAGS  := -Wall
BUILD   := build
OUT     := $(BUILD)/out
OBJ     := $(BUILD)/obj
JSON    := $(BUILD)/json
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
	@mkdir -p $(JSON)
	$(CC) $(CFLAGS) $(INCLUDE) -MJ $(JSON)/$(@F).json -c $< -o $@

$(OUT)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CLFAGS) -o $(OUT)/$(TARGET) $^
	@cat $(JSON)/* >> temp.json
	@sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' temp.json > compile_commands.json
	@rm temp.json

.PHONY: all clean run info

clean:
	-@rm -rvf $(BUILD) *.json .cache/
	-@rm -rvf $(YEAR)/$(DAY)/$(DAY).out

run:
	./$(YEAR)/$(DAY)/$(TARGET) $(YEAR)/$(DAY)/input.txt

test:
	./$(YEAR)/$(DAY)/$(TARGET) $(YEAR)/$(DAY)/test_input.txt

info:
	@echo "[*] Src:  ${SRC}            "
	@echo "[*] Obj:  ${OBJECTS}        "
	@echo "[*] Out:  ${DAY}/${TARGET}  "
