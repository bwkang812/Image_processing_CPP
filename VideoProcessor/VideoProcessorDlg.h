
// VideoProcessorDlg.h : 헤더 파일
//

#pragma once

#include "VideoProcessing.h"
#include "afxcmn.h"

// CVideoProcessorDlg 대화 상자
class CVideoProcessorDlg : public CDialogEx
{
// 생성입니다.
public:
	CVideoProcessorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEOPROCESSOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	


	
protected:
	//카메라 캡쳐 관련 멤버 변수
	CWebCam m_webCam; 

protected:
	CByteImage		m_imageIn;
	CByteImage		m_imageOut;



public:
	afx_msg void OnBnClickedButtonStartCam();
	afx_msg void OnBnClickedButtonStopCam();
	afx_msg void OnBnClickedButtonRecCam();
	afx_msg void OnBnClickedButtonStopRec();
	afx_msg void OnBnClickedButtonBinariztion();

	int m_nThreshold;
	CSliderCtrl m_sliderThres;

	afx_msg void OnBnClickedButtonViewimage();
	afx_msg void OnBnClickedButtonCloseImage();
};
