// CGwOpenGLDlg.h : header file
//

#if !defined(AFX_CGWOPENGLDLG_H__14A99595_27D9_43A4_A0FB_E25453F2FA05__INCLUDED_)
#define AFX_CGWOPENGLDLG_H__14A99595_27D9_43A4_A0FB_E25453F2FA05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCGwOpenGLDlg dialog

//!1 b 
#include "CGlView.h"
//!1 e

class CCGwOpenGLDlg : public CDialog
{
// Construction
public:

//!1 b
	CGlView* m_pclGLView;
//!1 e

	CCGwOpenGLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCGwOpenGLDlg)
	enum { IDD = IDD_CGWOPENGL_DIALOG };
	CScrollBar	m_Scroll12;
	CScrollBar	m_Scroll11;
	CScrollBar	m_Scroll10;
	CScrollBar	m_Scroll9;
	CScrollBar	m_Scroll8;
	CScrollBar	m_Scroll7;
	CScrollBar	m_Scroll6;
	CScrollBar	m_Scroll5;
	CScrollBar	m_Scroll4;
	CScrollBar	m_Scroll3;
	CScrollBar	m_Scroll2;
	CScrollBar	m_Scroll1;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGwOpenGLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCGwOpenGLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpinX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinZ(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnDeltaposShiftX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposShiftY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposShiftZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedGrow();
	afx_msg void OnBnClickedLight();
	afx_msg void OnBnClickedDrawTube();
	afx_msg void OnBnClickedFrenetFrame();
	afx_msg void OnBnClickedDrawCurve();
	afx_msg void OnBnClickedDrawAxis();
	afx_msg void OnBnClickedDrawShadow();
	afx_msg void OnBnClickedDrawReflection();
	afx_msg void OnBnClickedReset();
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWOPENGLDLG_H__14A99595_27D9_43A4_A0FB_E25453F2FA05__INCLUDED_)
