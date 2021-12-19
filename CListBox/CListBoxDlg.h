
// CListBoxDlg.h : header file
//

#pragma once


// CCListBoxDlg dialog
class CCListBoxDlg : public CDialogEx
{
// Construction
public:
	CCListBoxDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLISTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// A text inputted by a user.
	CString m_InputText;
	CListBox m_IgnoredListBox;
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedRemoveButton();
	afx_msg void OnBnClickedClearallButton();
};
