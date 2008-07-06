#include "JK_Template.h"

#include "M_Vector.h"
#include "W_Thing.h"


JK_Template::JK_Template( const string& n, JK_Template *p )
{
	int i;

	name = n;
	
	if( p != NULL )
	{
		for( i = 0 ; i < p->params.size() ; i++ )
			params.push_back( p->params[i] );
	}
}

JK_Template::JK_Template( JK_Template &c )
{
	TemplateParam *cursor, *toDelete, *newParam, *tail;

	name = c.name;
	params = c.params;
}

JK_Template::~JK_Template()
{
}

/*
void JK_Template::SetName(char *string)
{
	name=new char[strlen(string)+1];
	strcpy(name, string);
}

char *JK_Template::GetName()
{
	return name;
}
*/

void JK_Template::CopyParams(JK_Template *source)
{
	int i, j;
	bool found;

	for( i = 0 ; i < source->params.size() ; i++ )
	{
		found = false;
		for( j = 0 ; j < params.size() ; j++ )
		{
			if( params[j].name == source->params[i].name )
			{
				found = true;
				params[j].data = source->params[i].data;
				break;
			}
		}

		if( !found )
		{
			params.push_back( source->params[i] );
		}
	}
}

void JK_Template::AddParam( const string& s)
{
	int i;
	TemplateParam newParam;
	bool found;

	for( i = 0 ; i < s.length() ; i++ )
		if( s[i] == '=' ) break;
	
	newParam.name=s.substr( 0, i );
	newParam.data=s.substr( i + 1 );
	
	found = false;
	for( i = 0 ; i < params.size() ; i++ )
	{
		if( params[i].name == newParam.name & params[i].name != "frame" )
		{
			found = true;
			params[i] = newParam;
			break;
		}
	}

	if( !found )
	{
		params.push_back( newParam );
	}
}

string JK_Template::FindParam( const string& paramName)
{
	int i;
	for( i = 0 ; i < params.size() ; i++ )
	{
		if( params[i].name == paramName )
			return params[i].data;
	}

	return "";
}

bool JK_Template::GetString(const string& paramName, string& s )
{
	string result;

	result = FindParam(paramName);
	if( result == "" ) return false;
	
	s = result;
	return true;
	
}

bool JK_Template::GetVector( const string& paramName, M_Vector& vector )
{
	string result;
	float pieces[3];
	string newString;
	M_Vector newVector;
	int i, j, n;

	result = FindParam(paramName);
	if( result == "" ) return false;
		
	i=1;
	for( n = 0 ; n < 3 ; n++ )
	{
		for( j = i ; result[j] != '/' && result[j] != ')' ; j++ ) ;
		
		newString = result.substr( i, j - i );
		pieces[n] = atof( newString.c_str() );
		i = j + 1;
	}

	vector.x = pieces[0];
	vector.y = pieces[1];
	vector.z = pieces[2];

	return true;
}

bool JK_Template::GetFloat( const string& paramName, float& data)
{
	string result;

	result = FindParam( paramName );
	if( result == "" ) return false;
	
	data = atof( result.c_str() );
	return true;
	
}

bool JK_Template::GetInt( const string& paramName, int& data)
{
	string result;

	result = FindParam( paramName );
	if( result == "" ) return false;
	
	data = atoi( result.c_str() );
	return true;
	
}

bool JK_Template::GetHex( const string& paramName, int& data)
{
	string result;
	int placeVal;
	int i;

	result = FindParam( paramName );
	if( result == "" ) return false;
	
	data = 0;
	
	placeVal = 1;
	for( i = result.length() - 1 ; i >= 2 ; i-- )
	{
		if( result[i] >= '0' && result[i] <= '9' )
			data += placeVal * ( result[i] - '0' );
		else if( result[i] >= 'A' && result[i] <= 'F' )
			data += placeVal * ( result[i] - 'A' + 10 );
		else if( result[i] >= 'a' && result[i] <= 'f' )
			data += placeVal * ( result[i] - 'a' + 10 );

		placeVal *= 16;
	}
	return true;
}

bool JK_Template::GetFrames( vector<W_Frame>& frames )
{
	int i;
	W_Frame frame;

	frames.clear();
	for( i = 0 ; i < params.size() ; i++ )
	{
		if( params[i].name == "frame" )
		{
			frame = ParseFrame( params[i].data );
			frames.push_back( frame );
		}
	}
	
	return true;
}

W_Frame JK_Template::ParseFrame( const string& data )
{
	float pieces[6];
	string newString;
	W_Frame newFrame;
	int i, j, n;

	for( i = 0 ; i < 6 ; i++ ) pieces[i] = 0;

	i = 1;
	for( n = 0 ; n < 6 ; n++ )
	{
		for( j = i ; data[j] != '/' && data[j] != ')' && data[j] != ':' ; j++ ) ;
		
		newString = data.substr( i, j-i );
		pieces[n] = atof( newString.c_str() );
		i = j + 1;
		if( data[j] == ')' ) break;
	}

	newFrame.position.x = pieces[0];
	newFrame.position.y = pieces[1];
	newFrame.position.z = pieces[2];

	newFrame.rotation.x = pieces[3];
	newFrame.rotation.y = pieces[4];
	newFrame.rotation.z = pieces[5];

	return newFrame;
}
