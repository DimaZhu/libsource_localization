#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <vector>



class Profile_entity
{
public:
    Profile_entity();

    QString name;
    int time_total;
    int time_aver;
    int count;
    float time_percentage;
};

typedef std::vector<Profile_entity> Profile;

#endif // PROFILE_H
