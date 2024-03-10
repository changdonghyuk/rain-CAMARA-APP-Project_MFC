// CBRIGHTNESSCTRL.cpp: 구현 파일
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

	GetDlgItem(IDC_STATIC8)->MoveWindow(960, 140, 25, 25); // 밝기
	GetDlgItem(IDC_STATIC9)->MoveWindow(960, 260, 25, 25);// 대비
	GetDlgItem(IDC_STATIC7)->MoveWindow(960, 710, 50, 50); // 'SETTING'그룹창

	GetDlgItem(IDC_SPIN2)->MoveWindow(1170, 284, 20, 20);
	GetDlgItem(IDC_EDIT2)->MoveWindow(1150, 284, 20, 20);

	GetDlgItem(IDC_SPIN1)->MoveWindow(1170, 161, 20, 20);
	GetDlgItem(IDC_EDIT1)->MoveWindow(1150, 161, 20, 20);

	GetDlgItem(IDC_SLIDER1)->MoveWindow(960, 160, 180, 45);
	GetDlgItem(IDC_SLIDER2)->MoveWindow(960, 280, 180, 45);

	GetDlgItem(IDCANCEL)->MoveWindow(1000, 720 - 100, 200, 45);
	GetDlgItem(IDOK)->MoveWindow(1000, 720 - 160, 200, 45);

	GetDlgItem(IDC_BUTTON2)->MoveWindow(980, 370 ,75,40); 
	GetDlgItem(IDC_BUTTON3)->MoveWindow(1100, 370, 75, 40); 
	GetDlgItem(IDC_BUTTON4)->MoveWindow(1000, 500 ,200, 45);
	//GetDlgItem(IDC_BUTTON1)->MoveWindow(1100, 450, 75, 40);

	// 슬라이더 초기화
	m_slider.SetRange(0, 100); // 밝기 범위 설정
	m_slider.SetPos(50); // 초기 밝기 값 설정
	m_slider.SetTicFreq(10);

	m_slider2.SetRange(0, 100); // 명암비 범위 설정
	m_slider2.SetPos(50); // 초기 명암비 값 설정
	m_slider2.SetTicFreq(10);



	// 스핀 컨트롤 초기화
	//m_spin.SetRange(-100, 100); // 밝기 범위 설정
	//m_spin.SetPos(50); // 초기 밝기 값 설정

	// 에디트 컨트롤 초기화
	m_edit2.SetWindowText(_T("0")); // 초기 명암비 값 설정

	m_edit.SetWindowText(_T("0")); // 초기 밝기 값 설정

	m_edit_val = 0; 
	m_edit_val2 = 0;

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
{
	KillTimer(1);
	CRect canvas; // 먼저 메인에서 불러온 이미지를 이어서 받을 사각형 과(의)객체 'canvas' 생성
	this->GetClientRect(&canvas); //사각형 객체에='canvas' [사각형의 x, y 좌표설정이되는 = (0, 0) 함수'GetClientRect']
	int wx = int(canvas.right * 5 / 6);
	int wy = canvas.bottom;

	//불러올 사진 cols 가져오기.

	CClientDC dc(GetDlgItem(IDC_CANVAS));
	
	if (requestImg.cols > wx) {
		
		int resize_h = cvRound((wx * requestImg.rows) / requestImg.cols);
		int y = cvRound((wy - resize_h) / 2);
	
		GetDlgItem(IDC_CANVAS)->MoveWindow(0, y, requestImg.cols, resize_h);
	}
	else {
		int x = cvRound((wx - requestImg.cols) / 2);
		int y = cvRound((wy - requestImg.rows) / 2);
		GetDlgItem(IDC_CANVAS)->MoveWindow(x, y, requestImg.cols, requestImg.rows);
	}
	GetDlgItem(IDC_CANVAS)->GetClientRect(&canvas);
	SetStretchBltMode(dc.GetSafeHdc(),COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, canvas.Width(), canvas.Height(), 0, 0, requestImg.cols, requestImg.rows,
		requestImg.data, requestBmpInfo, DIB_RGB_COLORS, SRCCOPY);
	
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
	(*btmInfo)->bmiHeader.biClrImportant = 0;//비트맵을 표시하는 데 중요한 것으로 간주되는 색 인덱스의 수를 지정합니다.이 값이 0이면 모든 색이 중요합니다.

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

void CBRIGHTNESSCTRL::OnBnClickedButton4()
{
	//if ((myImg.channels()==3 )||(myImg.channels()==1)) {
		colorToGray();
		GrayToColor();
		DrawImage(myImg, myBitmapInfo);

		MessageBox(L"원본으로 돌아갑니다", _T("원초적본능"), MB_OK | MB_ICONINFORMATION);
	
}

void CBRIGHTNESSCTRL::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case 1:
			DrawImage(myImg ,myBitmapInfo);
	}
	KillTimer(1);
	CDialogEx::OnTimer(nIDEvent);
}

void CBRIGHTNESSCTRL::OnBnClickedButton2()
{
	if (colorToGray()) 
	{
		DrawImage(myImg, BitChangeImg); // 변환된 이미지 표시
	}
}

void CBRIGHTNESSCTRL::OnBnClickedButton3()
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
		cvtColor(myImg, myImg, cv::COLOR_BGR2GRAY);

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
		CreateBitmapInfo(&BitChangeImg2, myImg.cols, myImg.rows,myImg.channels() *8); // 컬러 이미지로 변환되었으므로 채널 수는 3이므로 24로 설정

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
//
void CBRIGHTNESSCTRL::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	
		int newValue = 0;
		int newValue2 = 0;
		if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER1) {
			// 슬라이더에서 값 변경될 때
			newValue = m_slider.GetPos(); // 슬라이더에서 값 가져오기
			m_spin.SetPos(newValue); // 스핀 컨트롤에 값 설정
			CString strValue;
			strValue.Format(_T("%d"), newValue);
			m_edit.SetWindowText(strValue); // 에디트 컨트롤에 값 설정

			if ((myImg.channels() == 3))
			{
				Mat tmpImg = myImg.clone(); //open cv4 책= ycrcv 참조
				cvtColor(tmpImg, tmpImg, COLOR_BGR2YCrCb);

				std::vector<Mat> ycrcb_planes;
				split(tmpImg, ycrcb_planes);

				Mat adjustedImage;
				ycrcb_planes[0] = ycrcb_planes[0] + newValue;
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
				Mat adjustedImage = myImg + newValue;

				// 이미지 표시
				CreateBitmapInfo(&BitChangeImg, adjustedImage.cols, adjustedImage.rows, adjustedImage.channels() * 8);
				DrawImage(adjustedImage, BitChangeImg);
				
				resultImg = adjustedImage.clone();
				BitChangeResultImg = BitChangeImg2;

			}

		}
		else if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER2) {
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

				ycrcb_planes2[0] = ycrcb_planes2[0] + (newValue2 - 128)* contrast;

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

				// 흑백 이미지를 BGR 이미지로 변환
				Mat colorImage;
				cvtColor(myImg, colorImage, COLOR_GRAY2BGR);

				// BGR 이미지를 YCrCb 이미지로 변환
				Mat ycrcbImage;
				cvtColor(colorImage, ycrcbImage, COLOR_BGR2YCrCb);

				// YCrCb 이미지에서 Y 채널 추출
				std::vector<Mat> channels;
				split(ycrcbImage, channels);
				float contrast=1.0;
				
				// Y 채널에 대해 히스토그램 평활화 적용
				equalizeHist(channels[0], channels[0]);
				channels [0] = channels[0] + (newValue2 - 128) * contrast;
				Mat yChannel = channels[0];
				// 평활화된 Y 채널을 다시 복원
				merge(channels, ycrcbImage);

				// YCrCb 이미지를 BGR 이미지로 변환
				cvtColor(ycrcbImage, colorImage, COLOR_YCrCb2BGR);

				// 변환된 이미지를 다시 흑백으로 변환
				cvtColor(colorImage, myImg, COLOR_BGR2GRAY);

				// 이미지 표시
				CreateBitmapInfo(&BitChangeImg, myImg.cols, myImg.rows, myImg.channels() * 8);
				DrawImage(myImg, BitChangeImg);

				resultImg = myImg.clone();
				BitChangeResultImg = BitChangeImg2;
				
			}


			

		}

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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

	//m_spin.SetPos(m_edit_val); // 스핀 컨트롤에 새로운 값 설정
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
		CreateBitmapInfo(&BitChangeImg, adjustedImage.cols, adjustedImage.rows, adjustedImage.channels() * 8);
		DrawImage(adjustedImage, BitChangeImg);
		resultImg = adjustedImage.clone();
		BitChangeResultImg = BitChangeImg;

	}

	*pResult = 0;
}



//
void CBRIGHTNESSCTRL::mspin_updown2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown2 = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

		// 스핀 컨트롤의 값이 변경될 때
	if (pNMUpDown2->iDelta < 0) {//위로 증가 버튼눌렀을 때 iDelta 음수가 출력 
		m_edit_val2 += 10;
	}
	else {//아래 버튼을 눌렀을 때 iDelta 양수가 출력된다 
		m_edit_val2 -= 10;
	}

	m_spin2.SetPos(m_edit_val2); // 스핀 컨트롤에 새로운 값 설정
	m_slider2.SetPos(m_edit_val2); // 슬라이더에도 새로운 값 설정
	CString strValue2;
	strValue2.Format(_T("%d"), m_edit_val2);
	m_edit2.SetWindowText(strValue2); // 에디트 컨트롤에도 새로운 값 설정

	if ((myImg.channels() == 3))
	{
		Mat tmpImg2 = myImg.clone();
		cvtColor(tmpImg2, tmpImg2, COLOR_BGR2YCrCb);

		std::vector<Mat> ycrcb_planes2;
		

		split(tmpImg2, ycrcb_planes2);
		float contrast = 1.0;

		ycrcb_planes2[0] = ycrcb_planes2[0] + (m_edit_val2 - 128) * contrast;

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
		// 흑백 이미지를 BGR 이미지로 변환
		Mat colorImage;
		cvtColor(myImg, colorImage, COLOR_GRAY2BGR);

		// BGR 이미지를 YCrCb 이미지로 변환
		Mat ycrcbImage;
		cvtColor(colorImage, ycrcbImage, COLOR_BGR2YCrCb);

		// YCrCb 이미지에서 Y 채널 추출
		std::vector<Mat> channels;
		split(ycrcbImage, channels);
		float contrast = 1.0;

		// Y 채널에 대해 히스토그램 평활화 적용
		equalizeHist(channels[0], channels[0]);
		channels[0] = channels[0] + (m_edit_val2 - 128) * contrast;
		Mat yChannel = channels[0];
		// 평활화된 Y 채널을 다시 복원
		merge(channels, ycrcbImage);

		// YCrCb 이미지를 BGR 이미지로 변환
		cvtColor(ycrcbImage, colorImage, COLOR_YCrCb2BGR);

		// 변환된 이미지를 다시 흑백으로 변환
		cvtColor(colorImage, myImg, COLOR_BGR2GRAY);

		// 이미지 표시
		CreateBitmapInfo(&BitChangeImg, myImg.cols, myImg.rows, myImg.channels() * 8);
		DrawImage(myImg, BitChangeImg);

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
