#include "profile_analizer.h"

Profile_Analizer::Profile_Analizer():
    active(false)
{


}

bool Profile_Analizer::is_active()
{
    return active;
}


void Profile_Analizer::start()
{
    active = true;
    profile.clear();
}

void Profile_Analizer::stop()
{
    active = false;
}


void Profile_Analizer::add_profile(Profile new_profile)
{
    for (auto i_entity:new_profile)
    {
        bool new_entity = true;

        for (uint i = 0; i < profile.size(); ++i){
            if (profile[i].name == i_entity.name) {
                new_entity = false;
                profile[i].time_total += i_entity.time_total;
                ++profile[i].count;
                break;
            }
        }

        if (new_entity) {
            Profile_entity entity;
            entity.name = i_entity.name;
            entity.time_total = i_entity.time_total;
            entity.count = 1;
            profile.push_back(entity);
        }
    }
}

Profile Profile_Analizer::get_statistic()
{
    int time_per_loop = 0;
    for (uint i = 0; i < profile.size(); ++i)
    {
        profile[i].time_aver = profile[i].time_total / profile[i].count;
        time_per_loop += profile[i].time_aver;
    }

    for (uint i = 0; i < profile.size(); ++i)
    {
        profile[i].time_percentage = (float)profile[i].time_aver / (float) time_per_loop * 100;
    }
    return profile;
}
