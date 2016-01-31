#pragma once
#include "Point3D.h"
#include "Edge.h"
//����Ķ���ζ��������Σ��������
class CPolygon
{
public:
	CPolygon(void);
	CPolygon(CPolygon &rth);
	void Point2Face(CPoint3D p1, CPoint3D p2, CPoint3D p3);
	void Point2Face(CPoint3D *p,int npoints);

	void get_dy();
	CEdge* getEdgeList();//��ȡ�߱�
	void   ClearHorizontalEdge();//���ˮƽ��.ƽ��X�ı�


	~CPolygon(void);
public:
	CPolygon&operator=(CPolygon &rth);
public:
	double a,b,c,d;//���������ƽ��ķ���ϵ��ax+by+cz+d=0
	int id;//����εı��
	int dy;//����ο�Խ��ɨ������Ŀ
	COLORREF color;//����ε���ɫ
	
	int yMax;
	int nPoints;//����


	CPolygon *next;

	CPoint3D *point;//��˳��ĵ�,Ҫ��ǰ���������ǹ��ߵĵ�
};
