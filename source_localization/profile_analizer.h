#ifndef PROFILE_ANALIZER_H
#define PROFILE_ANALIZER_H

#include <profile.h>

class Profile_Analizer
{
public:
    Profile_Analizer();
    void start();
    void stop();
    bool is_active();
    void add_profile(Profile);
    Profile get_statistic();
    void reset();

private:
    Profile profile;
    bool active;
};


#endif // PROFILE_ANALIZER_H
