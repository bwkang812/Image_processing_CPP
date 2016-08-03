#include "stdafx.h"
#include "VideoProcessing.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
//생성자 소멸자
CWebCam::CWebCam() 	
{
	m_hCam = NULL;
}

CWebCam::~CWebCam()
{
	if (m_hCam)
		StopCam();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//카메라 초기화.
bool CWebCam::StartCam(CWnd* wnd, int nWidth, int nHeight, BITMAPINFO& bmpInfo) //wnd :캡처하는 영상이 나타날, 윈도우의 포인터를 넘겨받는다. 
                                                                                // bmpInfo : 영상의 규격을 얻는것 , 크래스 외부에서 작업을 수행할 수 있다.
{
	// 캡쳐 영상을 출력할 윈도우의 크기 및 위치 조절
	m_hCam = capCreateCaptureWindow("Cam Capture", WS_CHILD | WS_VISIBLE,
		0, 0, nWidth, nHeight, wnd->m_hWnd, NULL);   // 유니코드 지원 여부에 따라 각각 함수 실행. 촬영하는 영상의 규격과는 상관이 없다. 

	if (!capDriverConnect(m_hCam, 0))                //캡처 윈도우를 카메라와 연결한다. 
	{
		AfxMessageBox("연결된 카메라를 찾을 수 없습니다.");
		return false;
	}

	//	capDlgVideoFormat(m_hCam);
	capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));     //캡처영상을 사용하려면 영상의 규격정보를 알아야한다. 

	// 캡쳐 영상의 크기를 희망 값으로 설정
	bmpInfo.bmiHeader.biWidth = nWidth;
	bmpInfo.bmiHeader.biHeight = nHeight;
	bmpInfo.bmiHeader.biSizeImage = nWidth*nHeight*bmpInfo.bmiHeader.biBitCount / 8;

	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))  //카메라의 영상 규격(크기와 색 형식 정보등)을 설정한다. 
	{
		// 설정에 실패하면 원래 값으로 되돌림
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	// 캡쳐 영상이 24비트 RGB가 될 수 있도록 설정
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	if (!capSetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO)))
	{
		// 설정에 실패하면 원래 값으로 되돌림
		capGetVideoFormat(m_hCam, &bmpInfo, sizeof(BITMAPINFO));
	}

	capOverlay(m_hCam, TRUE); //그래픽 하드웨어를 이용할 것인지 결정.   *오버레이를 활성화하면 미리보기가 자동으로 비활성화됨.
	capPreviewRate(m_hCam,30); //미리보기를 갱신하는 시간 간격을 설정.
	capPreview(m_hCam, TRUE);   //미리보기를 활성화 할 것인지 결정.

	return true;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
//카메라를 종료하는 함수.
void CWebCam::StopCam() 
{
	capCaptureAbort(m_hCam);  //캡처동작 중지.
	capDriverDisconnect(m_hCam);  //카메라와 캡처윈도우 사이의 연결을 끊는 메시지를 발생시킨다.
	m_hCam = NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
// AVI파일로 저장하는 함수.

BOOL CWebCam::StartRecord(const char* filename)
{
	capFileSetCaptureFile(m_hCam, filename);
	return capCaptureSequence(m_hCam);
}

BOOL CWebCam::StopRecord()
{
	capCaptureStop(m_hCam); //캡처동작 정지. 캡처한 영상을 파일에 저장. 

	return capPreview(m_hCam, TRUE);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//미리보기에 나타낼 때마다. 호출될 사용자 콜백함수를 등록할 수 있다, 
BOOL CWebCam::SetCallBackOnFrame(LRESULT(*fpProc)(HWND, LPVIDEOHDR)) //매개변수로 받은 함수 포인터를 그대로  넘긴다.
{
	return capSetCallbackOnFrame(m_hCam, fpProc);  
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//YUY2로부터 RGB형식으로 변환하는 함수.
void YUY2ToRGB24(int nWidth, int nHeight, const BYTE* pYUY2, BYTE* pRGB24)
{
	int nSize = nHeight * (nWidth / 2);

	int idxSrc = 0;
	int idxDst = 0;

	BYTE y1, u, y2, v;
	BYTE r, g, b;

	for (int i = 0; i<nSize; i++)
	{
		y1 = pYUY2[idxSrc];
		u = pYUY2[idxSrc + 1];
		y2 = pYUY2[idxSrc + 2];
		v = pYUY2[idxSrc + 3];

		// 첫 번째 픽셀
		b = CLIP((76284 * (y1 - 16) + 132252 * (u - 128)) >> 16);
		g = CLIP((76284 * (y1 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
		r = CLIP((76284 * (y1 - 16) + 104595 * (v - 128)) >> 16);

		pRGB24[idxDst] = b;
		pRGB24[idxDst + 1] = g;
		pRGB24[idxDst + 2] = r;

		idxDst += 3;

		// 두 번째 픽셀
		b = CLIP((76284 * (y2 - 16) + 132252 * (u - 128)) >> 16);
		g = CLIP((76284 * (y2 - 16) - 53281 * (v - 128) - 25625 * (u - 128)) >> 16);
		r = CLIP((76284 * (y2 - 16) + 104595 * (v - 128)) >> 16);

		pRGB24[idxDst] = b;
		pRGB24[idxDst + 1] = g;
		pRGB24[idxDst + 2] = r;

		idxDst += 3;
		idxSrc += 4;
	}
}