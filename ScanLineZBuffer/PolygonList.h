#pragma once
#include "Polygon.h"

class CPolygonList
{
public:
	CPolygonList(void);
	~CPolygonList(void);
public:
	int yMax;//����ε����y����
    CPolygon *head;
};
