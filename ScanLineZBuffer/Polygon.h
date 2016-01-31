#pragma once
#include "Point3D.h"
#include "Edge.h"
//这里的多边形都是三角形，方便计算
class CPolygon
{
public:
	CPolygon(void);
	CPolygon(CPolygon &rth);
	void Point2Face(CPoint3D p1, CPoint3D p2, CPoint3D p3);
	void Point2Face(CPoint3D *p,int npoints);

	void get_dy();
	CEdge* getEdgeList();//获取边表
	void   ClearHorizontalEdge();//清除水平边.平行X的边


	~CPolygon(void);
public:
	CPolygon&operator=(CPolygon &rth);
public:
	double a,b,c,d;//多边形所在平面的方程系数ax+by+cz+d=0
	int id;//多边形的编号
	int dy;//多边形跨越的扫描线数目
	COLORREF color;//多边形的颜色
	
	int yMax;
	int nPoints;//点数


	CPolygon *next;

	CPoint3D *point;//按顺序的点,要求前三个不能是共线的点
};
