#pragma once

#include "stdafx.h"
#include "VectorPElem.h"

// actually no need Template

template <class T>
class VectorP
{
public:
	VectorP();
	VectorP(int reserveSize);

	void push_back(T elem);
	void erase(T elem);
	void erase(int idx);
	void clear();
	size_t vecPSize();
	typename std::vector<T>::iterator begin();
	typename std::vector<T>::iterator end();
	typename std::vector<T>::iterator erase(typename std::vector<T>::iterator it);

	T getElem(int idx);
	T operator[](int idx);

private:
	std::vector<T> elemVec;
};

template<class T>
VectorP<T>::VectorP()
{
	elemVec = std::vector<T>();
	static_assert(true == std::is_pointer<T>::value, "only allowed pointer of VectorPElem subclass in VectorP");
}

template<class T>
VectorP<T>::VectorP(int reserveSize)
{
	static_assert(true == std::is_pointer<T>::value, "only allowed pointer of VectorPElem subclass in VectorP");
	elemVec.reserve(reserveSize);
}

template<class T>
void VectorP<T>::push_back(T elem)
{
	static_assert(true == std::is_convertible<T, VectorPElem*>::value, "only allowed VectorPElem in VectorP<T>::push_back(T& elem)");

	if (_DEBUG)
	{
		int originIdx = static_cast<VectorPElem*>(elem)->getPElemIdx();
		if (originIdx != -1)
		{
			printf_s("[ERR] : elem is already binded to vector in VectorP<T>::push_back(T& elem)\n");
		}
	}

	elemVec.push_back(elem);
	static_cast<VectorPElem*>(elem)->setPElemIdx(elemVec.size() - 1);
}

template<class T>
void VectorP<T>::erase(T elem)
{
	static_assert(true == std::is_convertible<T, VectorPElem*>::value, "only allowed VectorPElem in VectorP<T>::erase(T& elem)");
	int originIdx = static_cast<VectorPElem*>(elem)->getPElemIdx();

	if (_DEBUG)
	{
		if (originIdx == -1)
		{
			printf_s("[ERR] : elem is not binded to vector in VectorP<T>::erase(T& elem)\n");
		}
	}

	static_cast<VectorPElem*>(elemVec[originIdx])->setPElemIdx(-1);

	elemVec[originIdx] = elemVec[elemVec.size() - 1];
	static_cast<VectorPElem*>(elemVec[originIdx])->setPElemIdx(originIdx);
	elemVec.pop_back();
}

template<class T>
void VectorP<T>::erase(int idx)
{
	static_cast<VectorPElem*>(elemVec[idx])->setPElemIdx(-1);

	elemVec[idx] = elemVec[elemVec.size() - 1];
	static_cast<VectorPElem*>(elemVec[idx])->setPElemIdx(idx);
	elemVec.pop_back();
}


template<class T>
void VectorP<T>::clear()
{
	elemVec.clear();
}

template<class T>
size_t VectorP<T>::vecPSize()
{
	return elemVec.size();
}


template<class T>
typename std::vector<T>::iterator VectorP<T>::begin()
{
	return elemVec.begin();
}

template<class T>
typename std::vector<T>::iterator VectorP<T>::end()
{
	return elemVec.end();
}

template<class T>
typename std::vector<T>::iterator VectorP<T>::erase(typename std::vector<T>::iterator it)
{
	int originIdx = static_cast<VectorPElem*>(*it)->getPElemIdx();
	erase(originIdx);

	return std::next(elemVec.begin(), originIdx);
}


template<class T>
T VectorP<T>::getElem(int idx)
{
	return elemVec[idx];
}

template<class T>
T VectorP<T>::operator[](int idx)
{
	return elemVec[idx];
}
