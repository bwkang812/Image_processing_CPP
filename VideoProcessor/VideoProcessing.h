#pragma once

#include"MyImage.h"
#include<vfw.h>                 //카메라 관련 변수들을 사용하고자 선언. 

#pragma comment(lib,"vfw32.lib")  //파일 링크. 


// 카메라 캡쳐 클래스
class CWebCam
{
public:
	CWebCam();
	virtual ~CWebCam();

	bool StartCam(CWnd* wnd, int nWidth, int nHeight, BITMAPINFO& bmpInfo); //카메라 초기화. 화면 출력 시작.
	void StopCam();                                                         //카메라 종료.
	BOOL StartRecord(const char* filename);                               //AVI 파일로 저장 (사용 안할것임.)
	BOOL StopRecord();                                                    
	BOOL SetCallBackOnFrame(LRESULT(*fpProc)(HWND, LPVIDEOHDR));             //영상을 캡처하여 화면에 출력. * SetCall함수에 입력할 콜백 함수는 전역 함수,전역변수로 선언해야 한다. 

protected:
	HWND		m_hCam;   // 캡처 윈도우의 핸들, 캡처한 영상은 하나의 원도우와 연동하여 이 핸들을 통하여 제어할 수 있게 된다. 
};

void YUY2ToRGB24(int nWidth, int nHeight, const BYTE* pYUY2, BYTE* pRGB24);