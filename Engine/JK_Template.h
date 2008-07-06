#ifndef JK_TEMPLATE_H
#define JK_TEMPLATE_H

#include "Global.h"

#include "U_Collection.h"

#include <string>
#include <vector>

class M_Vector;
struct W_Frame;

using std::string;
using std::vector;

class JK_Template : public U_NameableItem
{
	struct TemplateParam {
		string name;
		string data;
	};

public:
	JK_Template( const string& n = "none", JK_Template *p = NULL );
	JK_Template( JK_Template &c );

	~JK_Template();

	void CopyParams( JK_Template *source );
	void AddParam( const string& s );
	
	bool GetString( const string& paramName, string& s );
	bool GetVector( const string& paramName, M_Vector& vector);
	bool GetFloat( const string& paramName, float& data);
	bool GetInt( const string& paramName, int& data);
	bool GetHex( const string& paramName, int& data);
	bool GetFrames( vector<W_Frame>& frames );

protected:
	string FindParam( const string& paramName );

	W_Frame ParseFrame( const string& data );
	vector<TemplateParam> params;
};

#endif