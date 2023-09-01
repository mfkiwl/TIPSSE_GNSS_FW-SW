dist/index.html: $(wildcard *) $(wildcard src/*) $(wildcard src/*/*)
	npm run build

node_modules:
	npm i

.PHONY: clean
clean:
	rm -rf build dist

.PHONY: cleanall
cleanall: clean
	rm -rf node_modules package-lock.json .parcel-cache