#ifndef PROFILER_H
#define PROFILER_H

#include <QMap>
#include <chrono>
#include <profile.h>


class Profiler
{
public:
    Profiler();
    void start(QString name);
    void stop(QString name);
    Profile get_profile();


private:
    Profile profile;
    std::chrono::time_point<std::chrono::steady_clock> begin;
    QString currentName;
};

#endif // PROFILER_H
