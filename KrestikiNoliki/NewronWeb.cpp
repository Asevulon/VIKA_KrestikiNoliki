#include "pch.h"
#include "NewronWeb.h"
#include "Game.h"
double rand(double left, double right)
{
	return left + (right - left) * double(rand()) / double(RAND_MAX);
}
NeuronWeb::Neuron::Neuron(int size)
{
	fi = rand(-0.5, 0.5);
	w.resize(size);
	for (int i = 0; i < size; i++)
	{
		w[i] = rand(-0.5, 0.5);
	}
}

inline double NeuronWeb::Neuron::activation(double x)
{
	//return 1. / (1. + exp(-2 * x));
	return max(0, x);
}

void NeuronWeb::Neuron::Process(vector<double>& in)
{
	out = 0;
	for (int i = 0; i < w.size(); i++)
	{
		out += in[i] * w[i];
	}
	out -= fi;
	out = activation(out);
}

void NeuronWeb::Neuron::Process(vector<int>& in)
{
	out = 0;
	for (int i = 0; i < w.size(); i++)
	{
		out += in[i] * w[i];
	}
	out -= fi;
	out = activation(out);
}

void NeuronWeb::Neuron::ProcessNoActivation(vector<double>& in)
{
	out = 0;
	for (int i = 0; i < w.size(); i++)
	{
		out += in[i] * w[i];
	}
	out -= fi;
}

void NeuronWeb::Fill(vector<int> str)
{
	Structure = str;
	
	layers.resize(Structure.size() - 1);

	for (int i = 1; i < Structure.size(); i++)
	{
		layers[i - 1].clear();
		for (int j = 0; j < Structure[i]; j++)
		{
			layers[i - 1].push_back(Neuron(Structure[i - 1]));
		}
	}
	MaxLayerSize = *max_element(Structure.begin(), Structure.end());
}

double NeuronWeb::Calculate(vector<double>& input)
{
	vector<double>in(MaxLayerSize, 0);

	for (int i = 0; i < layers[0].size(); i++)
	{
		layers[0][i].Process(input);
	}

	for (int i = 1; i < layers.size(); i++)
	{
		auto& layer = layers[i];
		auto& prevlayer = layers[i - 1];
		FillIn(in, prevlayer);

		for (auto& neuron : layer)
		{
			neuron.Process(in);
		}
	}

	return layers.back()[0].out;
}

double NeuronWeb::Calculate(vector<int>& input)
{
	vector<double>in(MaxLayerSize, 0);

	for (int i = 0; i < layers[0].size(); i++)
	{
		layers[0][i].Process(input);
	}

	for (int i = 1; i < layers.size() - 1; i++)
	{
		auto& layer = layers[i];
		auto& prevlayer = layers[i - 1];
		FillIn(in, prevlayer);

		for (auto& neuron : layer)
		{
			neuron.Process(in);
		}
	}

	auto& layer = layers[layers.size() - 1];
	auto& prevlayer = layers[layers.size() - 2];
	FillIn(in, prevlayer);
	for (auto& neuron : layer)
	{
		neuron.ProcessNoActivation(in);
	}

	double summ = 0;
	for (int i = 0; i < layer.size(); i++)
	{
		in[i] = exp(layer[i].out);
		summ += in[i];
	}

	for (int i = 0; i < layer.size(); i++)
	{
		layer[i].out = in[i] / summ;
	}

	return layers.back()[0].out;
}

int NeuronWeb::Move(vector<int>& input)
{
	double best = -1;
	double cur = 0;
	Calculate(input);
	auto& layer = layers.back();
	for (int i = 0; i < 9; i++)
	{
		if (input[i] == 0)
		{
			cur = layer[i].out;
			if (cur > best)
			{
				best = cur;
				LastMove = i;
			}
		}
	}

	return LastMove;
}

CString NeuronWeb::ToString()
{
	CString res = L"";
	CString temp;
	
	res.Format(L"%d\n", Structure.size());
	for (auto& layer : Structure)
	{
		temp.Format(L"%d ", layer);
		res += temp;
	}
	res += L"\n";

	for (auto& layer : layers)
	{
		for (auto& n : layer)
		{
			for (auto& w : n.w)
			{
				temp.Format(L"%f ", w);
				res += temp;
			}
			temp.Format(L"%f ", n.fi);
			res += temp;
			res += L"\n";
		}
	}

	return res;
}

void NeuronWeb::Load(CString source)
{
	ifstream ifstr(source);

	int StructureSize = 0;
	ifstr >> StructureSize;
	Structure.resize(StructureSize);

	for (int i = 0; i < StructureSize; i++)
	{
		int LayerSize = 0;
		ifstr >> LayerSize;
		Structure[i] = LayerSize;
	}

	layers.resize(StructureSize - 1);
	for (int i = 1; i < StructureSize; i++)
	{
		layers[i - 1].resize(Structure[i]);
		for (int j = 0; j < Structure[i]; j++)
		{
			layers[i - 1][j].w.resize(Structure[i - 1]);
			for (int k = 0; k < Structure[i - 1]; k++)
			{
				double w = 0;
				ifstr >> w;
				layers[i - 1][j].w[k] = w;
			}
			double fi = 0;
			ifstr >> fi;
			layers[i - 1][j].fi = fi;
		}
	}

	MaxLayerSize = *max_element(Structure.begin(), Structure.end());
	ifstr.close();
}

void NeuronWeb::ReplaceWithChildOf(const NeuronWeb& p1, const NeuronWeb& p2)
{
	for (int i = 0; i < layers.size(); i++)
	{
		auto& layer = layers[i];		//без индекса локальный
		auto& layer1 = p1.layers[i];	//с индексом 1 - от первого родителя
		auto& layer2 = p2.layers[i];	//с индексом 2 - от второго родителя
		for (int j = 0; j < layer.size(); j++)
		{
			auto& n = layer[j];
			auto& n1 = layer1[j];
			auto& n2 = layer2[j];
			for (int k = 0; k < n.w.size(); k++)
			{
				if (FromParent1())
				{
					n.w[k] = n1.w[k];
				}
				else
				{
					n.w[k] = n2.w[k];
				}
				
			}
			if (FromParent1())
			{
				n.fi = n1.fi;
			}
			else
			{
				n.fi = n2.fi;
			}
		}
	}
}

void NeuronWeb::Mutate()
{

	for (auto& layer : layers)
	{
		for (auto& n : layer)
		{
			for (auto& w : n.w)
			{
				w *= rand(1 - MutationInfluence, 1 + MutationInfluence);
			}
			n.fi *= rand(1 - MutationInfluence, 1 + MutationInfluence);
		}
	}
}

void NeuronWeb::FillIn(vector<double>& in, vector<Neuron>& layer)
{
	for (int i = 0; i < layer.size(); i++)
	{
		in[i] = layer[i].out;
	}
}

inline bool NeuronWeb::FromParent1()
{
	if (rand(0, 1) < 0.5)return true;
	return false;
}

NeuronWeb Trainer::NewNW()
{
	NeuronWeb nw;
	nw.Fill(NWStructure);
	return nw;
}

void Trainer::PlayGame(NeuronWeb& p1, NeuronWeb& p2, int Tries)
{
	NeuronPlayer np1;
	np1.SetNW(p1);

	NeuronPlayer np2;
	np2.SetNW(p2);
	
	for (int i = 0; i < Tries; i++)
	{
		Game game;
		game.SetPlayers(&np1, &np2);
		game.Autogame();
		game.Score();
		
		p1.score += np1.GetScore();
		p2.score += np2.GetScore();

		p1.GamesPlayed++;
		p2.GamesPlayed++;
	}
}

void Trainer::SaveBest()
{
	wofstream out(bestDir + L"Best.nw");
	out << (LPCTSTR)BestNW.ToString();
	out.close();
}

void Trainer::Tournament()
{
	int cycles = PopulationSize / TournamentParticipants;
	int best = rand(0, PopulationSize - 1e-10);
	int oponent = 0;
	for (int i = 0; i < cycles; i++)
	{
		for (int j = 0; j < TournamentParticipants; j++)
		{
			oponent = rand(i, PopulationSize - 1e-10);
			if (population[best].score < population[oponent].score)
			{
				best = oponent;
			}
		}
		if (i != best)
		{
			auto buf = population[i];
			population[i] = population[best];
			population[best] = buf;
		}

	}
}

void Trainer::InitStudyBase()
{
	studyBase.clear();

	set<OptTurn>initBase;
	initBase.insert(OptTurn({ 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 4));  // Пустая доска, ход в центр
	initBase.insert(OptTurn({ -1, 0, 0, 0, 1, 0, 0, 0, 0 }, 2));  // Крестик в центре, нолик в клетке 1, ход в клетку 3
	initBase.insert(OptTurn({ 0, 0, -1, 0, 1, 0, 0, 0, 0 }, 0));  // Крестик в центре, нолик в клетке 3, ход в клетку 1
	initBase.insert(OptTurn({ 0, 0, 0, 0, 1, 0, -1, 0, 0 }, 2));  // Крестик в центре, нолик в клетке 7, ход в клетку 3
	initBase.insert(OptTurn({ 0, 0, 0, 0, 1, 0, 0, 0, -1 }, 0));  // Крестик в центре, нолик в клетке 9, ход в клетку 1
	initBase.insert(OptTurn({ 1, 0, 0, 0, -1, 0, 0, 0, 0 }, 2));  // Крестик в клетке 1, нолик в центре, ход в клетку 3
	initBase.insert(OptTurn({ 0, 0, 1, 0, -1, 0, 0, 0, 0 }, 0));  // Крестик в клетке 3, нолик в центре, ход в клетку 1
	initBase.insert(OptTurn({ 0, 0, 0, 0, -1, 0, 1, 0, 0 }, 2));  // Крестик в клетке 7, нолик в центре, ход в клетку 3
	initBase.insert(OptTurn({ 0, 0, 0, 0, -1, 0, 0, 0, 1 }, 0));  // Крестик в клетке 9, нолик в центре, ход в клетку 1
	initBase.insert(OptTurn({ 1, 0, -1, 0, 0, 0, 0, 0, 0 }, 4));  // Крестик в клетке 1, нолик в клетке 3, ход в центр
	initBase.insert(OptTurn({ 1, 0, 0, 0, 0, 0, -1, 0, 0 }, 4));  // Крестик в клетке 1, нолик в клетке 7, ход в центр
	initBase.insert(OptTurn({ 1, 0, 0, 0, 0, 0, 0, 0, -1 }, 4));  // Крестик в клетке 1, нолик в клетке 9, ход в центр
	initBase.insert(OptTurn({ -1, 0, 1, 0, 0, 0, 0, 0, 0 }, 4));  // Крестик в клетке 3, нолик в клетке 1, ход в центр
	initBase.insert(OptTurn({ 0, 0, 1, 0, 0, 0, -1, 0, 0 }, 4));  // Крестик в клетке 3, нолик в клетке 7, ход в центр
	initBase.insert(OptTurn({ 0, 0, 1, 0, 0, 0, 0, 0, -1 }, 4));  // Крестик в клетке 3, нолик в клетке 9, ход в центр
	initBase.insert(OptTurn({ -1, 0, 0, 0, 0, 0, 1, 0, 0 }, 4));  // Крестик в клетке 7, нолик в клетке 1, ход в центр
	initBase.insert(OptTurn({ 0, 0, -1, 0, 0, 0, 1, 0, 0 }, 4));  // Крестик в клетке 7, нолик в клетке 3, ход в центр
	initBase.insert(OptTurn({ 0, 0, 0, 0, 0, 0, 1, 0, -1 }, 4));  // Крестик в клетке 7, нолик в клетке 9, ход в центр
	initBase.insert(OptTurn({ -1, 0, 0, 0, 0, 0, 0, 0, 1 }, 4));  // Крестик в клетке 9, нолик в клетке 1, ход в центр
	initBase.insert(OptTurn({ 0, 0, -1, 0, 0, 0, 0, 0, 1 }, 4));  // Крестик в клетке 9, нолик в клетке 3, ход в центр
	initBase.insert(OptTurn({ 0, 0, 0, 0, 0, 0, -1, 0, 1 }, 4));  // Крестик в клетке 9, нолик в клетке 7, ход в центр
	initBase.insert(OptTurn({ 1, 0, 1, 0, -1, 0, 0, 0, 0 }, 6));  // Крестик в клетке 1 и 3, нолик в центре, ход в клетку 7
	initBase.insert(OptTurn({ 1, 0, 0, 0, -1, 0, 1, 0, 0 }, 2));  // Крестик в клетке 1 и 7, нолик в центре, ход в клетку 3
	initBase.insert(OptTurn({ 0, 0, 1, 0, -1, 0, 1, 0, 0 }, 0));  // Крестик в клетке 3 и 7, нолик в центре, ход в клетку 1
	initBase.insert(OptTurn({ 1, 0, 0, 0, -1, 0, 0, 0, 1 }, 2));  // Крестик в клетке 1 и 9, нолик в центре, ход в клетку 3

	auto ReflectSummary = [&](set<OptTurn>& data, const OptTurn& optTurn)
		{
			auto temp = reflectHorizontal(optTurn);
			data.insert(temp);

			temp = reflectVertical(optTurn);
			data.insert(temp);

			temp = reflectMainDiagonal(optTurn);
			data.insert(temp);

			temp = reflectAntiDiagonal(optTurn);
			data.insert(temp);
		};
	
	for (auto& optTurn : initBase)
	{
		studyBase.insert(optTurn);

		auto temp = rotate90(optTurn);
		studyBase.insert(temp);
		ReflectSummary(studyBase, temp);

		temp = rotate180(optTurn);
		studyBase.insert(temp);
		ReflectSummary(studyBase, temp);
	
		temp = rotate270(optTurn);
		studyBase.insert(temp);
		ReflectSummary(studyBase, temp);
	}
}

OptTurn Trainer::rotate90(const OptTurn& optTurn)
{
	const auto& board = optTurn.first;
	int move = optTurn.second;

	std::vector<int> newBoard(9);
	newBoard[0] = board[6];
	newBoard[1] = board[3];
	newBoard[2] = board[0];
	newBoard[3] = board[7];
	newBoard[4] = board[4];
	newBoard[5] = board[1];
	newBoard[6] = board[8];
	newBoard[7] = board[5];
	newBoard[8] = board[2];

	// Преобразуем ход
	std::vector<int> rotationMap = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
	int newMove = rotationMap[move];

	return { newBoard, newMove };
}

OptTurn Trainer::rotate180(const OptTurn& optTurn)
{
	const auto& board = optTurn.first;
	int move = optTurn.second;

	std::vector<int> newBoard(9);
	newBoard[0] = board[8];
	newBoard[1] = board[7];
	newBoard[2] = board[6];
	newBoard[3] = board[5];
	newBoard[4] = board[4];
	newBoard[5] = board[3];
	newBoard[6] = board[2];
	newBoard[7] = board[1];
	newBoard[8] = board[0];

	// Преобразуем ход
	std::vector<int> rotationMap = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	int newMove = rotationMap[move];

	return { newBoard, newMove };
}

OptTurn Trainer::rotate270(const OptTurn& optTurn)
{
	const auto& board = optTurn.first;
	int move = optTurn.second;

	std::vector<int> newBoard(9);
	newBoard[0] = board[2];
	newBoard[1] = board[5];
	newBoard[2] = board[8];
	newBoard[3] = board[1];
	newBoard[4] = board[4];
	newBoard[5] = board[7];
	newBoard[6] = board[0];
	newBoard[7] = board[3];
	newBoard[8] = board[6];

	// Преобразуем ход
	std::vector<int> rotationMap = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
	int newMove = rotationMap[move];

	return { newBoard, newMove };
}

OptTurn Trainer::reflectHorizontal(const OptTurn& optTurn)
{
	const auto& board = optTurn.first;
	int move = optTurn.second;

	std::vector<int> newBoard(9);
	newBoard[0] = board[6];
	newBoard[1] = board[7];
	newBoard[2] = board[8];
	newBoard[3] = board[3];
	newBoard[4] = board[4];
	newBoard[5] = board[5];
	newBoard[6] = board[0];
	newBoard[7] = board[1];
	newBoard[8] = board[2];

	// Преобразуем ход
	std::vector<int> reflectionMap = { 6, 7, 8, 3, 4, 5, 0, 1, 2 };
	int newMove = reflectionMap[move];

	return { newBoard, newMove };
}

OptTurn Trainer::reflectVertical(const OptTurn& optTurn)
{
	const auto& board = optTurn.first;
	int move = optTurn.second;

	std::vector<int> newBoard(9);
	newBoard[0] = board[2];
	newBoard[1] = board[1];
	newBoard[2] = board[0];
	newBoard[3] = board[5];
	newBoard[4] = board[4];
	newBoard[5] = board[3];
	newBoard[6] = board[8];
	newBoard[7] = board[7];
	newBoard[8] = board[6];

	// Преобразуем ход
	std::vector<int> reflectionMap = { 2, 1, 0, 5, 4, 3, 8, 7, 6 };
	int newMove = reflectionMap[move];

	return { newBoard, newMove };
}

OptTurn Trainer::reflectMainDiagonal(const OptTurn& optTurn)
{
	const auto& board = optTurn.first;
	int move = optTurn.second;

	std::vector<int> newBoard(9);
	newBoard[0] = board[0];
	newBoard[1] = board[3];
	newBoard[2] = board[6];
	newBoard[3] = board[1];
	newBoard[4] = board[4];
	newBoard[5] = board[7];
	newBoard[6] = board[2];
	newBoard[7] = board[5];
	newBoard[8] = board[8];

	// Преобразуем ход
	std::vector<int> reflectionMap = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
	int newMove = reflectionMap[move];

	return { newBoard, newMove };
}

OptTurn Trainer::reflectAntiDiagonal(const OptTurn& optTurn)
{
	const auto& board = optTurn.first;
	int move = optTurn.second;

	std::vector<int> newBoard(9);
	newBoard[0] = board[8];
	newBoard[1] = board[5];
	newBoard[2] = board[2];
	newBoard[3] = board[7];
	newBoard[4] = board[4];
	newBoard[5] = board[1];
	newBoard[6] = board[6];
	newBoard[7] = board[3];
	newBoard[8] = board[0];

	// Преобразуем ход
	std::vector<int> reflectionMap = { 8, 5, 2, 7, 4, 1, 6, 3, 0 };
	int newMove = reflectionMap[move];

	return { newBoard, newMove };
}

void Trainer::AddInitBase(OptTurn& optTurn)
{
	auto ReflectSummary = [&](set<OptTurn>& data, const OptTurn& optTurn)
		{
			auto temp = reflectHorizontal(optTurn);
			data.insert(temp);

			temp = reflectVertical(optTurn);
			data.insert(temp);

			temp = reflectMainDiagonal(optTurn);
			data.insert(temp);

			temp = reflectAntiDiagonal(optTurn);
			data.insert(temp);
		};

		studyBase.insert(optTurn);

		auto temp = rotate90(optTurn);
		studyBase.insert(temp);
		ReflectSummary(studyBase, temp);

		temp = rotate180(optTurn);
		studyBase.insert(temp);
		ReflectSummary(studyBase, temp);

		temp = rotate270(optTurn);
		studyBase.insert(temp);
		ReflectSummary(studyBase, temp);
}

void Trainer::Train()
{
	int SavedPopulation = PopulationSize / 2;

	for (int i = 0; i < MaxTrainCycles; i++)
	{
		progress = i;

		for (auto& nw : population)
		{
			nw.score = 0;
			nw.GamesPlayed = 0;
		}

		for (int j = 0; j < PopulationSize; j ++)
		{
			for (int k = 0; k < GameTries; k++)
			{
				PlayGame(population[j], population[rand(0, PopulationSize - 0.00000001)], 1);
			}
			for (int k = 0; k < GameTries; k++)
			{
				PlayGame(population[rand(0, PopulationSize - 0.00000001)], population[j], 1);
			}
		}

		for (auto& nw : population)
		{
			nw.score /= nw.GamesPlayed;
		}

		//sort(population.begin(), population.end(), [](const NeuronWeb& a, const NeuronWeb& b) {return a.score > b.score; });
		Tournament();

		int id1 = 0;
		int id2 = 0;
		for (int j = SavedPopulation; j < PopulationSize; j++)
		{
			id1 = rand(0, SavedPopulation - 0.0000001);
			id2 = rand(0, SavedPopulation - 0.0000001);
			population[j].ReplaceWithChildOf(population[id1], population[id2]);
		}

		for (auto& nw : population)
		{
			nw.Mutate();
		}
	}

	sort(population.begin(), population.end(), [](const NeuronWeb& a, const NeuronWeb& b) {return a.score > b.score; });
	BestNW = population.front();
	SaveBest();
	update();
}

void Trainer::TrainORO(bool ContinueTraining)
{
	InitStudyBase();

	population.clear();

	MatrixForm student;
	if (ContinueTraining)student.Load(bestDir + L"Best.MatrixForm");
	else student.Fill(NWStructure);

	auto Inverse = [](vector<int>& data) {for (auto& item : data)item *= -1; };

	for (int i = 0; i < MaxTrainCycles; i++) 
	{
		progress = i;
		studyBaseSize = studyBase.size();
		MatrixFormPlayer p1;
		p1.SetMatrixForm(student);

		RandomPlayer p2;

		MinMax mm;

		Game game;
		if(rand(0,1) < 0.5)game.SetPlayers(&p1, &p2);
		else game.SetPlayers(&p2, &p1);

		game.SetDelay(0);
		game.Start();

		bool turn = true;

		do
		{
			turn = game.GetTurn();

			OptTurn ot;
			ot.first = game.GetField();
			if (!turn)Inverse(ot.first);

			TurnResponse tr(&ot.first);
			mm.Advice(tr);
			ot.second = tr.pos;

			//studyBase.insert(ot);
			AddInitBase(ot);

			if (turn)game.P1Turn();
			else game.P2Turn();

		} while (!game.GameEnd());

		for (auto lesson : studyBase)student.ORO(lesson);
	}

	wofstream out(bestDir + L"Best.MatrixForm");
	out << (LPCTSTR)student.ToString();
	out.close();
}

void Trainer::Fill()
{
	srand(time(NULL));

	for (int i = 0; i < PopulationSize; i++)
	{
		population.push_back(NewNW());
	}
}

void Trainer::Save()
{
	
	for (int i = 0; i < PopulationSize; i++)
	{
		CString dir;
		dir.Format(L"%d.nw", i);
		dir = outDir + dir;
		wofstream out(dir);
		out << (LPCTSTR)population[i].ToString();
		out.close();
	}
}

void Trainer::Load()
{
	population.resize(PopulationSize);
	for (int i = 0; i < PopulationSize; i++)
	{
		CString dir;
		dir.Format(L"%d.nw", i);
		dir = inDir + dir;
		population[i].Load(dir);
	}
}

NeuronWeb Trainer::GetBestNeuronWeb()
{
	return BestNW;
}

bool Trainer::Init()
{
	return !population.empty();
}

float Trainer::GetProgress()
{
	return float(progress)/float(MaxTrainCycles) * 100.;
}

void Trainer::reset()
{
	ofstream ofstr(bestDir + L"itr");
	ofstr << 0;
	ofstr.close();
}

void Trainer::update()
{
	ifstream in(bestDir + L"itr");
	int ctr = 0;
	in >> ctr;
	in.close();
	ctr += MaxTrainCycles;
	ofstream out(bestDir + L"itr");
	out << ctr;
	out.close();
}

int Trainer::size()
{
	return studyBaseSize;
}

MatrixForm::MatrixForm()
{

}

MatrixForm::MatrixForm(const NeuronWeb& source)
{
	Structure = source.Structure;

	W.resize(Structure.size() - 1);
	Fi.resize(Structure.size() - 1);
	Out.resize(Structure.size() - 1);

	for (int i = 1; i < Structure.size(); i++)
	{
		W[i - 1].Resize(Structure[i], Structure[i - 1]);
		Fi[i - 1].Resize(Structure[i], 1);
	}

	for (int i = 0; i < W.size(); i++)
	{
		auto& Wi = W[i];
		auto& Fii = Fi[i];
		auto& layer = source.layers[i];

		for (int j = 0; j < layer.size(); j++)
		{
			auto& n = layer[j];

			for (int k = 0; k < n.w.size(); k++)
			{
				Wi(j, k) = n.w[k];
			}

			Fii(j, 1) = n.fi;
		}
	}
}

void MatrixForm::Fill(vector<int>& pStructure)
{
	srand(time(NULL));

	Structure = pStructure;

	W.resize(Structure.size() - 1);
	Fi.resize(Structure.size() - 1);
	Out.resize(Structure.size() - 1);

	for (int i = 1; i < Structure.size(); ++i)
	{
		W[i - 1].Resize(Structure[i], Structure[i - 1]);
		Fi[i - 1].Resize(Structure[i], 1);
		Out[i - 1].Resize(Structure[i], 1);
	}

	for (auto& w : W)w.apply([](double) {return rand(-0.5, 0.5); });
	for (auto& w : Fi)w.apply([](double) {return rand(-0.5, 0.5); });
}

void MatrixForm::Calculate(vector<int>& input)
{
	Out[0] = (W[0] * Matrix2d(input).Transpose() + Fi[0]).Transform([](double val) {return max(0, val); });

	for (int i = 1; i < W.size() - 1; i++)
	{
		Out[i] = (W[i] * Out[i - 1] + Fi[i]).Transform([](double val) {return max(0, val); });
	}

	int id = W.size() - 1;
	Out[id] = (W[id] * Out[id - 1] + Fi[id]).Transform([](double val) {return exp(val); });

	double summ = Out[id].fold(0, [](double acc, double val) {return acc + val; });
	Out[id].apply([summ](double val) {return val / summ; });
}

int MatrixForm::Move(vector<int>& board)
{
	double best = -1;
	double cur = 0;
	Calculate(board);
	auto& layer = Out.back();
	for (int i = 0; i < 9; i++)
	{
		if (board[i] == 0)
		{
			cur = layer(i,0);
			if (cur > best)
			{
				best = cur;
				LastMove = i;
			}
		}
	}

	return LastMove;
}

void MatrixForm::ORO(OptTurn& ot)
{
	Matrix2d t;
	t.Resize(Structure.back(), 1);
	t(ot.second, 0) = 1;

	Calculate(ot.first);

	auto& y = Out.back();

	auto dldy = y - t;
	
	double diff = dldy.fold(0, [](double acc, double elem) {return acc + elem * elem; });
	if (diff < Accuracy)return;
	
	int size = W.size() - 1;

	auto dldw4 = dldy * (Out[size - 1].Transpose()) ;

	auto dldb4 = dldy;

	auto dldh = W[size].Transpose() * dldy;

	vector<Matrix2d>dw;
	dw.resize(size);
	vector<Matrix2d>db;
	db.resize(size);

	Matrix2d dldz;

	for (int i = size - 1; i >= 1; i--)
	{
		dldz = dldh.HadamardProduct(Out[i].Transform([](double val) {return (val > 0) ? 1 : 0; }));
		dw[i] = dldz * (Out[i - 1].Transpose());
		db[i] = dldz;
		dldh = W[i].Transpose() * dldz;
	}

	dldz = dldh.HadamardProduct(Out[0].Transform([](double val) {return (val > 0) ? 1 : 0; }));
	dw[0] = (dldz)*(Matrix2d(ot.first));
	db[0] = dldz;

	W[size] = W[size] - (dldw4 * etta);
	Fi[size] = Fi[size] - (dldb4 * etta);

	for (int i = 0; i < size - 1; i++)
	{
		W[i] = W[i] - (dw[i] * etta);
		Fi[i] = Fi[i] - (db[i] * etta);
	}
}

CString MatrixForm::ToString()
{
	CString res = L"";
	CString temp;

	res.Format(L"%d\n", Structure.size());
	for (auto& layer : Structure)
	{
		temp.Format(L"%d ", layer);
		res += temp;
	}
	res += L"\n";

	for (auto& w : W)
	{
		res += w.ToStringData();
	}
	for (auto& fi : Fi)
	{
		res += fi.ToStringData();
	}
	return res;
}

void MatrixForm::Load(CString& source)
{
	ifstream ifstr(source);

	int StructureSize = 0;
	ifstr >> StructureSize;
	Structure.resize(StructureSize);

	for (int i = 0; i < StructureSize; i++)
	{
		int LayerSize = 0;
		ifstr >> LayerSize;
		Structure[i] = LayerSize;
	}

	W.resize(StructureSize - 1);
	Fi.resize(StructureSize - 1);
	Out.resize(StructureSize - 1);

	for (int i = 1; i < StructureSize; i++)
	{
		int rows = Structure[i];
		int cols = Structure[i - 1];
		W[i - 1].Resize(rows, cols);

		double w = 0;
		for (int j = 0; j < rows; j++)
		{
			for (int k = 0; k < cols; k++)
			{
				ifstr >> w;
				W[i - 1](j, k) = w;
			}
		}
	}

	for (int i = 1; i < StructureSize; i++)
	{
		int rows = Structure[i];
		int cols = 1;
		Fi[i - 1].Resize(rows, cols);

		double fi = 0;
		for (int j = 0; j < rows; j++)
		{
			for (int k = 0; k < cols; k++)
			{
				ifstr >> fi;
				Fi[i - 1](j, k) = fi;
			}
		}
	}

	ifstr.close();
}
