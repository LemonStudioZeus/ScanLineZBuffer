// ScanLineZBufferDoc.h : interface of the CScanLineZBufferDoc class
//


#pragma once


class CScanLineZBufferDoc : public CDocument
{
protected: // create from serialization only
	CScanLineZBufferDoc();
	DECLARE_DYNCREATE(CScanLineZBufferDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CScanLineZBufferDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


