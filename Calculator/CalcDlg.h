// CalcDlg.h : header file
//

#if !defined(AFX_CALCDLG_H__170DEC9E_1C38_4D9C_959D_E469BC9B5637__INCLUDED_)
#define AFX_CALCDLG_H__170DEC9E_1C38_4D9C_959D_E469BC9B5637__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCalcDlg dialog

class CCalcDlg : public CDialog
{
// Construction
public:
	CCalcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcDlg)
	enum { IDD = IDD_CALC_DIALOG };
	
	CString	m_CString;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCalcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void operateNumber(char *number);
	afx_msg	void operateCharacter();
	afx_msg void clearLpResult2();
	afx_msg void Square();
	afx_msg void OnAdd();
	afx_msg void OnClear();
	afx_msg void OnDivide();
	afx_msg void OnEqual();
	afx_msg void OnMultiply();
	afx_msg void OnNum0();
	afx_msg void OnNum1();
	afx_msg void OnNum2();
	afx_msg void OnNum3();
	afx_msg void OnNum4();
	afx_msg void OnNum5();
	afx_msg void OnNum6();
	afx_msg void OnNum7();
	afx_msg void OnNum8();
	afx_msg void OnNum9();
	afx_msg void OnPoint();
	afx_msg void OnRemainder();
	afx_msg void OnSquare();
	afx_msg void OnSubtract();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCDLG_H__170DEC9E_1C38_4D9C_959D_E469BC9B5637__INCLUDED_)
