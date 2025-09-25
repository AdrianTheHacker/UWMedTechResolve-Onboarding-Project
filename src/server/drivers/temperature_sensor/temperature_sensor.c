#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SEED srand(time(NULL));

double get_temperature_in_degrees_celsius() {
  const double minimum_temperature_in_degrees_celsius = -50;
  const double maximum_temperature_in_degrees_celsius = 150;
  const double temperature_range_in_degrees_celsius = maximum_temperature_in_degrees_celsius - minimum_temperature_in_degrees_celsius;

  double temperature_in_degrees_celsius = (double)rand() / RAND_MAX;
  temperature_in_degrees_celsius = temperature_in_degrees_celsius * temperature_range_in_degrees_celsius;
  temperature_in_degrees_celsius = temperature_in_degrees_celsius + minimum_temperature_in_degrees_celsius;

  return temperature_in_degrees_celsius;
}
