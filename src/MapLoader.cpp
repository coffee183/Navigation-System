#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment*> mySeg;
	size_t m_size;
};

MapLoaderImpl::MapLoaderImpl()
	: m_size(0)
{}

MapLoaderImpl::~MapLoaderImpl()
{
	for (int i = 0; i < m_size; i++)
		delete mySeg[i];
}

bool MapLoaderImpl::load(string mapFile)
{
	
	ifstream infile(mapFile);
	if (!infile)
	{
		cerr << "Error: Cannot open the file" << endl;
		return false;
	}
	string line;
	string tempLat, tempLon;
	int nAttractions;
	StreetSegment* myStreet;
	Attraction myAttraction;
	char c;
	while (getline(infile, line))
	{
		myStreet = new StreetSegment;
		tempLat = "";
		tempLon = "";
		myStreet->streetName = line;
		infile.get(c);
		while (c != ',')
		{
			tempLat += c;
			infile.get(c);
		}
		infile.get(c);
		if (c == ' ')
			infile.get(c);
		while (c != ' ')
		{
			tempLon += c;
			infile.get(c);
		}
		myStreet->segment.start = GeoCoord(tempLat, tempLon);
		tempLat = "";
		tempLon = "";
		infile.get(c);
		while (c != ',')
		{
			tempLat += c;
			infile.get(c);
		}
		infile.get(c);
		if (c == ' ')
			infile.get(c);
		while (c != '\n')
		{
			tempLon += c;
			infile.get(c);
		}
		myStreet->segment.end = GeoCoord(tempLat, tempLon);
		infile >> nAttractions;
		infile.ignore(10000, '\n');
		while (nAttractions > 0)
		{
			line = "";
			tempLat = "";
			tempLon = "";
			infile.get(c);
			while (c != '|')
			{
				//c = tolower(c);
				line += c;
				infile.get(c);
			}
			infile.get(c);
			while (c != ',')
			{
				tempLat += c;
				infile.get(c);
			}
			infile.get(c);
			if (c == ' ')
				infile.get(c);
			while (c != '\n')
			{
				tempLon += c;
				infile.get(c);
			}
			myAttraction.name = line;
			myAttraction.geocoordinates = GeoCoord(tempLat, tempLon);
			myStreet->attractions.push_back(myAttraction);
			nAttractions--;
		}
		mySeg.push_back(myStreet);
		m_size++;
	}
	
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_size;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum > m_size - 1)
		return false;
	seg.streetName = mySeg[segNum]->streetName;
	seg.segment = mySeg[segNum]->segment;
	seg.attractions = mySeg[segNum]->attractions;
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
