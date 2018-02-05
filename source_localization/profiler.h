#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <profile.h>


using namespace std;
using ns = std::chrono::nanoseconds;
class Profiler
{
public:
    Profiler();
    void start(string name);
    void stop(string name);
    void clear();
    Profile get_profile();


private:
    Profile profile;
    std::chrono::time_point<std::chrono::steady_clock> begin;
    string currentName;
};

#endif // PROFILER_H
