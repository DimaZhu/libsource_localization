#include "profiler.h"

Profiler::Profiler()
{

}

void Profiler::start(QString name)
{
    begin = std::chrono::steady_clock::now();
    currentName = name;
}

void Profiler::stop(QString name)
{
    if (currentName != name)
        return;

    auto duration = std::chrono::duration_cast< std::chrono::milliseconds>
                              (std::chrono::steady_clock::now() - begin);

    stat[name] = duration.count();

}
