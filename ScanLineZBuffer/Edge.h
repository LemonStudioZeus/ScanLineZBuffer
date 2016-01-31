#pragma once
#include "Point3D.h"

class CEdge
{
public:
	CEdge(void);
	CEdge(CEdge &rth);
	~CEdge(void);
public:
	CEdge&operator=(CEdge &rth);

	void Points2Edge(CPoint3D p1, CPoint3D p2);

public:
	double x;//边的上端点x的坐标
	double dx;//相邻两条扫描线交点的x坐标差dx (-1/k)
	int dy;//边跨越的扫描线数目
	int id;//边所属多边形的编号

	int yMax;//

	CEdge *next;
};
