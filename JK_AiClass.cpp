#include "JK_AiClass.h"

#include "JK_Parse.h"
#include "JK_GOB.h"

namespace Jk
{
	AiClass::AiClass(const string &filename)
	{
        string fullname;
		string data;
        Jk::Parser::Line line;
		bool error;

	    fullname = "misc\\ai\\" + filename;
        data = Jk::Gob::getFile(fullname);
        Jk::Parser parser(data);

        line = parser.getLine(error);

		line.matchString("alignment=", error);
		alignment = line.getFloat(error);
		line.matchString(", rank=", error);
		rank = line.getFloat(error);
		line.matchString(", fov=", error);
		fov = line.getFloat(error);
		line.matchString(", maxstep=", error);
		maxstep = line.getFloat(error);
		line.matchString(", sightdist=", error);
		sightdist = line.getFloat(error);
		line.matchString(", heardist=", error);
		heardist = line.getFloat(error);

		while(1)
		{
			line = parser.getLine(error);

			if(error)
			{
				break;
			}

			Instinct newInstinct;
			float val;

			newInstinct.name = line.getString(error);

			while(1)
			{
				val = line.getFloat(error);
				if(error)
				{
					break;
				}

				newInstinct.arguments.push_back(val);

				line.matchString(",", error);

				if(error)
				{
					break;
				}
			}

			instincts.push_back(newInstinct);
		}
	}

	
	AiClass::Instinct *AiClass::getInstinct(const string &name)
	{
		unsigned int i;

		for(i=0; i<instincts.size(); i++)
		{
			if(instincts[i].name == name) {
				return &instincts[i];
			}
		}

		return NULL;
	}
}