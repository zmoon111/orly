/* <orly/indy/disk/util/device_util.cc>

   Copyright 2010-2014 OrlyAtomics, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#include <orly/indy/disk/util/device_util.h>

#include <fstream>
#include <istream>
#include <stdexcept>

#include <base/thrower.h>

using namespace Orly::Indy::Disk::Util;
using namespace std;

bool TDeviceUtil::ForEachDevice(const function<bool(const char *)> &cb) {
  ifstream partitions("/proc/partitions");
  if(!partitions.is_open()) {
    THROW << "Unable to open /proc/partitions";
  }

  std::string line;

  //Make sure the formatting is what we expect (first line)
  getline(partitions, line);
  if (line != "major minor  #blocks  name") {
    THROW << "Unkwnown format of /proc/partitions";
  }

  // Skip empty line
  getline(partitions, line);

  //Read each entry, extract the last field 'name', and send it to the callback
  while(getline(partitions, line)) {
    if(!cb(line.substr(line.rfind(' ') + 1).c_str())) {
      return false;
    }
  }

  return true;
}

bool TDeviceUtil::IsHardDrive(const std::string &dev_path) {
  return dev_path.compare(0, 2, "sd") == 0;
}

std::string TDeviceUtil::GetPathToPartitionInfo(const std::string &dev_path) {
  if(IsHardDrive(dev_path)) {
    return GetPathToDeviceInfo(dev_path) + dev_path + '/';
  }
  return GetPathToDeviceInfo(dev_path);
}

std::string TDeviceUtil::GetPathToDeviceInfo(const std::string &dev_path) {
  std::string path_to_partition_info = "/sys/block/";
  if (IsHardDrive(dev_path)) {
    auto digit_start = dev_path.find_last_not_of("0123456789");
    if (digit_start == std::string::npos) {
      path_to_partition_info += dev_path;
    } else {
      path_to_partition_info += dev_path.substr(0, digit_start + 1);
    }
  } else {
    path_to_partition_info += dev_path;
  }
  // NOTE: We add the '/'  on a seperate line from the return so that we can take advantage of NRVO
  path_to_partition_info += '/';

  return path_to_partition_info;
}