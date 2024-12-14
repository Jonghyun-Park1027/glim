
// gPrjDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "gPrj.h"
#include "gPrjDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "CProcess.h"
#include <chrono>
#include <thread>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
using namespace std;

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


// CgPrjDlg 대화 상자



CgPrjDlg::CgPrjDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GPRJ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	int m_x1 = 0;
	int m_x2 = 0;
	int m_y1 = 0;
	int m_y2 = 0;
	int pixel_offset = 0;
}

void CgPrjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X1, m_x1);
	DDX_Text(pDX, IDC_EDIT_Y1, m_y1);
	DDX_Text(pDX, IDC_EDIT_X2, m_x2);
	DDX_Text(pDX, IDC_EDIT_Y2, m_y2);
	DDX_Text(pDX, IDC_EDIT_PIXEL_OFFSET, pixel_offset);
}

BEGIN_MESSAGE_MAP(CgPrjDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_DRAW, &CgPrjDlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CgPrjDlg::OnBnClickedBtnAction)
	//ON_BN_CLICKED(IDC_BTN_MAKE_PATTERN, &CgPrjDlg::OnBnClickedBtnMakePattern)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CgPrjDlg::OnBnClickedBtnOpen)
	//ON_EN_CHANGE(IDC_EDIT_X1, &CgPrjDlg::OnEnChangeEditX1)
END_MESSAGE_MAP()


// CgPrjDlg 메시지 처리기

BOOL CgPrjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(0, 0, 1500, 1000);
	m_pDlgImage = new CDlgImage;
	m_pDlgImage->Create(IDD_CDlgImage, this);
	m_pDlgImage->ShowWindow(SW_SHOW);
	m_pDlgImage->MoveWindow(0, 0, 1280, 800);
	unsigned char* fm = (unsigned char*)m_pDlgImage->m_image.GetBits();
	int nWidth = m_pDlgImage->m_image.GetWidth();
	int nHeight = m_pDlgImage->m_image.GetHeight();
	int nPitch = m_pDlgImage->m_image.GetPitch();
	memset(fm, 0, nWidth * nHeight);
	/*m_pDlgImgResult = new CDlgImage;
	m_pDlgImgResult->Create(IDD_CDlgImage, this);
	m_pDlgImgResult->ShowWindow(SW_SHOW);
	m_pDlgImgResult->MoveWindow(640, 0, 640, 480);*/
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CgPrjDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CgPrjDlg::OnPaint()
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
HCURSOR CgPrjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CgPrjDlg::OnBnClickedBtnDlg()
//{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	m_pDlgImage->ShowWindow(SW_SHOW);
//}


void CgPrjDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pDlgImage) delete m_pDlgImage;
	//if (m_pDlgImgResult) delete m_pDlgImgResult;
}

void CgPrjDlg::callFunc(int n)
{
	//int nData = n;
	cout << n << endl;

}

//void CgPrjDlg::OnBnClickedBtnDraw()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	unsigned char* fm = (unsigned char*)m_pDlgImage->m_image.GetBits();
//	int nWidth = m_pDlgImage->m_image.GetWidth();
//	int nHeight = m_pDlgImage->m_image.GetHeight();
//	int nPitch = m_pDlgImage->m_image.GetPitch();
//	memset(fm, 0, nWidth * nHeight);
//
//
//	
//
//	m_pDlgImage->Invalidate();
//
//	
//}
void CgPrjDlg::OnBnClickedBtnDraw()
{
	// 에디트 박스 값 읽기
	UpdateData(TRUE);

	// 사용자가 입력한 좌표 (X1, Y1)를 가져옵니다.
	int x1 = m_x1; // IDC_EDIT_X1에서 입력된 값
	int y1 = m_y1; // IDC_EDIT_Y1에서 입력된 값

	// 이미지 버퍼 가져오기
	unsigned char* fm = (unsigned char*)m_pDlgImage->m_image.GetBits();
	int nWidth = m_pDlgImage->m_image.GetWidth();
	int nHeight = m_pDlgImage->m_image.GetHeight();
	int nPitch = m_pDlgImage->m_image.GetPitch();

	// 데이터 유효성 검증
	if (fm == nullptr || nWidth == 0 || nHeight == 0 || nPitch == 0) {
		AfxMessageBox(_T("이미지 데이터가 유효하지 않습니다."));
		return;
	}

	// 이미지 버퍼 초기화
	try {
		memset(fm, 0, nPitch * nHeight);
	}
	catch (...) {
		AfxMessageBox(_T("이미지 버퍼 초기화 중 오류가 발생했습니다."));
		return;
	}

	// 랜덤 반지름 생성
	srand((unsigned)time(NULL));
	int radius = rand() % 50 + 10; // 반지름은 10~60 사이로 랜덤 설정

	// GDI를 사용하여 원 그리기
	HDC hdc = m_pDlgImage->m_image.GetDC();
	if (!hdc) {
		AfxMessageBox(_T("GDI DC를 가져올 수 없습니다."));
		return;
	}

	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 빨간색 펜
	if (!hPen) {
		AfxMessageBox(_T("펜을 생성할 수 없습니다."));
		m_pDlgImage->m_image.ReleaseDC();
		return;
	}

	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // 투명 브러시
	HGDIOBJ oldPen = SelectObject(hdc, hPen);
	HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

	// 원 그리기
	Ellipse(hdc, x1 - radius, y1 - radius, x1 + radius, y1 + radius);

	// GDI 객체 해제
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hPen);
	m_pDlgImage->m_image.ReleaseDC();

	// 화면 갱신
	m_pDlgImage->Invalidate();
}




void CgPrjDlg::OnBnClickedBtnAction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int x1 = m_x1; // 시작 좌표
	int y1 = m_y1;
	int x2 = m_x2; // 목표 좌표
	int y2 = m_y2;
	int pixel = pixel_offset; // 이동 간격

	// 이미지 버퍼 초기화
	unsigned char* fm = (unsigned char*)m_pDlgImage->m_image.GetBits();
	int nWidth = m_pDlgImage->m_image.GetWidth();
	int nHeight = m_pDlgImage->m_image.GetHeight();
	int nPitch = m_pDlgImage->m_image.GetPitch();

	std::thread animationThread([=]() {
		HDC hdc = m_pDlgImage->m_image.GetDC();
		HPEN threadPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HBRUSH threadBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

		int currentX = x1;
		int currentY = y1;
		int frame = 0; // 프레임 번호

		while (true)
		{
			// 현재 좌표에서 목표 좌표로 pixel만큼 이동
			int dx = (x2 > currentX) ? pixel : -pixel;
			int dy = (y2 > currentY) ? pixel : -pixel;

			if (abs(x2 - currentX) < pixel) dx = x2 - currentX;
			if (abs(y2 - currentY) < pixel) dy = y2 - currentY;

			currentX += dx;
			currentY += dy;

			// 화면 초기화
			memset(fm, 0, nWidth * nHeight);

			// 원 그리기
			HGDIOBJ oldPen = SelectObject(hdc, threadPen);
			HGDIOBJ oldBrush = SelectObject(hdc, threadBrush);
			int radius = 30; // 고정 반지름
			Ellipse(hdc, currentX - radius, currentY - radius, currentX + radius, currentY + radius);

			SelectObject(hdc, oldPen);
			SelectObject(hdc, oldBrush);

			// 화면 갱신
			m_pDlgImage->Invalidate(FALSE);
			m_pDlgImage->UpdateWindow();

			// BMP 파일로 저장
			CString folderPath = _T("image");
			CreateDirectory(folderPath, NULL);
			CString filePath;
			filePath.Format(_T("%s\\frame_%03d.bmp"), folderPath, frame++);
			m_pDlgImage->m_image.Save(filePath);

			// 목표 지점 도달 시 종료
			if (currentX == x2 && currentY == y2) break;

			// 0.5초 대기
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		DeleteObject(threadPen);
		DeleteObject(threadBrush);
		m_pDlgImage->m_image.ReleaseDC();
		});

	animationThread.detach();
}



//void CgPrjDlg::OnBnClickedBtnMakePattern()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	unsigned char* fm = (unsigned char*)m_pDlgImage->m_image.GetBits();
//	int nWidth = m_pDlgImage->m_image.GetWidth();
//	int nHeight = m_pDlgImage->m_image.GetHeight();
//	int nPitch = m_pDlgImage->m_image.GetPitch();
//	memset(fm, 0, nWidth * nHeight);
//
//	CRect rect(100, 100, 200, 200);
//
//	for (int j = rect.top; j < rect.bottom; j++) {
//		for (int i = rect.left; i < rect.right; i++) {
//			fm[j * nPitch + i] = rand()%0xff;
//		}
//	}
//	m_pDlgImage->Invalidate();
//}


//void CgPrjDlg::OnBnClickedBtnOpen()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	unsigned char* fm = (unsigned char*)m_pDlgImage->m_image.GetBits();
//	int nWidth = m_pDlgImage->m_image.GetWidth();
//	int nHeight = m_pDlgImage->m_image.GetHeight();
//	int nPitch = m_pDlgImage->m_image.GetPitch();
//
//	int nTh = 0x80;
//	CRect rect(0, 0, nWidth, nHeight);
//	int nSumX = 0;
//	int nSumY = 0;
//	int nCount = 0;
//	for (int j = rect.top; j < rect.bottom; j++) {
//		for (int i = rect.left; i < rect.right; i++) {
//			if (fm[j * nPitch + i] > nTh) {
//				nSumX += i;
//				nSumY += j;
//				nCount++;
//			}
//		}
//	}
//	double dCenterX = (double)nSumX / nCount;
//	double dCenterY = (double)nSumY / nCount;
//
//	cout << dCenterX << "\t" << dCenterY << endl;
//
//
//}
#include <atlimage.h>
#include <commdlg.h>

void CgPrjDlg::OnBnClickedBtnOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	 // 파일 열기 대화상자 표시
	CFileDialog fileDlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Bitmap Files (*.bmp)|*.bmp||"));

	if (fileDlg.DoModal() != IDOK) {
		return; // 사용자가 취소를 누르면 종료
	}

	CString filePath = fileDlg.GetPathName();

	// 기존 이미지를 해제
	m_pDlgImage->m_image.Destroy();

	// 이미지 로드
	if (FAILED(m_pDlgImage->m_image.Load(filePath))) {
		AfxMessageBox(_T("이미지를 로드할 수 없습니다."));
		return;
	}

	// 이미지 버퍼 가져오기
	unsigned char* fm = (unsigned char*)m_pDlgImage->m_image.GetBits();
	int nWidth = m_pDlgImage->m_image.GetWidth();
	int nHeight = m_pDlgImage->m_image.GetHeight();
	int nPitch = m_pDlgImage->m_image.GetPitch();

	if (fm == nullptr || nWidth == 0 || nHeight == 0) {
		AfxMessageBox(_T("이미지 데이터가 유효하지 않습니다."));
		return;
	}

	CRect rect(0, 0, nWidth, nHeight);
	int nSumX = 0;
	int nSumY = 0;
	int nCount = 0;

	// 검정색이 아닌 픽셀 기준으로 유효 픽셀 찾기
	for (int j = rect.top; j < rect.bottom; j++) {
		for (int i = rect.left; i < rect.right; i++) {
			unsigned char pixelValue = fm[j * nPitch + i];
			if (pixelValue != 0) { // 검정색(0)이 아닌 경우
				nSumX += i;
				nSumY += j;
				nCount++;
			}
		}
	}

	if (nCount == 0) {
		AfxMessageBox(_T("이미지에서 유효한 픽셀을 찾을 수 없습니다."));
		return;
	}

	double dCenterX = (double)nSumX / nCount;
	double dCenterY = (double)nSumY / nCount;

	// GDI를 사용하여 X 모양 그리기 및 좌표 표시
	HDC hdc = m_pDlgImage->m_image.GetDC();
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HGDIOBJ oldPen = SelectObject(hdc, hPen);

	// X 모양 그리기
	int crossSize = 10;
	MoveToEx(hdc, (int)dCenterX - crossSize, (int)dCenterY - crossSize, NULL);
	LineTo(hdc, (int)dCenterX + crossSize, (int)dCenterY + crossSize);
	MoveToEx(hdc, (int)dCenterX - crossSize, (int)dCenterY + crossSize, NULL);
	LineTo(hdc, (int)dCenterX + crossSize, (int)dCenterY - crossSize);

	// 좌표값 출력
	CString coordText;
	coordText.Format(_T("(%.1f, %.1f)"), dCenterX, dCenterY);
	TextOut(hdc, (int)dCenterX + crossSize, (int)dCenterY + crossSize, coordText, coordText.GetLength());

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	m_pDlgImage->m_image.ReleaseDC();

	// 화면 갱신
	m_pDlgImage->Invalidate();
	m_pDlgImage->UpdateWindow();
}