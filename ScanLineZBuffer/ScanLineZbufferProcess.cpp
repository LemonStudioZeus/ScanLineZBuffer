#include "StdAfx.h"
#include "ScanLineZbufferProcess.h"
#include <math.h>


CScanLineZbufferProcess::CScanLineZbufferProcess(void)
{
	faceid = -1;
	BackGroundColor = RGB(255,255,255);//����Ϊ��ɫ
	minZ = -100000;

	bInit = false;

	m_bYZ = false;//false��Y�����ϣ�Z��Ϊ���  true��Z�����ϣ�Y��Ϊ���

	angle_x = 0.0;
	angle_y = 0.0;

	m_EdgeShow = false;


}

CScanLineZbufferProcess::~CScanLineZbufferProcess(void)
{
	Clear();
}

//�����������
void CScanLineZbufferProcess::Clear()
{

	for(int y = nHeight/2; y>-nHeight/2;y--)//��������ɨ��
	{
		try
		{
			//ɾ������α�
			CPolygon *oldPolygon = PolygonList[y+nHeight/2].head;//y�ǵ�ǰ��ɨ����yֵ

			if(oldPolygon)
				delete oldPolygon;

			PolygonList[y+nHeight/2].head = NULL;


			
			CEdge* tpEdge = EdgeList[y+nHeight/2].head;//ɾ���߱�

			if(tpEdge)
				delete tpEdge;


			EdgeList[y+nHeight/2].head = NULL;
			
		}
		catch(...)
		{
		}
	}


	//���FrameBuffer��ZBuffer
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
//��ʼ��buffer
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


//������
void CScanLineZbufferProcess::AddFace(CPoint3D *pFacePoint,int npoints, COLORREF faceColor)
{
	CPolygon *face = new CPolygon();

	face->Point2Face(pFacePoint, 3);

	face->id = ++faceid;
    face->color = faceColor;

	face->get_dy();

	if(face->c!=0)
	{//��xoy��û��ͶӰ����ȥ��
		face->next = PolygonList[face->yMax+nHeight/2].head;
		PolygonList[face->yMax+nHeight/2].head = face;//����α�

		CEdge *pEdge = face->getEdgeList();//�߱�

		SortEdge(pEdge);
	}

}
//���߰���yMax������Ӧ�ı߱�
void CScanLineZbufferProcess::SortEdge(CEdge* pEdge)
{
	CEdge* tpEdge = pEdge;
	do
	{
		CEdge* tOldpEdge = tpEdge->next;

		if(tpEdge->yMax+nHeight/2<=nHeight)//ȥ��Ymax�����߽��ֵ
		{
			tpEdge->next = EdgeList[tpEdge->yMax+nHeight/2].head;
			EdgeList[tpEdge->yMax+nHeight/2].head = tpEdge;//����ԭ���ڻ�������
			EdgeList[tpEdge->yMax+nHeight/2].ymax = tpEdge->yMax;
		}


		tpEdge = tOldpEdge;
		
	}while(tpEdge);
}
//step1 ��ӻ����α�
bool CScanLineZbufferProcess::AddActivePolygonList(int y)
{
	if(PolygonList[y+nHeight/2].head!=NULL)
	{
	    CPolygon *tPolygon = ActivePolygonList->head;

		while(tPolygon && tPolygon->next)
		{

			tPolygon = tPolygon->next;
		}

		CPolygon *oldPolygon = PolygonList[y+nHeight/2].head;//y�ǵ�ǰ��ɨ����yֵ
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

		return true;//���¶���μ���
	}

	return false;
}
//���»����α�ɾ������ɨ������Ķ����
void CScanLineZbufferProcess::UpdateActivePolygonList()
{

	//�����α��е�Ԫ���޸�
		CPolygon *tPolygon = ActivePolygonList->head;

		CPolygon *tOldPolygon = tPolygon;

		while(tPolygon)
		{
			

			tPolygon->dy -=1;//ÿһ������ε�dy��dy = dy-1
			

			if(tPolygon->dy<=0)//��dy <=0ʱ���ö����Ҫ�Ӷ���λ����ɾ��
			{
				if(ActivePolygonList->head==tPolygon)
					ActivePolygonList->head = tPolygon->next;//ɾ����һ���ڵ�
				else
				{
				    tOldPolygon->next = tPolygon->next;
				}
			}
			else
			{//��ɾ�������������һ��ָ��

			  tOldPolygon = tPolygon;
			}

			tPolygon = tPolygon->next;
		}
}
//���¶���μ���ʱ����±߶Ե���߱�
bool CScanLineZbufferProcess::AddActiveEdgeList(int y)
{

	CPolygon* tPolygon = PolygonList[y+nHeight/2].head;

	while(tPolygon)
	{
		int i=0;

		CEdge* tpEdge = EdgeList[y+nHeight/2].head;//����Ķ���ζ��������Σ��������

		CActiveEdge* tActiveEdge = new CActiveEdge();
		while(tpEdge)
		{
			if(tpEdge->id==tPolygon->id)
			{
				if(tpEdge->dy<=1)//��ˮƽ��
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
					{//�������߽���
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
					
					/******************������*****************************/
					if(ActiveEdgeList==NULL||ActiveEdgeList->id==-1)
						ActiveEdgeList = tActiveEdge;
					else
					{
						tActiveEdge->next = ActiveEdgeList;//�����ͷ
						ActiveEdgeList = tActiveEdge;
					}
					/*******************end ������**************************/

					i = 0;

					break;//Ѱ����һ�������
				}
			}

			tpEdge = tpEdge->next;
		}

		tPolygon = tPolygon->next;
		
	}


	return true;
}

//���»�߱�
void CScanLineZbufferProcess::UpdateActiveEdgeList(int y)
{
	CActiveEdge *tActiveEdgeList = ActiveEdgeList;

	CActiveEdge *tOldActiveEdgeList = tActiveEdgeList;
	while(tActiveEdgeList&&tActiveEdgeList->id!=-1)
	{
		tActiveEdgeList->dyl -=1; tActiveEdgeList->dyr -=1;
		

		tActiveEdgeList->xl +=tActiveEdgeList->dxl; tActiveEdgeList->xr +=tActiveEdgeList->dxr;//�ߺ���һ��ɨ���߽����xֵ

		tActiveEdgeList->zl += tActiveEdgeList->dzx * tActiveEdgeList->dxl + tActiveEdgeList->dzy;


		//��dyl��dyrС��0����Ӧ�ı߾�Ҫ��һ���߶�
		if(tActiveEdgeList->dyl<=0||tActiveEdgeList->dyr<=0)
		{
			if(IsInActivePolygonList(tActiveEdgeList->id))
			{
				tOldActiveEdgeList = tActiveEdgeList;//����Ϊǰ��ڵ� 

				//���ڻ����α��У�ѡ���޸ı߶ԣ�ɾ��ԭ���ģ������±�

				CEdge *tpEdge = EdgeList[y+nHeight/2].head;

				while(tpEdge)
				{
					if(tpEdge->id==tActiveEdgeList->id)
					{
						if(tActiveEdgeList->dyl<=0)
						{
							//�������
							tActiveEdgeList->dxl = tpEdge->dx;
				            tActiveEdgeList->dyl = tpEdge->dy;
				            tActiveEdgeList->xl = tpEdge->x;

						}
						else
						{//�����ұ�
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
				//ɾ���ñ߶�

				if(tActiveEdgeList == ActiveEdgeList)
				{
					//��ͷ
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
			tOldActiveEdgeList = tActiveEdgeList;//����Ϊǰ��ڵ� 
		}


		tActiveEdgeList = tActiveEdgeList->next;//��һ���߶�
	}

}
//��ȡ�������ɫֵ
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

	return this->BackGroundColor;//û�ҵ����ر���ɫ
}
//ָ���Ķ�����Ƿ��ڻ����α���
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
//step2 ����ʽ����ȸ���
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
				ZBuffer[index] = Zx;//�������ֵ

				if(m_EdgeShow)
				{

					if( x == (int)tActiveEdgeList->xl || x==(int)tActiveEdgeList->xr)
						FrameBuffer[index] = RGB(255,255,255);//�߽��߶��ǰ�ɫ

					else
					{
						FrameBuffer[index] = GetPolygonColor(tActiveEdgeList->id);//������ɫ
					}
				}
				else
				{

				   FrameBuffer[index] = GetPolygonColor(tActiveEdgeList->id);//������ɫ
				}

			}
			

			Zx += tActiveEdgeList->dzx;

		}

		tActiveEdgeList = tActiveEdgeList->next;//��һ���߶�
	}
}
//ɨ��
void CScanLineZbufferProcess::Scan()
{
	ActivePolygonList = new CPolygonList();//�����α�
	ActiveEdgeList = new CActiveEdge();//��߱�


	for(int y = nHeight/2; y>-nHeight/2;y--)//��������ɨ��
	{
		try
		{
			//step1 ����µĶ���ν������α�,���¶���μ���ʱ��ӱ߶Խ���߱�
			AddActivePolygonList(y);
			AddActiveEdgeList(y);

			//step2 ����ʽ����ȸ���

			UpdateZBufferColor(y);
			
			//�����α��е�Ԫ���޸�
			UpdateActivePolygonList();

			//step3 ��߱��е�Ԫ���޸�:�޸ĺ�Ļ�߱�����һ��ɨ���ߵĻ�߱�

			UpdateActiveEdgeList(y);
	    }
		catch(...)
		{
			AfxMessageBox(_T("����"));
		}

	}

}
//��ʾFrameBufferͼ��
void CScanLineZbufferProcess::Show()
{//dc ��ԭ����(nWidth/2,nHeight/2)]

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

	// �ݹ�   
    for (p=node->childs; p!=0; p=p->next)
	{
		render_node(p);   
    }
	if (strcmp(node->name,"$$$DUMMY")==0)//����Ƿ���mesh
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
					pFacePoint[i] = CPoint3D(a,c,b);//YZ��Ե�
				}

				OnX(this->angle_x, &pFacePoint[i]);//��x����ת
				OnY(this->angle_y, &pFacePoint[i]);//��y����ת
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
//����3ds�ļ�
bool CScanLineZbufferProcess::Load3ds(char *argv)
{
	getScalFor3dsFile(argv);//�������ű���


	
	 model = lib3ds_file_open(argv);//���ص�λ

	 if (!model) {
        //fprintf(stderr, "***ERROR***\nLoading file failed: \n");
        //exit(1);
		 AfxMessageBox(_T("��3ds�ļ�����\n"));
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







/***************************************��ȡ3ds�ļ������ű���**********************************************/
void CScanLineZbufferProcess::getScalFor3dsFile(char *argv)
{
	m_Scale = 1.0;//Ĭ����1
	m_Max = 0;


	model = lib3ds_file_open(argv);

	 if (!model) {
        //fprintf(stderr, "***ERROR***\nLoading file failed: \n");
        //exit(1);
		 AfxMessageBox(_T("��3ds�ļ�����\n"));
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

	// �ݹ�   
    for (p=node->childs; p!=0; p=p->next)
	{
		get_nodeMax(p);   
    }
	if (strcmp(node->name,"$$$DUMMY")==0)//����Ƿ���mesh
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