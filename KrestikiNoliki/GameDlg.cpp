// GameDlg.cpp: файл реализации
//

#include "pch.h"
#include "KrestikiNoliki.h"
#include "afxdialogex.h"
#include "GameDlg.h"


// Диалоговое окно GameDlg

IMPLEMENT_DYNAMIC(GameDlg, CDialogEx)

GameDlg::GameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

GameDlg::~GameDlg()
{
}

void GameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, Delay);
	DDX_Control(pDX, IDC_GAME_PICTURE, GameDrw);
	DDX_Control(pDX, IDC_TEXT, GameText);
}


BEGIN_MESSAGE_MAP(GameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &GameDlg::OnBnClickedOk)
	ON_MESSAGE(TURN_PLAYER1, &GameDlg::OnTurnPlayer1)
	ON_MESSAGE(TURN_PLAYER2, &GameDlg::OnTurnPlayer2)
	ON_MESSAGE(TURN_DONE, &GameDlg::OnTurnDone)
	ON_MESSAGE(GAME_DONE, &GameDlg::OnGameDone)
END_MESSAGE_MAP()


// Обработчики сообщений GameDlg


BOOL GameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	Delay.SetRange(0, 10);
	Delay.SetPos(1);

	GameDrw.parent = GetSafeHwnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void GameDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	game = Game();
	game.SetPlayers(p1, p2);
	game.SetDelay(Delay.GetPos() * 250);
	game.Start();

	GameText.SetWindowTextW(L"");

	GameDrw.SetData(game.GetField());
	SendMessageW(TURN_PLAYER1);
}


afx_msg LRESULT GameDlg::OnTurnPlayer1(WPARAM wParam, LPARAM lParam)
{
	game.P1Turn();
	GameDrw.SetData(game.GetField());
	if ((!game.GameEnd()) && (!game.ManualTurn()))SendMessageW(TURN_PLAYER2);
	else SendMessageW(GAME_DONE);
	return 0;
}


afx_msg LRESULT GameDlg::OnTurnPlayer2(WPARAM wParam, LPARAM lParam)
{
	game.P2Turn();
	GameDrw.SetData(game.GetField());
	if ((!game.GameEnd()) && (!game.ManualTurn()))SendMessageW(TURN_PLAYER1);
	else SendMessageW(GAME_DONE);
	return 0;
}


afx_msg LRESULT GameDlg::OnTurnDone(WPARAM wParam, LPARAM lParam)
{
	if (game.GetFinish())return 0;
	game.manPTurn(wParam);
	GameDrw.SetData(game.GetField());
	if (!game.GameEnd())
	{
		if(game.GetTurn())SendMessageW(TURN_PLAYER1);
		else SendMessageW(TURN_PLAYER2);
	}
	else SendMessageW(GAME_DONE);

	return 0;
}


afx_msg LRESULT GameDlg::OnGameDone(WPARAM wParam, LPARAM lParam)
{
	CString str;
	if (p1->GetGameResult() == winner)str = L"Победа игрока 1";
	else if (p2->GetGameResult() == winner)str = L"Победа игрока 2";
	else str = L"Ничья";
	GameText.SetWindowTextW(str);
	return 0;
}
