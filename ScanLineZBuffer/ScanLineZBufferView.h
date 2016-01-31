// ScanLineZBufferView.h : interface of the CScanLineZBufferView class
//


#pragma once

#include "ScanLineZbufferProcess.h"
#include <lib3ds.h>

class CScanLineZBufferView : public CView
{
protected: // create from serialization only
	CScanLineZBufferView();
	DECLARE_DYNCREATE(CScanLineZBufferView)

// Attributes
public:
	CScanLineZBufferDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CScanLineZBufferView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
	afx_msg void OnPaint();

public:
	CScanLineZbufferProcess *pScanLineZbufferProcess;

public:
	// ����������
	bool bLButtonDown;
	CPoint MouseDownPoint;

	bool m_bYZ;//YZ���Ƿ�Ե�
	char   *pFilePathName;//��3ds�ļ�·��
	double angle_x;//��x����ת�Ƕ�
	double angle_y;//��y����ת�Ƕ�
	double PI;
	bool m_EdgeShow;//��ʾ����α���

public:
	afx_msg void OnFileOpen();
public:
	afx_msg void OnToolYzchange();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnToolEdge();
};

#ifndef _DEBUG  // debug version in ScanLineZBufferView.cpp
inline CScanLineZBufferDoc* CScanLineZBufferView::GetDocument() const
   { return reinterpret_cast<CScanLineZBufferDoc*>(m_pDocument); }
#endif

