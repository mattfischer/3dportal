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

		while( line[0] == ' ' ) line.erase( 0, 1 );

		if( line.length() < 2 ) continue;
		if( line[0] == '#' ) continue;
		if( line[0] == '/' && ( line[1] == '*' || line[1] == '/' ) ) continue;

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

		outputFile << "if( !strcmp( (char*)node->lexData, \"" << functionName << "\" ) )" << endl;
		outputFile << "{" << endl;
		for( j=0; j<functions[i].parameterTypes.size(); j++ )
		{
			paramType = functions[i].parameterTypes[j];
			
			if( paramType[paramType.size()-1] == '&' )
				paramType.erase( paramType.size()-1, 1 );
			if( paramType[paramType.size()-1] == '*' )
				paramType.erase( paramType.size()-1, 1 );

			if( paramType == "C_Context" )
			{
				outputFile << "    C_Context& param" << j << " = c;" << endl;
				continue;
			}
			
			if( paramType == "C_Script" )
			{
				outputFile << "    C_Script* param" << j << " = this;" << endl;
				continue;
			}

			outputFile << "    " << paramType << " param" << j << " = ";
			if( paramType == "C_Type_AI" ||
				paramType == "C_Type_Cog" ||
				paramType == "C_Type_Thing" || 
				paramType == "C_Type_Sector" ||
				paramType == "C_Type_Surface" ||
				paramType == "C_Type_Int" ||
				paramType == "C_Type_Keyframe" ||
				paramType == "C_Type_Material" ||
				paramType == "C_Type_Message" ||
				paramType == "C_Type_Model" ||
				paramType == "C_Type_Sound" ||
				paramType == "C_Type_Template" )
			{
				outputFile << "c.stack.PopInt();" << endl;
			}
				
			if(	paramType == "C_Type_Vector" )
			{
				outputFile << "c.stack.PopVector();" << endl;
			}

			if( paramType == "C_Type_Float" )
			{
				outputFile << "c.stack.PopFloat();" << endl;
			}

			if( paramType == "C_Type_Flex" )
			{
				//outputFile << "c.stack.PopFlex( node->children[" << j << "]->type );" << endl;
				outputFile << "c.stack.PopFloat();" << endl;
			}
		}
		
		outputFile << "    ";
		if( returnType != "C_Type_Void" )
			 outputFile << returnType << " retval = ";
		
		outputFile << functions[i].name << "( ";

		for( j=0; j<functions[i].parameterTypes.size(); j++ )
		{
			outputFile << "param" << j;

			if( j < functions[i].parameterTypes.size()-1 )
				outputFile << ", ";
		}
		outputFile << " );" << endl;

		if( returnType != "C_Type_Void" )
		{
			outputFile << "    if( expression )" << endl;
			outputFile << "    {" << endl;

			if( returnType == "C_Type_AI" ||
				returnType == "C_Type_Cog" ||
				returnType == "C_Type_Thing" || 
				returnType == "C_Type_Sector" ||
				returnType == "C_Type_Surface" ||
				returnType == "C_Type_Int" ||
				returnType == "C_Type_Keyframe" ||
				returnType == "C_Type_Material" ||
				returnType == "C_Type_Message" ||
				returnType == "C_Type_Model" ||
				returnType == "C_Type_Sound" ||
				returnType == "C_Type_Template" )
			{
				outputFile << "        c.stack.PushInt( retval );" << endl;
				outputFile << "        node->type = " << TypeToSymbolType( returnType ) << ";" << endl;
			}
				
			if(	returnType == "C_Type_Vector" )
			{
				outputFile << "        c.stack.PushVector( retval );" << endl;
				outputFile << "        node->type = " << TypeToSymbolType( returnType ) << ";" << endl;
			}

			if( returnType == "C_Type_Flex" || returnType == "C_Type_Float" )
			{
				outputFile << "        c.stack.PushFloat( retval );" << endl;
				outputFile << "        node->type = " << TypeToSymbolType( returnType ) << ";" << endl;
			}

			if( returnType == "C_Type_Variant" )
			{
				outputFile << "        switch( retval.type ) " << endl;
				outputFile << "        {" << endl;
				outputFile << "        case C_TYPE_AI:" << endl;
				outputFile << "        case C_TYPE_COG:" << endl;
				outputFile << "        case C_TYPE_THING:" << endl;
				outputFile << "        case C_TYPE_SECTOR:" << endl;
				outputFile << "        case C_TYPE_SURFACE:" << endl; 
				outputFile << "        case C_TYPE_INT:" << endl;
				outputFile << "        case C_TYPE_KEYFRAME:" << endl;
				outputFile << "        case C_TYPE_MATERIAL:" << endl;
				outputFile << "        case C_TYPE_MESSAGE:" << endl;
				outputFile << "        case C_TYPE_MODEL:" << endl;
				outputFile << "        case C_TYPE_SOUND:" << endl;
				outputFile << "        case C_TYPE_TEMPLATE:" << endl;
				outputFile << "            c.stack.PushInt( retval.v_int );" << endl;
				outputFile << "            break;" << endl;
				outputFile << "        case C_TYPE_VECTOR:" << endl;
				outputFile << "            c.stack.PushVector( retval.v_vector );" << endl;
				outputFile << "            break;" << endl;
				outputFile << "        case C_TYPE_FLOAT:" << endl;
				outputFile << "        case C_TYPE_FLEX:" << endl;
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
