.PHONY: all
all: build/Makefile
	cd build && make -j

build/Makefile:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..

.PHONY: clean
clean:
	rm -rf build

.PHONY: load
load: build/tipsse_gnss.uf2
	picotool load -f $<

.PHONY: openocd
openocd:
	openocd -f board/pico-debug.cfg

.PHONY: load-elf
load-elf: build/tipsse_gnss.elf
	arm-none-eabi-gdb -x load.txt --batch --args $<