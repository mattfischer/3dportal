#ifndef JK_TEMPLATE_H
#define JK_TEMPLATE_H

#include "M_Vector.h"

#include <string>
#include <vector>

namespace World
{
    struct Frame;
}

using std::string;
using std::vector;

namespace Jk
{
    class Template
    {
	    struct TemplateParam {
		    string name;
		    string data;
	    };

    public:
	    Template( const string& n = "none", Template *p = NULL );
	    Template( Template &c );

	    ~Template();

        const string &GetName();

	    void CopyParams( Template *source );
	    void AddParam( const string& s );
    	
	    bool GetString( const string& paramName, string& s );
	    bool GetVector( const string& paramName, Math::Vector& vector);
	    bool GetFloat( const string& paramName, float& data);
	    bool GetInt( const string& paramName, int& data);
	    bool GetHex( const string& paramName, int& data);
        bool GetFrames( vector<World::Frame>& frames );

    protected:
	    string FindParam( const string& paramName );

        World::Frame ParseFrame( const string& data );
	    vector<TemplateParam> params;

        string name;
    };
}
#endif