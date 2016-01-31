#include "StdAfx.h"
#include "ActiveEdge.h"

CActiveEdge::CActiveEdge(void)
{
	dxl =0;
	dxr=0;
	dyl=0;
	dyr=0;
	dzx=0;
	dzy=0;
	xl=0;
	xr=0;
	zl=0;

	next =NULL;
	id =-1;
}

CActiveEdge::~CActiveEdge(void)
{
}
