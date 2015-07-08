#pragma once


// DialogSugar 对话框

class DialogSugar : public CDialogEx
{
	DECLARE_DYNAMIC(DialogSugar)

public:
	DialogSugar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogSugar();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
