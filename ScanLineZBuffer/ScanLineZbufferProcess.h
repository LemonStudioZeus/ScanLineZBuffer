#pragma once
#include "stdafx.h"
#include "Edge.h"
#include "Edgelist.h"
#include "PolygonList.h"
#include "Polygon.h"
#include "Point3D.h"
#include "ActiveEdge.h"
#include <lib3ds.h>


class CScanLineZbufferProcess
{
public:
	CScanLineZbufferProcess(void);
	~CScanLineZbufferProcess(void);
public:
	int nWidth,nHeight;//显示屏幕的尺寸
	CPaintDC *pDC;
    CPolygonList *PolygonList;//分类的多边形表
	CEdgeList *EdgeList;//分类边表

	CPolygonList *ActivePolygonList;//活化分类的多边形表
	CActiveEdge *ActiveEdgeList;//活化分类边表

	COLORREF *FrameBuffer;//帧缓冲器
	double *ZBuffer;//z缓冲器

	COLORREF BackGroundColor;//背景色
	double minZ;//最小z值


public:
	void InitBuffer(int width,int height,CPaintDC *pdc);
	//void InitPolygonListEdgeList();
	
	void SortEdge(CEdge* pEdge);
	bool AddActivePolygonList(int y);//添加新多边形进活化多边形表
	void UpdateActivePolygonList();//更新多边形进活化多边形表
	bool AddActiveEdgeList(int y);//添加新边堆到活化边表
	void UpdateActiveEdgeList( int y);//更新新边对中的一边，由于一边结束，而多边形还在扫描区域，删除一条边，加入新边

	bool IsInActivePolygonList(int id);//指定的多边形是否在活化多边形表中
	COLORREF GetPolygonColor(int id);//获取多边形的颜色

	void UpdateZBufferColor(int y);
	void Scan();
	void Show();

	void OnX(double angle, CPoint3D *p);
	void OnY(double angle, CPoint3D *p);
	
	void AddFace(CPoint3D *pFacePoint,int npoints, COLORREF faceColor);//增加面
	int faceid;
public:
	Lib3dsFile *model;
	bool Load3ds(char *argv);//加载3ds文件
	void render_node(Lib3dsNode *node); 

	bool bInit;

	float m_Scale;//缩放比例

	float m_Max;

	void getScalFor3dsFile(char *argv);
	void get_nodeMax(Lib3dsNode *node) ;
	void Clear();//清除所有数据

	bool m_bYZ;
	bool m_EdgeShow;

	double angle_x;//绕x轴旋转角度
	double angle_y;//绕y轴旋转角度

};
