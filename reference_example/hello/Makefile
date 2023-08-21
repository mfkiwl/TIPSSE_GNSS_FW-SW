.PHONY: all
all: build/hello.uf2

build/hello.elf build/hello.uf2: main.c build/Makefile
	cd build && make -j

build/Makefile:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..

.PHONY: clean
clean:
	rm -rf build

.PHONY: load
load: build/hello.uf2
	picotool load -f $<

.PHONY: openocd
openocd:
	openocd -f board/pico-debug.cfg

.PHONY: load-elf
load-elf: build/hello.elf
	arm-none-eabi-gdb -x load.txt --batch --args $<