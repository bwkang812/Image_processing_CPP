
// VideoProcessorDlg.cpp : 구현 파일
//

#include <stdio.h>
#include "stdafx.h"
#include "VideoProcessor.h"
#include "VideoProcessorDlg.h"
#include "afxdialogex.h"
#include "Features.h"
#include <memory.h>

#include "Segmentation.h"
#include "MyImage.h"
#include "MyImageFunc.h"
#include "ImageFrameWnd.h"
#include "ImageFrameWndManager.h"
#include "LoadImageFromFileDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CVideoProcessorDlg 대화 상자



CVideoProcessorDlg::CVideoProcessorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIDEOPROCESSOR_DIALOG, pParent)
	, m_nThreshold(45)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoProcessorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THRES, m_nThreshold);
	DDV_MinMaxInt(pDX, m_nThreshold, 0, 255);
	DDX_Control(pDX, IDC_SLIDER_THRES, m_sliderThres);
}

BEGIN_MESSAGE_MAP(CVideoProcessorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_CAM, &CVideoProcessorDlg::OnBnClickedButtonStartCam)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CAM, &CVideoProcessorDlg::OnBnClickedButtonStopCam)
	ON_BN_CLICKED(IDC_BUTTON_REC_CAM, &CVideoProcessorDlg::OnBnClickedButtonRecCam)
	ON_BN_CLICKED(IDC_BUTTON_STOP_REC, &CVideoProcessorDlg::OnBnClickedButtonStopRec)
	ON_BN_CLICKED(IDC_BUTTON_BINARIZTION, &CVideoProcessorDlg::OnBnClickedButtonBinariztion)
	ON_BN_CLICKED(IDC_BUTTON_ViewImage, &CVideoProcessorDlg::OnBnClickedButtonViewimage)
	ON_BN_CLICKED(IDC_BUTTON_Close_Image, &CVideoProcessorDlg::OnBnClickedButtonCloseImage)
END_MESSAGE_MAP()


// CVideoProcessorDlg 메시지 처리기

BOOL CVideoProcessorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_sliderThres.SetRange(0, 255);
	m_sliderThres.SetPos(m_nThreshold);







	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CVideoProcessorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CVideoProcessorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CVideoProcessorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 카메라 영상 및 정보를 저장하기 위한 전역 변수
BITMAPINFO gBmpInfo;    // *콜백함수에서 살펴보아야 하므로 전역 변수로 선언한다. 
CByteImage gImageBuf;
CByteImage m_imageOut;
CByteImage d_imageOut;


// 물체 검출 관련 전역 변수
CByteImage	gBgImage;
CByteImage	gBinImage;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT ProcessCamFrame(HWND hWnd, LPVIDEOHDR lpVHdr)
{

	//픽셀 형식 변환----------------------------------------------------------------------------------
	if (gBmpInfo.bmiHeader.biCompression == BI_RGB) // RGB 영상인 경우에는, 그대로 복사.
	{
		memcpy(gImageBuf.GetPtr(), lpVHdr->lpData,
			3*gBmpInfo.bmiHeader.biHeight*gBmpInfo.bmiHeader.biWidth);  //*3을 곱해주어야 실제 높이이다. 
	}

	

	else if (gBmpInfo.bmiHeader.biCompression == MAKEFOURCC('Y', 'U', 'Y', '2')) // 16비트 영상(YUY2) 일경우에는 함수수행.
	{
		YUY2ToRGB24(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight,
			lpVHdr->lpData, gImageBuf.GetPtr());
	}
	
	else
	{
		return FALSE;
	}

	//gImageBuf.GetChannelImg(2);
    //ShowImage(gImageBuf.PEPSI(), "펩시_콜라");    //영상처리를 위해서 BITINFO 형식을 BMP 형식으로 변환하였다. 
	

	//------------------------------------------------------------------------------------------------
	
	//소벨검출

	/*
	CByteImage e_imageOut(gImageBuf);
	SobelEdge(RGB2Gray(gImageBuf), e_imageOut);
	*/

	/*
	//허프변환
	double arrRho[100];
	double arrTheta[100];
	int nLine = HoughLines(e_imageOut,227,100,1,100,arrRho,arrTheta);

	CByteImage	m_imageOut;
	m_imageOut = gImageBuf;

	// RGB직선 그리기
	for (int i = 0; i<nLine; i++)
	{
		if (arrTheta[i] == 90) // 수직선
		{
			DrawLine(m_imageOut, arrRho[i], 0, arrRho[i], m_imageOut.GetHeight() - 1, 255, 0, 0);
		}
		else
		{
			int x1 = 0;
			int y1 = (int)(arrRho[i] / cos(arrTheta[i] * M_PI / 180) + 0.5);
			int x2 = m_imageOut.GetWidth() - 1;
			int y2 = (int)((arrRho[i] - x2*sin(arrTheta[i] * M_PI / 180)) / cos(arrTheta[i] * M_PI / 180) + 0.5);
			DrawLine(m_imageOut, x1, y1, x2, y2, 255, 0, 0);
		}
	}
	ShowImage(m_imageOut, "직선 검출 결과");
	*/
	//---------------------------------------------------------------------------------------------


	


	//ShowImage(e_imageOut, "소벨검출"); 
	
	//AfxMessageBox("테스트");

	//-----------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------------
	

	                                   //영상처리 알고리즘//

	
	// 펩시(영상처리된 영상)를 똑바로 출력하기위하여 회색조로 변경한 후에 상하반전 시켰다.
	
	
	CByteImage c_imageOut(gImageBuf.PEPSI());
	CByteImage d_imageOut(gImageBuf.PEPSI());

	for (int r = 0; r < c_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < c_imageOut.GetWidth(); c++)
		{
			d_imageOut.GetAt(c, r) = c_imageOut.GetAt(c, c_imageOut.GetHeight() - 1 - r);
		}

	}

	//이진화된 영상의 (0.0)번 부터 (640.480)번 픽셀까지 접근하면서 콜라의 특징을 찾아냄
	//세로로 100개의 픽셀이 1 이면 콜라로 인식
	int _y=0;
	int _pepsi_Y_line_count=0;
	int _p_flag = 0;
	for (int r = 0; r < c_imageOut.GetHeight()-101 && _p_flag == 0; r++)
	{
		for (int c = 0; c < c_imageOut.GetWidth()-11 && _p_flag == 0; c++)
		{
			if (d_imageOut.GetAt(c, r) == 0)
			{
				_y = r;
				while (d_imageOut.GetAt(c, _y) == 0&& _pepsi_Y_line_count < 100&& _p_flag == 0)
				{
					_y++;
					_pepsi_Y_line_count++;
					if (_pepsi_Y_line_count == 100)
					{
						AfxMessageBox("펩시콜라가 검출 되었습니다.(By Line)");
						_p_flag = 1;
						break;
					}

		
				}
				_pepsi_Y_line_count = 0;
			}

		
		}

	}
	
	ShowImage(d_imageOut, "펩시콜라 검출");

	
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	// 사이다(영상처리된 영상)를 똑바로 출력하기위하여 회색조로 변경한 후에 상하반전 시켰다.

	
	CByteImage j_imageOut(gImageBuf.CIDER());
	CByteImage k_imageOut(gImageBuf.CIDER());

	for (int r = 0; r < j_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < j_imageOut.GetWidth(); c++)
		{
			k_imageOut.GetAt(c, r) = j_imageOut.GetAt(c, j_imageOut.GetHeight() - 1 - r);
		}

	}

	int _CIDER_Area_count = 0;
	int _c_flag = 0;
	int x__;
	int y__;

	for (int r = 0; r < j_imageOut.GetHeight()-136 && _c_flag == 0; r++)
	{
		for (int c = 49; c < j_imageOut.GetWidth()-49  && _c_flag == 0; c++)
		{

			if (k_imageOut.GetAt(c, r) == 0&& k_imageOut.GetAt(c-1, r) == 0&& k_imageOut.GetAt(c-2, r) == 0&& k_imageOut.GetAt(c-3, r) == 0&& k_imageOut.GetAt(c-4, r) == 0)
			{

				x__ = c;
				y__ = r;
				_CIDER_Area_count = 0;

				for (int k = x__-48; k < x__+48  && _c_flag == 0; k++)
				{
					for (int j = y__; j < y__+135 && _c_flag == 0; j++)
					{
						if (k_imageOut.GetAt(k, j) == 0 && _c_flag == 0)
						{
							_CIDER_Area_count++;
							if (_CIDER_Area_count > 400 && _c_flag == 0)
							{
								AfxMessageBox("사이다가 검출 되었습니다.(By Area)");
								_c_flag = 1;
								break;
							}

						}
					}

				}


			}

		}

	}


	ShowImage(k_imageOut, "사이다 검출");
	
	
	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	// 조리퐁(영상처리된 영상)를 똑바로 출력하기위하여 회색조로 변경한 후에 상하반전 시켰다.

   
	
	CByteImage l_imageOut(gImageBuf.JOLLY_PONG());
	CByteImage m_imageOut(gImageBuf.JOLLY_PONG());

	for (int r = 0; r < l_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < l_imageOut.GetWidth(); c++)
		{
			m_imageOut.GetAt(c, r) = l_imageOut.GetAt(c, l_imageOut.GetHeight() - 1 - r);
		}

	}
	
	
	int _zolly_Area_count = 0;
	int _z_flag = 0;
	int x_;
	int y_;

	for (int r = 0; r < l_imageOut.GetHeight()-229 && _z_flag == 0; r++)
	{
		for (int c = 0; c < l_imageOut.GetWidth()-175 && _z_flag == 0; c=c+5)
		{

		if (   m_imageOut.GetAt(c, r) == 0     && m_imageOut.GetAt(c + 1, r) == 0 && m_imageOut.GetAt(c + 2, r) == 0 && m_imageOut.GetAt(c + 3, r) == 0
		    && m_imageOut.GetAt(c + 4, r) == 0 && m_imageOut.GetAt(c + 5, r) == 0 && m_imageOut.GetAt(c + 6, r) == 0 && m_imageOut.GetAt(c + 7, r) == 0 
		    && m_imageOut.GetAt(c + 8, r) == 0 && m_imageOut.GetAt(c + 9, r) == 0&& m_imageOut.GetAt(c+10, r) == 0 && m_imageOut.GetAt(c + 11, r) == 0 
			&& m_imageOut.GetAt(c + 12, r) == 0 && m_imageOut.GetAt(c + 13, r) == 0&& m_imageOut.GetAt(c + 14, r) == 0 && m_imageOut.GetAt(c + 15, r) == 0
			&& m_imageOut.GetAt(c + 16, r) == 0 && m_imageOut.GetAt(c + 17, r) == 0&& m_imageOut.GetAt(c + 18, r) == 0 && m_imageOut.GetAt(c + 19, r) == 0
			
			&& m_imageOut.GetAt(c, r+1) == 0 && m_imageOut.GetAt(c + 1, r+1) == 0 && m_imageOut.GetAt(c + 2, r+1) == 0 && m_imageOut.GetAt(c + 3, r+1) == 0
			&& m_imageOut.GetAt(c + 4, r+1) == 0 && m_imageOut.GetAt(c + 5, r+1) == 0 && m_imageOut.GetAt(c + 6, r+1) == 0 && m_imageOut.GetAt(c + 7, r+1) == 0
			&& m_imageOut.GetAt(c + 8, r+1) == 0 && m_imageOut.GetAt(c + 9, r+1) == 0 && m_imageOut.GetAt(c + 10, r+1) == 0 && m_imageOut.GetAt(c + 11, r+1) == 0
			&& m_imageOut.GetAt(c + 12, r+1) == 0 && m_imageOut.GetAt(c + 13, r+1) == 0 && m_imageOut.GetAt(c + 14, r+1) == 0 && m_imageOut.GetAt(c + 15, r+1) == 0
			&& m_imageOut.GetAt(c + 16, r+1) == 0 && m_imageOut.GetAt(c + 17, r+1) == 0 && m_imageOut.GetAt(c + 18, r+1) == 0 && m_imageOut.GetAt(c + 19, r+1) == 0

			&&m_imageOut.GetAt(c, r+2) == 0 && m_imageOut.GetAt(c + 1, r+2) == 0 && m_imageOut.GetAt(c + 2, r+2) == 0 && m_imageOut.GetAt(c + 3, r+2) == 0
			&& m_imageOut.GetAt(c + 4, r+2) == 0 && m_imageOut.GetAt(c + 5, r+2) == 0 && m_imageOut.GetAt(c + 6, r+2) == 0 && m_imageOut.GetAt(c + 7, r+2) == 0
			&& m_imageOut.GetAt(c + 8, r+2) == 0 && m_imageOut.GetAt(c + 9, r+2) == 0 && m_imageOut.GetAt(c + 10, r+2) == 0 && m_imageOut.GetAt(c + 11, r+2) == 0
			&& m_imageOut.GetAt(c + 12, r+2) == 0 && m_imageOut.GetAt(c + 13, r+2) == 0 && m_imageOut.GetAt(c + 14, r+2) == 0 && m_imageOut.GetAt(c + 15, r+2) == 0
			&& m_imageOut.GetAt(c + 16, r+2) == 0 && m_imageOut.GetAt(c + 17, r+2) == 0 && m_imageOut.GetAt(c + 18, r+2) == 0 && m_imageOut.GetAt(c + 19, r+2) == 0

			&& m_imageOut.GetAt(c, r + 3) == 0 && m_imageOut.GetAt(c + 1, r + 3) == 0 && m_imageOut.GetAt(c + 2, r + 3) == 0 && m_imageOut.GetAt(c + 3, r + 3) == 0
			&& m_imageOut.GetAt(c + 4, r + 3) == 0 && m_imageOut.GetAt(c + 5, r + 3) == 0 && m_imageOut.GetAt(c + 6, r + 3) == 0 && m_imageOut.GetAt(c + 7, r + 3) == 0
			&& m_imageOut.GetAt(c + 8, r + 3) == 0 && m_imageOut.GetAt(c + 9, r + 3) == 0 && m_imageOut.GetAt(c + 10, r + 3) == 0 && m_imageOut.GetAt(c + 11, r + 3) == 0
			&& m_imageOut.GetAt(c + 12, r + 3) == 0 && m_imageOut.GetAt(c + 13, r + 3) == 0 && m_imageOut.GetAt(c + 14, r + 3) == 0 && m_imageOut.GetAt(c + 15, r + 3) == 0
			&& m_imageOut.GetAt(c + 16, r + 3) == 0 && m_imageOut.GetAt(c + 17, r + 3) == 0 && m_imageOut.GetAt(c + 18, r + 3) == 0 && m_imageOut.GetAt(c + 19, r + 3) == 0


		)
		   {
				x_ = c;
				y_ = r;
				_zolly_Area_count = 0;

				for (int k = x_; k < x_ + 174 && _z_flag == 0; k++)
				{
					for (int j = y_; j < y_ + 228 && _z_flag == 0; j++)
					{
						if (m_imageOut.GetAt(k, j) == 0&& _z_flag == 0)
						{
							_zolly_Area_count++;
							if (_zolly_Area_count > 8000&& _z_flag == 0)
							{
								AfxMessageBox("조리퐁이 검출 되었습니다.(By Area)");
								_z_flag = 1;
								break;
							}

						}
					}

				}

		
			}
			
		}

	}
	
	


	ShowImage(m_imageOut, "조리퐁 검출");
	
	

	//------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	// 오징어땅콩(영상처리된 영상)를 똑바로 출력하기위하여 회색조로 변경한 후에 상하반전 시켰다.
	
	
	CByteImage n_imageOut(gImageBuf.CUTTLE_PEANUT());
	CByteImage o_imageOut(gImageBuf.CUTTLE_PEANUT());

	for (int r = 0; r < n_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < n_imageOut.GetWidth(); c++)
		{
			o_imageOut.GetAt(c, r) = n_imageOut.GetAt(c, n_imageOut.GetHeight() - 1 - r);
		}

	}
	
	int _O_zinger_Area_count = 0;
	int Y_O_zinger_Area_count = 0;
	int _O_flag = 0;
	int _x_;
	int _y_;

	for (int r = 0; r < n_imageOut.GetHeight() - 114 && _O_flag == 0; r++)
	{
		for (int c = 0; c < n_imageOut.GetWidth() - 100 && _O_flag == 0; c++)
		{

			if (o_imageOut.GetAt(c, r) ==0 && o_imageOut.GetAt(c+1, r) == 0 && o_imageOut.GetAt(c + 2, r) == 0 && o_imageOut.GetAt(c + 3, r) == 0
				&& o_imageOut.GetAt(c + 4, r) == 0 

				&& o_imageOut.GetAt(c, r+10) == 127&&o_imageOut.GetAt(c+1, r + 10) == 127 && o_imageOut.GetAt(c+2, r + 10) == 127 && o_imageOut.GetAt(c+3, r + 10) == 127
				&& o_imageOut.GetAt(c+4, r + 10) == 127 
				)
				
			{
				_x_ = c;
				_y_ = r;
				_O_zinger_Area_count = 0;
				Y_O_zinger_Area_count = 0;

				for (int k = _x_; k < _x_ + 99 && _O_flag == 0; k++)
				{
					for (int j = _y_; j < _y_ + 114 && _O_flag == 0; j++)
					{
						if (o_imageOut.GetAt(k, j) == 127 && _O_flag == 0)
						{
							Y_O_zinger_Area_count++;
						}

						if (o_imageOut.GetAt(k, j) == 0 && _O_flag == 0)
						{
							_O_zinger_Area_count++;
							if (_O_zinger_Area_count > 2500 && Y_O_zinger_Area_count > 1500 && _O_flag == 0)
							{
								AfxMessageBox("오징어땅콩이 검출 되었습니다.(By Area)");
								_O_flag = 1;
								break;
							}
						}
					}

				}


			}

		}

	}
	


	ShowImage(o_imageOut, "오징어땅콩 검출");
    
	
	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	//소벨검출 뒤집기

	/*
	CByteImage f_imageOut(e_imageOut);
	CByteImage g_imageOut(e_imageOut);

	for (int r = 0; r < f_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < f_imageOut.GetWidth(); c++)
		{
			g_imageOut.GetAt(c, r) = f_imageOut.GetAt(c, f_imageOut.GetHeight() - 1 - r);
		}

	}
	*/
	//ShowImage(g_imageOut, "소벨 검출");

	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	/*
	//경계제거하기

	CByteImage h_imageOut(RGB2Gray(gImageBuf));
	CByteImage i_imageOut(RGB2Gray(gImageBuf));

	for (int r = 0; r < h_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < h_imageOut.GetWidth(); c++)
		{
			i_imageOut.GetAt(c, r) = h_imageOut.GetAt(c, h_imageOut.GetHeight() - 1 - r);
		}

	}

	for (int r = 0; r < h_imageOut.GetHeight(); r++)
	{
		for (int c = 0; c < h_imageOut.GetWidth(); c++)
		{
			if (r < 126 || r>416 || c < 120 || c>522)
			{
				i_imageOut.GetAt(c, r) = 255;
			}
		}

	}
	ShowImage(i_imageOut, "경계를 없앰");
	*/
	
	return TRUE;
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
//카메라 시작 
// 카메라 해상도에 맞게 변경, StartCam() 호출, 
void CVideoProcessorDlg::OnBnClickedButtonStartCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOZORDER | SWP_NOMOVE);
	m_webCam.StartCam(GetDlgItem(IDC_STATIC_PREVIEW), 640, 480, gBmpInfo);


	gImageBuf = CByteImage(gBmpInfo.bmiHeader.biWidth, gBmpInfo.bmiHeader.biHeight, 3);
	m_webCam.SetCallBackOnFrame(ProcessCamFrame);


}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//영상파일로 저장. 
void CVideoProcessorDlg::OnBnClickedButtonRecCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szFilter[] = "Descriptor File (*.AVI) | *.AVI; | All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal()) // 파일 대화 상자 열기
	{
		CString strPathName = dlg.GetPathName();
		m_webCam.StartRecord(strPathName);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//종료함수 -호출 
void CVideoProcessorDlg::OnBnClickedButtonStopCam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_webCam.StopCam();
}

void CVideoProcessorDlg::OnBnClickedButtonStopRec()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_webCam.StopRecord();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//이진화 함수.


void CVideoProcessorDlg::OnBnClickedButtonBinariztion()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_imageIn.IsEmpty())
	{
		AfxMessageBox("입력 영상이 없습니다.");
		return;
	}



	m_imageOut = CByteImage(m_imageIn.GetWidth(), m_imageIn.GetHeight());

	if (m_imageIn.GetChannel() != 1)	// 컬러 영상
		Binarization(RGB2Gray(m_imageIn), m_imageOut, m_nThreshold);
	else							// 회색조 영상
		Binarization(m_imageIn,m_imageOut, m_nThreshold);

	ShowImage(m_imageOut, "결과 영상");
}


void CVideoProcessorDlg::OnBnClickedButtonViewimage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_imageIn = LoadImageFromDialog();
	ShowImage(m_imageIn, "Image");
}


void CVideoProcessorDlg::OnBnClickedButtonCloseImage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CloseImage("Image");
}
