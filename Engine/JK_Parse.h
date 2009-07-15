#ifndef JK_PARSE_H
#define JK_PARSE_H

#include "Global.h"

#include "M_Vector.h"

#include <string>

using std::string;

namespace Jk
{
    class Parser
    {
    public:
        class Line
        {
        public:
            Line(const string &input);
            Line();

            int getInt(bool &error);
            float getFloat(bool &error);
            string getString(bool &error);
            int getHex(bool &error);

            void matchString(const string &match, bool &error);

            const string &line();
            string rest();

        private:
            string data;
            int p;
        };

        Parser(const string &input);

        Line getLine(bool &error);
        void findString(const string &match, bool &error);

        void reset();

    private:
        string data;
        int p;
    };
}
#endif