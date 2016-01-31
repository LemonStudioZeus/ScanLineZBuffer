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
	int nWidth,nHeight;//��ʾ��Ļ�ĳߴ�
	CPaintDC *pDC;
    CPolygonList *PolygonList;//����Ķ���α�
	CEdgeList *EdgeList;//����߱�

	CPolygonList *ActivePolygonList;//�����Ķ���α�
	CActiveEdge *ActiveEdgeList;//�����߱�

	COLORREF *FrameBuffer;//֡������
	double *ZBuffer;//z������

	COLORREF BackGroundColor;//����ɫ
	double minZ;//��Сzֵ


public:
	void InitBuffer(int width,int height,CPaintDC *pdc);
	//void InitPolygonListEdgeList();
	
	void SortEdge(CEdge* pEdge);
	bool AddActivePolygonList(int y);//����¶���ν������α�
	void UpdateActivePolygonList();//���¶���ν������α�
	bool AddActiveEdgeList(int y);//����±߶ѵ���߱�
	void UpdateActiveEdgeList( int y);//�����±߶��е�һ�ߣ�����һ�߽�����������λ���ɨ������ɾ��һ���ߣ������±�

	bool IsInActivePolygonList(int id);//ָ���Ķ�����Ƿ��ڻ����α���
	COLORREF GetPolygonColor(int id);//��ȡ����ε���ɫ

	void UpdateZBufferColor(int y);
	void Scan();
	void Show();

	void OnX(double angle, CPoint3D *p);
	void OnY(double angle, CPoint3D *p);
	
	void AddFace(CPoint3D *pFacePoint,int npoints, COLORREF faceColor);//������
	int faceid;
public:
	Lib3dsFile *model;
	bool Load3ds(char *argv);//����3ds�ļ�
	void render_node(Lib3dsNode *node); 

	bool bInit;

	float m_Scale;//���ű���

	float m_Max;

	void getScalFor3dsFile(char *argv);
	void get_nodeMax(Lib3dsNode *node) ;
	void Clear();//�����������

	bool m_bYZ;
	bool m_EdgeShow;

	double angle_x;//��x����ת�Ƕ�
	double angle_y;//��y����ת�Ƕ�

};
