// CalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Calc.h"
#include "CalcDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char operateDir[] = "N";		//操作符集合
int NumOrOpe = 0;		//标记是否输入了运算符
bool bPoint = false;		//标记是否输入了点
bool squareFlag = false;	//标记当前操作符为Square

int strLength;

char *stop; //字符数组转化double的函数第二个参数

char lpAddItem[50] = "";

char lpResult1[50] = "";
char lpResult2[50] = "";

double dAddItem = 0;

CEdit *cedit;

	double dResult1 = 0;
	double dResult2 = 0;

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

/////////////////////////////////////////////////////////////////////////////
// CCalcDlg dialog

CCalcDlg::CCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcDlg)
	m_CString = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcDlg)
	DDX_Text(pDX, IDC_EDIT_SHOW, m_CString); //将m_CString打印在编辑框
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCalcDlg, CDialog)
	//{{AFX_MSG_MAP(CCalcDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDB_ADD, OnAdd)
	ON_BN_CLICKED(IDB_CLEAR, OnClear)
	ON_BN_CLICKED(IDB_DIVIDE, OnDivide)
	ON_BN_CLICKED(IDB_EQUAL, OnEqual)
	ON_BN_CLICKED(IDB_MULTIPLY, OnMultiply)
	ON_BN_CLICKED(IDB_NUM0, OnNum0)
	ON_BN_CLICKED(IDB_NUM1, OnNum1)
	ON_BN_CLICKED(IDB_NUM2, OnNum2)
	ON_BN_CLICKED(IDB_NUM3, OnNum3)
	ON_BN_CLICKED(IDB_NUM4, OnNum4)
	ON_BN_CLICKED(IDB_NUM5, OnNum5)
	ON_BN_CLICKED(IDB_NUM6, OnNum6)
	ON_BN_CLICKED(IDB_NUM7, OnNum7)
	ON_BN_CLICKED(IDB_NUM8, OnNum8)
	ON_BN_CLICKED(IDB_NUM9, OnNum9)
	ON_BN_CLICKED(IDB_POINT, OnPoint)
	ON_BN_CLICKED(IDB_REMAINDER, OnRemainder)
	ON_BN_CLICKED(IDB_SQUARE, OnSquare)
	ON_BN_CLICKED(IDB_SUBTRACT, OnSubtract)
	ON_COMMAND(ID_EXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcDlg message handlers

BOOL CCalcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	//限制键盘输入
	cedit = (CEdit *) GetDlgItem(IDC_EDIT_SHOW);
	cedit->SetReadOnly(true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCalcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCalcDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCalcDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//**************************message handlers******************************
void CCalcDlg::OnNum0() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("0");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("1");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("2");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("3");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("4");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("5");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum6() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("6");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum7() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("7");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum8() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("8");
	UpdateData(FALSE);
}

void CCalcDlg::OnNum9() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateNumber("9");
	UpdateData(FALSE);
}

void CCalcDlg::OnPoint() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if (bPoint == true)
	{
		return ;
	}
	operateNumber(".");
	NumOrOpe = 1;
	bPoint = true;
	UpdateData(FALSE);
}

void CCalcDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateCharacter();
    strcpy(operateDir, "+");
	UpdateData(FALSE);
}

void CCalcDlg::OnSubtract() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateCharacter();
    strcpy(operateDir, "-");
	UpdateData(FALSE);
}

void CCalcDlg::OnMultiply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateCharacter();
    strcpy(operateDir, "*");
	UpdateData(FALSE);
}

void CCalcDlg::OnDivide() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateCharacter();
    strcpy(operateDir, "/");
	UpdateData(FALSE);
}

void CCalcDlg::OnRemainder() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateCharacter();
    strcpy(operateDir, "%");
	UpdateData(FALSE);	
}

void CCalcDlg::OnSquare() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	squareFlag = true;
	Square();
	UpdateData(FALSE);
}

void CCalcDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//SetWindowText(hEditResultLine, "0");
	m_CString = "0";
	NumOrOpe = 0;
	bPoint = false;
	squareFlag = false;
	for (int i = 0 ; i < 50 ; i++)
	{
		lpResult1[i] = ' ';
		lpResult2[i] = ' ';
	}
	strcpy(operateDir, "N");

	UpdateData(FALSE);
}

void CCalcDlg::OnEqual() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	operateCharacter();
    strcpy(operateDir, "N");
	UpdateData(FALSE);
}


//**************************数字，小数点 处理函数***************************

void CCalcDlg::operateNumber(char *number)
{

    if (NumOrOpe == 0)
    {
        //SetWindowText(hEditResultLine, "");
		m_CString = "";
	}
    //strLength = GetWindowTextLength(hEditResultLine) + 1;
    strLength = m_CString.GetLength() + 1;
	if (strLength > 20)
    {
        return;
    }
    //GetWindowText(hEditResultLine, lpAddItem, strLength);
	strcpy(lpAddItem,(LPSTR)(LPCSTR)m_CString);

    strcat(lpAddItem, number);
    if (strcmp(operateDir, "N") == 0 || squareFlag == true)
    {
		if (*number != '0' && bPoint == false && lpResult1[0] == '0')
		{
			lpResult1[0] = *number;
			m_CString.Format("%s", lpResult1);
			return;
		}
        strcpy(lpResult1, lpAddItem);
        dResult1 = strtod(lpResult1, &stop);
        if (*number == '0' && bPoint == false && dResult1 == 0)
        {
            //SetWindowText(hEditResultLine, "0");
			m_CString = "0";
		}
        else
        {
            //SetWindowText(hEditResultLine, lpResult1);
			m_CString.Format("%s", lpResult1); 
		}
    }
    else
    {
        strcpy(lpResult2, lpAddItem);
        dResult2 = strtod(lpResult2, &stop);
        if (*number == '0' && bPoint == false && dResult2 == 0)
        {
            //SetWindowText(hEditResultLine, "0");
			m_CString = "0";
		}
        else
        {
            //SetWindowText(hEditResultLine, lpResult2);
			m_CString.Format("%s", lpResult2);
		}
    }
    NumOrOpe = 1;  //按下了数字键
}


//************************操作符处理函数***************************

void CCalcDlg::operateCharacter()
{
	
    if (strcmp(operateDir, "N") == 0)
    {
        dResult1 = strtod(lpResult1, &stop);
    }
    else
    {
        switch (*operateDir)
        {
        case '+':
        {
            dResult1 = strtod(lpResult1, &stop);
            dResult2 = strtod(lpResult2, &stop);
            dResult1 = dResult1 + dResult2;
            clearLpResult2();
            break;
        }
        case '-':
        {
            dResult1 = strtod(lpResult1, &stop);
            dResult2 = strtod(lpResult2, &stop);
            dResult1 = dResult1 - dResult2;
            clearLpResult2();
            break;
        }
        case '*':
        {
            dResult1 = strtod(lpResult1, &stop);
            strtod(lpResult2, &stop);
            if ( stop == lpResult2)
            {
                break;
            }
            else
            {
                dResult2 = strtod(lpResult2, &stop);
            }
            dResult1 = dResult1 * dResult2;
            clearLpResult2();
            break;
        }
        case '/':
        {
            dResult1 = strtod(lpResult1, &stop);
            strtod(lpResult2, &stop);
            if ( stop == lpResult2)
            {
                break;
            }
            else
            {
                dResult2 = strtod(lpResult2, &stop);
            }
            if (dResult2 == 0)
            {
                ::MessageBox(NULL, "除数不能为0！", "警告", MB_OK);
                MessageBeep(MB_OK);
                return;
            }
            else
            {
                dResult1 = dResult1 / dResult2;
                clearLpResult2();
                break;
            }
        }
        case '%':
        {
            dResult1 = strtod(lpResult1, &stop);
            if ((dResult1 - (int)dResult1) != 0)
            {
                ::MessageBox(NULL, "小数不能取余！", "警告", MB_OK);
                MessageBeep(MB_OK);
                return;
            }
            strtod(lpResult2, &stop);
            if ( stop == lpResult2)
            {
                break;
            }
            else
            {
                dResult2 = strtod(lpResult2, &stop);
                if ((dResult2 - (int)dResult2) != 0)
                {
                    ::MessageBox(NULL, "小数不能取余！", "警告", MB_OK);
                    MessageBeep(MB_OK);
                    return;
                }
            }
            if (dResult2 == 0)
            {
                ::MessageBox(NULL, "取余数不能为0！", "警告", MB_OK);
                MessageBeep(MB_OK);
                clearLpResult2();
                return;
            }
            else
            {
                int a = (int)dResult1, b = (int)dResult2;
                dResult1 = a % b;
                clearLpResult2();
                break;
            }
        }
        }
    }
    //将双精度转换为字符串
    _gcvt(dResult1, 15, lpResult1);
    //SetWindowText(hEditResultLine, lpResult1);
    m_CString.Format("%s", lpResult1);

	NumOrOpe = 0;
    bPoint = false;
}

//将开方和其他运算符区分开来
void CCalcDlg::Square()
{
	dResult1 = strtod(lpResult1, &stop);
	if (dResult1 < 0)
	{
		::MessageBox(NULL, "负数没有实平方根！", "警告", MB_OK);
		NumOrOpe = 0;
		bPoint = false;
		squareFlag = false;
		clearLpResult2();
		return;
	}
	dResult1 = sqrt(dResult1);
	_gcvt(dResult1, 15, lpResult1);
	//SetWindowText(hEditResultLine, lpResult1);
	m_CString.Format("%s", lpResult1);
	NumOrOpe = 0;
	bPoint = false;
	squareFlag = false;
	clearLpResult2();
}

void CCalcDlg::clearLpResult2()
{
    for (int i = 0 ; i < 50 ; i++)
    {
        lpResult2[i] = ' ';
    }
}

void CCalcDlg::OnExit() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE);
}