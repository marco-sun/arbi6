// PointValue.h: interface for the CPointValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTVALUE_H__38F3550C_4108_4AFA_9D05_6C5740861EB2__INCLUDED_)
#define AFX_POINTVALUE_H__38F3550C_4108_4AFA_9D05_6C5740861EB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPointValue  
{
public:
private:
	BOOL start;
	float Val;
	CPointValue * previous;
	CPointValue * next;
	CPointValue * head;
	CPointValue * end;
	CPointValue * current;
	int count;

public:
	float GetPointValue(int span,BOOL &outState);
	void AddNode();
	void DeleteNode();
	void DeleteAll();
	BOOL GetStart();
	float GetValue();
	int GetCount();
	void MovePrevious();
	void MoveNext();
	void MoveLast();
	void MoveFirst();
	void SetValue(float val, BOOL state);
	BOOL CreateLink(int Count);
	CPointValue();
	virtual ~CPointValue();

};

#endif // !defined(AFX_POINTVALUE_H__38F3550C_4108_4AFA_9D05_6C5740861EB2__INCLUDED_)
