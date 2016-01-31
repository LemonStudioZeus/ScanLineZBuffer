#pragma once

class CPoint3D
{
public:
	CPoint3D(void);
	CPoint3D(double a,double b,double c);
	~CPoint3D(void);

	CPoint3D(CPoint3D&rth);
	CPoint3D&operator=(CPoint3D&rth);
	CPoint GetPoint2D();

public:
	double x;
	double y;
	double z;

	
};
