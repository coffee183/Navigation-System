#include "provided.h"
#include "MyMap.h"
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <iostream>
#include <cctype>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	MapLoader* ml;
	AttractionMapper* am;
	SegmentMapper* sm;
	void addAnItem(const GeoCoord& gc);
	MyMap<GeoCoord, vector<GeoCoord>>* container;
	//void printAll(const vector<NavSegment> &directions) const;
};

NavigatorImpl::NavigatorImpl()
{
	ml = new MapLoader;
	am = new AttractionMapper;
	sm = new SegmentMapper;
	container = new MyMap<GeoCoord, vector<GeoCoord>>;
}

NavigatorImpl::~NavigatorImpl()
{
	delete ml;
	delete am;
	delete sm;
	delete container;
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	if (!(ml->load(mapFile)))
		return false;
	am->init(*ml);
	sm->init(*ml);
	GeoCoord gc;
	StreetSegment s;
	vector<GeoCoord>* vgc;
	for (size_t i = 0; i < ml->getNumSegments(); i++)
	{
		
		if ((ml->getSegment(i, s)))
		{

			gc = s.segment.start;
			vgc = container->find(gc);
			if (vgc == nullptr)
			{
				addAnItem(gc);
			}
			gc = s.segment.end;			
			vgc = container->find(gc);
			if (vgc == nullptr)
			{
				addAnItem(gc);
			}
			if (!((s.attractions).empty()))
			{
				for (int i = 0; i < s.attractions.size(); i++)
				{

					am->getGeoCoord(s.attractions[i].name, gc);
					vgc = container->find(gc);
					if (vgc == nullptr)
					{
						addAnItem(gc);						
					}
				}
			}
		}
	}
	return true;
}
void NavigatorImpl::addAnItem(const GeoCoord& gc)
{
	vector<GeoCoord> vg;
	vector<StreetSegment> vs;
	GeoCoord temp;
	vs = sm->getSegments(gc);
	for (size_t i = 0; i< vs.size(); i++)
	{
		temp = vs[i].segment.start;
		if (!(temp.latitudeText == gc.latitudeText && temp.longitudeText == gc.longitudeText))
		{
			vg.push_back(temp);
		}
			
		temp = vs[i].segment.end;
		if (!(temp.latitudeText == gc.latitudeText && temp.longitudeText == gc.longitudeText))
		{
			vg.push_back(temp);
		}
	}
	container->associate(gc, vg);
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord gc1, gc2;
	vector<GeoCoord>* vg1, *vg2, *vg;
	stack<GeoSegment> sg;
	am->getGeoCoord(start, gc1);
	am->getGeoCoord(end, gc2);
	vg1= container->find(gc1);
	if (vg1 == nullptr)
		return NAV_BAD_SOURCE;
	vg2 = container->find(gc2);
	if (vg2 == nullptr)
		return NAV_BAD_DESTINATION;
	queue<GeoCoord> qg;
	MyMap<GeoCoord, GeoCoord> myMap;
	double angle;
	double my_distance;
	string my_angle;
	GeoCoord *p;
	vector<StreetSegment> vs1;
	vector<StreetSegment> vs2;
	vs1 = sm->getSegments(gc1);
	vs2 = sm->getSegments(gc2);
	for (size_t i = 0; i < vs1.size(); i++)
	{
		for (size_t j = 0; j < vs2.size(); j++)
		{
			if (vs1[i].streetName == vs2[i].streetName)
			{
				directions.clear();
				angle = angleOfLine(GeoSegment(gc1, gc2));
				if (angle >= 0 && angle <= 22.5)
					my_angle = "east";
				else if (angle > 22.5 && angle <= 67.5)
					my_angle = "northeast";
				else if (angle > 67.5 && angle <= 112.5)
					my_angle = "north";
				else if (angle > 112.5 && angle <= 157.5)
					my_angle = "northwest";
				else if (angle > 157.5 && angle <= 202.5)
					my_angle = "west";
				else if (angle > 202.5 && angle <= 247.5)
					my_angle = "southwest";
				else if (angle > 247.5 && angle <= 292.5)
					my_angle = "south";
				else if (angle > 292.5 && angle <= 337.5)
					my_angle = "southeast";
				else if (angle > 337.5 && angle < 360)
					my_angle = "east";
				my_distance = distanceEarthMiles(gc1, gc2);
				NavSegment nv(my_angle, vs1[i].streetName, my_distance, GeoSegment(gc1, gc2));
				directions.push_back(nv);
				//printAll(directions);
				//cerr << "You have reached your destination: " << end << endl;
				//cerr << "Total travel distance: " << my_distance << " miles" << endl;
				return NAV_SUCCESS;
			}
		}
	}
	double total = 0;
	
	qg.push(gc1);
	myMap.associate(gc1, gc1);
	while (!qg.empty())
	{
		GeoCoord curr = qg.front();
		qg.pop();
		for (size_t i = 0; i < vg2->size(); i++)
		{
			if (curr == (*vg2)[i])
			{
				directions.clear();
				//cerr << "You are starting at: " << start << endl;
				myMap.associate(gc2, curr);
				sg.push(GeoSegment(curr, gc2));
				GeoCoord prev = *myMap.find(curr);
				while (prev != curr)
				{
					sg.push(GeoSegment(prev, curr));
					curr = prev;
					prev = *myMap.find(curr);
				}
				while (!sg.empty())
				{
					GeoSegment curr = sg.top();
					angle = angleOfLine(curr);
					my_distance = distanceEarthMiles(curr.start, curr.end);
					
					if (angle >= 0 && angle <= 22.5)
						my_angle = "east";
					else if (angle > 22.5 && angle <= 67.5)
						my_angle = "northeast";
					else if (angle > 67.5 && angle <= 112.5)
						my_angle = "north";
					else if (angle > 112.5 && angle <= 157.5)
						my_angle = "northwest";
					else if (angle > 157.5 && angle <= 202.5)
						my_angle = "west";
					else if (angle > 202.5 && angle <= 247.5)
						my_angle = "southwest";
					else if (angle > 247.5 && angle <= 292.5)
						my_angle = "south";
					else if (angle > 292.5 && angle <= 337.5)
						my_angle = "southeast";
					else if (angle > 337.5 && angle < 360 )
						my_angle = "east";
					sg.pop();
					vs1 = sm->getSegments(curr.start);
					vs2 = sm->getSegments(curr.end);
					for (int i = 0; i < vs1.size(); i++)
					{
						bool found = false;
						for (int j = 0; j < vs2.size(); j++)
						{
							if (vs1[i].streetName == vs2[j].streetName)
							{							
								string name = vs1[i].streetName;
								if (!directions.empty())
								{
									if (name != directions[directions.size() - 1].m_streetName)
									{
										string my_turnAngle;
										double twoLineAngle = angleBetween2Lines(
											directions[directions.size() - 1].m_geoSegment,curr);										
										if (twoLineAngle >= 0 && twoLineAngle < 180)
											my_turnAngle = "left";
										else 
											my_turnAngle = "right";
										
										NavSegment sgTurn(my_turnAngle, name);
										directions.push_back(sgTurn);
									}
								}
								total += my_distance;
								NavSegment sg(my_angle, name, my_distance, curr);
								directions.push_back(sg);
								found = true;
								break;
							}
							
						}
						if (found == true)
							break;
					}

				}
				//printAll(directions);
				//cerr << "You have reached your destination: " << end << endl;
				//cerr << "Total travel distance: " << total << " miles" << endl;
				return NAV_SUCCESS;
			}
		}
		vg = container->find(curr);
		for (size_t i = 0; i < vg->size(); i++)
		{
			p = myMap.find((*vg)[i]);
			if (p == nullptr)
			{
				qg.push((*vg)[i]);
				myMap.associate((*vg)[i], curr);
			}
		}
		

	}
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}
//void NavigatorImpl::printAll(const vector<NavSegment> &directions) const
//{
//	if (directions.empty())
//		return;
//	for (size_t i = 0; i < directions.size(); i++)
//	{
//		 
//		if (directions[i].m_command == NavSegment::TURN)
//		{
//			cerr << "Turn " << directions[i].m_direction << " onto " << directions[i].m_streetName << endl;
//		}
//		else
//		{
//			cerr << "Proceed " << directions[i].m_distance << " miles ";
//			cerr<< directions[i].m_direction <<" on "<< directions[i].m_streetName<<endl;
//			
//		}
//	}
//}
//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
