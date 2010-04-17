#ifndef JK_PUP_H
#define JK_PUP_H

#include <map>
#include <string>
#include <vector>

namespace Jk
{
    struct AnimClass
    {
        AnimClass(const std::string &filename);

        struct Submode
        {
            std::string key;
            int flags;
            int priLow;
            int priHigh;
        };

        struct Mode
        {
            int mode;
            std::map<std::string, Submode> submodes;
        };

        std::vector<Mode> modes;
    };
}

#endif