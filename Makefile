credits:
	@echo "UW Med-Tech Resolve Onboarding Project"
	@echo "Created on September 19, 2025"
	@echo "Authored by Adrian Tarantino" 
	@echo "GitHub: AdrianTheHacker"

source_path := ./src/
build_path := ./build/

drivers_path := ./src/drivers/
drivers_build_path := ./build/drivers/
driver_names := temperature_sensor

.drivers_build_setup:
	mkdir -p $(drivers_build_path)

$(driver_names): .drivers_build_setup
	@echo 'Building object file for: $@'
	mkdir -p $(drivers_build_path)$@
	touch $(drivers_build_path)$@/$@.o
	cc -c $(drivers_path)$@/$@.c -o ./build/drivers/$@/$@.o

.PHONY: build
build: $(driver_names)
	cc -c $(source_path)main.c -o $(build_path)main.o
	cc $(build_path)main.o $(foreach f, $^, $(drivers_build_path)$f/$f.o) -I $(foreach f, $^, $(drivers_path)$f/) -o $(build_path)main.exe

.PHONY: clean
clean:
	rm -rf ./build/*

.PHONY: run
run:
	./build/main.exe