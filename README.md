# UW Med-Tech Resolve Onboarding Project
Written by Adrian Tarantino<br>
GitHub: [@AdrianTheHacker](https://github.com/AdrianTheHacker)

## Running the Project with Pre-Compiled Build
The pre-compiled project build can be found inside of `./build/`. To run the pre-compiled build follow the steps below:
1. Open your terminal from the project's root.
2. Type ```./build/main.exe``` to run the project.

## Fresh Compilation with Makefile
To compile the program using `make` follow the steps listed below:

<b>Prerequisite</b>: Ensure `make` is installed and added to path ([Stack OverFlow - How can I install and use "make" in Windows?](https://stackoverflow.com/questions/32127524/how-can-i-install-and-use-make-in-windows)).
1. Open you terminal from the project's root of chose and type ```make clean``` to delete all previous build files.

2. Type ```make build``` to build the project, and all it's dependencies.
3. Type ```make run``` to run the project's executables.

## Instructions for Development
### Creating a Driver
1. Navigate to `./src/drivers/` and create a directory named after the driver you wish to make (Refer to `./drivers/temperature_sensor` for an example).

2. Inside of the newly created directory, create a `.c` and `.h` named after the driver you wish to make (Refer to `./drivers/temperature_sensor` for an example).
3. Return to the root of the project directory and open `./Makefile`.
4. change the `drivers_names` variable to include your driver's name.
