
#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>
//#include <algorithm>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// IN PROGRESS
// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid : pids) {
    Process process(pid, LinuxParser::User(pid), LinuxParser::Command(pid),
                    LinuxParser::CpuUtilization(pid), LinuxParser::UpTime(pid), LinuxParser::Ram(pid));
    processes_.emplace_back(process);
  }

  std::sort(processes_.begin(), processes_.end(),
            [](const Process& a, const Process& b) { return b < a; });

  return processes_;
}

// DONE
// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE
// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE
// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE
// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE
// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }