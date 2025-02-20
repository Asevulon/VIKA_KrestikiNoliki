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
