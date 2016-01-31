#include "StdAfx.h"
#include "ScanLineZbufferProcess.h"
#include <math.h>


CScanLineZbufferProcess::CScanLineZbufferProcess(void)
{
	faceid = -1;
	BackGroundColor = RGB(255,255,255);//背景为白色
	minZ = -100000;

	bInit = false;

	m_bYZ = false;//false：Y轴在上，Z轴为深度  true：Z轴在上，Y轴为深度

	angle_x = 0.0;
	angle_y = 0.0;

	m_EdgeShow = false;


}

CScanLineZbufferProcess::~CScanLineZbufferProcess(void)
{
	Clear();
}

//清除所有数据
void CScanLineZbufferProcess::Clear()
{

	for(int y = nHeight/2; y>-nHeight/2;y--)//从上往下扫描
	{
		try
		{
			//删除多边形表
			CPolygon *oldPolygon = PolygonList[y+nHeight/2].head;//y是当前的扫描线y值

			if(oldPolygon)
				delete oldPolygon;

			PolygonList[y+nHeight/2].head = NULL;


			
			CEdge* tpEdge = EdgeList[y+nHeight/2].head;//删除边表

			if(tpEdge)
				delete tpEdge;


			EdgeList[y+nHeight/2].head = NULL;
			
		}
		catch(...)
		{
		}
	}


	//清除FrameBuffer、ZBuffer
	int nCount = nWidth*nHeight;

	for(int i=0;i<nCount;i++)
	{
		FrameBuffer[i] = BackGroundColor;

		ZBuffer[i] = minZ;
	}

	faceid = -1;

	angle_x = 0.0;
	angle_y = 0.0;
}
//初始化buffer
void CScanLineZbufferProcess::InitBuffer(int width,int height,CPaintDC *pdc)
{
	faceid = -1;
	nWidth = width;
	nHeight = height;
	pDC = pdc;


	
	int nCount = nWidth*nHeight;

	FrameBuffer = new COLORREF[nCount];
	ZBuffer = new double[nCount];

	for(int i=0;i<nCount;i++)
	{
		FrameBuffer[i] = BackGroundColor;

		ZBuffer[i] = minZ;
	}

    PolygonList = new CPolygonList[nHeight+1];
	EdgeList = new CEdgeList[nHeight+1];

	bInit = true;

}


//增加面
void CScanLineZbufferProcess::AddFace(CPoint3D *pFacePoint,int npoints, COLORREF faceColor)
{
	CPolygon *face = new CPolygon();

	face->Point2Face(pFacePoint, 3);

	face->id = ++faceid;
    face->color = faceColor;

	face->get_dy();

	if(face->c!=0)
	{//与xoy面没有投影的面去掉
		face->next = PolygonList[face->yMax+nHeight/2].head;
		PolygonList[face->yMax+nHeight/2].head = face;//多边形表

		CEdge *pEdge = face->getEdgeList();//边表

		SortEdge(pEdge);
	}

}
//将边按照yMax存入相应的边表
void CScanLineZbufferProcess::SortEdge(CEdge* pEdge)
{
	CEdge* tpEdge = pEdge;
	do
	{
		CEdge* tOldpEdge = tpEdge->next;

		if(tpEdge->yMax+nHeight/2<=nHeight)//去除Ymax超过边界的值
		{
			tpEdge->next = EdgeList[tpEdge->yMax+nHeight/2].head;
			EdgeList[tpEdge->yMax+nHeight/2].head = tpEdge;//坐标原点在画板中心
			EdgeList[tpEdge->yMax+nHeight/2].ymax = tpEdge->yMax;
		}


		tpEdge = tOldpEdge;
		
	}while(tpEdge);
}
//step1 添加活化多边形表
bool CScanLineZbufferProcess::AddActivePolygonList(int y)
{
	if(PolygonList[y+nHeight/2].head!=NULL)
	{
	    CPolygon *tPolygon = ActivePolygonList->head;

		while(tPolygon && tPolygon->next)
		{

			tPolygon = tPolygon->next;
		}

		CPolygon *oldPolygon = PolygonList[y+nHeight/2].head;//y是当前的扫描线y值
		while(oldPolygon)
		{
			if(oldPolygon->c!=0)
			{
				CPolygon *tnewPolygon = new CPolygon();

				*tnewPolygon = *oldPolygon;

				if(ActivePolygonList->head==NULL)
				{
					ActivePolygonList->head = tnewPolygon;
				}
				else
				{
					tPolygon->next = tnewPolygon; 
				}

				tPolygon = tnewPolygon;
			}

			oldPolygon = oldPolygon->next;
		}

		return true;//有新多边形加入
	}

	return false;
}
//更新活化多边形表，删除不再扫描区域的多边形
void CScanLineZbufferProcess::UpdateActivePolygonList()
{

	//活化多边形表中的元素修改
		CPolygon *tPolygon = ActivePolygonList->head;

		CPolygon *tOldPolygon = tPolygon;

		while(tPolygon)
		{
			

			tPolygon->dy -=1;//每一个多边形的dy：dy = dy-1
			

			if(tPolygon->dy<=0)//当dy <=0时，该多边形要从多边形活化表中删除
			{
				if(ActivePolygonList->head==tPolygon)
					ActivePolygonList->head = tPolygon->next;//删除第一个节点
				else
				{
				    tOldPolygon->next = tPolygon->next;
				}
			}
			else
			{//不删除，才需更新上一个指针

			  tOldPolygon = tPolygon;
			}

			tPolygon = tPolygon->next;
		}
}
//有新多边形加入时添加新边对到活化边表
bool CScanLineZbufferProcess::AddActiveEdgeList(int y)
{

	CPolygon* tPolygon = PolygonList[y+nHeight/2].head;

	while(tPolygon)
	{
		int i=0;

		CEdge* tpEdge = EdgeList[y+nHeight/2].head;//这里的多边形都是三角形，方便计算

		CActiveEdge* tActiveEdge = new CActiveEdge();
		while(tpEdge)
		{
			if(tpEdge->id==tPolygon->id)
			{
				if(tpEdge->dy<=1)//是水平线
				{
				  tpEdge = tpEdge->next;
				  continue;
				}

				if(i==0)
				{
					tActiveEdge->id = tPolygon->id;

					tActiveEdge->dxl = tpEdge->dx;
					tActiveEdge->dyl = tpEdge->dy;
					tActiveEdge->xl = tpEdge->x;

					tActiveEdge->dzx = -(tPolygon->a / tPolygon->c);
					tActiveEdge->dzy = tPolygon->b / tPolygon->c;
					tActiveEdge->zl = -( tPolygon->a * tpEdge->x  + tPolygon->b*y +tPolygon->d )/tPolygon->c;

					i++;

				}
				else
				{
					if((tActiveEdge->xl + tActiveEdge->dxl) > (tpEdge->x+tpEdge->dx))
					{//左右两边交换
						tActiveEdge->dxr = tActiveEdge->dxl;
						tActiveEdge->dyr = tActiveEdge->dyl;
						tActiveEdge->xr = tActiveEdge->xl;

						tActiveEdge->dxl = tpEdge->dx;
						tActiveEdge->dyl = tpEdge->dy;
						tActiveEdge->xl = tpEdge->x;
					}
					else
					{
						tActiveEdge->dxr = tpEdge->dx;
						tActiveEdge->dyr = tpEdge->dy;
						tActiveEdge->xr = tpEdge->x;
					}
					
					/******************加入活化表*****************************/
					if(ActiveEdgeList==NULL||ActiveEdgeList->id==-1)
						ActiveEdgeList = tActiveEdge;
					else
					{
						tActiveEdge->next = ActiveEdgeList;//插入表头
						ActiveEdgeList = tActiveEdge;
					}
					/*******************end 加入活化表**************************/

					i = 0;

					break;//寻找下一个多边形
				}
			}

			tpEdge = tpEdge->next;
		}

		tPolygon = tPolygon->next;
		
	}


	return true;
}

//更新活化边表
void CScanLineZbufferProcess::UpdateActiveEdgeList(int y)
{
	CActiveEdge *tActiveEdgeList = ActiveEdgeList;

	CActiveEdge *tOldActiveEdgeList = tActiveEdgeList;
	while(tActiveEdgeList&&tActiveEdgeList->id!=-1)
	{
		tActiveEdgeList->dyl -=1; tActiveEdgeList->dyr -=1;
		

		tActiveEdgeList->xl +=tActiveEdgeList->dxl; tActiveEdgeList->xr +=tActiveEdgeList->dxr;//边和下一条扫描线交点的x值

		tActiveEdgeList->zl += tActiveEdgeList->dzx * tActiveEdgeList->dxl + tActiveEdgeList->dzy;


		//若dyl或dyr小于0，相应的边就要从一个边对
		if(tActiveEdgeList->dyl<=0||tActiveEdgeList->dyr<=0)
		{
			if(IsInActivePolygonList(tActiveEdgeList->id))
			{
				tOldActiveEdgeList = tActiveEdgeList;//保存为前向节点 

				//还在活化多边形表中，选择修改边对，删除原来的，加入新边

				CEdge *tpEdge = EdgeList[y+nHeight/2].head;

				while(tpEdge)
				{
					if(tpEdge->id==tActiveEdgeList->id)
					{
						if(tActiveEdgeList->dyl<=0)
						{
							//更新左边
							tActiveEdgeList->dxl = tpEdge->dx;
				            tActiveEdgeList->dyl = tpEdge->dy;
				            tActiveEdgeList->xl = tpEdge->x;

						}
						else
						{//更新右边
							tActiveEdgeList->dxr = tpEdge->dx;
				            tActiveEdgeList->dyr = tpEdge->dy;
				            tActiveEdgeList->xr = tpEdge->x;
						}

						break;

					}

					tpEdge = tpEdge->next;
				}

				
			}
			else
			{
				//删除该边对

				if(tActiveEdgeList == ActiveEdgeList)
				{
					//表头
					ActiveEdgeList = tActiveEdgeList->next;

					tOldActiveEdgeList = ActiveEdgeList;

					 
				}
				else
				{
					tOldActiveEdgeList->next = tActiveEdgeList->next;

				}
			}
		}
		else
		{
			tOldActiveEdgeList = tActiveEdgeList;//保存为前向节点 
		}


		tActiveEdgeList = tActiveEdgeList->next;//下一个边对
	}

}
//获取多边形颜色值
COLORREF CScanLineZbufferProcess::GetPolygonColor(int id)
{
	 CPolygon *tPolygon = ActivePolygonList->head;

	while(tPolygon)
	{

		if(tPolygon->id == id)
		{

			return tPolygon->color;
		}
		tPolygon = tPolygon->next;
	}

	return this->BackGroundColor;//没找到返回背景色
}
//指定的多边形是否在活化多边形表中
bool CScanLineZbufferProcess::IsInActivePolygonList(int id)
{
	 CPolygon *tPolygon = ActivePolygonList->head;

	while(tPolygon)
	{

		if(tPolygon->id == id)
		{

			return true;
		}
		tPolygon = tPolygon->next;
	}

	return false;

}
//step2 增量式的深度更新
void CScanLineZbufferProcess::UpdateZBufferColor(int y)
{
	CActiveEdge *tActiveEdgeList = ActiveEdgeList;

	while(tActiveEdgeList && tActiveEdgeList->id!=-1)
	{
		double Zx = tActiveEdgeList->zl;

		for(int x = tActiveEdgeList->xl;x<=tActiveEdgeList->xr;x++)
		{
			if(abs(x)>nWidth/2)
			{
				Zx += tActiveEdgeList->dzx;
				continue;
			}

			int index = (nHeight/2-y)*nWidth + nWidth/2+x;

			

			if(Zx > ZBuffer[index])
			{
				ZBuffer[index] = Zx;//更新深度值

				if(m_EdgeShow)
				{

					if( x == (int)tActiveEdgeList->xl || x==(int)tActiveEdgeList->xr)
						FrameBuffer[index] = RGB(255,255,255);//边界线都是白色

					else
					{
						FrameBuffer[index] = GetPolygonColor(tActiveEdgeList->id);//更新颜色
					}
				}
				else
				{

				   FrameBuffer[index] = GetPolygonColor(tActiveEdgeList->id);//更新颜色
				}

			}
			

			Zx += tActiveEdgeList->dzx;

		}

		tActiveEdgeList = tActiveEdgeList->next;//下一个边对
	}
}
//扫描
void CScanLineZbufferProcess::Scan()
{
	ActivePolygonList = new CPolygonList();//活化多边形表
	ActiveEdgeList = new CActiveEdge();//活化边表


	for(int y = nHeight/2; y>-nHeight/2;y--)//从上往下扫描
	{
		try
		{
			//step1 添加新的多边形进活化多边形表,有新多变形加入时添加边对进活化边表
			AddActivePolygonList(y);
			AddActiveEdgeList(y);

			//step2 增量式的深度更新

			UpdateZBufferColor(y);
			
			//活化多边形表中的元素修改
			UpdateActivePolygonList();

			//step3 活化边表中的元素修改:修改后的活化边表是下一条扫描线的活化边表

			UpdateActiveEdgeList(y);
	    }
		catch(...)
		{
			AfxMessageBox(_T("出错！"));
		}

	}

}
//显示FrameBuffer图形
void CScanLineZbufferProcess::Show()
{//dc 的原点在(nWidth/2,nHeight/2)]

	for(int y=nHeight/2; y>-nHeight/2; y--)
	{
		for(int x=-nWidth/2; x<nWidth/2; x++)
		{
			int index = (nHeight/2-y)*nWidth + nWidth/2+x;

			pDC->SetPixel(x,y,FrameBuffer[index]);
		}
	}
	
	
}

void CScanLineZbufferProcess::OnX(double angle, CPoint3D *p)
{
	
		float x=p->x;
		float y=cos(angle)*p->y-sin(angle)*p->z;
		float z=sin(angle)*p->y+cos(angle)*p->z;

		*p = CPoint3D((int)x,(int)y,(int)z);
	
}

void CScanLineZbufferProcess::OnY(double angle, CPoint3D *p)
{

	    float y=p->y;
		float x=cos(angle)*p->x-sin(angle)*p->z;
		float z=sin(angle)*p->x+cos(angle)*p->z;

		*p = CPoint3D((int)x,(int)y,(int)z);
}

/***************************************load 3ds********************************************/
void CScanLineZbufferProcess::render_node(Lib3dsNode *node) 
{
	Lib3dsNode      *p;
	Lib3dsMesh      *mesh;
	Lib3dsFace      *face;
	Lib3dsMaterial  *mat;
	Lib3dsMeshInstanceNode *meshData;

	// 递归   
    for (p=node->childs; p!=0; p=p->next)
	{
		render_node(p);   
    }
	if (strcmp(node->name,"$$$DUMMY")==0)//检查是否是mesh
	{
		return;
	}
	if (!node->user_id)
	{
		mesh = lib3ds_file_mesh_for_node(model, node);
		
		 if (!mesh || !mesh->vertices) return;

		COLORREF faceColor = BackGroundColor;

		for (int fi=0; fi<mesh->nfaces; ++fi)
		{
			face = &(mesh->faces[fi]);
			mat = 0;

			CPoint3D pFacePoint[3];

			for(int i=0;i<3;i++)
			{
				float a = mesh->vertices[face->index[i]][0]/m_Scale;   
			    float b = mesh->vertices[face->index[i]][1]/m_Scale; 
			    float c = mesh->vertices[face->index[i]][2]/m_Scale; 

				if(abs(a)>=nWidth/2||abs(b)>=nHeight/2)
					goto next;

				if(!this->m_bYZ)
				{

				   pFacePoint[i] = CPoint3D(a,b,c);
				}
				else
				{
					pFacePoint[i] = CPoint3D(a,c,b);//YZ轴对调
				}

				OnX(this->angle_x, &pFacePoint[i]);//绕x轴旋转
				OnY(this->angle_y, &pFacePoint[i]);//绕y轴旋转
			}


			if (face->material>=0 && face->material<model->nmaterials)
			{
				mat=model->materials[face->material];

				faceColor = RGB(mat->diffuse[0]*255, mat->diffuse[1]*255, mat->diffuse[2]*255);
			}
			else
			{
				faceColor = BackGroundColor;
			}

			AddFace(pFacePoint,3, faceColor);

next:     continue;

		}
	}

}
//加载3ds文件
bool CScanLineZbufferProcess::Load3ds(char *argv)
{
	getScalFor3dsFile(argv);//计算缩放比例


	
	 model = lib3ds_file_open(argv);//加载点位

	 if (!model) {
        //fprintf(stderr, "***ERROR***\nLoading file failed: \n");
        //exit(1);
		 AfxMessageBox(_T("打开3ds文件出错\n"));
		return false;
    }

	 Lib3dsNode *nod;
	 for (nod=model->nodes; nod!=NULL; nod=nod->next)
	 {
		 if(nod->type==LIB3DS_NODE_MESH_INSTANCE)
		 {
		    render_node(nod);
		 }
	 }   


	 lib3ds_file_free(model);
	 return true;
}

/*************************************end load3ds****************************************/







/***************************************获取3ds文件的缩放比例**********************************************/
void CScanLineZbufferProcess::getScalFor3dsFile(char *argv)
{
	m_Scale = 1.0;//默认是1
	m_Max = 0;


	model = lib3ds_file_open(argv);

	 if (!model) {
        //fprintf(stderr, "***ERROR***\nLoading file failed: \n");
        //exit(1);
		 AfxMessageBox(_T("打开3ds文件出错\n"));
		return;
    }

	 Lib3dsNode *nod;
	 for (nod=model->nodes; nod!=NULL; nod=nod->next)
	 {
		 if(nod->type==LIB3DS_NODE_MESH_INSTANCE)
		 {
		    get_nodeMax(nod);
		 }
	 }   


	 lib3ds_file_free(model);


	 if(m_Max!=0)
	 {
	    m_Scale = 2*m_Max / (nHeight-5);
	 }
}

void CScanLineZbufferProcess::get_nodeMax(Lib3dsNode *node) 
{
	Lib3dsNode      *p;
	Lib3dsMesh      *mesh;
	Lib3dsFace      *face;
	Lib3dsMaterial  *mat;
	Lib3dsMeshInstanceNode *meshData;

	// 递归   
    for (p=node->childs; p!=0; p=p->next)
	{
		get_nodeMax(p);   
    }
	if (strcmp(node->name,"$$$DUMMY")==0)//检查是否是mesh
	{
		return;
	}
	if (!node->user_id)
	{
		mesh = lib3ds_file_mesh_for_node(model, node);
		
		 if (!mesh || !mesh->vertices) return;

		COLORREF faceColor = BackGroundColor;

		for (int fi=0; fi<mesh->nfaces; ++fi)
		{
			face = &(mesh->faces[fi]);
			mat = 0;

			CPoint3D pFacePoint[3];

			for(int i=0;i<3;i++)
			{
				float a = mesh->vertices[face->index[i]][0];   
			    float b = mesh->vertices[face->index[i]][1]; 
			    float c = mesh->vertices[face->index[i]][2]; 

	
				if(abs(a)>m_Max)
					m_Max = abs(a);

				if(abs(b)>m_Max)
					m_Max = abs(b);

				if(abs(c)>m_Max)
					m_Max = abs(c);


			}
			
		}
	}

}


/*****************************************end********************************************/