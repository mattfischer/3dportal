#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct FunctionInfo
{
	string returnType;
	string name;
	vector<string> parameterTypes;
	bool needsContext;
};

void LoadFunctionDefs( const string& filename, vector<FunctionInfo>& functions )
{
	ifstream inputFile( filename.c_str() );
	string line;
	FunctionInfo newFunction;
	string tempString;
	int index;
	int i;

	while( !inputFile.fail() && !inputFile.eof() )
	{
		newFunction.parameterTypes.clear();

		getline( inputFile, line );
		if( line == "" ) continue;
        printf("Processing line %s\n", line.c_str());
		while( line[0] == ' ' ) line.erase( 0, 1 );

		if( line.length() < 2 ) continue;
		if( line[0] == '#' ) continue;
		if( line[0] == '/' && ( line[1] == '*' || line[1] == '/' ) ) continue;
        if( line.find("namespace") != string::npos ) continue;

		index = line.find( " " );
		newFunction.returnType = line.substr( 0, index );
		line.erase( 0, index );

		while( line[0] == ' ' ) line.erase( 0, 1 );
		index = line.find( "(" );
		tempString = line.substr( 0, index );
		while( tempString[tempString.size()-1] == ' ' ) tempString.erase( tempString.size() - 1, 1 );
		newFunction.name = tempString;

		line.erase( 0, index );
		
		while( line[0] != ')' )
		{
			while( line[0] == '(' || line[0] == ' ' || line[0] == ',' ) 
				line.erase( 0, 1 );

			if( line[0] == ')' ) 
				break;

			index = line.find( " " );
			tempString = line.substr( 0, index );
			newFunction.parameterTypes.push_back( tempString );
			
			index = line.find_first_of( ",)" );
			line.erase( 0, index );
		}

		printf("%s %s ( ", newFunction.returnType.c_str(), newFunction.name.c_str() );
		for( i=0; i<newFunction.parameterTypes.size(); i++ )
			printf( "%s ", newFunction.parameterTypes[i].c_str() );
		printf( ") \n" );

		functions.push_back( newFunction );
	}
}

string TypeToSymbolType( string type )
{
	transform( type.begin(), type.end(), type.begin(), toupper );

	return type;
}

void OutputMarshallers( const std::string& filename, vector<FunctionInfo>& functions )
{
	int i;
	int j;
	ofstream outputFile( filename.c_str() );
	string paramType;
	string returnType;
	string functionName;

	for( i=0; i<functions.size(); i++ )
	{
		returnType = functions[i].returnType;

		functionName = functions[i].name;
		transform( functionName.begin(), functionName.end(), functionName.begin(), tolower );

		outputFile << "if( !strcmp( node->lexData.stringVal, \"" << functionName << "\" ) )" << endl;
		outputFile << "{" << endl;
		for( j=0; j<functions[i].parameterTypes.size(); j++ )
		{
			paramType = functions[i].parameterTypes[j];
			
			if( paramType[paramType.size()-1] == '&' )
				paramType.erase( paramType.size()-1, 1 );
			if( paramType[paramType.size()-1] == '*' )
				paramType.erase( paramType.size()-1, 1 );

			if( paramType == "Context" )
			{
				outputFile << "    Context& param" << j << " = c;" << endl;
				continue;
			}
			
			if( paramType == "Script" )
			{
				outputFile << "    Script* param" << j << " = this;" << endl;
				continue;
			}

			outputFile << "    " << paramType << " param" << j << " = ";
			if( paramType == "Type_AI" ||
				paramType == "Type_Cog" ||
				paramType == "Type_Thing" || 
				paramType == "Type_Sector" ||
				paramType == "Type_Surface" ||
				paramType == "Type_Int" ||
				paramType == "Type_Keyframe" ||
				paramType == "Type_Material" ||
				paramType == "Type_Message" ||
				paramType == "Type_Model" ||
				paramType == "Type_Sound" ||
				paramType == "Type_Template" )
			{
				outputFile << "c.stack.PopInt();" << endl;
			}
				
			if(	paramType == "Type_Vector" )
			{
				outputFile << "c.stack.PopVector();" << endl;
			}

			if( paramType == "Type_Float" )
			{
				outputFile << "c.stack.PopFloat();" << endl;
			}

			if( paramType == "Type_Flex" )
			{
				//outputFile << "c.stack.PopFlex( node->children[" << j << "]->type );" << endl;
				outputFile << "c.stack.PopFloat();" << endl;
			}
		}
		
		outputFile << "    ";
		if( returnType != "Type_Void" )
			 outputFile << returnType << " retval = ";
		
		outputFile << functions[i].name << "( ";

		for( j=0; j<functions[i].parameterTypes.size(); j++ )
		{
			outputFile << "param" << j;

			if( j < functions[i].parameterTypes.size()-1 )
				outputFile << ", ";
		}
		outputFile << " );" << endl;

		if( returnType != "Type_Void" )
		{
			outputFile << "    if( expression )" << endl;
			outputFile << "    {" << endl;

			if( returnType == "Type_AI" ||
				returnType == "Type_Cog" ||
				returnType == "Type_Thing" || 
				returnType == "Type_Sector" ||
				returnType == "Type_Surface" ||
				returnType == "Type_Int" ||
				returnType == "Type_Keyframe" ||
				returnType == "Type_Material" ||
				returnType == "Type_Message" ||
				returnType == "Type_Model" ||
				returnType == "Type_Sound" ||
				returnType == "Type_Template" )
			{
				outputFile << "        c.stack.PushInt( retval );" << endl;
				outputFile << "        node->type = " << TypeToSymbolType( returnType ) << ";" << endl;
			}
				
			if(	returnType == "Type_Vector" )
			{
				outputFile << "        c.stack.PushVector( retval );" << endl;
				outputFile << "        node->type = " << TypeToSymbolType( returnType ) << ";" << endl;
			}

			if( returnType == "Type_Flex" || returnType == "Type_Float" )
			{
				outputFile << "        c.stack.PushFloat( retval );" << endl;
				outputFile << "        node->type = " << TypeToSymbolType( returnType ) << ";" << endl;
			}

			if( returnType == "Type_Variant" )
			{
				outputFile << "        switch( retval.type ) " << endl;
				outputFile << "        {" << endl;
				outputFile << "        case TYPE_AI:" << endl;
				outputFile << "        case TYPE_COG:" << endl;
				outputFile << "        case TYPE_THING:" << endl;
				outputFile << "        case TYPE_SECTOR:" << endl;
				outputFile << "        case TYPE_SURFACE:" << endl; 
				outputFile << "        case TYPE_INT:" << endl;
				outputFile << "        case TYPE_KEYFRAME:" << endl;
				outputFile << "        case TYPE_MATERIAL:" << endl;
				outputFile << "        case TYPE_MESSAGE:" << endl;
				outputFile << "        case TYPE_MODEL:" << endl;
				outputFile << "        case TYPE_SOUND:" << endl;
				outputFile << "        case TYPE_TEMPLATE:" << endl;
				outputFile << "            c.stack.PushInt( retval.v_int );" << endl;
				outputFile << "            break;" << endl;
				outputFile << "        case TYPE_VECTOR:" << endl;
				outputFile << "            c.stack.PushVector( retval.v_vector );" << endl;
				outputFile << "            break;" << endl;
				outputFile << "        case TYPE_FLOAT:" << endl;
				outputFile << "        case TYPE_FLEX:" << endl;
				outputFile << "            c.stack.PushFloat( retval.v_float );" << endl;
				outputFile << "            break;" << endl;
				outputFile << "        }" << endl;
				outputFile << "        node->type = retval.type;" << endl;
			}
			outputFile << "    }" << endl;
		}
		outputFile << "    return;" << endl;
		outputFile << "}" << endl;
		outputFile << endl;
	}
}

void main()
{
	vector<FunctionInfo> functions;
	
	LoadFunctionDefs( "..\\Engine\\C_Verb.h", functions );
	OutputMarshallers( "..\\Engine\\C_VerbMarshallers.inc", functions );
}
