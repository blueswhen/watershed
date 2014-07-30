// Copyright sxniu 2014-6-9

#include "include/CountTime.h"
#include <stdlib.h>
#include <stdio.h>

void CountTime::ContBegin() {
  gettimeofday(&time_start, NULL);
}

void CountTime::ContEnd() {
  gettimeofday(&time_end, NULL);
}

// unit is "ms"
double CountTime::ContResult() {
  double time = (time_end.tv_sec - time_start.tv_sec) +
                (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
  printf("the running time = %fms\n", time * 1000);
  return time;
}
