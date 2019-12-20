#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iomanip>
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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string lineLabel;
  long memTotal, memFree, buffers, cached, sReclaimable, shmem, swapTotal, swapFree;
  string val;
  string line;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> lineLabel;

      if (lineLabel == kLabelMemTotal) {
        linestream >> val;
        memTotal = stol(val);
      } else if (lineLabel == kLabelMemFree) {
        linestream >> val;
        memFree = stol(val);
      }
      /* Uncomment for alternative memory utilization
      as necessary */
      /*
      else if (lineLabel == kLabelBuffers) {
        linestream >> val;
        buffers = stol(val);
      } else if (lineLabel == kLabelCached) {
        linestream >> val;
        cached = stol(val);
      } else if (lineLabel == kLabelSReclaimable) {
        linestream >> val;
        sReclaimable = stol(val);
      } else if (lineLabel == kLabelShmem) {
        linestream >> val;
        shmem = stol(val);
      } else if (lineLabel == kLabelSwapTotal) {
        linestream >> val;
        swapTotal = stol(val);
      } else if (lineLabel == kLabelSwapFree) {
        linestream >> val;
        swapFree = stol(val);
      }
      */
    }

    // Calculate memory usage
     return (memTotal - memFree) / (float)memTotal;
  }

  return NULL;
}

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
vector<string> LinuxParser::CpuUtilization() {
  string item;
  string line, lineLabel;
  std::vector<string> stats{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> lineLabel;
      if (lineLabel == kLabelCpu) {
        while (std::getline(linestream, item, ' ')) {
          stats.emplace_back(item);
        }
        return stats;
      }
    }
  }
  return stats;
}

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
  return val;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string lineLabel;
  string val;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> lineLabel;

      if (lineLabel == kLabelRam) {
        linestream >> val;
        float mb = stof(val) / 1024.0;
        std::stringstream ramStream;
        ramStream << std::fixed << std::setprecision(2) << mb;
        return ramStream.str();
      }
    }
  }
  return val;
}

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

// DONE
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string item;
  std::vector<string> stats;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, item, ' ')) {
      stats.emplace_back(item);
    }
  } else {
    return NULL;
  }

  return stol(stats[21]) / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid) {
  string item;
  std::vector<string> stats;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, item, ' ')) {
      stats.emplace_back(item);
    }
  } else {
    return NULL;
  }

  float utime = stol(stats[13]);
  float stime = stol(stats[14]);
  float cutime = stol(stats[15]);
  float cstime = stol(stats[16]);
  float starttime = stol(stats[21]);

  // int hertz = getconf(CLK_TCK);
  float hertz = sysconf(_SC_CLK_TCK);

  float total_time = utime + stime;
  // total_time += cutime + cstime;

  float seconds = UpTime() - (starttime / hertz);
  if (seconds > 3600) seconds = 3600;

  return ((total_time / hertz) / seconds);
}
