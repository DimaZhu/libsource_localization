#ifndef PROFILER_H
#define PROFILER_H

#include <QMap>
#include <chrono>

class Profiler
{
public:
    Profiler();
    void start(QString name);
    void stop(QString name);
    QMap<QString, int> get_statistic();


private:
    QMap<QString, int> stat;
    std::chrono::time_point<std::chrono::steady_clock> begin;
    QString currentName;
};

#endif // PROFILER_H
