all: build

clean:
	rm src/generated/*
	rm -r webapp/node_modules
	rm -r webapp/dist
	pio run -t clean

build_webapp:
	cd webapp/ && npm install
	cd webapp/ && npm run build

prepare_webapp: build_webapp
	python3 scripts/pack_webapp.py

prepare: prepare_webapp
	python3 scripts/create_lib_definition.py

build:
	pio run