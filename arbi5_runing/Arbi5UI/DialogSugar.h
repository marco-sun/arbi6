#pragma once


// DialogSugar �Ի���

class DialogSugar : public CDialogEx
{
	DECLARE_DYNAMIC(DialogSugar)

public:
	DialogSugar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogSugar();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
