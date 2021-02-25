#if !defined(__DRINKING_H__)
#define __DRINKING_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <string>
#include <algorithm>

using std::string;
using std::thread;
using std::cout;
using std::endl;
using std::vector;
using std::this_thread::sleep_for;
using std::rand;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::mutex;
using std::lock_guard;

#endif // #if !defined(__DRINKING_H__)