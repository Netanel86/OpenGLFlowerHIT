// CGwOpenGLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGwOpenGL.h"
#include "CGwOpenGLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CCGwOpenGLDlg dialog

CCGwOpenGLDlg::CCGwOpenGLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCGwOpenGLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCGwOpenGLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CCGwOpenGLDlg, CDialog)
	//{{AFX_MSG_MAP(CCGwOpenGLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINX, &CCGwOpenGLDlg::OnDeltaposSpinX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINY, &CCGwOpenGLDlg::OnDeltaposSpinY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINZ, &CCGwOpenGLDlg::OnDeltaposSpinZ)
	
	ON_NOTIFY(UDN_DELTAPOS, IDC_SHIFTX, &CCGwOpenGLDlg::OnDeltaposShiftX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SHIFTY, &CCGwOpenGLDlg::OnDeltaposShiftY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SHIFTZ, &CCGwOpenGLDlg::OnDeltaposShiftZ)

	ON_BN_CLICKED(IDC_LIGHT, &CCGwOpenGLDlg::OnBnClickedLight)
	ON_BN_CLICKED(IDC_GROW, &CCGwOpenGLDlg::OnBnClickedGrow)
	ON_BN_CLICKED(IDC_LIGHT, &CCGwOpenGLDlg::OnBnClickedLight)
	ON_BN_CLICKED(IDC_DTUBE, &CCGwOpenGLDlg::OnBnClickedDrawTube)
	ON_BN_CLICKED(IDC_DFRAME, &CCGwOpenGLDlg::OnBnClickedFrenetFrame)
	ON_BN_CLICKED(IDC_DCURVE, &CCGwOpenGLDlg::OnBnClickedDrawCurve)
	ON_BN_CLICKED(IDC_DAXIS, &CCGwOpenGLDlg::OnBnClickedDrawAxis)
	ON_BN_CLICKED(IDC_DSHADOW, &CCGwOpenGLDlg::OnBnClickedDrawShadow)
	ON_BN_CLICKED(IDC_DREFLECTION, &CCGwOpenGLDlg::OnBnClickedDrawReflection)
	ON_BN_CLICKED(IDC_RESET, &CCGwOpenGLDlg::OnBnClickedReset)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCGwOpenGLDlg message handlers

BOOL CCGwOpenGLDlg::OnInitDialog()
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
    //! 1b 
	
	CWnd *w = GetDlgItem(IDwndOG);

	m_pclGLView = new CGlView(w);
   
	//!!!!!!


	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

	//to redraw after initialization
	     //  ID 
	     //    delay 
	     //      WM_TIMER messages, else special procedure 
	SetTimer(1,25,NULL); // see OnTimer event 

	//view scrolling:
   SCROLLINFO info;
   info.cbSize = sizeof(SCROLLINFO);     
   info.fMask = SIF_ALL;     
   info.nMin = 0;     
   info.nMax = 200; 
   info.nPage = 2;     
   info.nPos = 110;    
   info.nTrackPos = 0; 
   m_Scroll1.SetScrollInfo(&info);
   m_Scroll2.SetScrollInfo(&info);
   m_Scroll3.SetScrollInfo(&info);
   m_Scroll4.SetScrollInfo(&info);
   m_Scroll5.SetScrollInfo(&info);
   m_Scroll6.SetScrollInfo(&info);
   m_Scroll7.SetScrollInfo(&info);
   m_Scroll8.SetScrollInfo(&info);
   m_Scroll9.SetScrollInfo(&info);
   //Light position
   m_Scroll10.SetScrollInfo(&info);
   m_Scroll11.SetScrollInfo(&info);
   m_Scroll12.SetScrollInfo(&info);
   
   m_Scroll1.SetScrollPos(100);
   m_Scroll2.SetScrollPos(100);
   m_Scroll3.SetScrollPos(200);
   m_Scroll4.SetScrollPos(100);
   m_Scroll5.SetScrollPos(100);
   m_Scroll6.SetScrollPos(100);
   m_Scroll7.SetScrollPos(100);
   m_Scroll8.SetScrollPos(120);
   m_Scroll9.SetScrollPos(100);
   //Light position
   m_Scroll10.SetScrollPos(100);
   m_Scroll11.SetScrollPos(100);
   m_Scroll12.SetScrollPos(100);
   UpdateData(FALSE);

    for(int i=0;i<11;i++)
      m_pclGLView->ScrollValue[i]=0.0;

    m_pclGLView->ScrollValue[2]=10.0;
    m_pclGLView->ScrollValue[7]=2.0;
	
	m_pclGLView->ScrollValue[9]=4.0;
	m_pclGLView->ScrollValue[10]=4.0;
	m_pclGLView->ScrollValue[11]=4.0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCGwOpenGLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCGwOpenGLDlg::OnPaint() 
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
HCURSOR CCGwOpenGLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCGwOpenGLDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	// test if it's our timer
	if (nIDEvent==1)
	    m_pclGLView->vDrawGLScene();
	
	CDialog::OnTimer(nIDEvent);
}

void CCGwOpenGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCGwOpenGLDlg)
	DDX_Control(pDX, IDC_SCROLLBAR12, m_Scroll12);
	DDX_Control(pDX, IDC_SCROLLBAR11, m_Scroll11);
	DDX_Control(pDX, IDC_SCROLLBAR10, m_Scroll10);
	DDX_Control(pDX, IDC_SCROLLBAR9, m_Scroll9);
	DDX_Control(pDX, IDC_SCROLLBAR8, m_Scroll8);
	DDX_Control(pDX, IDC_SCROLLBAR7, m_Scroll7);
	DDX_Control(pDX, IDC_SCROLLBAR6, m_Scroll6);
	DDX_Control(pDX, IDC_SCROLLBAR5, m_Scroll5);
	DDX_Control(pDX, IDC_SCROLLBAR4, m_Scroll4);
	DDX_Control(pDX, IDC_SCROLLBAR3, m_Scroll3);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_Scroll2);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_Scroll1);
	//}}AFX_DATA_MAP

}

void CCGwOpenGLDlg::OnDeltaposSpinX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CSpinButtonCtrl *pclSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SPINX);
	static int oldPos=0;
	int pos=pclSpin->GetPos(); 
	if (pos>oldPos) 
		m_pclGLView->xAngle+=5;
	else
		m_pclGLView->xAngle-=5;
	
	m_pclGLView->vDrawGLScene();
    oldPos=pos;
	*pResult = 0;
}
void CCGwOpenGLDlg::OnDeltaposSpinY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CSpinButtonCtrl *pclSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SPINY);
	static int oldPos=0;
	int pos=pclSpin->GetPos(); 
	if (pos>oldPos) 
		m_pclGLView->yAngle+=5;
	else
		m_pclGLView->yAngle-=5;
	
	m_pclGLView->vDrawGLScene();
    oldPos=pos;
	*pResult = 0;
}
void CCGwOpenGLDlg::OnDeltaposSpinZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CSpinButtonCtrl *pclSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SPINZ);
	static int oldPos=0;
	int pos=pclSpin->GetPos(); 
	if (pos>oldPos) 
		m_pclGLView->zAngle+=5;
	else
		m_pclGLView->zAngle-=5;
	
	m_pclGLView->vDrawGLScene();
    oldPos=pos;
	*pResult = 0;
}
void CCGwOpenGLDlg::OnDeltaposShiftX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CSpinButtonCtrl *pclSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SHIFTX);
	static int oldPos=0;
	int pos=pclSpin->GetPos(); 
	if (pos>oldPos) 
		m_pclGLView->xShift+=0.25;
	else
		m_pclGLView->xShift-=0.25;
	
	m_pclGLView->vDrawGLScene();
    oldPos=pos;
	*pResult = 0;
}
void CCGwOpenGLDlg::OnDeltaposShiftY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CSpinButtonCtrl *pclSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SHIFTY);
	static int oldPos=0;
	int pos=pclSpin->GetPos(); 
	if (pos>oldPos) 
		m_pclGLView->yShift+=0.25;
	else
		m_pclGLView->yShift-=0.25;
	
	m_pclGLView->vDrawGLScene();
    oldPos=pos;
	*pResult = 0;
}
void CCGwOpenGLDlg::OnDeltaposShiftZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	CSpinButtonCtrl *pclSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SHIFTZ);
	static int oldPos=0;
	int pos=pclSpin->GetPos(); 
	if (pos>oldPos) 
		m_pclGLView->zShift+=0.25;
	else
		m_pclGLView->zShift-=0.25;
	
	m_pclGLView->vDrawGLScene();
    oldPos=pos;
	*pResult = 0;
}

void CCGwOpenGLDlg::OnBnClickedGrow()
{
	m_pclGLView->onGrow = !m_pclGLView->onGrow;
}
void CCGwOpenGLDlg::OnBnClickedLight()
{
	m_pclGLView->onLight = !m_pclGLView->onLight;
}
void CCGwOpenGLDlg::OnBnClickedDrawTube()
{
	m_pclGLView->onDrawTube = !m_pclGLView->onDrawTube;
}
void CCGwOpenGLDlg::OnBnClickedFrenetFrame()
{
	m_pclGLView->onDrawFrame = !m_pclGLView->onDrawFrame;
}
void CCGwOpenGLDlg::OnBnClickedDrawCurve()
{
	m_pclGLView->onDrawCurve =!m_pclGLView->onDrawCurve;
}
void CCGwOpenGLDlg::OnBnClickedDrawAxis()
{
	m_pclGLView->onDrawAxis = !m_pclGLView->onDrawAxis;
}
void CCGwOpenGLDlg::OnBnClickedDrawShadow()
{
	m_pclGLView->onShadow = !m_pclGLView->onShadow;
}
void CCGwOpenGLDlg::OnBnClickedDrawReflection()
{
	m_pclGLView->onReflection = !m_pclGLView->onReflection;
}
void CCGwOpenGLDlg::OnBnClickedReset()
{
	//reset lookat scroll bars
	for(int i=0;i<11;i++)
      m_pclGLView->ScrollValue[i]=0.0;

    m_pclGLView->ScrollValue[2]=10.0;
    m_pclGLView->ScrollValue[7]=2.0;
	
	//reset light scrollbar
	m_pclGLView->ScrollValue[9]=4.0;
	m_pclGLView->ScrollValue[10]=4.0;
	m_pclGLView->ScrollValue[11]=4.0;
	
	m_Scroll1.SetScrollPos(100);
	m_Scroll2.SetScrollPos(100);
	m_Scroll3.SetScrollPos(200);
	m_Scroll4.SetScrollPos(100);
	m_Scroll5.SetScrollPos(100);
	m_Scroll6.SetScrollPos(100);
	m_Scroll7.SetScrollPos(100);
	m_Scroll8.SetScrollPos(120);
	m_Scroll9.SetScrollPos(100);
	//Light position
	m_Scroll10.SetScrollPos(100);
	m_Scroll11.SetScrollPos(100);
	m_Scroll12.SetScrollPos(100);

	//reset flags
	m_pclGLView->onGrow=0;
	m_pclGLView->onLight=0;
	m_pclGLView->onDrawCurve=0;
	m_pclGLView->onDrawFrame=0;
	m_pclGLView->onDrawTube=0;
	m_pclGLView->onShadow=0;
	m_pclGLView->onReflection=0;
	
	//restet rotations/translations
	m_pclGLView->xAngle = 0.0f;
	m_pclGLView->yAngle = 0.0f;
	m_pclGLView->zAngle = 0.0f;
	m_pclGLView->xShift = 0.0f;
	m_pclGLView->yShift = 0.0f;
	m_pclGLView->zShift = 0.0f;

	//reset growth
	m_pclGLView->maxAngle = 0.0f;
	
	m_pclGLView->controlPoints[0]=2.0;
	m_pclGLView->controlPoints[1]=4.5;
	m_pclGLView->controlPoints[2]=0.0;
	m_pclGLView->controlPoints[3]=0.0;
}
void CCGwOpenGLDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

   // Get the minimum and maximum scroll-bar positions.
   int minpos;
   int maxpos;
   pScrollBar->GetScrollRange(&minpos, &maxpos); 
   maxpos = pScrollBar->GetScrollLimit();

   // Get the current position of scroll box.
   int curpos = pScrollBar->GetScrollPos();

   // Determine the new position of scroll box.
   switch (nSBCode)
   {
   case SB_LEFT:      // Scroll to far left.
      curpos = minpos;
      break;

   case SB_RIGHT:      // Scroll to far right.
      curpos = maxpos;
      break;

   case SB_ENDSCROLL:   // End scroll.
      break;

   case SB_LINELEFT:      // Scroll left.
      if (curpos > minpos)
         curpos--;
      break;

   case SB_LINERIGHT:   // Scroll right.
      if (curpos < maxpos)
         curpos++;
      break;

   case SB_PAGELEFT:    // Scroll one page left.
   {
      // Get the page size. 
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
   
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);
   }
      break;

   case SB_PAGERIGHT:      // Scroll one page right.
   {
      // Get the page size. 
      SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);
   }
      break;

   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
      curpos = nPos;      // of the scroll box at the end of the drag operation.
      break;

   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
      curpos = nPos;     // position that the scroll box has been dragged to.
      break;
   }

   // Set the new position of the thumb (scroll box).
   //pScrollBar->SetScrollPos(curpos);
	

	if (pScrollBar==&m_Scroll1)
		m_Scroll1.SetScrollPos(curpos);
	m_pclGLView->ScrollValue[0] = (m_Scroll1.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll2)
		m_Scroll2.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[1]=(m_Scroll2.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll3)
		m_Scroll3.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[2]=(m_Scroll3.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll4)
		m_Scroll4.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[3]=(m_Scroll4.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll5)
		m_Scroll5.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[4]=(m_Scroll5.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll6)
		m_Scroll6.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[5]=(m_Scroll6.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll7)
		m_Scroll7.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[6]=(m_Scroll7.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll8)
		m_Scroll8.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[7]=(m_Scroll8.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll9)
		m_Scroll9.SetScrollPos(curpos);
    m_pclGLView->ScrollValue[8]=(m_Scroll9.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll10)
		m_Scroll10.SetScrollPos(curpos);
	m_pclGLView->ScrollValue[9]=(m_Scroll10.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll11)
		m_Scroll11.SetScrollPos(curpos);
	m_pclGLView->ScrollValue[10]=(m_Scroll11.GetScrollPos()-100)/10;

	if (pScrollBar==&m_Scroll12)
		m_Scroll12.SetScrollPos(curpos);
	m_pclGLView->ScrollValue[11]=(m_Scroll12.GetScrollPos()-100)/10;

    UpdateData(FALSE);
////L9! b
//    m_pclGLView->intOptionC=0;
//L9! e
    m_pclGLView->vDrawGLScene();

	// TODO: Add your message handler code here and/or call default
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

}
