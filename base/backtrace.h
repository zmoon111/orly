/* <base/backtrace.h>

   Print a backtrace

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

#pragma once

#include <functional>
#include <string>

namespace Base {

  void PrintBacktrace(int max_frame_count);

  /* Generate a backtrace one line at a time, calling the callback once for each backtrace frame. */
  void GenBacktrace(int max_frame_count, const std::function<void (const std::string &)> &cb);

  void SetBacktraceOnTerminate();

} // Base
