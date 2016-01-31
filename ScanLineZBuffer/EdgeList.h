#pragma once
#include "Edge.h"
class CEdgeList
{
public:
	CEdgeList(void);
	~CEdgeList(void);
public:

	int ymax;
	CEdge *head;
};
