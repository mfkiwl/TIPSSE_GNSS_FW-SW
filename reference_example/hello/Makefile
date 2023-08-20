PHONY: all
all: build/hello.uf2

hello.uf2: main.c build/Makefile
	cd build && make -j

build/Makefile:
	mkdir -p build
	cd build && cmake ..

.PHONY: flash
flash: build/hello.uf2
	picotool save $<