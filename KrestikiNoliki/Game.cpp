#include "pch.h"
#include "Game.h"
#include "MinMax.h"

bool Game::DetermineWinner()
{
	bool found = false;
	if (equal(0, 1, 2))
	{
		parse(0);
		found = true;
	}
	else if (equal(3, 4, 5))
	{
		parse(3);
		found = true;
	}
	else if (equal(6, 7, 8))
	{
		parse(6);
		found = true;
	}

	else if (equal(0, 3, 6))
	{
		parse(0);
		found = true;
	}
	else if (equal(1, 4, 7))
	{
		parse(1);
		found = true;
	}
	else if (equal(2, 5, 8))
	{
		parse(2);
		found = true;
	}

	else if (equal(0, 4, 8))
	{
		parse(0);
		found = true;
	}
	else if (equal(2, 4, 6))
	{
		parse(2);
		found = true;
	}

	else if (empty < 1)
	{
		p1->SetGameResult(draw);
		p2->SetGameResult(draw);
		found = true;
	}
	return found;
}

Game& Game::operator=(const Game& rhs)
{
	// TODO: вставьте здесь оператор return
	init = rhs.init;
	p1 = rhs.p1;
	p2 = rhs.p2;
	field = rhs.field;
	empty = rhs.empty;
	finish = rhs.finish;
	turn = rhs.turn;
	manualturn = rhs.manualturn;
	delay = rhs.delay;
	return *this;
}

inline bool Game::equal(int id1, int id2, int id3)
{
	return (field[id1] == field[id2]) && (field[id1] == field[id3]) && (field[id1] != 0);
}

inline void Game::parse(int id)
{
	switch (field[id])
	{
	case 1:
		p1->SetGameResult(winner);
		p2->SetGameResult(loser);
		break;
	case -1:
		p1->SetGameResult(loser);
		p2->SetGameResult(winner);
		break;
	default:
		p1->SetGameResult(draw);
		p2->SetGameResult(draw);
		break;
	}
}

void Game::SetPlayers(Player* P1, Player* P2)
{
	p1 = P1;
	p2 = P2;
	init = true;
}

void Game::Start()
{
	if (!init)return;
	field = vector<int>(9, 0);
	finish = false;
	empty = 9;
	turn = true;
}

void Game::P1Turn()
{
	if ((!init) || (finish))return;
	TurnResponse resp(&field);
	p1->DoTurn(resp);
	p1->Delay(delay);
	if (resp.forbidden)
	{
		p1->SetGameResult(forbedden);
		p2->SetGameResult(winner);
		finish = true;
	}
	if (resp.success)
	{
		turn = !turn;
		empty--;
		lastPos = resp.pos;
	}
	else manualturn = true;
}

inline void Game::InverseField()
{
	for (int i = 0; i < 9; i++)field[i] = -field[i];
}

void Game::P2Turn()
{
	if ((!init) || (finish))return;
	InverseField();
	TurnResponse resp(&field);
	p2->DoTurn(resp);
	p2->Delay(delay);
	InverseField();
	if (resp.forbidden)
	{
		p1->SetGameResult(winner);
		p2->SetGameResult(forbedden);
		finish = true;
	}
	if (resp.success)
	{
		turn = !turn;
		empty--;
		lastPos = resp.pos;
	}
	else manualturn = true;
}

bool Game::GetFinish()
{
	return finish;
}

bool Game::GameEnd()
{
	if (finish)return finish;
	finish = DetermineWinner();
	return finish;
}

vector<int> Game::GetField()
{
	return field;
}

void Game::manP1Turn(int pos)
{
	field[pos] = 1;
	turn = !turn;
	manualturn = false;
	empty--;
}

void Game::manPTurn(int pos)
{
	if (turn)field[pos] = 1;
	else field[pos] = -1;
	turn = !turn;
	manualturn = false;
	empty--;
}

int Game::GetMoves()
{
	return 9 - empty;
}

bool Game::GetTurn()
{
	return turn;
}

void Game::SetDelay(int val)
{
	delay = val;
}

bool Game::ManualTurn()
{
	return manualturn;
}

void Game::Score(Player* target)
{
	GameResult gr = target->GetGameResult();
	switch (gr)
	{
	case draw:
		target->SetScore(drawscore);
		break;
	case winner:
		target->SetScore(winnerscore);
		break;
	case loser:
		target->SetScore(loserscore);
		break;
	case forbedden:
		target->SetScore(forbiddenscore);
		break;
	default:
		break;
	}
}

void Game::Autogame()
{
	Start();
	do
	{
		P1Turn();
		if (GameEnd())return;
		P2Turn();
		if (GameEnd())return;
	} while (!finish);
}

void Game::Score()
{
	if (!finish)return;
	Score(p1);
	Score(p2);
}

int Game::GetLastTurn()
{
	return lastPos;
}

void Game::manP2Turn(int pos)
{
	field[pos] = -1;
	turn = !turn;
	manualturn = false;
	empty--;
}

void Player::SetGameResult(GameResult val)
{
	res = val;
}

GameResult Player::GetGameResult()
{
	return res;
}

void Player::SetScore(double val)
{
	score = val;
}

int Player::GetScore()
{
	return score;
}

void Human::DoTurn(TurnResponse& resp)
{
	resp.success = false;
}

void Human::Delay(int val)
{
	return;
}

void MinMax::DoTurn(TurnResponse& resp)
{
	auto& data = *resp.field;
	int zeroes = 0;
	for (int i = 0; i < 9; i++)
	{
		if (data[i] == 0) zeroes++;
	}
	minmax(data, zeroes, 1, true, 0);
	resp.pos = minmaxres();
	data[minmaxres()] = 1;
	resp.success = true;
}

void MinMax::Advice(TurnResponse& resp)
{
	auto& data = *resp.field;
	int zeroes = 0;
	for (int i = 0; i < 9; i++)
	{
		if (data[i] == 0) zeroes++;
	}
	minmax(data, zeroes, 1, true, 0);
	resp.pos = minmaxres();
	resp.success = true;
}

void MinMax::Delay(int val)
{
	Sleep(val);
}

void NeuronPlayer::DoTurn(TurnResponse& resp)
{
	auto& data = *resp.field;
	int pos = nw.Move(data);
	if (data[pos] != 0)resp.forbidden = true;
	else data[pos] = 1;
	resp.success = true;
	resp.pos = pos;
}

void NeuronPlayer::Delay(int val)
{
	Sleep(val);
}

void NeuronPlayer::SetNW(NeuronWeb& p)
{
	nw = p;
}

void MatrixFormPlayer::SetMatrixForm(MatrixForm& MF)
{
	mf = MF;
}

void MatrixFormPlayer::Delay(int val)
{
	Sleep(val);
}

void MatrixFormPlayer::DoTurn(TurnResponse& resp)
{
	auto& data = *resp.field;
	int pos = mf.Move(data);
	if (data[pos] != 0)resp.forbidden = true;
	else data[pos] = 1;
	resp.success = true;
	resp.pos = pos;
}

void RandomPlayer::DoTurn(TurnResponse& resp)
{
	auto& data = *resp.field;
	vector<int>empty;
	for (int i = 0; i < 9; i++)if (data[i] == 0)empty.push_back(i);
	int pos = empty[(empty.size() - 1e-14) * (double)rand()/(double)RAND_MAX];
	if (data[pos] != 0)abort();
	else data[pos] = 1;
	resp.success = true;
	resp.pos = pos;

}
void RandomPlayer::Delay(int val)
{
	Sleep(val);
}




























































































































































void NeuronWebPlayer::SetNW(NeuronWeb& nw)
{
	np.SetNW(nw);
	const CString bestDir = L"Best\\";
	ifstream in(bestDir + L"itr");
	int counter = 0;
	in >> counter;
	in.close();

	if (counter > cap)rp = &mm;
	else rp = &np;
}

void NeuronWebPlayer::DoTurn(TurnResponse& resp)
{
	rp->DoTurn(resp);
}

void NeuronWebPlayer::Delay(int val)
{
	rp->Delay(val);
}

void MatrixFormOroPlayer::SetMatrixForm(MatrixForm& MF)
{
}

void MatrixFormOroPlayer::DoTurn(TurnResponse& resp)
{
	mm.DoTurn(resp);
}

void MatrixFormOroPlayer::Delay(int val)
{
	mm.Delay(val);
}

