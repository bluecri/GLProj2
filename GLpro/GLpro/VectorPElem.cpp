#include "VectorPElem.h"

VectorPElem::VectorPElem()
{
	vectorIndex = -1;
}

void VectorPElem::setPElemIdx(int idx)
{
	vectorIndex = idx;
}

int VectorPElem::getPElemIdx()
{
	return vectorIndex;
}
