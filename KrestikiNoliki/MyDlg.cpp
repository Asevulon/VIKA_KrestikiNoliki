
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "KrestikiNoliki.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KRESTIKINOLIKI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, ComboPlayer1);
	DDX_Control(pDX, IDC_COMBO2, ComboPlayer2);
	DDX_Control(pDX, IDC_CHOSEN, chosenNW);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MyDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	auto FillCombo = [](CComboBox& target) {target.AddString(L"Игрок"); target.AddString(L"OРO"); target.AddString(L"Нейросеть"); target.SetCurSel(1); };
	FillCombo(ComboPlayer1);
	FillCombo(ComboPlayer2);


	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	GameDlg gdlg;
	auto GetPlayer = [](CComboBox& source, CString& path) -> Player* {
		CString str; source.GetWindowTextW(str);
		if (str == L"Игрок")return new Human();
		else if (str == L"OРO") {
			if (path.IsEmpty())path = L"Best\\Best.MatrixForm";
			MatrixForm mf; mf.Load(path); MatrixFormPlayer* p = new MatrixFormPlayer; p->SetMatrixForm(mf); return p;
		}
		else if (str == L"Нейросеть")
			if (path.IsEmpty())return nullptr;
			else { NeuronWeb nw; nw.Load(path); NeuronWebPlayer* p = new NeuronWebPlayer; p->SetNW(nw); return p; }
		};

	Player* p1 = GetPlayer(ComboPlayer1, ChosenPath);
	if (p1 == nullptr)return;
	Player* p2 = GetPlayer(ComboPlayer2, ChosenPath);
	if (p2 == nullptr) { delete p1; return; }

	gdlg.p1 = p1;
	gdlg.p2 = p2;

	auto InputInit = [](CComboBox& source) -> bool {CString str; source.GetWindowTextW(str); if (str == L"Игрок")return true; return false; };
	gdlg.GameDrw.init = InputInit(ComboPlayer1) || InputInit(ComboPlayer2);

	gdlg.DoModal();
	delete p1;
	delete p2;
}

void MyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	NeuronDlg dlg;
	dlg.DoModal();
}

CString GetFileNameFromPath(const CString& filePath) {
	int nFind = filePath.ReverseFind('\\'); // Find the last backslash
	if (nFind >= 0) {
		return filePath.Right(filePath.GetLength() - nFind - 1); // Extract the filename from the right
	}
	else {
		// If there's no backslash, it means the filePath *is* the filename
		return filePath;
	}
}
void MyDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	CFileDialog fileDialog(
		TRUE,  // TRUE for Open dialog
		L".*", // Default extension
		NULL,  // Initial file name (none)
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, // Flags
		//L"NW Files (*.nw)|*.nw||" // Filter
		L""
	);
	fileDialog.m_ofn.lpstrInitialDir = L"C:\\Users\\asevu\\source\\repos\\KrestikiNoliki\\KrestikiNoliki\\Best";

	if (fileDialog.DoModal() == IDOK) {
		CString filePath = fileDialog.GetPathName();
		chosenNW.SetWindowTextW(GetFileNameFromPath(filePath));
		ChosenPath = filePath;
	}
}
