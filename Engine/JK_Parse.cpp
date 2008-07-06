#include <windows.h>

#include "JK_Parse.h"

string JKP_GetFileLine( const string& input, int& start, int size )
{
	int i;
	string newdata;
	i = start;
	while( input[i] != '\n' && input[i] != '\r' && i < size) i++;
	i++;
	newdata = input.substr( start, i - start - 1 );
	start = i + 1;
	if( start > size ) start = size;
	return newdata;
}

string JKP_RemoveComments( const string& input )
{
	int i = 0;
	string newdata;
	while( i < input.length() && input[i] != '#') i++;
	newdata = input.substr( 0, i );
	return newdata;
}

float JKP_GetFloat( const string& input, int& start, int& error )
{
	float retval;
	int i = start;
	int i2;
	string newdata;
	error = 0;
	if( i >= input.length() )
	{
		error = 1;
		return 0;
	}
	if( input[i] == ' ' || input[i] == ':' || input[i] == ',' || input[i] == '\t' ) 
	{
		while( ( input[i] == ' ' || input[i] == ':' || input[i] == ',' || input[i] == '\t' ) && input[i] != '\0' ) i++;
		if( i >= input.length() ) 
		{
			error = 1;
			return 0;
		}
	}
	i2 = i;
	while( i2 < input.length() && input[i2] != ' ' && input[i2] != ':' && input[i2] != ',' && input[i2] != '\t' ) i2++;
	newdata = input.substr( i, i2 - i );
	start=i2;

	retval = atof( newdata.c_str() );
	return retval;
}

int JKP_GetInt( const string& input, int& start, int& error )
{
	int retval;
	int i = start;
	int i2;
	string newdata;
	error = 0;
	if( i >= input.length() )
	{
		error = 1;
		return 0;
	}
	if( input[i] == ' ' || input[i] == ':' || input[i] == ',' || input[i] == '\t' ) 
	{
		while( i < input.length() && ( input[i] == ' ' || input[i] == ':' || input[i] == ',' || input[i] == '\t') ) i++;
		if( i >= input.length() ) 
		{
			error = 1;
			return 0;
		}
	}
	i2 = i;
	while( i2 < input.length() && input[i2] != ' ' && input[i2] != ':' && input[i2] != ',' && input[i2] != '\t' ) i2++;
	newdata = input.substr( i, i2 - i );
	start = i2;

	retval = atoi( newdata.c_str() );
	return retval;
}

string JKP_GetNonEmptyLine( const string& input, int& start, int size, int &error )
{
	string newdata;
	bool empty;
	int i;

	error = 0;
	while( 1 )
	{
	    if( start == size )
		{
			error = 1;
			return "";
		}
		newdata = JKP_GetFileLine( input, start, size );
		newdata = JKP_RemoveComments( newdata );
		
		empty = true;
		i = 0;
		while( i < newdata.length() )
		{
			if( newdata[i] != ' ' && newdata[i] != '\t')
			{
				empty = false;
				break;
			}
			i++;
		}

		if( !empty ) break;
	}

	return newdata;
}

string JKP_GetString( const string& input, int& start, int& error )
{
	int i = start;
	int i2;
	string newdata;
	error = 0;
	if( i >= input.length() )
	{
		error = 1;
		return "";
	}
	if( input[i] == ' ' || input[i] == '\t' || input[i] == ':' ) 
	{
		while( i < input.length() && (input[i] == ' ' || input[i] == '\t' || input[i] == ':' ) ) i++;
		if( i == input.length() ) 
		{
			error = 1;
			return "";
		}
	}
	i2 = i;
	while( i2 < input.length() && input[i2] != ' ' && input[i2] != '\t')// && input[i2]!=':') 
		i2++;
	newdata = input.substr( i, i2 - i );
	start = i2;
	return newdata;
}

void JKP_FindString( const string& input, int& start, int size, const string& match, int& error )
{
	string line;

	while( 1 )
	{
		line = JKP_GetNonEmptyLine( input, start, size, error );
		if( error ) break;

		if( line == match) break;
	}
}

void JKP_MatchString( const string& input, int& start, const string& match, int& error)
{
	if( input.substr( start, match.length() ) == match ) start += match.length();
	else error = 1;
}

int JKP_GetHex( const string& input, int& start, int& error)
{
	string hexString;
	int retVal;
	int placeVal;
	int i;

	
	hexString = JKP_GetString( input, start, error );
	if( error ) return 0;

	retVal = 0;
	placeVal = 1;
	for( i = hexString.length() - 1 ; i >= 2 ; i-- )
	{
		if( hexString[i] >= '0' && hexString[i] <= '9' )
			retVal += placeVal * ( hexString[i] - '0' );
		else if( hexString[i] >= 'A' && hexString[i] <= 'F' )
			retVal += placeVal * ( hexString[i] - 'A' + 10 );
		else if( hexString[i] >= 'a' && hexString[i] <= 'f' )
			retVal += placeVal * ( hexString[i] - 'a' + 10 );

		placeVal *= 16;
	}
	return retVal;
}

