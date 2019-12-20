#include "format.h"

#include <string>

using std::string;

// DONE
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int fmtHours = seconds / 3600;
  int fmtMinutes = (seconds % 3600) / 60;
  int fmtSeconds = seconds % 60;
  char buffer[8];
  sprintf(buffer, "%02d:%02d:%02d", fmtHours, fmtMinutes, fmtSeconds);
  return buffer;
};