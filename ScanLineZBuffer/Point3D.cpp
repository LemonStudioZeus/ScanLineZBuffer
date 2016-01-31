#include "StdAfx.h"
#include "Point3D.h"

CPoint3D::CPoint3D(void)
{
	x=y=z=0;
}
CPoint3D::CPoint3D(double a,double b,double c)
{
	x=a;y=b;z=c;
}
CPoint3D::CPoint3D(CPoint3D&rth)
{
	x=rth.x;
	y=rth.y;
	z=rth.z;
}

CPoint3D& CPoint3D::operator=(CPoint3D&rth)
{
		x=rth.x;
		y=rth.y;
		z=rth.z;
		return *this;
}

CPoint CPoint3D::GetPoint2D()
{
		CPoint p(x,y);
		return p;
}

CPoint3D::~CPoint3D(void)
{
}
