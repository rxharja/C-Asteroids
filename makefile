CC      := gcc
AR      := ar
CFLAGS  := -O1 -g -Wall -D_GNU_SOURCE=1 -D_THREAD_SAFE
DEPFLAGS := -MMD -MP
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS   := $(shell sdl2-config --libs) -lSDL2_ttf

BIN_DIR   := bin
BUILD_DIR := build
LIB_DIR   := lib

GAME_DIR  := src
PHYS_DIR  := physics

PHYS_SRC := $(wildcard $(PHYS_DIR)/*.c)
PHYS_OBJ := $(patsubst $(PHYS_DIR)/%.c,$(BUILD_DIR)/physics/%.o,$(PHYS_SRC))
PHYS_DEP := $(PHYS_OBJ:.o=.d)
PHYS_LIB := $(LIB_DIR)/libphysics.a

GAME_SRC := $(wildcard $(GAME_DIR)/*.c)
GAME_OBJ := $(patsubst $(GAME_DIR)/%.c,$(BUILD_DIR)/game/%.o,$(GAME_SRC))
GAME_DEP := $(GAME_OBJ:.o=.d)

GAME_INCLUDES := -I$(GAME_DIR) -I$(PHYS_DIR)
GAME_CFLAGS   := $(CFLAGS) $(DEPFLAGS) $(SDL_CFLAGS) $(GAME_INCLUDES)
PHYS_CFLAGS   := $(CFLAGS) $(DEPFLAGS) -I$(PHYS_DIR)

TARGET := $(BIN_DIR)/asteroids

.PHONY: all clean physics

all: $(TARGET)

physics: $(PHYS_LIB)

$(TARGET): $(GAME_OBJ) $(PHYS_LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(GAME_OBJ) -o $@ -L$(LIB_DIR) -lphysics $(SDL_LIBS) -lm

$(PHYS_LIB): $(PHYS_OBJ) | $(LIB_DIR)
	$(AR) rcs $@ $^

$(BUILD_DIR)/game/%.o: $(GAME_DIR)/%.c | $(BUILD_DIR)/game
	$(CC) $(GAME_CFLAGS) -c $< -o $@

$(BUILD_DIR)/physics/%.o: $(PHYS_DIR)/%.c | $(BUILD_DIR)/physics
	$(CC) $(PHYS_CFLAGS) -c $< -o $@

$(BIN_DIR) $(LIB_DIR) $(BUILD_DIR)/game $(BUILD_DIR)/physics:
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(PHYS_LIB)

# Pull in compiler-generated header dependencies. Leading dash suppresses
# the "no such file" error on first build, before any .d exists.
-include $(GAME_DEP) $(PHYS_DEP)
