#include "provided.h"
#include "MyMap.h"
#include <string>
#include <iostream>
#include <cctype>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord>* container;
	
};

AttractionMapperImpl::AttractionMapperImpl()
{
	container = new MyMap<string, GeoCoord>;
}

AttractionMapperImpl::~AttractionMapperImpl()
{
	delete container;
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment s;
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		if ((ml.getSegment(i, s)))
		{
			
			if (!((s.attractions).empty()))
			{
				for (int i =0; i < s.attractions.size(); i++)
				{
					container->associate((s.attractions[i]).name, (s.attractions[i]).geocoordinates);

				}
			}
		}
	}
	//cout << "The total number of attractions are ----------" << container->size() << endl;
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	attraction[0] = toupper(attraction[0]);
	size_t i = 1;
	while ( i < attraction.size())
	{
		if ((!isalpha(attraction[i])) && (i + 1) < attraction.size())
		{
			attraction[i + 1] = toupper(attraction[i + 1]);
			i += 2;
		}
		else
		{
			attraction[i] = tolower(attraction[i]);
			i++;
		}
			
	}
	GeoCoord* g = container->find(attraction);
	if (g == nullptr)
	{
		cerr << "Not Found" << endl;
		return false;
	}
	gc.latitudeText = g->latitudeText;
	gc.latitude = g->latitude;
	gc.longitudeText = g->longitudeText;
	gc.longitude = g->longitude;
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
