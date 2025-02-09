#pragma once
#include<vector>
#include<algorithm>
#include<fstream>
using namespace std;

class NeuronWeb
{
public:
	struct Neuron {
		double out = 0;
		vector<double> w;
		double fi = 0;
		Neuron() {};
		Neuron(int size);
		inline double activation(double x);
		void Process(vector<double>& in);
		void Process(vector<int>& in);
		void ProcessNoActivation(vector<double>& in);
	};
private:
	int LastMove = 0;
	double MutationInfluence = 0.03;

	vector<int>Structure;
	vector<vector<Neuron>>layers;
	int MaxLayerSize = 0;
protected:
	void FillIn(vector<double>& in, vector<Neuron>& layer);
	inline bool FromParent1();
public:
	//первое значение в структуре показывает количество входов
	void Fill(vector<int>Structure);

	double Calculate(vector<double>& input);
	double Calculate(vector<int>& input);
	int Move(vector<int>& input);
	CString ToString();
	void Load(CString source);
	double score = 0;
	int GamesPlayed = 0;
	void ReplaceWithChildOf(const NeuronWeb& parent1, const NeuronWeb& parent2);
	void Mutate();
};

class Trainer
{
private:
	vector<NeuronWeb> population;
	const int PopulationSize = 200;
	vector<int>NWStructure = { 9,9,32,16,9 };

	int MaxTrainCycles = 500;
	int GameTries = 20;

	const CString outDir = L"Saves\\";
	const CString inDir = L"Saves\\";
	const CString bestDir = L"Best\\";

	NeuronWeb BestNW;
	int progress = 0;
	const int TournamentParticipants = 5;
protected:
	NeuronWeb NewNW();
	void PlayGame(NeuronWeb& p1, NeuronWeb& p2, int Tries);
	void SaveBest();
	void Tournament();
public:
	void Train();
	void Fill();
	void Save();
	void Load();
	NeuronWeb GetBestNeuronWeb();
	bool Init();
	float GetProgress();
};