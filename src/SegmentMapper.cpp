#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>>* container;
};

SegmentMapperImpl::SegmentMapperImpl()
{
	container = new MyMap<GeoCoord, vector<StreetSegment>>;
}

SegmentMapperImpl::~SegmentMapperImpl()
{
	delete container;
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	StreetSegment s;
	GeoCoord gc_start, gc_end;
	vector<StreetSegment>* p;
	
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		ml.getSegment(i, s);
		gc_start = s.segment.start;
		gc_end = s.segment.end;
		p = (container->find(gc_start));
		if (p == nullptr)
		{
			vector<StreetSegment> vs;
			vs.push_back(s);
			container->associate(gc_start, vs);
		}
		else
			p->push_back(s);
		
		p = container->find(gc_end);
		if (p == nullptr)
		{
			vector<StreetSegment> vs;
			vs.push_back(s);
			container->associate(gc_end, vs);
		}
		else
			p->push_back(s);
		for (size_t j = 0; j < s.attractions.size(); j++)
		{
			gc_start = s.attractions[j].geocoordinates;
			p = (container->find(gc_start));
			if (p == nullptr)
			{
				vector<StreetSegment> vs;
				vs.push_back(s);
				container->associate(gc_start, vs);
			}
			else
				p->push_back(s);
		}
		

	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
	vector<StreetSegment>* p = (container->find(gc));
	if (p != nullptr)
	{
		for (auto it = p->begin(); it != p->end(); it++)
		{
			segments.push_back((*it));
		}
	}
	return segments;  
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
