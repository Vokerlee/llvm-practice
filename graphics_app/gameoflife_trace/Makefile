CC                      = clang
LLVM_OPT                = opt

BUILD_DIR               = build
APP_NAME                = gameoflife

GR_LIB_NAME             = sgl
GR_LIB_FILENAME         = lib$(GR_LIB_NAME).a

GR_APP_LIB_PATH         = ../$(APP_NAME)_sdl

GR_APP_SRC_PATH         = $(GR_APP_LIB_PATH)/src
GR_APP_BUILD_PATH       = $(GR_APP_LIB_PATH)/$(BUILD_DIR)

GR_OTHER_SOURCES        = main.c
GR_BASE_SOURCES         = $(APP_NAME).c
GR_BASE_LLVM_BC         = $(APP_NAME).bc
GR_BASE_LLVM_IR         = $(APP_NAME).ll

GR_BASE_LLVM_IR_PATCHED = $(APP_NAME)_patched.ll
GR_APP_PATCHED          = $(APP_NAME)_patched
GR_APP_TRACE_FILENAME   = $(APP_NAME).trace

GR_SOURCES              = $(GR_BASE_SOURCES) $(GR_OTHER_SOURCES)

LLVM_PASS_LOGGER_SRC    = log.c
LLVM_PASS_LOGGER_PATH   = llvm_pass_logger
LLVM_PASS_LOGGER_LIB    = pass_logger.so

.PHONY: all build debug run clean

all: build

build: $(GR_APP_PATCHED)

$(GR_APP_PATCHED):
	mkdir -p $(BUILD_DIR)

	$(CC) -I$(GR_APP_LIB_PATH) -O2 -emit-llvm $(GR_APP_SRC_PATH)/$(GR_BASE_SOURCES) -c -o $(BUILD_DIR)/$(GR_BASE_LLVM_BC)
	$(CC) -I$(GR_APP_LIB_PATH) -O2 -emit-llvm $(GR_APP_SRC_PATH)/$(GR_BASE_SOURCES) -S -o $(BUILD_DIR)/$(GR_BASE_LLVM_IR)

	$(LLVM_OPT) -enable-new-pm=0 -load $(LLVM_PASS_LOGGER_PATH)/$(BUILD_DIR)/$(LLVM_PASS_LOGGER_LIB) \
		-logger -S -o $(BUILD_DIR)/$(GR_BASE_LLVM_IR_PATCHED) < $(BUILD_DIR)/$(GR_BASE_LLVM_BC)

	$(CC) $(BUILD_DIR)/$(GR_BASE_LLVM_IR_PATCHED) $(GR_APP_SRC_PATH)/$(GR_OTHER_SOURCES) $(LLVM_PASS_LOGGER_PATH)/$(LLVM_PASS_LOGGER_SRC) \
		$(GR_APP_BUILD_PATH)/$(GR_LIB_FILENAME) -I$(GR_APP_LIB_PATH) -lSDL2 -o $(BUILD_DIR)/$@

trace_gen:
	./$(BUILD_DIR)/$(GR_APP_PATCHED) > $(GR_APP_TRACE_FILENAME)

clean:
	rm -rf *.o $(BUILD_DIR) *.trace
