#include "StdAfx.h"
#include "Edge.h"

CEdge::CEdge(void)
{
	x =0;
	dx = 0;
	dy = 0;
	id =-1;
	next = NULL;

	yMax = -100000;
} 
CEdge::CEdge(CEdge &rth)
{
	x = rth.x;
	dx = rth.dx;
	dy = rth.dy;
	id = rth.id;
	next = rth.next;
	yMax = rth.yMax;
}
CEdge& CEdge::operator=(CEdge &rth)
{
	x = rth.x;
	dx = rth.dx;
	dy = rth.dy;
	id = rth.id;
	next = rth.next;
	yMax = rth.yMax;

	return *this;
}

CEdge::~CEdge(void)
{
	if(next)
		delete next;
}

void CEdge::Points2Edge(CPoint3D p1, CPoint3D p2)
{
    /*CPoint3D topPoint = p1;

	yMax = p1.y;

	dy = abs(p1.y-p2.y);*/
	

	dy = abs(p1.y-p2.y);

	if(p1.y>p2.y)
	{//p1ÊÇTOP point
		yMax = p1.y;

		x = p1.x;

		if(dy!=0)
		{
			dx = (p2.x-p1.x)/(p1.y-p2.y);
		}
	}
	else
	{
		
		//p2ÊÇTOP point

		yMax = p2.y;

		x = p2.x;

		if(dy!=0)
		{
			dx = (p1.x-p2.x)/(p2.y-p1.y);
		}
	}

	dy+=1;
}