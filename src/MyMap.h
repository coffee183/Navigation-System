#ifndef MYMAP_INCLUDED
#define MYMAP_INCLUDED
#include "provided.h"
#include <vector>

// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		KeyType myKey;
		ValueType myValue;
		Node* m_left;
		Node* m_right;
		Node(const KeyType& key, const ValueType& value)
			:myKey(key),myValue(value), m_left(nullptr),m_right(nullptr)
		{}
		void add(const KeyType& key, const ValueType& value)
		{
			if (key == myKey)
				return;
			if (key < myKey)
			{
				if (m_left == nullptr)
				{
					m_left = new Node(key, value);
				}
				else
					m_left->add(key, value);
			}
			else if (key > myKey)
			{
				if (m_right == nullptr)
				{
					m_right = new Node(key, value);
				}
				else
					m_right->add(key, value);
			}
		}
	};
	Node* head;
	int m_size;
	void remove(Node* ptr);
	void add(const KeyType& key, const ValueType& value);
	//void printAll(Node* ptr);
};

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
	head = nullptr;
	m_size = 0;
}
template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap()
{
	clear();
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
	if (head != nullptr)
	{
		remove(head);
		head = nullptr;
	}
		
	m_size = 0;
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::remove(Node* ptr)
{
	if (ptr == nullptr)
		return;
	remove(ptr->m_left);
	remove(ptr->m_right);
	delete ptr;
	ptr = nullptr;
}
template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
	return m_size;
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	ValueType* ptr = find(key);
	if ( ptr != nullptr)
	{
		(*ptr) = value;
		return;
	}
	else
	{
		add(key, value);
		m_size++;
	}
}
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::add(const KeyType& key, const ValueType& value)
{
	if (head == nullptr)
	{
		head = new Node(key, value);
	}
	else
	{
		head->add(key, value);
	}
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const
{
	
	if (head == nullptr)
	{
		return nullptr;
	}
		
	Node* ptr = head;
	while (ptr != nullptr)
	{
		if (ptr->myKey == key)
			return &(ptr->myValue);
		else if (ptr->myKey > key)
			ptr = ptr->m_left;
		else if (ptr->myKey < key)
			ptr = ptr->m_right;
	}
	return nullptr;
}
inline bool operator<(const GeoCoord& gc1, const GeoCoord& gc2)
{
	if (gc1.latitudeText < gc2.latitudeText)
		return true;
	if (gc1.latitudeText == gc2.latitudeText && gc1.longitudeText < gc2.longitudeText)
		return true;
	return false;
}
inline bool operator==(const GeoCoord& gc1, const GeoCoord& gc2)
{
	if (gc1.latitudeText == gc2.latitudeText && gc1.longitudeText == gc2.longitudeText)
		return true;
	return false;
}
inline bool operator>(const GeoCoord& gc1, const GeoCoord& gc2)
{
	if (gc1.latitudeText > gc2.latitudeText)
		return true;
	if (gc1.latitudeText == gc2.latitudeText && gc1.longitudeText > gc2.longitudeText)
		return true;
	return false;
}
inline bool operator!=(const GeoCoord& gc1, const GeoCoord& gc2)
{
	
	if (!(gc1.latitudeText == gc2.latitudeText && gc1.longitudeText == gc2.longitudeText))
		return true;
	return false;
}
#endif