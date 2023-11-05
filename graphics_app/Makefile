.ONESHELL:

GR_APP_FOLDER        = gameoflife_sdl
GR_APP_TRACE_FOLDER  = gameoflife_trace
GR_APP_IR_GEN_FOLDER = gameoflife_ir_gen

.PHONY: all build $(GR_APP_FOLDER) $(GR_APP_TRACE_FOLDER) $(GR_APP_IR_GEN_FOLDER)

all: build

build: $(GR_APP_FOLDER) $(GR_APP_TRACE_FOLDER) $(GR_APP_IR_GEN_FOLDER)

$(GR_APP_FOLDER):
	cd $(GR_APP_FOLDER)
	mkdir -p build && cd build
	cmake ..
	make clean
	make
	cd ../..

$(GR_APP_TRACE_FOLDER): $(GR_APP_FOLDER)
	cd $(GR_APP_TRACE_FOLDER)
	make clean
	make
	cd ../..

$(GR_APP_IR_GEN_FOLDER): $(GR_APP_FOLDER)
	cd $(GR_APP_IR_GEN_FOLDER)
	make clean
	make
	cd ../..
