#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return kErr;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, versionLabel, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> versionLabel >> kernel;
    return kernel;
  }

  return kErr;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// DONE
// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return uptime;
  }

  return NULL;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string lineLabel;
  int val;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> lineLabel;

      if (lineLabel == kLabelTotalProcesses) {
        linestream >> val;
        break;
      }
    }
  }
  return val;
}

// DONE
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string lineLabel;
  int val;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> lineLabel;

      if (lineLabel == kLabelRunningProcesses) {
        linestream >> val;
        break;
      }
    }
  }
  return val;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string val;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> val;
      return val;
    }
  }
  return kErr;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// DONE
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string lineLabel;
  string val;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> lineLabel;

      if (lineLabel == kLabelUid) {
        linestream >> val;
        return val;
      }
    }
  }
  return kErr;
}

// DONE
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  int uid = stoi(LinuxParser::Uid(pid));
  string lineLabel;
  string val;
  string line;

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      int unameIdxEnd = line.find(':', 0);
      int uidIdxStart = line.find(':', unameIdxEnd + 1);
      int uidIdxEnd = line.find(':', uidIdxStart + 1);
      int foundUid =
          stoi(line.substr(uidIdxStart + 1, uidIdxEnd - uidIdxStart));
      if (uid == foundUid) {
        return line.substr(0, unameIdxEnd);
      }
    }
  }
  return kErr;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

float LinuxParser::CpuUtilization(int pid) {
  string item;
  std::vector<string> stats;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, item, ' ')) {
      stats.emplace_back(item);
    }
  } else {
    return -1.0;
  }

  long int utime = stol(stats[13]);
  long int stime = stol(stats[14]);
  long int cutime = stol(stats[15]);
  long int cstime = stol(stats[16]);
  long int starttime = stol(stats[21]);

  // getconf_TCK_CLK;
  int hertz = sysconf(_SC_CLK_TCK);

  long int total_time = utime + stime;
  // total_time += cutime + cstime;
  long int recentUptime = UpTime() > 3600 ? 3600 : UpTime();
  long int seconds = recentUptime - (starttime / hertz);

  float cpu_usage = 100.0 * ((total_time / hertz) / (float)seconds);
  return cpu_usage;
}
