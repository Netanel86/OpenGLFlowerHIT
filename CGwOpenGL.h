// CGwOpenGL.h : main header file for the CGWOPENGL application
//

#if !defined(AFX_CGWOPENGL_H__99E1C9D5_8721_4DC9_8DC4_EE9FD037A443__INCLUDED_)
#define AFX_CGWOPENGL_H__99E1C9D5_8721_4DC9_8DC4_EE9FD037A443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCGwOpenGLApp:
// See CGwOpenGL.cpp for the implementation of this class
//

class CCGwOpenGLApp : public CWinApp
{
public:
	CCGwOpenGLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGwOpenGLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCGwOpenGLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWOPENGL_H__99E1C9D5_8721_4DC9_8DC4_EE9FD037A443__INCLUDED_)
