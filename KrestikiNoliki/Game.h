#pragma once
#include<vector>
#include"NewronWeb.h"
using namespace std;

enum GameResult {
	draw = 0,
	winner = 1,
	loser = -1,
	forbedden = -2
};

struct TurnResponse {
	vector<int>*field = nullptr;
	bool success = false;
	bool forbidden = false;
	TurnResponse(vector<int>* field) :field(field) {};
	TurnResponse(vector<int>* field, bool success) :field(field), success(success) {};
};
class Player {
private:
	GameResult res;
	double score = 0;
protected:
public:
	virtual void DoTurn(TurnResponse&) = 0;
	void SetGameResult(GameResult val);
	GameResult GetGameResult();
	virtual void Delay(int val) = 0;
	void SetScore(double val);
	int GetScore();
};

class Human : public Player {
private:
protected:
public:
	void DoTurn(TurnResponse& resp);
	void Delay(int val);
};

class MinMax : public Player {
private:
protected:
public:
	void DoTurn(TurnResponse& resp);
	void Delay(int val);
};

class NeuronPlayer : public Player {
private:
	NeuronWeb nw;
protected:
public:
	void SetNW(NeuronWeb& p);
	void DoTurn(TurnResponse& resp);
	void Delay(int val);
};
class Game
{
private:
	bool init = false;

	Player* p1 = nullptr;
	Player* p2 = nullptr;
	
	vector<int> field;
	int empty = 0;

	bool finish = false;

	bool turn = true;
	bool manualturn = false;

	int delay = 0;

	const double winnerscore = 1;
	const double loserscore = 0;
	const double drawscore = 0.5;
	const double forbiddenscore = -1;
protected:
	bool DetermineWinner();
	inline bool equal(int id1, int id2, int id3);
	inline void parse(int id); 
	inline void InverseField();
	void Score(Player* target);
public:
	Game& Game::operator =(const Game&);
	void SetPlayers(Player* p1, Player* p2);
	void Start();
	void P1Turn();
	void P2Turn();
	void manP1Turn(int pos);
	void manP2Turn(int pos);
	void manPTurn(int pos);
	bool GetFinish();
	bool GameEnd();
	vector<int> GetField();
	int GetMoves();
	bool GetTurn();
	void SetDelay(int val);
	bool ManualTurn();
	void Autogame();
	void Score();
};














































































































































































































































































































class NeuronWebPlayer : public Player {
private:
	MinMax mm;
	NeuronPlayer np;
	Player* rp = nullptr;
	int cap = 500;
public:
	void SetNW(NeuronWeb& nw);
	void DoTurn(TurnResponse& resp);
	void Delay(int val);
};
