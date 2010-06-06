#ifndef JK_AI_H
#define JK_AI_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Jk
{
	class AiClass
	{
	public:
		AiClass(const string &filename);

		struct Instinct
		{
			string name;
			vector<float> arguments;
		};

		Instinct *getInstinct(const string &name);

		float alignment;
		float rank;
		float fov;
		float maxstep;
		float sightdist;
		float heardist;

		vector<Instinct> instincts;
	};
}
#endif