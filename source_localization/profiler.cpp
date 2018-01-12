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

    auto end = std::chrono::steady_clock::now();
    auto diff = end - begin;


    bool new_entity = true;

    for (uint i = 0; i < profile.size(); ++i){
        if (profile[i].name == currentName) {
            new_entity = false;

            profile[i].time_total += chrono::duration_cast<ns>(diff).count();
            break;
        }
    }

    if (new_entity) {
        Profile_entity entity;
        entity.name = currentName;
        entity.time_total = chrono::duration_cast<ns>(diff).count();
        profile.push_back(entity);
    }

}

Profile Profiler::get_profile()
{
    return profile;
}


void Profiler::clear()
{
    profile.clear();
}
