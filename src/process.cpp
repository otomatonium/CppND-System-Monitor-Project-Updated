
#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int pid, const string user, const string command,
                 const float cpuUtilization, const string ram,
                 const long upTime)
    : pid_(pid),
      user_(user),
      command_(command),
      cpuUtilization_(cpuUtilization),
      ram_(ram),
      upTime_(upTime) {}

// DONE
// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// DONE
// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization_; }

// DONE
// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// DONE
// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// DONE
// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE
// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// DONE
// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return cpuUtilization_ < a.cpuUtilization_;
}