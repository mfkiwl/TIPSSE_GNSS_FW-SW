.PHONY: all
all: build/Makefile
	cd build && make -j

build/Makefile:
	mkdir -p build
	cd build && cmake ..

.PHONY: clean
clean:
	rm -rf build

.PHONY: load
load: build/tipsse_gnss.uf2
	picotool load -f $<