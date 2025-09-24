credits:
	@echo "Temperature Sensor Simulation - UW Med-Tech Resolve Onboarding Project"
	@echo "Programmed by Adrian Tarantino"
	@echo "GitHub: AdrianTheHacker"

source_path :=./src
build_path :=./build

# Start Server
server_path :=$(source_path)/server
server_drivers_path :=$(server_path)/drivers
server_driver_names :=temperature_sensor

server_build_path :=$(build_path)/server
server_drivers_build_path :=$(server_build_path)/drivers

.server_drivers_build_setup:
	mkdir -p $(server_drivers_build_path)

$(server_driver_names): .server_drivers_build_setup
	@echo 'Building object file for: $@'
	mkdir -p $(server_drivers_build_path)/$@
	touch $(server_drivers_build_path)/$@/$@.o
	cc -c $(server_drivers_path)/$@/$@.c -o $(server_drivers_build_path)/$@/$@.o

.PHONY: server-build
server-build: $(server_driver_names)
	echo "Hello, world"
	cc -c $(server_path)/server.c -o $(server_build_path)/server.o
	cc $(server_build_path)/server.o $(foreach f, $^, $(server_drivers_build_path)/$f/$f.o) $(foreach f, $^, -I $(server_drivers_path)/$f/) -o $(server_build_path)/server.exe -pthread

.PHONY: server-run
server-run:
	$(server_build_path)/server.exe

.PHONY: server
server-clean:
	rm -rf $(server_build_path)/*

# End Server

# Start Client
client_path :=$(source_path)/client
client_drivers_path :=$(client_path)/drivers
client_driver_names :=

client_build_path :=$(build_path)/client
client_drivers_build_path :=$(client_build_path)/drivers

.client_drivers_build_setup:
	mkdir -p $(client_drivers_build_path)

$(client_driver_names): .client_drivers_build_setup
	@echo 'Building object file for: $@'
	mkdir -p $(client_drivers_build_path)/$@
	touch $(client_drivers_build_path)/$@/$@.o
	cc -c $(client_drivers_path)/$@/$@.c -o $(client_drivers_build_path)/$@/$@.o

.PHONY: client-build
client-build: $(client_driver_names)
	echo "Hello, world"
	cc -c $(client_path)/client.c -o $(client_build_path)/client.o
	cc $(client_build_path)/client.o $(foreach f, $^, $(client_drivers_build_path)/$f/$f.o) $(foreach f, $^, -I $(client_drivers_path)/$f/) -o $(client_build_path)/client.exe -pthread

.PHONY: client-run
client-run:
	$(client_build_path)/client.exe

.PHONY: client-clean
clean-client:
	rm -rf $(client_build_path)/*

# End Client

.PHONY: all-clean
all-clean:
	rm -rf $(build_path)/*
	rm -rf $(build_path)/.[!.]*
	rm -rf $(build_path)/..?*
