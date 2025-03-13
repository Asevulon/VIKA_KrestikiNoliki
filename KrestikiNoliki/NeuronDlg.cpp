// NeuronDlg.cpp: файл реализации
//

#include "pch.h"
#include "KrestikiNoliki.h"
#include "afxdialogex.h"
#include "NeuronDlg.h"
#include <thread>


// Диалоговое окно NeuronDlg

IMPLEMENT_DYNAMIC(NeuronDlg, CDialogEx)

NeuronDlg::NeuronDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

NeuronDlg::~NeuronDlg()
{
}

void NeuronDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, prog);
	DDX_Control(pDX, IDC_PROG_TEXT, progtext);
}


BEGIN_MESSAGE_MAP(NeuronDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NeuronDlg::OnBnClickedOk)
	ON_MESSAGE(TRAIN_DONE, &NeuronDlg::OnTrainDone)
	ON_BN_CLICKED(IDC_BUTTON1, &NeuronDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &NeuronDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &NeuronDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &NeuronDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Обработчики сообщений NeuronDlg


void NeuronDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!t.Init())
	{
		t.reset();
		t.Fill();
	}
	timerid = SetTimer(123, 500, NULL);
	thread TrainThread([&]() {t.Train(); KillTimer(timerid); PostMessageW(TRAIN_DONE); });
	TrainThread.detach();
}


afx_msg LRESULT NeuronDlg::OnTrainDone(WPARAM wParam, LPARAM lParam)
{
	prog.SetPos(100);
	progtext.SetWindowTextW(L"Завершено!");
	return 0;
}


BOOL NeuronDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	prog.SetRange(0, 100);
	prog.SetPos(0);

	progtext.SetWindowTextW(L"");
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void NeuronDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	t.Save();
}


void NeuronDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	t.Load();
}


void NeuronDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	prog.SetPos(t.GetProgress());
	CString str;
	str.Format(L"Прогресс %.2f%%, error: %.3f, Выборка: %d", t.GetProgress(), t.Error(), t.size());
	progtext.SetWindowTextW(str);
	CDialogEx::OnTimer(nIDEvent);
}


void NeuronDlg::OnBnClickedButton3()
{
	// TODO: добавьте свой код обработчика уведомлений
	timerid = SetTimer(123, 500, NULL);
	thread TrainThread([&]() {t.TrainORO(); KillTimer(timerid); PostMessageW(TRAIN_DONE); });
	TrainThread.detach();
}


void NeuronDlg::OnBnClickedButton4()
{
	// TODO: добавьте свой код обработчика уведомлений
	timerid = SetTimer(123, 500, NULL);
	thread TrainThread([&]() {t.TrainORO(true); KillTimer(timerid); PostMessageW(TRAIN_DONE); });
	TrainThread.detach();
}
