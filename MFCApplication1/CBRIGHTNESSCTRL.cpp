﻿﻿// CBRIGHTNESSCTRL.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "CBRIGHTNESSCTRL.h"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
// CBRIGHTNESSCTRL 대화 상자

IMPLEMENT_DYNAMIC(CBRIGHTNESSCTRL, CDialogEx)

CBRIGHTNESSCTRL::CBRIGHTNESSCTRL()
	: CDialogEx(IDC_BRIGHTNESSCTRL_BTN_1)
{}

CBRIGHTNESSCTRL::CBRIGHTNESSCTRL(Mat Img, BITMAPINFO* bitmapInfo)
	: CDialogEx(IDC_BRIGHTNESSCTRL_BTN_1)
{
	myImg = Img; // 이미지 매트릭스 정보 가져오기 
	myBitmapInfo = bitmapInfo;
}


//void CBRIGHTNESSCTRL::OnBnClickedButton1()
//{
//	
//}


CBRIGHTNESSCTRL::~CBRIGHTNESSCTRL()
{}
BOOL CBRIGHTNESSCTRL::OnInitDialog()
{
	CDialog::OnInitDialog();
	MoveWindow(350, 120, 1280, 720);

	CRect wndSz;
	this->GetWindowRect(wndSz);// right:창의 너비 bottm: 창의 높이 
	GetDlgItem(IDC_CANVAS)->MoveWindow(0, 0, int(wndSz.right * 5 / 6), wndSz.bottom);
	// 픽쳐 컨트롤 시작 좌표 , 전체창의 5/6 부분 으로 출력

	GetDlgItem(IDC_STATIC7)->MoveWindow(960, 710, 500, 500); // 'SETTING'그룹창

	GetDlgItem(IDC_STATIC8)->MoveWindow(960, 140, 25, 25);// 밝기
	GetDlgItem(IDC_STATIC9)->MoveWindow(960, 260, 25, 25);// 대비

	GetDlgItem(IDC_SPIN2)->MoveWindow(1170, 284, 20, 20);// 스핀 컨트롤
	GetDlgItem(IDC_EDIT2)->MoveWindow(1150, 284, 20, 20);// 에딧 컨트롤 ver=스핀 에딧 컨트롤 상태 

	GetDlgItem(IDC_SPIN1)->MoveWindow(1170, 161, 20, 20);// 스핀 컨트롤
	GetDlgItem(IDC_EDIT1)->MoveWindow(1150, 161, 20, 20);// 에딧 컨트롤 ver=스핀 에딧 컨트롤 상태 

	GetDlgItem(IDC_SLIDER1)->MoveWindow(960, 160, 180, 45);// 트랙바 
	GetDlgItem(IDC_SLIDER2)->MoveWindow(960, 280, 180, 45);// 트랙바

	GetDlgItem(IDCANCEL)->MoveWindow(1000, 720 - 100, 200, 45); //취소
	GetDlgItem(IDOK)->MoveWindow(1000, 720 - 160, 200, 45);// 확인 

	GetDlgItem(IDC_BUTTON2)->MoveWindow(980, 370, 75, 40); //흑백모드
	GetDlgItem(IDC_BUTTON3)->MoveWindow(1100, 370, 75, 40); //컬러모드
	GetDlgItem(IDC_BUTTON4)->MoveWindow(1000, 500, 200, 45);// 되돌리기 
	//GetDlgItem(IDC_BUTTON1)->MoveWindow(1100, 450, 75, 40);

	// 슬라이더 초기화
	m_slider.SetRange(0, 100); // 밝기 범위 설정
	m_slider.SetPos(50); // 초기 밝기 값 설정
	m_slider.SetTicFreq(10);

	m_slider2.SetRange(-300, 300); // 명암비 범위 설정
	m_slider2.SetPos(100); // 초기 명암비 값 설정
	m_slider2.SetTicFreq(10);

	// 에디트 컨트롤 초기화
	m_edit2.SetWindowText(_T("0")); // 초기 명암비 값 설정
	m_edit.SetWindowText(_T("0")); // 초기 밝기 값 설정
	m_edit_val = 0;
	m_edit_val2 = 0;

	// 스핀 컨트롤 초기화
	//m_spin.SetRange(-100, 100); // 밝기 범위 설정
	//m_spin.SetPos(50); // 초기 밝기 값 설정

	SetTimer(1, 80, NULL);
	return TRUE; // 포커스 설정을 위한 기본값 반환
}
void CBRIGHTNESSCTRL::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_SPIN1, m_spin);

	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
}


BEGIN_MESSAGE_MAP(CBRIGHTNESSCTRL, CDialogEx)
	//	ON_STN_ENABLE(IDC_CANVAS, &CBRIGHTNESSCTRL::OnStnEnableCanvas)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CBRIGHTNESSCTRL::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CBRIGHTNESSCTRL::OnBnClickedButton3)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CBRIGHTNESSCTRL::OnNMCustomdrawSlider1)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CBRIGHTNESSCTRL::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CBRIGHTNESSCTRL::mspin_updown)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CBRIGHTNESSCTRL::OnNMCustomdrawSlider2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CBRIGHTNESSCTRL::mspin_updown2)
	ON_BN_CLICKED(IDC_BUTTON4, &CBRIGHTNESSCTRL::OnBnClickedButton4)
	//ON_BN_CLICKED(IDC_BUTTON1, &CBRIGHTNESSCTRL::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CBRIGHTNESSCTRL::OnBnClickedOk)
END_MESSAGE_MAP()
// CBRIGHTNESSCTRL 메시지 처리기

void CBRIGHTNESSCTRL::DrawImage(Mat requestImg, BITMAPINFO* requestBmpInfo) // 매트릭스 판때기 , 비트맵 판때기
{		// 타이머 중지
	KillTimer(1);

	// 캔버스 영역 가져오기
	CRect canvas;
	this->GetClientRect(&canvas);
	int wx = int(canvas.right * 5 / 6); // 캔버스의 가로 길이 설정
	int wy = canvas.bottom; // 캔버스의 세로 길이 설정

	// 캔버스 DC 생성
	CClientDC dc(GetDlgItem(IDC_CANVAS));

	// 이미지 크기에 따라 캔버스 크기 조정
	if (requestImg.cols > wx) {
		int resize_h = cvRound((wx * requestImg.rows) / requestImg.cols); // 이미지 비율 유지를 위한 높이 조정
		int y = cvRound((wy - resize_h) / 2); // 이미지를 캔버스의 중앙에 배치하기 위한 Y 좌표 계산
		GetDlgItem(IDC_CANVAS)->MoveWindow(0, y, requestImg.cols, resize_h); // 캔버스 크기 조정
	}
	else {
		int x = cvRound((wx - requestImg.cols) / 2); // 이미지를 캔버스의 중앙에 배치하기 위한 X 좌표 계산
		int y = cvRound((wy - requestImg.rows) / 2); // 이미지를 캔버스의 중앙에 배치하기 위한 Y 좌표 계산
		GetDlgItem(IDC_CANVAS)->MoveWindow(x, y, requestImg.cols, requestImg.rows); // 캔버스 크기 조정
	}

	// 캔버스의 크기 다시 가져오기
	GetDlgItem(IDC_CANVAS)->GetClientRect(&canvas);

	// 비트맵을 사용하여 이미지 화면에 출력
	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR); // 비트맵을 스트레치하여 출력할 때 색상 유지 모드 설정
	StretchDIBits(dc.GetSafeHdc(), 0, 0, canvas.Width(), canvas.Height(), 0, 0, requestImg.cols, requestImg.rows,
		requestImg.data, requestBmpInfo, DIB_RGB_COLORS, SRCCOPY); // 비트맵 출력
}

void CBRIGHTNESSCTRL::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CBRIGHTNESSCTRL::CreateBitmapInfo(BITMAPINFO** btmInfo, int w, int h, int bpp) {
	if (*btmInfo != NULL) //기존 비트맵 정보 초기화 
	{
		delete* btmInfo;
		*btmInfo = NULL; //기존 BITMAP 정보 삭제 
	}

	if (bpp == 8) //1채널 
		*btmInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24(3채널) or 32bit(4채널)
		*btmInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	(*btmInfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//구조체에 필요한 바이트 수
	(*btmInfo)->bmiHeader.biPlanes = 1;// 대상 디바이스의 평면 수를 지정합니다. 이 값은 1로 설정해야 합니다.
	(*btmInfo)->bmiHeader.biBitCount = bpp;//픽셀당 비트 수(bpp)를 지정합니다. 압축되지 않은 형식의 경우 이 값은 픽셀당 평균 비트 수입니다.
	(*btmInfo)->bmiHeader.biCompression = BI_RGB;//압축되지 않은 RGB.
	(*btmInfo)->bmiHeader.biSizeImage = 0;//이미지의 크기(바이트)를 지정합니다.
	(*btmInfo)->bmiHeader.biXPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 가로 해상도(미터당 픽셀)
	(*btmInfo)->bmiHeader.biYPelsPerMeter = 0;//비트맵에 대한 대상 디바이스의 세로 해상도(미터당 픽셀)를 지정합니다.
	(*btmInfo)->bmiHeader.biClrUsed = 0;//비트맵에서 실제로 사용되는 색 테이블의 색 인덱스 수를 지정합니다.
	(*btmInfo)->bmiHeader.biClrImportant = 0;//비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.
	//이 값이 0이면 모든 색이 중요합니다.

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			(*btmInfo)->bmiColors[i].rgbBlue = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbGreen = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbRed = (BYTE)i;
			(*btmInfo)->bmiColors[i].rgbReserved = 0;
		}
	}

	(*btmInfo)->bmiHeader.biWidth = w;
	(*btmInfo)->bmiHeader.biHeight = -h;//음수는 원본이 왼쪽 위 모서리에 있는 하향식 DIB입니다.
}

void CBRIGHTNESSCTRL::OnBnClickedButton4() // 되돌리기 로 전환 버튼 
{
	//if ((myImg.channels()==3 )||(myImg.channels()==1)) {
	colorToGray();// 흑백 모드
	GrayToColor();// 컬러 모드
	m_spin.SetPos(50);
	m_slider.SetPos(50);
	m_spin2.SetPos(50);
	m_slider2.SetPos(50);
	DrawImage(myImg, myBitmapInfo);

	MessageBox(L"원본으로 돌아갑니다", _T("원초적본능"), MB_OK | MB_ICONINFORMATION);

}

void CBRIGHTNESSCTRL::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1:
		DrawImage(myImg, myBitmapInfo);
	}
	KillTimer(1);
	CDialogEx::OnTimer(nIDEvent);
}

void CBRIGHTNESSCTRL::OnBnClickedButton2() // 흑백 모드 전환 버튼
{
	if (colorToGray())
	{
		DrawImage(myImg, BitChangeImg); // 변환된 이미지 표시

	}
}

void CBRIGHTNESSCTRL::OnBnClickedButton3() // 컬러 모드 전환 버튼 
{
	if (GrayToColor())
	{
		DrawImage(myImg, BitChangeImg2);

	}
}

BOOL CBRIGHTNESSCTRL::colorToGray()
{
	if (myImg.channels() == 3) // 이미지가 컬러 이미지인지 확인
	{
		// 컬러 이미지를 백업
		backupImg = myImg.clone();

		// 흑백 이미지로 변환
		cvtColor(myImg, myImg, COLOR_BGR2GRAY);

		// 이미지 정보 업데이트
		CreateBitmapInfo(&BitChangeImg, myImg.cols, myImg.rows, 8);
		// 흑백 이미지로 변환되었으므로 채널 수는 1이므로 8로 설정
		// 즉 컬러체널 원소 = 3 ==> 흑백으로 변환되니 흑백 기준의 채널 수 '8'로 변경되는것! 

		// 성공 메시지 출력
		//CString successMessage = _T("성공적으로 변환되었습니다.");
		//MessageBox(successMessage, _T("성공"), MB_OK | MB_ICONINFORMATION);
		return true;
	}
	else
	{
		// 에러 메시지 출력
		//CString errorMessage = _T("이미 흑백모드 이미지입니다.");
		//MessageBox(errorMessage, _T("에러"), MB_OK | MB_ICONERROR);
		return false;
	}
}

BOOL CBRIGHTNESSCTRL::GrayToColor()
{
	if (myImg.channels() == 1) // 이미지가 흑백 이미지인지 확인
	{
		// 백업된 컬러 이미지를 불러오기
		myImg = backupImg.clone();

		// 이미지 정보 업데이트
		CreateBitmapInfo(&BitChangeImg2, myImg.cols, myImg.rows, myImg.channels() * 8);
		// 컬러 이미지로 변환되었으므로 채널 수는 3이므로 24로 설정

		// 성공 메시지 출력
		//CString successMessage = _T("성공적으로 변환되었습니다.");
		//MessageBox(successMessage, _T("성공"), MB_OK | MB_ICONINFORMATION);
		return true;
	}
	else
	{
		// 에러 메시지 출력
		//CString errorMessage = _T("이미 컬러 이미지입니다.");
		//MessageBox(errorMessage, _T("에러"), MB_OK | MB_ICONERROR);
		return false;
	}
}

void CBRIGHTNESSCTRL::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	int newValue = 0;
	int newValue2 = 0;
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER1)
	{

		newValue = m_slider.GetPos(); // 슬라이더에서 값 가져오기
		m_spin.SetPos(newValue); // 스핀 컨트롤에 값 설정
		CString strValue; //문자열을 strvalue 에 받고 Format 서식 문자열을 사용하여 문자열을 생성 %d는 정수를 나타내는 서식 지정자이며, 
		strValue.Format(_T("%d"), newValue);//해당 지정자에 newValue의 값이 대입되어 문자열이 생성됩니다.
		m_edit.SetWindowText(strValue); // 에디트 컨트롤에 값 설정

		if ((myImg.channels() == 3))
		{//open cv4 책= ycrcv 참조
			Mat tmpImg = myImg.clone(); //입력 이미지 myImg를 복제하여 새로운 이미지 tmpImg에 저장합니다.
			cvtColor(tmpImg, tmpImg, COLOR_BGR2YCrCb);//BGR 색공간에서 YCrCb 색공간으로 이미지를 변환
			//=이미지를 밝기(Y)와 색상(크로마)의 세 가지 채널로 분리합니다.

			std::vector<Mat> ycrcb_planes;// YCrCb 이미지를 저장할 벡터를 선언합니다.
			split(tmpImg, ycrcb_planes);//split 통해 이미지를 각각의 채널로 나뉘고 벡터에 저장합니다.

			Mat adjustedImage;
			ycrcb_planes[0] = ycrcb_planes[0] + newValue; //밝기 채널(Y)에 값을 받고 더해서 밝지 제어
			merge(ycrcb_planes, adjustedImage);//수정된 밝기 채널이(input) 다시 새로운 Mat (output) 병합

			cvtColor(adjustedImage, adjustedImage, COLOR_YCrCb2BGR); //밝기 채널만 받은 Mat 을 BGR 색공간으로 변환

			// 이미지 표시
			CreateBitmapInfo(&BitChangeImg, adjustedImage.cols, adjustedImage.rows, adjustedImage.channels() * 8);
			//이미지를 표시하고 결과 이미지와 비트맵 정보를 업데이트합니다.입력 이미지의 채널이 3인 경우에는 *8
			DrawImage(adjustedImage, BitChangeImg);

			resultImg = adjustedImage.clone();
			BitChangeResultImg = BitChangeImg;
			//resultImg에 결과 이미지를 얕은복사하여 저장합니다.
			//BitChangeResultImg에 비트맵 정보를 얕은복사하여 저장합니다.
		}
		else if (myImg.channels() == 1)
		{
			Mat adjustedImage = myImg + newValue;//입력 myImg의 각 픽셀 값에 newValue를 더하여 명암 조정

			CreateBitmapInfo(&BitChangeImg, adjustedImage.cols, adjustedImage.rows, 8);
			DrawImage(adjustedImage, BitChangeImg);

			resultImg = adjustedImage.clone();
			BitChangeResultImg = BitChangeImg2;
		}

	}
	else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER2)
	{


		// 슬라이더에서 값 변경될 때
		int newValue2 = m_slider2.GetPos(); // 슬라이더에서 값 가져오기
		m_spin2.SetPos(newValue2); // 스핀 컨트롤에 값 설정
		CString strValue2;
		strValue2.Format(_T("%d"), newValue2);
		m_edit2.SetWindowText(strValue2); // 에디트 컨트롤에 값 설정

		if ((myImg.channels() == 3))
		{
			Mat tmpImg2 = myImg.clone(); //open cv4 책= ycrcv 참조
			cvtColor(tmpImg2, tmpImg2, COLOR_BGR2YCrCb);

			vector<Mat> ycrcb_planes2;
			split(tmpImg2, ycrcb_planes2);

			float contrast = 1.0;

			ycrcb_planes2[0] = ycrcb_planes2[0] + (newValue2 - 128) * contrast;

			Mat adjustedImage2;

			equalizeHist(ycrcb_planes2[0], ycrcb_planes2[0]); // 평활화 수행 = in  ->  out
			merge(ycrcb_planes2, adjustedImage2); // split 한 ycrcb_planes2 와 새로운 Mat adjustedImage2 =merge 시킨다


			cvtColor(adjustedImage2, adjustedImage2, COLOR_YCrCb2BGR);

			// 이미지 표시
			CreateBitmapInfo(&BitChangeImg2, adjustedImage2.cols, adjustedImage2.rows, adjustedImage2.channels() * 8);
			DrawImage(adjustedImage2, BitChangeImg2);

			resultImg = adjustedImage2.clone();
			BitChangeResultImg = BitChangeImg2;

		}
		else if (myImg.channels() == 1)
		{
			Mat tmpImg2;// gray 받을 Mat 객체 생성
			cvtColor(myImg, tmpImg2, COLOR_GRAY2BGR); // input = myimg -> output = tmpImg2 , 변환 혹은 기존 이미지를 output 결과로 보내고 bgr변경
			Mat tmpImg3;// bgr 을 받을 Mat 객체 생성
			cvtColor(tmpImg2, tmpImg3, COLOR_BGR2YCrCb);// input = tmpImg2 (이미 bgr로 변경된이미지)를 output = tmpImg3 결과로 보내고 Ycrcb 변경 
			std::vector<Mat> ycrcb_planes2; // y 채널 추출할 백터형 Mat 객체 생성

			split(tmpImg3, ycrcb_planes2);//split을 통해 tmpImg3(이미 Ycrcb변경된이미지)를 output=ycrcb_planes2(백터형 Mat에 보내고) 
			float contrast = 1.0;
			equalizeHist(ycrcb_planes2[0], ycrcb_planes2[0]);
			ycrcb_planes2[0] = ycrcb_planes2[0] + (newValue2 - 128) * contrast; // 명암비 계산 수식을 계산하고 

			Mat adjustedImage2;

			merge(ycrcb_planes2, adjustedImage2); // 명암비가 계산된 <백터형>Mat ycrcb_planes2 과 새로운 Mat adjustedImage2 객체를 합친다.

			cvtColor(adjustedImage2, tmpImg3, COLOR_YCrCb2BGR);//합친 Mat adjustedImage2 다시 Ycrcb 로 받았던 bgr Mat tmpImg3에 다시 전달

			cvtColor(tmpImg3, myImg, COLOR_BGR2GRAY);//전달받은 output = bgr을 받았던 Mat tmpImg3 를 gray 변환된 기존이미지에 보냄

			// 이미지 표시
			CreateBitmapInfo(&BitChangeImg2, myImg.cols, myImg.rows, 8);
			DrawImage(myImg, BitChangeImg2);

			resultImg = myImg.clone();
			BitChangeResultImg = BitChangeImg2;
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CBRIGHTNESSCTRL::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{


	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	// 슬라이더의 값이 변경될 때
	int newValue = m_slider.GetPos(); // 슬라이더의 현재 값 가져오기
	m_spin.SetPos(newValue); // 스핀 컨트롤에도 값 설정
	CString strValue;
	strValue.Format(_T("%d"), newValue);
	m_edit.SetWindowText(strValue); // 에디트 컨트롤에도 값 설정

	*pResult = 0;
}
void CBRIGHTNESSCTRL::OnNMCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int newValue2 = m_slider2.GetPos(); // 슬라이더의 현재 값 가져오기
	m_spin2.SetPos(newValue2); // 스핀 컨트롤에도 값 설정
	CString strValue2;
	strValue2.Format(_T("%d"), newValue2);
	m_edit2.SetWindowText(strValue2); // 에디트 컨트롤에도 값 설정

	*pResult = 0;
}
void CBRIGHTNESSCTRL::mspin_updown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

		// 스핀 컨트롤의 값이 변경될 때
	if (pNMUpDown->iDelta < 0) {//위로 증가 버튼눌렀을 때 iDelta 음수가 출력 
		m_edit_val += 10;
	}
	else {//아래 버튼을 눌렀을 때 iDelta 양수가 출력된다 
		m_edit_val -= 10;

	}

	m_spin.SetPos(m_edit_val); // 스핀 컨트롤에 새로운 값 설정
	m_slider.SetPos(m_edit_val); // 슬라이더에도 새로운 값 설정
	CString strValue;
	strValue.Format(_T("%d"), m_edit_val);
	m_edit.SetWindowText(strValue); // 에디트 컨트롤에도 새로운 값 설정

	if ((myImg.channels() == 3))
	{
		Mat tmpImg = myImg.clone();
		cvtColor(tmpImg, tmpImg, COLOR_BGR2YCrCb);

		std::vector<Mat> ycrcb_planes;
		split(tmpImg, ycrcb_planes);

		Mat adjustedImage;
		ycrcb_planes[0] = ycrcb_planes[0] + m_edit_val;
		merge(ycrcb_planes, adjustedImage);

		cvtColor(adjustedImage, adjustedImage, COLOR_YCrCb2BGR);

		// 이미지 표시
		CreateBitmapInfo(&BitChangeImg, adjustedImage.cols, adjustedImage.rows, adjustedImage.channels() * 8);
		DrawImage(adjustedImage, BitChangeImg);
		resultImg = adjustedImage.clone();
		BitChangeResultImg = BitChangeImg;

	}
	else if (myImg.channels() == 1)
	{
		Mat adjustedImage = myImg + m_edit_val;

		// 이미지 표시+
		CreateBitmapInfo(&BitChangeImg, adjustedImage.cols, adjustedImage.rows, 8);
		DrawImage(adjustedImage, BitChangeImg);
		resultImg = adjustedImage.clone();
		BitChangeResultImg = BitChangeImg;

	}

	*pResult = 0;
}
void CBRIGHTNESSCTRL::mspin_updown2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown2 = reinterpret_cast<LPNMUPDOWN>(pNMHDR);


	// 스핀 컨트롤의 값이 변경될 때
	if (pNMUpDown2->iDelta < 0) {//위로 증가 버튼눌렀을 때 iDelta 음수가 출력 



		m_edit_val2 += 100;


	}
	else {//아래 버튼을 눌렀을 때 iDelta 양수가 출력된다 


		m_edit_val2 -= 100;
	}

	m_spin2.SetPos(m_edit_val2);
	m_slider2.SetPos(m_edit_val2);
	CString strValue2;
	strValue2.Format(_T("%d"), m_edit_val2);
	m_edit2.SetWindowText(strValue2);
	if ((myImg.channels() == 3))
	{
		Mat tmpImg2 = myImg.clone(); //open cv4 책= ycrcv 참조
		cvtColor(tmpImg2, tmpImg2, COLOR_BGR2YCrCb);

		vector<Mat> ycrcb_planes2;
		split(tmpImg2, ycrcb_planes2);
		//
		float contrast = 1.0;

		ycrcb_planes2[0] = ycrcb_planes2[0] + (m_edit_val2 - 128) * contrast;
		//
		Mat adjustedImage2;

		equalizeHist(ycrcb_planes2[0], ycrcb_planes2[0]); // 평활화 수행 = in  ->  out
		merge(ycrcb_planes2, adjustedImage2); // split 한 ycrcb_planes2 와 새로운 Mat adjustedImage2 =merge 시킨다


		cvtColor(adjustedImage2, adjustedImage2, COLOR_YCrCb2BGR);

		// 이미지 표시
		CreateBitmapInfo(&BitChangeImg2, adjustedImage2.cols, adjustedImage2.rows, adjustedImage2.channels() * 8);
		DrawImage(adjustedImage2, BitChangeImg2);

		resultImg = adjustedImage2.clone();
		BitChangeResultImg = BitChangeImg2;

	}
	else if (myImg.channels() == 1)
	{
		Mat tmpImg2;// gray 받을 Mat 객체 생성
		cvtColor(myImg, tmpImg2, COLOR_GRAY2BGR); // input = myimg -> output = tmpImg2 , 변환 혹은 기존 이미지를 output 결과로 보내고 bgr변경
		Mat tmpImg3;// bgr 을 받을 Mat 객체 생성
		cvtColor(tmpImg2, tmpImg3, COLOR_BGR2YCrCb);// input = tmpImg2 (이미 bgr로 변경된이미지)를 output = tmpImg3 결과로 보내고 Ycrcb 변경 
		std::vector<Mat> ycrcb_planes2; // y 채널 추출할 백터형 Mat 객체 생성

		split(tmpImg3, ycrcb_planes2);//split을 통해 tmpImg3(이미 Ycrcb변경된이미지)를 output=ycrcb_planes2(백터형 Mat에 보내고) 
		float contrast = 1.0;
		equalizeHist(ycrcb_planes2[0], ycrcb_planes2[0]);
		ycrcb_planes2[0] = ycrcb_planes2[0] + (m_edit_val2 - 128) * contrast; // 명암비 계산 수식을 계산하고 

		Mat adjustedImage2;

		merge(ycrcb_planes2, adjustedImage2); // 명암비가 계산된 <백터형>Mat ycrcb_planes2 과 새로운 Mat adjustedImage2 객체를 합친다.

		cvtColor(adjustedImage2, tmpImg3, COLOR_YCrCb2BGR);//합친 Mat adjustedImage2 다시 Ycrcb 로 받았던 bgr Mat tmpImg3에 다시 전달

		cvtColor(tmpImg3, myImg, COLOR_BGR2GRAY);//전달받은 output = bgr을 받았던 Mat tmpImg3 를 gray 변환된 기존이미지에 보냄

		// 이미지 표시
		CreateBitmapInfo(&BitChangeImg2, myImg.cols, myImg.rows, 8);
		DrawImage(myImg, BitChangeImg2);

		resultImg = myImg.clone();
		BitChangeResultImg = BitChangeImg2;
	}
	*pResult = 0;
}
void CBRIGHTNESSCTRL::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}