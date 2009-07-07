#ifndef JK_PUP_H
#define JK_PUP_H

#include <map>
#include <string>
#include <vector>

struct JK_AC_Submode
{
    std::string key;
    int flags;
    int priLow;
    int priHigh;
};

struct JK_AC_Mode
{
    int mode;
    std::map<std::string, JK_AC_Submode> submodes;
};

struct JK_AnimClass
{
    JK_AnimClass(std::string filename);

    std::vector<JK_AC_Mode> modes;
};

#endif