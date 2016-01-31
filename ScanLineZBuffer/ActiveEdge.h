#pragma once

class CActiveEdge
{
public:
	CActiveEdge(void);
	~CActiveEdge(void);
public:

	double xl;//左交点的x坐标
	double dxl;//(左交点边上)两相邻扫描线交点的x坐标之差
	int dyl;//以和左交点所在边相交的扫描线数为初值，

	double xr;//右交点的x坐标
	double dxr;//(右交点边上)两相邻扫描线交点的x坐标之差
	int dyr;//以和右交点所在边相交的扫描线数为初值，

	double zl;//左交点处多边形所在平面的深度值；
	double dzx;//沿扫描线向右走过一个像素时，多边形所在平面的深度增量。对于平面方程，dzx＝-a/c(c≠0)；
	double dzy;//沿y方向向下移过一根扫描线时，多边形所在平面的深度增量。对于平面方程，dzy＝b/c(c≠0)；

	int id;//交点对所在的多边形的编号；


	CActiveEdge *next;

};
