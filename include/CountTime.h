// Copyright sxniu 2014-6-9

#ifndef INCLUDE_COUNTTIME_H_
#define INCLUDE_COUNTTIME_H_

#include <sys/time.h>

class CountTime {
 public:
  void ContBegin();
  void ContEnd();
  double ContResult();

 private:
  timeval time_start;
  timeval time_end;
};

#endif  // INCLUDE_COUNTTIME_H_
