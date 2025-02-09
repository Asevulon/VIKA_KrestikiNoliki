#pragma once
#include "afxdialogex.h"
#include "NewronWeb.h"

// Диалоговое окно NeuronDlg
#define TRAIN_DONE WM_USER + 1

class NeuronDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NeuronDlg)

public:
	NeuronDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~NeuronDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CProgressCtrl prog;
	CStatic progtext;
	Trainer t;
	UINT_PTR timerid;
protected:
	afx_msg LRESULT OnTrainDone(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
