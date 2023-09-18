.PHONY: all
all: build/Makefile bin/pack
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

.PHONY: pack
pack: bin/pack

bin/pack: mongoose/test/pack.c
	mkdir -p $(@D)
	$(CC) $< -o $@