#pragma once
#include "Polygon.h"

class CPolygonList
{
public:
	CPolygonList(void);
	~CPolygonList(void);
public:
	int yMax;//多边形的最大y坐标
    CPolygon *head;
};
