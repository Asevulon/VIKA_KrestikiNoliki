#pragma once
#include<vector>
#include"functional"
#include <string>
using namespace std;

class Matrix2d
{
private:
	vector<vector<double>>matrix;

	int rows = 1;
	int columns = 1;

	typedef double (*ZeroArgFuncType)();
	typedef std::function<double(double)> OneArgFuncType;
	typedef double (*TwoArgFuncType)(double, double);
	
	unsigned long long matrixid = 0;
protected:
public:
	Matrix2d();
	Matrix2d(vector<double>& init);
	Matrix2d(vector<int>& init);
	Matrix2d(int rows, int columns, double initVal = 0);
	~Matrix2d();
	void Resize(int rows, int columns, double initVal = 0);
	double& operator()(int row, int column);
	Matrix2d operator*(const Matrix2d& rhs);
	Matrix2d operator*(const double rhs);
	Matrix2d operator+(const Matrix2d& rhs);
	Matrix2d operator-(const Matrix2d& rhs);
	Matrix2d Transform(OneArgFuncType predicate);
	Matrix2d Transpose();
	Matrix2d HadamardProduct(const Matrix2d& rhs);
	double fold(double initval, TwoArgFuncType predicate);
	Matrix2d& apply(OneArgFuncType predicate);
	CString ToString(CString label = L"");
	CString ToStringData();

	int GetRows();
	int GetColumns();
};


