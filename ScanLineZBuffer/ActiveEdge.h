#pragma once

class CActiveEdge
{
public:
	CActiveEdge(void);
	~CActiveEdge(void);
public:

	double xl;//�󽻵��x����
	double dxl;//(�󽻵����)������ɨ���߽����x����֮��
	int dyl;//�Ժ��󽻵����ڱ��ཻ��ɨ������Ϊ��ֵ��

	double xr;//�ҽ����x����
	double dxr;//(�ҽ������)������ɨ���߽����x����֮��
	int dyr;//�Ժ��ҽ������ڱ��ཻ��ɨ������Ϊ��ֵ��

	double zl;//�󽻵㴦���������ƽ������ֵ��
	double dzx;//��ɨ���������߹�һ������ʱ�����������ƽ����������������ƽ�淽�̣�dzx��-a/c(c��0)��
	double dzy;//��y���������ƹ�һ��ɨ����ʱ�����������ƽ����������������ƽ�淽�̣�dzy��b/c(c��0)��

	int id;//��������ڵĶ���εı�ţ�


	CActiveEdge *next;

};
