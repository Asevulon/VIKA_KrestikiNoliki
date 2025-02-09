#pragma once
#include "afxdialogex.h"
#include "Drawer.h"
#include "Game.h"
// Диалоговое окно GameDlg
#define TURN_PLAYER1 WM_USER + 2
#define TURN_PLAYER2 WM_USER + 3
#define GAME_DONE WM_USER + 4

class GameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GameDlg)

public:
	GameDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~GameDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl Delay;
	virtual BOOL OnInitDialog();
	Drawer GameDrw;
	afx_msg void OnBnClickedOk();
	Game game;
	Player* p1;
	Player* p2;
protected:
	afx_msg LRESULT OnTurnPlayer1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTurnPlayer2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTurnDone(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGameDone(WPARAM wParam, LPARAM lParam);
public:
	CStatic GameText;
};
