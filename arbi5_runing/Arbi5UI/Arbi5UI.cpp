// Arbi5UI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Arbi5UI.h"

#include "MainFrm.h"
#include "Arbi5UIDoc.h"
#include "Arbi5UIView.h"
#include "UserLoadDlg.h"
#include "AuthDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma warning(disable : 4786)
/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIApp

BEGIN_MESSAGE_MAP(CMyArbi5UIApp, CWinApp)
	//{{AFX_MSG_MAP(CMyArbi5UIApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIApp construction

CMyArbi5UIApp::CMyArbi5UIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyArbi5UIApp object

CMyArbi5UIApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIApp initialization

BOOL CMyArbi5UIApp::InitInstance()
{
//	VERIFY( 1 == InitializeSkin());
//	VERIFY( 1 == LoadSkinFile(".\\WhiteFire.rss") ); 
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	
	
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyArbi5UIDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMyArbi5UIView));
	AddDocTemplate(pDocTemplate);

	//********add by for License**************//	
	/*
    CString strCupID;
	CString strDecode;
	CString strOldkey;
	char szOldkey[50];
    memset(szOldkey,0,50);
	if(GetPrivateProfileString("KEY", "key1", "00000000", szOldkey,50, ".\\AIB.ini"))
	{
         strOldkey = (CString)szOldkey; 
	}
	if (strOldkey.GetLength() < 26)
	{
		strOldkey = "000000000000000000000000000";
	}
	DecodeKey(strOldkey, strDecode);
	GetCUPID(strCupID);
	strCupID = strCupID.Mid(0,26);
	if (strCupID.Compare(strDecode.GetBuffer(strDecode.GetLength())) == 0)
	{
		CUserLoadDlg userLoadDlg;
		userLoadDlg.DoModal();
	}
	else
	{
		CAuthDlg userLoadDlg;
		userLoadDlg.DoModal();

//		AfxMessageBox("Please use the right Key!");
	}
	
	CAuthDlg userLoadDlg;
	userLoadDlg.DoModal();
	*/
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
//	m_pMainWnd->ShowWindow(SW_SHOW);

	CMainFrame * pMainFrame = NULL;
	pMainFrame = ((CMainFrame*)::AfxGetMainWnd());

//	pMainFrame->m_tPerson = userLoadDlg.m_tPerson;

	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED); 
	m_pMainWnd->UpdateWindow();
    
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMyArbi5UIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMyArbi5UIApp message handlers



