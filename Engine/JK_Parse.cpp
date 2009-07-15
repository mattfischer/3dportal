#include <windows.h>

#include "JK_Parse.h"

namespace Jk
{
    Parser::Parser(const string &input)
    {
        data = input;
        p = 0;
    }

    static string getRawLine( const string& input, int& start )
    {
	    int i;
	    string newdata;
	    i = start;
	    while( input[i] != '\n' && input[i] != '\r' && i < input.length()) i++;
	    i++;
	    newdata = input.substr( start, i - start - 1 );
	    start = i + 1;
	    if( start > input.size() ) start = input.length();
	    return newdata;
    }

    static string removeComments( const string& input )
    {
	    int i = 0;
	    string newdata;
	    while( i < input.length() && input[i] != '#') i++;
	    newdata = input.substr( 0, i );
	    return newdata;
    }

    Parser::Line Parser::getLine(bool &error)
    {
        string newdata;
	    bool empty;
	    int i;

	    error = false;
	    while( 1 )
	    {
	        if( p == data.length() )
		    {
			    error = true;
                return Line("");
		    }
		    newdata = getRawLine( data, p );
		    newdata = removeComments( newdata );
    		
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

	    return Line(newdata);
    }

    void Parser::findString( const string& match, bool& error )
    {
        error = false;
	    while( 1 )
	    {
		    Line line = getLine( error );
		    if( error ) break;

		    if( line.line() == match) break;
	    }
    }

    void Parser::reset()
    {
        p = 0;
    }

    Parser::Line::Line(const string &input)
    {
        data = input;
        p = 0;
    }

    Parser::Line::Line()
    {
        p = 0;
    }

    int Parser::Line::getInt(bool &error)
    {
        int retval;
        int i = p;
        int i2;
        string newdata;

        error = false;
        if( i >= data.length() )
        {
	        error = true;
	        return 0;
        }

        if( data[i] == ' ' || data[i] == ':' || data[i] == ',' || data[i] == '\t' ) 
        {
	        while( i < data.length() && ( data[i] == ' ' || data[i] == ':' || data[i] == ',' || data[i] == '\t') ) i++;
	        if( i >= data.length() ) 
	        {
		        error = true;
		        return 0;
	        }
        }

        i2 = i;
        while( i2 < data.length() && data[i2] != ' ' && data[i2] != ':' && data[i2] != ',' && data[i2] != '\t' ) i2++;

        newdata = data.substr( i, i2 - i );
        p = i2;

        retval = atoi( newdata.c_str() );
        return retval;
    }

    float Parser::Line::getFloat(bool &error)
    {
	    float retval;
	    int i = p;
	    int i2;
	    string newdata;

	    error = false;
	    if( i >= data.length() )
	    {
		    error = true;
		    return 0;
	    }

	    if( data[i] == ' ' || data[i] == ':' || data[i] == ',' || data[i] == '\t' ) 
	    {
		    while( ( data[i] == ' ' || data[i] == ':' || data[i] == ',' || data[i] == '\t' ) && data[i] != '\0' ) i++;
		    if( i >= data.length() ) 
		    {
			    error = true;
			    return 0;
		    }
	    }

	    i2 = i;
	    while( i2 < data.length() && data[i2] != ' ' && data[i2] != ':' && data[i2] != ',' && data[i2] != '\t' ) i2++;

	    newdata = data.substr( i, i2 - i );
	    p = i2;

	    retval = atof( newdata.c_str() );
	    return retval;
    }

    string Parser::Line::getString(bool &error)
    {
	    int i = p;
	    int i2;
	    string newdata;

	    error = false;
	    if( i >= data.length() )
	    {
		    error = true;
		    return "";
	    }

	    if( data[i] == ' ' || data[i] == '\t' || data[i] == ':' ) 
	    {
		    while( i < data.length() && (data[i] == ' ' || data[i] == '\t' || data[i] == ':' ) ) i++;
		    if( i == data.length() ) 
		    {
			    error = true;
			    return "";
		    }
	    }

	    i2 = i;
	    while( i2 < data.length() && data[i2] != ' ' && data[i2] != '\t')// && data[i2]!=':') 
		    i2++;

	    newdata = data.substr( i, i2 - i );
	    p = i2;

	    return newdata;
    }

    int Parser::Line::getHex(bool &error)
    {
	    string hexString;
	    int retVal;
	    int placeVal;
	    int i;

        error = false;
	    hexString = getString( error );
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

    void Parser::Line::matchString(const string &match, bool &error)
    {
        error = false;

	    if( data.substr( p, match.length() ) == match ) p += match.length();
	    else error = true;
    }

    const string &Parser::Line::line()
    {
        return data;
    }

    string Parser::Line::rest()
    {
        return data.substr(p);
    }
}