// PointValue.cpp: implementation of the CPointValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointValue::CPointValue()
{
	Val = 0;
	count = 1;
	start = FALSE;
	previous = this;
	next = this;
	head = this;
	end = this;
	current = this;
}

CPointValue::~CPointValue()
{
		
}

void CPointValue::AddNode()
{
	CPointValue * node = new(CPointValue);

	count++;
	node->previous = head->end;
	node->next = head;
	node->head = head;
	node->end = node;//NULL 
	node->current = NULL;

	head->end = node;
	head->previous = node;
	
	node->previous->next = node;
}

BOOL CPointValue::CreateLink(int Count)
{
	if(Count<1)
		return FALSE;

	if(Count<count)
		for(int j=count;j>Count;j--)
			DeleteNode();

	for(int i=count;i<Count;i++)
		AddNode();

	return TRUE;
}

float CPointValue::GetPointValue(int span,BOOL &outState)
{
	if(span>count)
		return NULL;

	CPointValue * cur;
	cur = current;

	for(int i=0;i<span;i++)
		cur = cur->next;

	outState = cur->start;
	return cur->Val;
}

void CPointValue::MoveFirst()
{
	current = head->head;
}

void CPointValue::MoveLast()
{
	current = head->end;
}

void CPointValue::MoveNext()
{
	current = current->next;
}	

void CPointValue::MovePrevious()
{
	current = current->previous;
}

void CPointValue::SetValue(float val, BOOL state)
{
	current->start = state;
	current->Val = val;
}

int CPointValue::GetCount()
{
	return count;
}

float CPointValue::GetValue()
{
	return current->Val;
}

BOOL CPointValue::GetStart()
{
	return current->start;
}

void CPointValue::DeleteNode()
{

	CPointValue * temp;
	if(head == head->end)
		return ;

	temp = head->end;
	if(current == temp)
		current = temp->previous;

	head->end = temp->previous;
	head->previous = temp->previous;

	temp->previous->next = temp->next;

	delete(temp);
	count--;
}

void CPointValue::DeleteAll()
{
	while(head != head->end)
		DeleteNode();

	head->Val = 0;
	head->start =FALSE;
}

