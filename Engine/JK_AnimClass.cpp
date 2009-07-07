#include "JK_AnimClass.h"

#include "JK_Parse.h"
#include "JK_GOB.h"

#include "U_Lowercase.h"

JK_AnimClass::JK_AnimClass(std::string filename)
{
    string fullname;
	string line;
	string data;
	char *dataPointer;
	int size;
	int error;
	int pos, pos2;
	int i, j;
	float f1, f2, f3;

	fullname = "misc\\pup\\" + filename;
	JK_GOB_GetFile( fullname, &dataPointer, &size );
	data = dataPointer;
	delete[] dataPointer;

    pos = 0;

    line = JKP_GetNonEmptyLine( data, pos, size, error );

    while(1)
    {
        int mode;
        int basedon = -1;

        while(1)
        {
            line = U_Lowercase( line );

            pos2 = 0;
            JKP_MatchString( line, pos2, "mode=", error );
            if( error == 0 )
            {
                mode = JKP_GetInt( line, pos2, error );

                JKP_MatchString( line, pos2, ", basedon=", error );
                if( error == 0 )
                {
                    basedon = JKP_GetInt( line, pos2, error );
                }

                error = 0;
                break;
            }

            line = JKP_GetNonEmptyLine( data, pos, size, error );
            if( error == 1 ) break;
        }

        if( error == 1 ) break;

        JK_AC_Mode newMode;

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
            line = JKP_GetNonEmptyLine( data, pos, size, error );
            if( error == 1) break;

            line = U_Lowercase( line );

            pos2 = 0;
            JKP_MatchString( line, pos2, "mode=", error );
            if( error == 0 )
            {
                break;
            }

            JKP_MatchString( line, pos2, "joints", error );
            if( error == 0 )
            {
                break;
            }
            JK_AC_Submode newSubmode;
            std::string submodeType;

            pos2 = 0;
            submodeType = JKP_GetString( line, pos2, error );

            newSubmode.key = JKP_GetString( line, pos2, error );
            newSubmode.flags = JKP_GetHex( line, pos2, error );
            newSubmode.priLow = JKP_GetInt( line, pos2, error );
            newSubmode.priHigh = JKP_GetInt( line, pos2, error );

            newMode.submodes[submodeType] = newSubmode;
        }

        modes.push_back( newMode );

        if( error == 1) break;
    }
}