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


    bool new_entity = true;

    for (auto entity : profile){
        if (entity.name == currentName) {
            new_entity = false;
            entity.time_total += duration.count();
            break;
        }
    }

    if (new_entity) {
        Profile_entity entity;
        entity.name = currentName;
        entity.time_total = duration.count();
        profile.push_back(entity);
    }

}

Profile Profiler::get_profile()
{
    return profile;
}
