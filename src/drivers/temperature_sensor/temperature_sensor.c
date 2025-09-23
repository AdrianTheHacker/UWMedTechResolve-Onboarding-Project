#include <stdio.h>
#include <stdlib.h>

#include "file_options.h"
#include "hardware_commands.h"

double get_temperature_in_degrees_celsius() {
  const double minimum_temperature_in_degrees_celsius = -50;
  const double maximum_temperature_in_degrees_celsius = 150;
  const double temperature_range_in_degrees_celsius = maximum_temperature_in_degrees_celsius - minimum_temperature_in_degrees_celsius;

  double temperature_in_degrees_celsius = (double)rand() / RAND_MAX; printf("Temperature Reading (Degrees C): %lf\n", temperature_in_degrees_celsius);
  temperature_in_degrees_celsius = temperature_in_degrees_celsius * temperature_range_in_degrees_celsius; printf("Temperature Reading (Degrees C): %lf\n", temperature_in_degrees_celsius);
  temperature_in_degrees_celsius = temperature_in_degrees_celsius + minimum_temperature_in_degrees_celsius; printf("Temperature Reading (Degrees C): %lf\n", temperature_in_degrees_celsius);

  return temperature_in_degrees_celsius;
}

int temperature_sensor_ioctl(int command, void *args) {
  temperature_sensor_args_t *temperature_sensor_args = (temperature_sensor_args_t *)args;

  switch (command) {
    case TEMPERATURE_SENSOR_GET_TEMPERATURE_IN_DEGREES_CELSIUS:
      temperature_sensor_args->temperature_in_degrees_celsius = get_temperature_in_degrees_celsius();
      return 0;

    default:
      -1;
  }
}

file_options_t temperature_sensor_file_options = {
  .open = NULL,
  .close = NULL,
  .read = NULL,
  .write = NULL,
  .ioctl = temperature_sensor_ioctl
};
