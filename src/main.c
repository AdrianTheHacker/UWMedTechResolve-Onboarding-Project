#include <stdio.h>
#include "./drivers/temperature_sensor/temperature_sensor.h"
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(NULL));
  double temperature_in_degrees_celsius = 0;

  for(int i = 0; i < 100; i++) {
    temperature_in_degrees_celsius = get_temperature_in_degrees_celsius();
    printf("Temperature Reading (Degrees C): %lf\n", temperature_in_degrees_celsius);
  }

  return 0;
}