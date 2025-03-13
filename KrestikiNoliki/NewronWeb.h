#pragma once
#include<vector>
#include<algorithm>
#include<fstream>
#include <set>
#include "Matrix.h"
using namespace std;

typedef pair<vector<int>, int> OptTurn;

class NeuronWeb
{
	friend class MatrixForm;
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


class MatrixForm {
private:
	vector<Matrix2d>W;
	vector<Matrix2d>Fi;
	vector<Matrix2d>Out;
	vector<int>Structure;

	int LastMove = 0;
	double Accuracy = 1e-6;
	typedef double (*ActivationFunctionType)(double);
protected:
public:
	MatrixForm();
	MatrixForm(const NeuronWeb& source);
	void Fill(vector<int>&Structure);
	void Calculate(vector<int>& input);
	int Move(vector<int>& board);
	void ORO(OptTurn& ot);
	CString ToString();
	void Load(CString& source);

	double etta = .005;
	double currentError = 0;
};

class Trainer
{
private:
	vector<NeuronWeb> population;
	const int PopulationSize = 500;
	vector<int>NWStructure = { 9,72,36,18,9 };

	int MaxTrainCycles = 500;
	int GameTries = 20;

	const CString outDir = L"Saves\\";
	const CString inDir = L"Saves\\";
	const CString bestDir = L"Best\\";

	NeuronWeb BestNW;
	int progress = 0;
	const int TournamentParticipants = 5;
	std::set<OptTurn> studyBase;
	int studyBaseSize = 0;
	double error = 0;
protected:
	NeuronWeb NewNW();
	void PlayGame(NeuronWeb& p1, NeuronWeb& p2, int Tries);
	void SaveBest();
	void Tournament();
	void InitStudyBase();
	OptTurn rotate90(const OptTurn& optTurn);
	OptTurn rotate180(const OptTurn& optTurn);
	OptTurn rotate270(const OptTurn& optTurn);
	OptTurn reflectHorizontal(const OptTurn& optTurn);
	OptTurn reflectVertical(const OptTurn& optTurn);
	OptTurn reflectMainDiagonal(const OptTurn& optTurn);
	OptTurn reflectAntiDiagonal(const OptTurn& optTurn);
	void AddInitBase(OptTurn& initBase);
public:
	void Train();
	void TrainORO(bool ContinueTraining = false);
	void Fill();
	void Save();
	void Load();
	NeuronWeb GetBestNeuronWeb();
	bool Init();
	float GetProgress();
	void reset();
	void update();
	int size();
	double Error();
};