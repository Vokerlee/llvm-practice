CC = clang++

BUILD_DIR = build

SOURCES = pass_logger.cpp
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all build debug run clean

BIN := $(SOURCES:.cpp=.so)

all: build

build: $(BIN)

$(BIN): $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(BUILD_DIR)/$^ -fPIC -shared -o $(BUILD_DIR)/$@

%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $^ -O2 -fPIC -c -o $(BUILD_DIR)/$@

clean:
	rm -rf *.o *.so $(BUILD_DIR)
