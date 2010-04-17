#ifndef JK_GOB_H
#define JK_GOB_H

#include <string>

using std::string;

namespace Jk
{
    class Gob
    {
    public:
        static void init();
        static void close();
        static int getFile(const string& filename, char **data, int *size);
        static string getFile(const string& filename);
    };
}

#endif
