CC                = clang++

BUILD_DIR         = build
APP_NAME          = gameoflife

GR_LIB_NAME       = sgl
GR_LIB_FILENAME   = lib$(GR_LIB_NAME).a

GR_APP_LIB_PATH   = ../$(APP_NAME)_sdl

GR_APP_SRC_PATH   = $(GR_APP_LIB_PATH)/src
GR_APP_BUILD_PATH = $(GR_APP_LIB_PATH)/$(BUILD_DIR)

GR_OTHER_SOURCES  = main.c
GR_BASE_SOURCES   = $(APP_NAME).c
GR_BASE_LLVM_IR   = $(APP_NAME).ll
GR_SOURCES        = $(GR_BASE_SOURCES) $(GR_OTHER_SOURCES)

GR_IR_GEN_APP     = gameoflife_ir_gen
GR_IR_GEN_SRC     = $(GR_IR_GEN_APP).cpp

.PHONY: all build debug run clean

all: build

build: $(GR_BASE_LLVM_IR)

$(GR_BASE_LLVM_IR): $(GR_IR_GEN_APP)

$(GR_IR_GEN_APP):
	mkdir -p $(BUILD_DIR)

	$(CC) -I$(GR_APP_LIB_PATH) -O2 -lLLVM -lSDL2 $(GR_APP_BUILD_PATH)/$(GR_LIB_FILENAME) $(GR_IR_GEN_SRC) -o $(BUILD_DIR)/$@

clean:
	rm -rf *.o $(BUILD_DIR) *.trace
