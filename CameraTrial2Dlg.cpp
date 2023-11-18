
// CameraTrial2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CameraTrial2.h"
#include "CameraTrial2Dlg.h"
#include "afxdialogex.h"
#include "bgapi2_genicam.hpp"
using namespace BGAPI2;
using namespace std;
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CStatic* g_baumerVideoWnd;
void printNodeRecursive(BGAPI2::Node* pNode, int level);
unsigned char* LoadImageFromFile(const TCHAR fileName[], int mode, int* iwidth, int* iheight);

//variable declaration 
BGAPI2::SystemList* systemList = NULL;
BGAPI2::System* pSystem = NULL;
BGAPI2::String sSystemID;

//Interface List and Interface Variables
BGAPI2::InterfaceList* interfaceList = NULL;
BGAPI2::Interface* pInterface = NULL;
BGAPI2::String sInterfaceID;

//DeviceList and Device Variabes
BGAPI2::DeviceList* deviceList = NULL;
BGAPI2::Device* pDevice = NULL;
BGAPI2::String sDeviceID;

//DataStream List and DataStream Variables
BGAPI2::DataStreamList* datastreamList = NULL;
BGAPI2::DataStream* pDataStream = NULL;
BGAPI2::String sDataStreamID;

//BufferList and Buffer Variables
BGAPI2::BufferList* bufferList = NULL;
BGAPI2::Buffer* pBuffer = NULL;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCameraTrial2Dlg dialog



CCameraTrial2Dlg::CCameraTrial2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAMERATRIAL2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCameraTrial2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, img_window);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_video_window);
}

BEGIN_MESSAGE_MAP(CCameraTrial2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCameraTrial2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCameraTrial2Dlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CCameraTrial2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCameraTrial2Dlg message handlers

BOOL CCameraTrial2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	video_window = (CStatic*)GetDlgItem(IDC_STATIC_IMAGE);
	g_baumerVideoWnd = video_window;
	

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCameraTrial2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCameraTrial2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCameraTrial2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


ofstream file("trial2.txt");
void CCameraTrial2Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	

	//creating a file in write  mode
	

	//variable declarations
	//System List and System Variables
	

	//Instatiating and Updating SystemList
	try
	{
		systemList = SystemList::GetInstance();
		systemList->Refresh();
		file << "**********************************************************" << endl;
		file<< "5.1.2 Detected systems: " << systemList->size() << endl;
		file << "**********************************************************" << endl;
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		file << "ExceptionType: " << ex.GetType() << endl;
		file << "ErrorDescription: " << ex.GetErrorDescription() << endl;
		file << "in function: " << ex.GetFunctionName() << endl;
	}

	
	//search for system
	for (SystemList::iterator sys = systemList->begin(); sys != systemList->end(); sys++)
	{
            //getting system information
			file << endl;
			file << "System Name:" << sys->second->GetFileName() << endl;
			file << "System Type:" << sys->second->GetTLType() << endl;
			file << "System Version:" << sys->second->GetVersion() << endl;
			file << endl;
		
		sys->second->Open();
		sSystemID = sys->first;
		//break;
	}

	//If systemID is availabe assign pointer
	if (sSystemID == "")
		return ; // no system found
	else
		pSystem = (*systemList)[sSystemID];

	//get interface list for selected system and update it
	interfaceList = pSystem->GetInterfaces();
	interfaceList->Refresh(100);
	file << "**********************************************************" << endl;
	file << "5.1.4 Detected interfaces: " << interfaceList->size() << endl;
	file << "**********************************************************" << endl;
	file << endl;

	//opening and inteface
	for (InterfaceList::iterator ifc = interfaceList->begin(); ifc != interfaceList->end(); ifc++)
	{
		//getting interface information 
		file << "Interface ID: " << ifc->first << endl;
		file << "Interface Type: " << ifc->second->GetTLType() << endl;
		file << "Interface Name: " << ifc->second->GetDisplayName() << endl;
		file << endl;
		ifc->second->Open();
		sInterfaceID = ifc->first;
		break;
	}

	//if interface ID available assign pointer
	if (sInterfaceID == "")
		return ; // no interface found
	else
		pInterface = (*interfaceList)[sInterfaceID];



	//Device List for selected Interface
	deviceList = pInterface->GetDevices();
	deviceList->Refresh(100);
	file << "**********************************************************" << endl;
	file << "5.1.6 Detected devices: " << deviceList->size() << endl;
	file << "**********************************************************" << endl;
	file << endl;
	//open device
	for (DeviceList::iterator dev = deviceList->begin(); dev != deviceList->end(); dev++)
	{
		//device information 
		file << "Device DeviceID:" << dev->first << endl;
		file << "Device Model:" << dev->second->GetModel() << endl;
		file << "Device SerialNumber:" << dev->second->GetSerialNumber() << endl;
		file << "Device Vendor:" << dev->second->GetVendor() << endl;
		file << "Device TLType:" << dev->second->GetTLType() << endl;
		file << "Device UserID:" << dev->second->GetDisplayName() << endl;
		file << endl;

		//opne device
		dev->second->Open();
		sDeviceID = dev->first;
		break;
	}

	//DeviceID filled Assign pointer
	if (sDeviceID == "")
		return ; // no device found
	else
		pDevice = (*deviceList)[sDeviceID];

	//Get datastream list and fill it 
	datastreamList = pDevice->GetDataStreams();
	datastreamList->Refresh();
	file << "**********************************************************" << endl;
	file << "5.1.8 Detected datastreams: " << datastreamList->size() << endl;
	file << "**********************************************************" << endl;
	file << endl;

	//open datastream 
	for (DataStreamList::iterator dst = datastreamList->begin(); dst != datastreamList->end(); dst++)
	{
		//getting datastream information
		
		file << "DataStream ID: " << dst->first << endl;
		file << endl;

		//open datastream
		dst->second->Open();
		sDataStreamID = dst->first;
		break;
	}

	//DataStreamID there assign pointer
	if (sDataStreamID == "")
		return ; // no datastream found
	else
		pDataStream = (*datastreamList)[sDataStreamID];

	//create empty bufferlist and fill with buffers
	bufferList = pDataStream->GetBufferList();
	for (int i = 0; i < 4; i++) // 4 buffers using internal buffers
	{
		pBuffer = new BGAPI2::Buffer();
		bufferList->Add(pBuffer);
	}
	file << "**********************************************************" << endl;
	file << "5.1.10 Announced buffers: " << bufferList->size() << endl;
	file << "**********************************************************" << endl;

	//allocate buffer to datastream
	for (BufferList::iterator buf = bufferList->begin(); buf != bufferList->end(); buf++)
	{
		buf->second->QueueBuffer();
	}
	file << "**********************************************************" << endl;
	file << "5.1.11 Queued buffers: " << bufferList->GetQueuedCount() << endl;
	file << "**********************************************************" << endl;

	BGAPI2::NodeMap* nmNodetree = pDevice->GetRemoteNodeTree();

	for (bo_uint64 i = 0; i < nmNodetree->GetNodeCount(); i++)
	{
		BGAPI2::Node* nNode = nmNodetree->GetNodeByIndex(i);
		file << nNode->GetInterface() << "  " << nNode->GetName() << endl;
	}
	//--------------------------------------//

	//----AcquisitionFrameRateEnable---//
	BGAPI2::Node* pNode = NULL;
	BGAPI2::NodeMap* pNodeTree = NULL;

	file << "DEVICE NODE TREE" << endl;
	file << "5.3.1   NodeTree Count: " << pDevice->GetRemoteNodeTree()->GetNodeCount() << endl << endl;

	pNodeTree = pDevice->GetRemoteNodeTree();

	for (bo_uint64 i = 0; i < pDevice->GetRemoteNodeTree()->GetNodeCount(); i++)
	{
		pNode = pNodeTree->GetNodeByIndex(i);
		printNodeRecursive(pNode, 0);
	}


	bo_int64 width = 0;
	bo_int64 height = 0;

	
	
		pDevice->GetRemoteNode("AcquisitionFrameRateEnable")->SetBool(true); // disble to get max frame rate
		pDevice->GetRemoteNode("AcquisitionFrameRate")->SetDouble(250);      // frame rate set to manually. if you want to set it manually you need to make AcquisitionFrameRateEnable true (above step)

		pDevice->GetRemoteNode("ExposureTime")->SetDouble(128000);             // Exposure of camera

		pDevice->GetRemoteNode("Gain")->SetDouble(9);                        // Gain

		//if (pDevice->GetRemoteNode("Width")->GetLocked() == false)
		//{
		//	pDevice->GetRemoteNode("OffsetX")->SetInt(0);                   // ROI Offset X
		//	pDevice->GetRemoteNode("Width")->SetInt(1280);                  // ROI Width
		//}
		//
		//if (pDevice->GetRemoteNode("Height")->GetLocked() == false)
		//{
		//	pDevice->GetRemoteNode("OffsetY")->SetInt(0);                  // ROI Offset Y
		//	pDevice->GetRemoteNode("Height")->SetInt(1024);                // ROI Height
		//}

	

	width = pDevice->GetRemoteNode("Width")->GetInt();                 // Taking the width of image
	height = pDevice->GetRemoteNode("Height")->GetInt();               // Taking the height of image

	


	

	//start camera and fill buffer
	//start datastream
	// if camera supports TLParamsLocked it is set automatically now
	pDataStream->StartAcquisitionContinuous();
	// CANNOT change payloadsize relevant features now
	if (pDevice->GetRemoteNode("Height")->IsWriteable() == true)
	{
		// safe way to check if feature is locked by TLParamsLocked
	}
	// if camera supports TLParamsLocked it is reset automatically now
	pDataStream->StopAcquisition();
	// can change payloadsize relevant features now
	if (pDevice->GetRemoteNode("Height")->IsWriteable() == true)
	{
		// safe way to check if feature is locked by TLParamsLocked
	}
	pDataStream->StartAcquisitionContinuous();
	file << " Width of image = " << width << endl;
	file << " height of image = " << height << endl;
	//start camera
	pDevice->GetRemoteNode("AcquisitionStart")->Execute();

	//fill image buffer
	BGAPI2::Buffer* pBufferFilled = NULL;
	
	unsigned char* imagebuffer = NULL;
	unsigned char* dispbytes = NULL;

	int counter = 0;
	while (true)
	{
		try
		{
			pBufferFilled = pDataStream->GetFilledBuffer(1000);
			imagebuffer = (unsigned char*)pBufferFilled->GetMemPtr();
			if (pBufferFilled == NULL)
			{
				file << "Error: Buffer Timeout after 1000 msec" << endl;
			}
			else
			{
				counter++;
				file << " Image " << pBufferFilled->GetFrameID();
				file << " received in memory address " << pBufferFilled->GetMemPtr();
				file << endl;
				
				
				
				CClientDC dc(g_baumerVideoWnd);
				CRect rect;
				g_baumerVideoWnd->GetClientRect(&rect);


				dispbytes = Scale(imagebuffer, IMAGE_MODE_GRAY, pBufferFilled->GetWidth(), pBufferFilled->GetHeight(), rect.right, rect.bottom);
				Show(dispbytes, IMAGE_MODE_GRAY, rect.right, rect.bottom, 0, 0, dc.m_hDC);
				ReleaseImageBuffer(dispbytes);

				pBufferFilled->QueueBuffer();

			}
		}
		catch (BGAPI2::Exceptions::IException& ex)
		{
			file << "ExceptionType: " << ex.GetType() << endl;
			file << "ErrorDescription: " << ex.GetErrorDescription() << endl;
			file << "in function: " << ex.GetFunctionName() << endl;
		}
	}
	//add buffer object into queue
	

	//stop camera
	pDevice->GetRemoteNode("AcquisitionStop")->Execute();

	//stop datastream and delete buffers
	pDataStream->StopAcquisition();
	bufferList->DiscardAllBuffers();
	while (bufferList->size() > 0)
	{
		pBuffer = bufferList->begin()->second;
		bufferList->RevokeBuffer(pBuffer);
		delete pBuffer;
	}

	//releasing resources
	pDataStream->Close();
	pDevice->Close();
	pInterface->Close();
	pSystem->Close();
	BGAPI2::SystemList::ReleaseInstance();

	//code to prevent application from closing 
	int endKey = 0;
	file << "Input any number to close the program:";
	std::cin >> endKey;
	return ;





	//closing file
	//file.close();

	//CDialogEx::OnOK();
}


void CCameraTrial2Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();

}




void printNodeRecursive(BGAPI2::Node* pNode, int level)
{
	int white_spaces = level * 7 + 1;
	for (int i = 0; i < white_spaces; i++) file << " ";

	if (pNode->GetInterface() == "ICategory")
	{
		file << "[" << std::left << std::setw(12) << pNode->GetInterface() << std::right << "]";
		file << " " << pNode->GetName() << endl;
		for (bo_uint64 j = 0; j < pNode->GetNodeTree()->GetNodeCount(); j++)
		{
			BGAPI2::Node* nSubNode = pNode->GetNodeTree()->GetNodeByIndex(j);
			printNodeRecursive(nSubNode, level + 1);
		}
	}
	else
	{
		try
		{
			file << "[" << std::left << std::setw(12) << pNode->GetInterface() << std::right << "]";
			file << " " << std::left << std::setw(44) << pNode->GetName() << std::right;
			if ((pNode->GetImplemented() == true) && (pNode->GetAvailable() == true) &&
				(pNode->GetLocked() == false) && (pNode->GetVisibility() != "Invisible") &&
				((pNode->GetCurrentAccessMode() == "RW") || (pNode->GetCurrentAccessMode() == "RO")))
			{
				if (pNode->GetInterface() == "IBoolean")
				{
					file << ": " << pNode->GetValue();
				}
				if (pNode->GetInterface() == "IEnumeration")
				{
					file << ": " << pNode->GetValue();
				}
				if (pNode->GetInterface() == "IFloat")
				{
					file << ": " << pNode->GetValue();
				}
				if (pNode->GetInterface() == "IInteger")
				{
					file << ": " << pNode->GetValue();
				}
				if (pNode->GetInterface() == "IString")
				{
					file << ": " << pNode->GetValue();
				}
			}
		}
		catch (BGAPI2::Exceptions::IException& ex)
		{
			file << "ExceptionType:    " << ex.GetType() << endl;
			file << "ErrorDescription: " << ex.GetErrorDescription() << endl;
			file << "in function:      " << ex.GetFunctionName() << endl;
		}
	}
	file << " " << endl;
	return;
}

void ReleaseImageBuffer(unsigned char* bytes)
{
	if (bytes != NULL)
	{
		delete[] bytes;
	}
	bytes = NULL;
}

void Show(BYTE* bytes, int mode, int iwidth, int iheight, int x, int y, HDC hdc)
{

	BYTE* dib;
	int w_bytes = (((iwidth * 24) + 31) & -31) >> 3;
	dib = new BYTE[w_bytes * iheight];

	BITMAPINFO bi;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biHeight = iheight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = iwidth;

	int ubytes = (int)((iwidth * (24.0 / 8.0)) + 0.5);
	int padding = (ubytes % 4 == 0) ? 0 : 4 - (ubytes % 4);


	int i, j, k, k2;
	k = 0;
	k2 = 0;

	if (mode == IMAGE_MODE_GRAY)
	{
		for (j = 0; j < iheight; j++)
		{
			for (i = 0; i < iwidth; i++)
			{
				*(dib + k2) = bytes[k];
				*(dib + k2 + 1) = bytes[k];
				*(dib + k2 + 2) = bytes[k];

				k2 = k2 + 3;
				k++;

			}
			k2 = k2 + padding;
		}
	}
	else if (mode == IMAGE_MODE_RGB32)
	{
		for (j = 0; j < iheight; j++)
		{
			for (i = 0; i < iwidth; i++)
			{
				*(dib + k2) = bytes[j * iwidth * 4 + i * 4];
				k++;
				*(dib + k2 + 1) = bytes[j * iwidth * 4 + i * 4 + 1];
				k++;
				*(dib + k2 + 2) = bytes[j * iwidth * 4 + i * 4 + 2];
				k++;
				k2 = k2 + 3;
			}
			k2 = k2 + padding;
		}

	}
	else if (mode == IMAGE_MODE_RGB24)
	{
		for (j = 0; j < iheight; j++)
		{
			for (i = 0; i < iwidth; i++)
			{
				*(dib + k2) = bytes[j * iwidth * 3 + i * 3];
				k++;
				*(dib + k2 + 1) = bytes[j * iwidth * 3 + i * 3 + 1];
				k++;
				*(dib + k2 + 2) = bytes[j * iwidth * 3 + i * 3 + 2];
				k++;
				k2 = k2 + 3;
			}
			k2 = k2 + padding;
		}

	}
	SetDIBitsToDevice(hdc, x, y, iwidth, iheight, 0, 0, 0, iheight, dib, &bi, DIB_RGB_COLORS);
	//StretchDIBits(hdc, 0,0, iwidth, iheight, 0,0, iwidth, iheight, dib, &bi, DIB_RGB_COLORS,SRCCOPY);
	delete[] dib;
}

unsigned char* Scale(BYTE* bytes, int mode, int iwidth, int iheight, int newwidth, int newheight)
{
	unsigned char* newbytes = NULL;

	if (mode == IMAGE_MODE_RGB32)
	{
		newbytes = new unsigned char[newwidth * newheight * 4];

		double xfactor = ((double)iwidth) / newwidth;
		double yfactor = ((double)iheight) / newheight;

		//ofstream ofs("c:\\text.txt");
		//ofs << xfactor << "  " << yfactor << endl;
		//ofs.close ();

		int i, j;

		for (j = 0; j < newheight; j++)
		{
			for (i = 0; i < newwidth; i++)
			{
				newbytes[j * newwidth * 4 + i * 4] = bytes[(int)(j * yfactor) * iwidth * 4 + (int)(i * xfactor) * 4];
				newbytes[j * newwidth * 4 + i * 4 + 1] = bytes[(int)(j * yfactor) * iwidth * 4 + (int)(i * xfactor) * 4 + 1];
				newbytes[j * newwidth * 4 + i * 4 + 2] = bytes[(int)(j * yfactor) * iwidth * 4 + (int)(i * xfactor) * 4 + 2];
				//newbytes[ j*newwidth*4 + i*4 ]  = bytes[ (int)j ];
			}
		}
	}
	else if (mode == IMAGE_MODE_GRAY)
	{
		double aspratio = ((double)iwidth) / iheight;

		newbytes = new unsigned char[newwidth * newheight];

		double xfactor = ((double)iwidth) / newwidth;
		//double yfactor = ((double)iheight)/newheight;

		int i, j;
		for (j = 0; j < newheight; j++)
		{
			for (i = 0; i < newwidth; i++)
			{
				if (j * xfactor < iheight && i * xfactor < iwidth)
				{
					newbytes[j * newwidth + i] = bytes[(int)(j * xfactor) * iwidth + (int)(i * xfactor)];
				}
			}
		}
	}

	return newbytes;


}

unsigned char* LoadImageFromFile(const TCHAR fileName[], int mode, int* iwidth, int* iheight)
{
	int i, j, k;
	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hbitmap == NULL)
	{
		return FALSE;
	}

	BITMAP bitmap;
	GetObject(hbitmap, sizeof(BITMAP), &bitmap);

	*iwidth = bitmap.bmWidth;
	*iheight = bitmap.bmHeight;


	int w_bytes = (((*iwidth * 24) + 31) & -31) >> 3;
	int h = *iheight;
	int padding = w_bytes - *iwidth * 3;

	BYTE* dib = new BYTE[w_bytes * h];


	BITMAPINFO bi;

	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biHeight = *iheight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = *iwidth;


	HDC hmemdc = CreateCompatibleDC(NULL);
	SelectObject(hmemdc, hbitmap);

	GetDIBits(hmemdc, hbitmap, 0, *iheight, dib, &bi, DIB_RGB_COLORS);

	DeleteObject(hbitmap);

	unsigned char* bytes = NULL;

	if (mode == IMAGE_MODE_RGB24)
	{
		bytes = (BYTE*)malloc((*iwidth) * (*iheight) * 3);

		k = 0;
		int gk = 0;


		for (j = 0; j < *iheight; j++)
		{
			for (i = 0; i < *iwidth; i++)
			{
				int blue = *(dib + k);
				int green = *(dib + k + 1);
				int red = *(dib + k + 2);

				int avg = (red + green + blue) / 3;

				bytes[gk] = blue;
				gk++;
				bytes[gk] = green;
				gk++;
				bytes[gk] = red;
				gk++;

				k = k + 3;

			}
			k = k + padding;
		}

	}
	if (mode == IMAGE_MODE_RGB32)
	{
		bytes = (BYTE*)malloc((*iwidth) * (*iheight) * 4);

		k = 0;
		int gk = 0;


		for (j = 0; j < *iheight; j++)
		{
			for (i = 0; i < *iwidth; i++)
			{
				int blue = *(dib + k);
				int green = *(dib + k + 1);
				int red = *(dib + k + 2);

				int avg = (red + green + blue) / 3;

				bytes[gk] = blue;
				gk++;
				bytes[gk] = green;
				gk++;
				bytes[gk] = red;
				gk++;
				gk++;

				k = k + 3;

			}
			k = k + padding;
		}

	}
	else if (mode == IMAGE_MODE_GRAY)
	{

		bytes = (BYTE*)malloc((*iwidth) * (*iheight));

		k = 0;
		int gk = 0;


		for (j = 0; j < *iheight; j++)
		{
			for (i = 0; i < *iwidth; i++)
			{
				int blue = *(dib + k);
				int green = *(dib + k + 1);
				int red = *(dib + k + 2);

				int avg = (red + green + blue) / 3;

				bytes[gk] = avg;

				k = k + 3;
				gk++;
			}
			k = k + padding;
		}
	}

	delete[] dib;
	DeleteDC(hmemdc);
	DeleteObject(hbitmap);
	return bytes;
}
void  SaveBMP(BYTE* inbytes, TCHAR* fileName, int mode, int width, int height, int roiX, int roiY, int roiWidth, int roiHeight)
{

	int imgsize = (((roiWidth * 24 + 31) & ~31) >> 3) * roiHeight;
	int padding = (((roiWidth * 24 + 31) & ~31) >> 3) - roiWidth * 3;
	int filesize = imgsize + 54;

	BITMAPFILEHEADER* fh = (BITMAPFILEHEADER*)malloc(filesize);
	BITMAPINFO* fi = (BITMAPINFO*)(fh + 1);
	BYTE* bytes = (BYTE*)fh + 54;

	fh->bfOffBits = 54;
	fh->bfReserved1 = 0;
	fh->bfReserved2 = 0;
	fh->bfSize = filesize;
	fh->bfType = 19778;


	fi->bmiHeader.biBitCount = 24;
	fi->bmiHeader.biClrImportant = 0;
	fi->bmiHeader.biClrUsed = 0;
	fi->bmiHeader.biCompression = 0;
	fi->bmiHeader.biHeight = roiHeight;
	fi->bmiHeader.biPlanes = 1;
	fi->bmiHeader.biSize = 40;
	fi->bmiHeader.biSizeImage = imgsize;
	fi->bmiHeader.biWidth = roiWidth;
	fi->bmiHeader.biXPelsPerMeter = 0;
	fi->bmiHeader.biYPelsPerMeter = 0;

	int i, j, k, k2;
	k = 0;
	k2 = 0;

	if (mode == IMAGE_MODE_RGB32)
	{
		for (j = roiY; j < roiY + roiHeight; j++)
		{
			for (i = roiX; i < roiX + roiWidth; i++)
			{
				bytes[k] = inbytes[j * width * 4 + i * 4];
				k++;
				bytes[k] = inbytes[j * width * 4 + i * 4 + 1];
				k++;
				bytes[k] = inbytes[j * width * 4 + i * 4 + 2];
				k++;
				//k2++;
			}
			k = k + padding;
		}
	}
	else if (mode == IMAGE_MODE_GRAY)
	{
		for (j = roiY; j < roiY + roiHeight; j++)
		{
			for (i = roiX; i < roiX + roiWidth; i++)
			{
				bytes[k] = inbytes[j * width + i];
				k++;
				bytes[k] = inbytes[j * width + i];
				k++;
				bytes[k] = inbytes[j * width + i];
				k++;
			}
			k = k + padding;
		}
	}
	if (mode == IMAGE_MODE_RGB24)
	{
		for (j = roiY; j < roiY + roiHeight; j++)
		{
			for (i = roiX; i < roiX + roiWidth; i++)
			{
				bytes[k] = inbytes[j * width * 3 + i * 3];
				k++;
				bytes[k] = inbytes[j * width * 3 + i * 3 + 1];
				k++;
				bytes[k] = inbytes[j * width * 3 + i * 3 + 2];
				k++;
				//k2++;
			}
			k = k + padding;
		}
	}

	//CString Str_image = "C:\\SW\\Images\\HXC.bmp";
	//Str_image.Format("C:\\SW\\Images\\HXC%d.bmp", savecntr++);
	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD byteswritten;
	WriteFile(hFile, fh, fh->bfSize, &byteswritten, NULL);
	CloseHandle(hFile);
	free(fh);
}

void CCameraTrial2Dlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	exit(0);
}


void CCameraTrial2Dlg::OnBnClickedButton1()
{
	SYSTEMTIME sr;

	CString Temp;

	GetLocalTime(&sr);

	Temp.Format("%d %d %d %d", sr.wHour, sr.wMinute, sr.wSecond, sr.wMilliseconds);
	

	ofstream date("trial3.txt");
	date << Temp<<endl;


	// TODO: Add your control notification handler code here
	unsigned char* imagebuffer = NULL;
	unsigned char* dispbytes = NULL;
	int width, height = 0;
	CClientDC dc(g_baumerVideoWnd);
	CRect rect;
	g_baumerVideoWnd->GetClientRect(&rect);
	imagebuffer = LoadImageFromFile("D:\\Intership\\Renovus\\Camera_python\\PyInt\\Integrate\\trial\\edge_image.bmp", IMAGE_MODE_GRAY, &width, &height);
	// code
	
	//imagebuffer[j * width + i] = 100;
	imagebuffer[(height -100) * width + 100] = 100;
	

	//code
	dispbytes = Scale(imagebuffer, IMAGE_MODE_GRAY, width, height, rect.right, rect.bottom);
	Show(dispbytes, IMAGE_MODE_GRAY, rect.right, rect.bottom, 0, 0, dc.m_hDC);
	SaveBMP(imagebuffer, "D:\\Intership\\Renovus\\Camera_python\\PyInt\\Integrate\\trial\\pros.bmp", IMAGE_MODE_GRAY, width, height, 0, 0, width, height);
	
	
	ReleaseImageBuffer(dispbytes);


	GetLocalTime(&sr);
	Temp.Format("%d %d %d %d", sr.wHour, sr.wMinute, sr.wSecond, sr.wMilliseconds);
	date << Temp;
	
	
	
}
