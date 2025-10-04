credits:
	@echo "Temperature Sensor Simulation - UW Med-Tech Resolve Onboarding Project"
	@echo "Programmed by Adrian Tarantino"
	@echo "GitHub: AdrianTheHacker"

utilities := $(foreach f, $(wildcard ./src/utilities/*/), $(shell basename $f))
client_drivers := $(foreach f, $(wildcard ./src/client/drivers/*/), $(shell basename $f))
server_drivers := $(foreach f, $(wildcard ./src/server/drivers/*/), $(shell basename $f))

.PHONY: build
.PHONY: build-client
.PHONY: build-server
.PHONY: run-client
.PHONY: run-server
.PHONY: clean

build: build-client build-server

build-client:
	@echo "Building Client Code"
	@echo $(utilities)
	@echo $(client_drivers)

	mkdir -p ./build/client/

	$(foreach f, $(utilities), mkdir --parent ./build/client/utilities/$f)
	$(foreach f, $(utilities), touch ./build/client/utilities/$f/$f.o)
	$(foreach f, $(utilities), cc -c ./src/utilities/$f/$f.c -o ./build/client/utilities/$f/$f.o)

	$(foreach f, $(client_drivers), mkdir --parent ./build/client/drivers/$f)
	$(foreach f, $(client_drivers), touch ./build/client/drivers/$f/$f.o)
	$(foreach f, $(client_drivers), cc -c ./src/client/drivers/$f/$f.c -o ./build/client/drivers/$f/$f.o)

	cc -c ./src/client/client.c -o ./build/client/client.o
	cc ./build/client/client.o \
	$(foreach f, $(client_drivers), ./build/client/drivers/$f/$f.o) \
	$(foreach f, $(utilities), ./build/client/utilities/$f/$f.o) \
	$(foreach f, $(client_drivers),  -I ./src/client/drivers/$f/) \
	$(foreach f, $(utilities), -I ./src/client/utilities/$f/) \
	-o ./build/client/client.exe -pthread

run-client:
	./build/client/client.exe

build-server:
	@echo "Building Server Code"
	@echo $(utilities)
	@echo $(server_drivers)

	mkdir -p ./build/server/

	$(foreach f, $(utilities), mkdir --parent ./build/server/utilities/$f)
	$(foreach f, $(utilities), touch ./build/server/utilities/$f/$f.o)
	$(foreach f, $(utilities), cc -c ./src/utilities/$f/$f.c -o ./build/server/utilities/$f/$f.o)

	$(foreach f, $(server_drivers), mkdir --parent ./build/server/drivers/$f)
	$(foreach f, $(server_drivers), touch ./build/server/drivers/$f/$f.o)
	$(foreach f, $(server_drivers), cc -c ./src/server/drivers/$f/$f.c -o ./build/server/drivers/$f/$f.o)

	cc -c ./src/server/server.c -o ./build/server/server.o
	cc ./build/server/server.o \
	$(foreach f, $(server_drivers), ./build/server/drivers/$f/$f.o) \
	$(foreach f, $(utilities), ./build/server/utilities/$f/$f.o) \
	$(foreach f, $(server_drivers), -I ./src/server/drivers/$f/) \
	$(foreach f, $(utilities), -I ./src/server/utilities/$f/) \
	-o ./build/server/server.exe -pthread

run-server:
	./build/server/server.exe

clean:
	rm -rf ./build/*
	rm -rf ./build/.[!.]*
	rm -rf ./build/..?*
