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
	double x;//�ߵ��϶˵�x������
	double dx;//��������ɨ���߽����x�����dx (-1/k)
	int dy;//�߿�Խ��ɨ������Ŀ
	int id;//����������εı��

	int yMax;//

	CEdge *next;
};
