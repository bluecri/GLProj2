#pragma once

#include "stdafx.h"
#include "VectorPElem.h"

// actually no need std::pair<First, Second>*emplate

template <class First, class Second>
class VectorPPairFirst
{
public:
	VectorPPairFirst();
	VectorPPairFirst(int reserveSize);

	void push_back(std::pair<First, Second>* elem);
	void erase(std::pair<First, Second>* elem);
	void erase(int idx);
	void clear();
	size_t vecPSize();
	typename std::vector<std::pair<First, Second>*>::iterator begin();
	typename std::vector<std::pair<First, Second>*>::iterator end();
	typename std::vector<std::pair<First, Second>*>::iterator erase(typename std::vector<std::pair<First, Second>*>::iterator it);

	std::pair<First, Second>* getElem(int idx);
	std::pair<First, Second>* operator[](int idx);

private:
	std::vector<std::pair<First, Second>*> elemVec;
};
/*
template<class First, class Second>
VectorPPairFirst<std::pair<First, Second>*>::VectorPPairFirst()
{
	elemVec = std::vector<std::pair<First, Second>*>();
	static_assert(true == std::is_pointer<std::pair<First, Second>*>::value, "only allowed pointer of VectorPElem subclass in VectorPPairFirst");
}

template<class First, class Second>
VectorPPairFirst<std::pair<First, Second>*>::VectorPPairFirst(int reserveSize)
{
	static_assert(true == std::is_pointer<std::pair<First, Second>*>::value, "only allowed pointer of VectorPElem subclass in VectorPPairFirst");
	elemVec.reserve(reserveSize);
}

template<class First, class Second>
void VectorPPairFirst<std::pair<First, Second>*>::push_back(std::pair<First, Second>* elem)
{
	static_assert(true == std::is_convertible<First, VectorPElem*>::value, "only allowed First Pair is child of VectorPElem in VectorPPairFirst<std::pair<First, Second>*>::push_back(std::pair<First, Second>*& elem)");

	if (_DEBUG)
	{
		int originIdx = static_cast<VectorPElem*>(elem->first)->getPElemIdx();
		if (originIdx != -1)
		{
			printf_s("[ERR] : elem is already binded to vector in VectorPPairFirst<std::pair<First, Second>*>::push_back(std::pair<First, Second>*& elem)\n");
		}
	}

	elemVec.push_back(elem);
	static_cast<VectorPElem*>(elem->first)->setPElemIdx(elemVec.size() - 1);
}

template<class First, class Second>
void VectorPPairFirst<std::pair<First, Second>*>::erase(std::pair<First, Second>* elem)
{
	static_assert(true == std::is_convertible<std::pair<First, Second>*, VectorPElem*>::value, "only allowed VectorPElem in VectorPPairFirst<std::pair<First, Second>*>::erase(std::pair<First, Second>*& elem)");
	int originIdx = static_cast<VectorPElem*>(elem->first)->getPElemIdx();

	if (_DEBUG)
	{
		if (originIdx == -1)
		{
			printf_s("[ERR] : elem is not binded to vector in VectorPPairFirst<std::pair<First, Second>*>::erase(std::pair<First, Second>*& elem)\n");
		}
	}

	static_cast<VectorPElem*>(elemVec[originIdx]->first)->setPElemIdx(-1);

	elemVec[originIdx] = elemVec[elemVec.size() - 1];
	static_cast<VectorPElem*>(elemVec[originIdx]->first)->setPElemIdx(originIdx);
	elemVec.pop_back();
}

template<class First, class Second>
void VectorPPairFirst<std::pair<First, Second>*>::erase(int idx)
{
	static_cast<VectorPElem*>(elemVec[idx]->first)->setPElemIdx(-1);

	elemVec[idx] = elemVec[elemVec.size() - 1];
	static_cast<VectorPElem*>(elemVec[idx]->first)->setPElemIdx(idx);
	elemVec.pop_back();
}


template<class First, class Second>
void VectorPPairFirst<std::pair<First, Second>*>::clear()
{
	elemVec.clear();
}

template<class First, class Second>
size_t VectorPPairFirst<std::pair<First, Second>*>::vecPSize()
{
	return elemVec.size();
}


template<class First, class Second>
typename std::vector<std::pair<First, Second>*>::iterator VectorPPairFirst<std::pair<First, Second>*>::begin()
{
	return elemVec.begin();
}

template<class First, class Second>
typename std::vector<std::pair<First, Second>*>::iterator VectorPPairFirst<std::pair<First, Second>*>::end()
{
	return elemVec.end();
}

template<class First, class Second>
typename std::vector<std::pair<First, Second>*>::iterator VectorPPairFirst<std::pair<First, Second>*>::erase(typename std::vector<std::pair<First, Second>*>::iterator it)
{
	int originIdx = static_cast<VectorPElem*>((*it)->first)->getPElemIdx();
	erase(originIdx);

	return std::next(elemVec.begin(), originIdx);
}


template<class First, class Second>
std::pair<First, Second>* VectorPPairFirst<std::pair<First, Second>*>::getElem(int idx)
{
	return elemVec[idx];
}

template<class First, class Second>
std::pair<First, Second>* VectorPPairFirst<std::pair<First, Second>*>::operator[](int idx)
{
	return elemVec[idx];
}
*/

template<class First, class Second>
VectorPPairFirst<First, Second>::VectorPPairFirst()
{
	elemVec = std::vector<std::pair<First, Second>*>();
	static_assert(true == std::is_pointer<std::pair<First, Second>*>::value, "only allowed pointer of VectorPElem subclass in VectorPPairFirst");
}

template<class First, class Second>
VectorPPairFirst<First, Second>::VectorPPairFirst(int reserveSize)
{
	static_assert(true == std::is_pointer<std::pair<First, Second>*>::value, "only allowed pointer of VectorPElem subclass in VectorPPairFirst");
	elemVec.reserve(reserveSize);
}

template<class First, class Second>
void VectorPPairFirst<First, Second>::push_back(std::pair<First, Second>* elem)
{
	static_assert(true == std::is_convertible<First, VectorPElem*>::value, "only allowed First Pair is child of VectorPElem in VectorPPairFirst<std::pair<First, Second>*>::push_back(std::pair<First, Second>*& elem)");

	int originIdx = static_cast<VectorPElem*>(elem->first)->getPElemIdx();
	if (originIdx != -1)
	{
		printf_s("[ERR] : elem is already binded to vector in VectorPPairFirst<std::pair<First, Second>*>::push_back(std::pair<First, Second>*& elem)\n");
	}


	elemVec.push_back(elem);
	static_cast<VectorPElem*>(elem->first)->setPElemIdx(elemVec.size() - 1);
}

template<class First, class Second>
void VectorPPairFirst<First, Second>::erase(std::pair<First, Second>* elem)
{
	static_assert(true == std::is_convertible<First, VectorPElem*>::value, "only allowed VectorPElem in VectorPPairFirst<std::pair<First, Second>*>::erase(std::pair<First, Second>*& elem)");
	int originIdx = static_cast<VectorPElem*>(elem->first)->getPElemIdx();

	if (originIdx == -1)
	{
		printf_s("[ERR] : elem is not binded to vector in VectorPPairFirst<std::pair<First, Second>*>::erase(std::pair<First, Second>*& elem)\n");
	}

	static_cast<VectorPElem*>(elemVec[originIdx]->first)->setPElemIdx(-1);

	if (originIdx != elemVec.size() - 1)
	{
		elemVec[originIdx] = elemVec[elemVec.size() - 1];
		static_cast<VectorPElem*>(elemVec[originIdx]->first)->setPElemIdx(originIdx);
	}

	elemVec.pop_back();
}

template<class First, class Second>
void VectorPPairFirst<First, Second>::erase(int idx)
{
	static_cast<VectorPElem*>(elemVec[idx]->first)->setPElemIdx(-1);

	if (idx != elemVec.size() - 1)
	{
		elemVec[idx] = elemVec[elemVec.size() - 1];
		static_cast<VectorPElem*>(elemVec[idx]->first)->setPElemIdx(idx);
	}
	elemVec.pop_back();
}

template<class First, class Second>
void VectorPPairFirst<First, Second>::clear()
{
	for (auto elem : elemVec)
	{
		static_cast<VectorPElem*>(elem->first)->setPElemIdx(-1);
	}
	elemVec.clear();
}

template<class First, class Second>
size_t VectorPPairFirst<First, Second>::vecPSize()
{
	return elemVec.size();
}

template<class First, class Second>
typename std::vector<std::pair<First, Second>*>::iterator VectorPPairFirst<First, Second>::begin()
{
	return elemVec.begin();
}

template<class First, class Second>
typename std::vector<std::pair<First, Second>*>::iterator VectorPPairFirst<First, Second>::end()
{
	return elemVec.end();
}

template<class First, class Second>
typename std::vector<std::pair<First, Second>*>::iterator VectorPPairFirst<First, Second>::erase(typename std::vector<std::pair<First, Second>*>::iterator it)
{
	int originIdx = static_cast<VectorPElem*>((*it)->first)->getPElemIdx();
	erase(originIdx);

	return std::next(elemVec.begin(), originIdx);
}

template<class First, class Second>
std::pair<First, Second>* VectorPPairFirst<First, Second>::getElem(int idx)
{
	return elemVec[idx];
}

template<class First, class Second>
std::pair<First, Second>* VectorPPairFirst<First, Second>::operator[](int idx)
{
	return elemVec[idx];
}
