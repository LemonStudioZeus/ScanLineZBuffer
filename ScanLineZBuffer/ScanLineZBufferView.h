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
	// 鼠标左键按下
	bool bLButtonDown;
	CPoint MouseDownPoint;

	bool m_bYZ;//YZ轴是否对调
	char   *pFilePathName;//打开3ds文件路径
	double angle_x;//绕x轴旋转角度
	double angle_y;//绕y轴旋转角度
	double PI;
	bool m_EdgeShow;//显示多边形边线

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

