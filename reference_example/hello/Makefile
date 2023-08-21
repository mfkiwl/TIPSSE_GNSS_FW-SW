.PHONY: all
all: build/hello.uf2

build/hello.uf2: main.c build/Makefile
	cd build && make -j

build/Makefile:
	mkdir -p build
	cd build && cmake ..

.PHONY: clean
clean:
	rm -rf build