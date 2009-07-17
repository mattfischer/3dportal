#include "U_Lowercase.h"

#include <algorithm>
#include <ctype.h>

namespace Util
{
    string Lowercase( string input )
    {
	    std::transform( input.begin(), input.end(), input.begin(), tolower );
	    return input;
    }
}