// ScanLineZBufferView.cpp : implementation of the CScanLineZBufferView class
//

#include "stdafx.h"
#include "ScanLineZBuffer.h"

#include "ScanLineZBufferDoc.h"
#include "ScanLineZBufferView.h"

#include "ScanLineZbufferProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanLineZBufferView

IMPLEMENT_DYNCREATE(CScanLineZBufferView, CView)

BEGIN_MESSAGE_MAP(CScanLineZBufferView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPEN, &CScanLineZBufferView::OnFileOpen)
	ON_COMMAND(ID_TOOL_YZChange, &CScanLineZBufferView::OnToolYzchange)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_TOOL_Edge, &CScanLineZBufferView::OnToolEdge)
END_MESSAGE_MAP()

// CScanLineZBufferView construction/destruction

CScanLineZBufferView::CScanLineZBufferView()
: bLButtonDown(false)
{
	// TODO: add construction code here
	PI = 3.1415926535897932384626433832795;
	angle_x = 0;
	angle_y = 0;
	bLButtonDown = false;
	pFilePathName = NULL;

	m_bYZ = false;
	m_EdgeShow = false;

	pScanLineZbufferProcess = new CScanLineZbufferProcess();

}

CScanLineZBufferView::~CScanLineZBufferView()
{
	delete pScanLineZbufferProcess;

	if(pFilePathName)
	   delete pFilePathName;
}

BOOL CScanLineZBufferView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	

	return CView::PreCreateWindow(cs);
}

// CScanLineZBufferView drawing

void CScanLineZBufferView::OnDraw(CDC* /*pDC*/)
{
	CScanLineZBufferDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CScanLineZBufferView printing

BOOL CScanLineZBufferView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CScanLineZBufferView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CScanLineZBufferView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CScanLineZBufferView diagnostics

#ifdef _DEBUG
void CScanLineZBufferView::AssertValid() const
{
	CView::AssertValid();
}

void CScanLineZBufferView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CScanLineZBufferDoc* CScanLineZBufferView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanLineZBufferDoc)));
	return (CScanLineZBufferDoc*)m_pDocument;
}
#endif //_DEBUG


// CScanLineZBufferView message handlers

void CScanLineZBufferView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class

	CView::OnPrint(pDC, pInfo);
}



void CScanLineZBufferView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CView::OnPaint() for painting messages

	CRect rect;
	GetClientRect(&rect);
	dc.SetViewportOrg(rect.Width()/2,rect.Height()/2);


	if(!pScanLineZbufferProcess->bInit)
	{
	  pScanLineZbufferProcess->InitBuffer(400,400,&dc);
	}

	pScanLineZbufferProcess->pDC = &dc;

	pScanLineZbufferProcess->Show();


	//显示多边形数量
	CString s;

	s.Format(_T("多边形数：%d"),pScanLineZbufferProcess->faceid+1);

	dc.SetTextColor(RGB(255,0,0));

	dc.TextOutW(350, 0,s);
	  
	dc.TextOutW(-550, -160,_T("使用说明:"));
	dc.TextOutW(-550, -120,_T("    本程序只能打开3ds文件,"));
	dc.TextOutW(-550, -100,_T("选Tool->YZ轴对调: "));
	dc.TextOutW(-550, -80,_T("    可将模型中的YZ轴值对调"));
	dc.TextOutW(-550, -60,_T("选Tool->显示多变形边线: "));
	dc.TextOutW(-550, -40,_T("    可将模型中的多边形边线用白线显示"));

	dc.TextOutW(-550, 0,_T("鼠标拖动：可将模型旋转！"));
	
}





void CScanLineZBufferView::OnFileOpen()
{
	// TODO: Add your command handler code here

	// Create an Open dialog; the default file name extension is ".3ds".
   CFileDialog fileDlg(TRUE,_T("3ds"), _T("*.3ds"),  OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, _T("3dsFiles (*.3ds)|*.3ds"), this, 0);



   if( fileDlg.DoModal ()==IDOK )
   {
      CString pathName = fileDlg.GetPathName();
	  CString fileName = fileDlg.GetFileTitle ();

	  SetWindowText(fileName);

	  int   nLen   =   wcslen(pathName)+1;  

	  if(pFilePathName)
	  {
		  delete pFilePathName;

		  pFilePathName = NULL;
	  }

	  pFilePathName = new char[2*nLen+1];
	  WideCharToMultiByte(CP_ACP,   0,   pathName.GetBuffer(),   nLen,   pFilePathName,   2*nLen,   NULL,   NULL);

	  pScanLineZbufferProcess->Clear();
	  pScanLineZbufferProcess->Load3ds(pFilePathName);
	  pScanLineZbufferProcess->Scan();

	  Invalidate();

	}

}
//YZ轴是否对调

void CScanLineZBufferView::OnToolYzchange()
{
	// TODO: Add your command handler code here
	if (AfxGetMainWnd()->GetMenu()->GetMenuState(ID_TOOL_YZChange,MF_CHECKED))
	{
	   AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_TOOL_YZChange,MF_UNCHECKED);

	   this->m_bYZ = false;
	}
	else
	{
	   AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_TOOL_YZChange,MF_CHECKED);

	   this->m_bYZ = true;
	}

	pScanLineZbufferProcess->m_bYZ = this->m_bYZ;


	if(pFilePathName)
	{
	   pScanLineZbufferProcess->Clear();
	  pScanLineZbufferProcess->m_bYZ = this->m_bYZ;

	   pScanLineZbufferProcess->Load3ds(pFilePathName);
	    pScanLineZbufferProcess->Scan();

	   Invalidate();
    }
	
}

void CScanLineZBufferView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	MouseDownPoint = point;

	bLButtonDown = true;

	CView::OnLButtonDown(nFlags, point);
}

void CScanLineZBufferView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(!bLButtonDown)
		return;

	bLButtonDown = false;

	long x = point.x - MouseDownPoint.x;
	long y = point.y - MouseDownPoint.y;

	angle_y -= 180.0*x/400.0;
	angle_x += 180.0*y/400.0;

	while(angle_x>180.0)
		angle_x -=360.0;

	while(angle_x<-180.0)
		angle_x +=360.0;

	while(angle_y>180.0)
		angle_y -=360.0;

	while(angle_y<-180.0)
		angle_y +=360.0;

	if(pFilePathName)
	{
	   pScanLineZbufferProcess->Clear();

	   pScanLineZbufferProcess->angle_x = this->angle_x*PI/180.0;
	   pScanLineZbufferProcess->angle_y = this->angle_y*PI/180.0;

	  
	   pScanLineZbufferProcess->Load3ds(pFilePathName);
	   pScanLineZbufferProcess->Scan();

	   Invalidate();
    }

	CView::OnLButtonUp(nFlags, point);
}

void CScanLineZBufferView::OnToolEdge()
{
	// TODO: Add your command update UI handler code hereID_TOOL_Edge

	if (AfxGetMainWnd()->GetMenu()->GetMenuState(ID_TOOL_Edge,MF_CHECKED))
	{
	   AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_TOOL_Edge,MF_UNCHECKED);

	   this->m_EdgeShow = false;
	}
	else
	{
	   AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_TOOL_Edge,MF_CHECKED);

	   this->m_EdgeShow = true;
	}

	 pScanLineZbufferProcess->m_EdgeShow = this->m_EdgeShow;

	if(pFilePathName)
	{
	   pScanLineZbufferProcess->Clear();
	   pScanLineZbufferProcess->m_EdgeShow = this->m_EdgeShow;

	   pScanLineZbufferProcess->Load3ds(pFilePathName);
	    pScanLineZbufferProcess->Scan();

	   Invalidate();
    }

}
