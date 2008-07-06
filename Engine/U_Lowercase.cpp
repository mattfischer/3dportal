#include "U_Lowercase.h"

#include <algorithm>
#include <ctype.h>

string U_Lowercase( string input )
{
	std::transform( input.begin(), input.end(), input.begin(), tolower );
	return input;
}