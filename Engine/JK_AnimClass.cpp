#include "JK_AnimClass.h"

#include "JK_Parse.h"
#include "JK_GOB.h"

#include "U_Lowercase.h"

namespace Jk
{
    AnimClass::AnimClass(const std::string &filename)
    {
        string fullname;
        Jk::Parser::Line line;
	    string data;
	    bool error;
	    int i, j;
	    float f1, f2, f3;

	    fullname = "misc\\pup\\" + filename;
        data = Jk::Gob::getFile( fullname );
        Jk::Parser parser(data);

        line = parser.getLine( error );

        while(1)
        {
            int mode;
            int basedon = -1;

            while(1)
            {
                line = Jk::Parser::Line(U_Lowercase( line.line() ) );

                line.matchString( "mode=", error );
                if( !error )
                {
                    mode = line.getInt( error );

                    line.matchString( ", basedon=", error );
                    if( !error )
                    {
                        basedon = line.getInt( error );
                    }

                    error = false;
                    break;
                }

                line = parser.getLine( error );
                if( error ) break;
            }

            if( error ) break;

            Mode newMode;

            newMode.mode = mode;

            if( basedon != -1 )
            {
                for(int i = 0; i < modes.size(); i++)
                {
                    if( modes[i].mode == basedon )
                    {
                        newMode = modes[i];
                        break;
                    }
                }
            }

            while(1)
            {
                line = parser.getLine( error );
                if( error ) break;

                line = Jk::Parser::Line(U_Lowercase( line.line() ) );

                line.matchString( "mode=", error );
                if( !error )
                {
                    break;
                }

                line.matchString( "joints", error );
                if( !error )
                {
                    break;
                }
                Submode newSubmode;
                std::string submodeType;

                submodeType = line.getString( error );

                newSubmode.key = line.getString( error );
                newSubmode.flags = line.getHex( error );
                newSubmode.priLow = line.getInt( error );
                newSubmode.priHigh = line.getInt( error );

                newMode.submodes[submodeType] = newSubmode;
            }

            modes.push_back( newMode );

            if( error == 1) break;
        }
    }
}