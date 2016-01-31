// ScanLineZBufferDoc.cpp : implementation of the CScanLineZBufferDoc class
//

#include "stdafx.h"
#include "ScanLineZBuffer.h"

#include "ScanLineZBufferDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScanLineZBufferDoc

IMPLEMENT_DYNCREATE(CScanLineZBufferDoc, CDocument)

BEGIN_MESSAGE_MAP(CScanLineZBufferDoc, CDocument)
END_MESSAGE_MAP()


// CScanLineZBufferDoc construction/destruction

CScanLineZBufferDoc::CScanLineZBufferDoc()
{
	// TODO: add one-time construction code here

}

CScanLineZBufferDoc::~CScanLineZBufferDoc()
{
}

BOOL CScanLineZBufferDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CScanLineZBufferDoc serialization

void CScanLineZBufferDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CScanLineZBufferDoc diagnostics

#ifdef _DEBUG
void CScanLineZBufferDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CScanLineZBufferDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CScanLineZBufferDoc commands
