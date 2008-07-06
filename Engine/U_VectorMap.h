#ifndef U_VECTORMAP_H
#define U_VECTORMAP_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>

template <class T>
class U_VectorMap
{
public:
	U_VectorMap()
	{
	}

	void push_back( const T& t, const std::string& key )
	{
		std::string lowercase = key;
		std::transform( lowercase.begin(), lowercase.end(), lowercase.begin(), tolower );

		m_vector.push_back( t );
		m_map[lowercase] = t;
	}

	unsigned int size()
	{
		return m_vector.size();
	}

	T& operator[]( int n )
	{
		return m_vector[n];
	}

	T& operator[]( const std::string& s )
	{
		std::string lowercase = s;
		std::transform( lowercase.begin(), lowercase.end(), lowercase.begin(), tolower );
		return m_map[lowercase];
	}

	int index( const std::string& s )
	{
		int i;
		T target;

		std::string lowercase = s;
		std::transform( lowercase.begin(), lowercase.end(), lowercase.begin(), tolower );

		target = m_map[lowercase];

		for( i = 0 ; i < m_vector.size() ; i++ )
		{
			if( m_vector[i] == target )
				return i;
		}

		return -1;
	}

protected:
	std::vector<T> m_vector;
	std::map<std::string, T> m_map;
};

#endif