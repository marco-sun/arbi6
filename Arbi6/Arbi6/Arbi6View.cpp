
// Arbi6View.cpp : CArbi6View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Arbi6.h"
#endif

#include "Arbi6Doc.h"
#include "Arbi6View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CArbi6View

IMPLEMENT_DYNCREATE(CArbi6View, CView)

BEGIN_MESSAGE_MAP(CArbi6View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CArbi6View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CArbi6View ����/����

CArbi6View::CArbi6View()
{
	// TODO: �ڴ˴���ӹ������

}

CArbi6View::~CArbi6View()
{
}

BOOL CArbi6View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CArbi6View ����

void CArbi6View::OnDraw(CDC* /*pDC*/)
{
	CArbi6Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CArbi6View ��ӡ


void CArbi6View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CArbi6View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CArbi6View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CArbi6View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CArbi6View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CArbi6View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CArbi6View ���

#ifdef _DEBUG
void CArbi6View::AssertValid() const
{
	CView::AssertValid();
}

void CArbi6View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CArbi6Doc* CArbi6View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CArbi6Doc)));
	return (CArbi6Doc*)m_pDocument;
}
#endif //_DEBUG


// CArbi6View ��Ϣ�������
