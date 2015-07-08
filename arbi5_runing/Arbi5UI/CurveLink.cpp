// CurveLink.cpp: implementation of the CCurveLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CurveLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurveLink::CCurveLink()
{
	m_Count = 0;
	head = NULL;
	end = NULL;
}

CCurveLink::~CCurveLink()
{

}

int CCurveLink::Add(int ptCount,LPCTSTR name,COLORREF nColor, int lineWidth)
{
	Curve * node = new(Curve);

	if(head==NULL)
	{
		head = node;
		end =node;
		node->previous = NULL;
		node->next = NULL;
	}
	else
	{
		node->previous = end;
		node->next = NULL;
		end->next = node;
		end = node;
	}

	//构造初始化对象
	node->Index = m_Count;
	node->ptVal.CreateLink(ptCount);
	node->lColor = nColor;
	node->name = name;
	node->LineWidth = lineWidth;
	//构造初完毕
	m_Count++;
	return node->Index;
}

Curve * CCurveLink::Find(int nIndex)
{
	Curve * temp;
	temp = head;

	while(temp)
	{
		if(temp->Index == nIndex)
			return temp;
		temp = temp->next;
	}
	
	return NULL;
}

int CCurveLink::GetCount()
{
	return m_Count;
}

void CCurveLink::Release()
{
	int count = m_Count;
	
	for(int i=0;i<count;i++)
		Delete(0);
}

void CCurveLink::Delete(int nIndex)
{
	Curve * temp = Find(nIndex);

	if(!temp)
		return ;

	m_Count--;
	
	//序号前移
	Curve * current;
	current = temp;
	while(current)
	{
		current->Index--;
		current = current->next;
	}
	//

	if(head == end)
	{
		head = NULL;
		end = NULL;
		temp->ptVal.DeleteAll();
		delete(temp);
		return ;
	}

	if(temp == end)
	{
		end = end->previous;
		end->next = NULL;
		temp->ptVal.DeleteAll();
		delete(temp);
		return ;
	}

	if(temp == head)
	{
		head = head->next;
		head->previous = NULL;
		temp->ptVal.DeleteAll();
		delete(temp);
		return ;
	}

	temp->previous->next = temp->next;
	temp->next->previous = temp->previous;
	temp->ptVal.DeleteAll();
	delete(temp);

}
