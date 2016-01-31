#include "StdAfx.h"
#include "Polygon.h"

CPolygon::CPolygon(void)
{
	a = 0;
	b = 0;
	c = 0;
	d = 0;

	id =-1;
	dy = 0;

	color = RGB(255,255,255);//默认底色为白

	next = NULL;

	nPoints = 0;
	yMax = -100000;
}
CPolygon::CPolygon(CPolygon &rth)
{
	a = rth.a;
	b = rth.b;
	c = rth.c;
	d = rth.d;

	id = rth.id;
	dy = rth.dy;
	color = rth.color;

	next = rth.next;


}
CPolygon& CPolygon::operator=(CPolygon &rth)
{
	a =rth.a;
	b =rth.b;
	c =rth.c;
	d = rth.d;

	id = rth.id;
	dy = rth.dy;
	color = rth.color;

	point = rth.point;
	nPoints = rth.nPoints;

	next = rth.next;

	return *this;
}
//三点不共线下求平面方程参数
void CPolygon::Point2Face(CPoint3D p1, CPoint3D p2, CPoint3D p3)
{

	
	double x1 = p1.x, y1 = p1.y, z1 = p1.z;
	double x2 = p2.x, y2 = p2.y, z2 = p2.z;
	double x3 = p3.x, y3 = p3.y, z3 = p3.z;

	a = (y2-y1)*(z3-z1)-(y3-y1)*(z2-z1);
	b = (x3-x1)*(z2-z1)-(x2-x1)*(z3-z1);
	c = (x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);

	d = -(a*x1+b*y1+c*z1);


}
//
void CPolygon::Point2Face(CPoint3D *p,int npoints)
{
	point = p;
	nPoints = npoints;

	Point2Face(p[0],p[1],p[2]);
}
//求出dy
void CPolygon::get_dy()
{
   CPoint3D *p  = point;


   int yMin  = 100000;


   for(int i=0;i<nPoints;i++)
   {

	   CPoint3D *tp = p++;
	   if(yMax < tp->y)
		   yMax = tp->y;

	   if(yMin>tp->y)
		   yMin = tp->y;

   }

   dy = yMax - yMin + 1;
}
//获取边表
CEdge* CPolygon::getEdgeList()
{
	CEdge* pHead = NULL;// = new CEdge();

	/*pHead->Points2Edge(point[0], point[1]);
	pHead->id = id;*/

	CEdge* pOld = NULL;// = pHead;

	for(int i=0;i<nPoints;i++)
	{
		int j = (i+1)%nPoints;

		if(point[i].y== point[j].y)//水平边不加入边表
			continue;

		CEdge* pt = new CEdge();

		pt->Points2Edge(point[i], point[j]);
		pt->id = id;
		pt->next = NULL;

		if(pHead==NULL)
		{
			pHead = pt;

			pOld =  pt;
		}
		else
		{
			pOld->next = pt;
			pOld = pt;
		}

	}

	return pHead;

}
void CPolygon::ClearHorizontalEdge()
{
}

CPolygon::~CPolygon(void)
{
	if(next)
		delete next;
}
