
// CameraTrial2Dlg.h : header file
//

#include "afxwin.h"
#define IMAGE_MODE_GRAY 2
#define IMAGE_MODE_RGB24 1
#define IMAGE_MODE_RGB32 3

#pragma once
void  SaveBMP(BYTE* inbytes, TCHAR* fileName, int mode, int width, int height, int roiX, int roiY, int roiWidth, int roiHeight);
void ReleaseImageBuffer(unsigned char* bytes);
void Show(BYTE* bytes, int mode, int iwidth, int iheight, int x, int y, HDC hdc);
unsigned char* Scale(BYTE* bytes, int mode, int iwidth, int iheight, int newwidth, int newheight);
#pragma once


// CCameraTrial2Dlg dialog
class CCameraTrial2Dlg : public CDialogEx
{
// Construction
public:
	CCameraTrial2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERATRIAL2_DIALOG };
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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton img_window;
	CStatic m_video_window;
	CStatic* video_window;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
};
