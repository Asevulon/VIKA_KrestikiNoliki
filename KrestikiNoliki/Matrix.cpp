#include "pch.h"
#include "Matrix.h"
#include<fstream>

unsigned long long matrix_counter = 0;

Matrix2d::Matrix2d()
{
	matrix = vector<vector<double>>(rows, vector<double>(columns, 0));
	matrixid = matrix_counter;
	matrix_counter++;
}

Matrix2d::Matrix2d(vector<double>& init)
{
	matrix.resize(1);
	matrix[0] = init;
	rows = 1;
	columns = init.size();
	matrixid = matrix_counter;
	matrix_counter++;
}

Matrix2d::Matrix2d(vector<int>& init)
{
	matrix.resize(1);
	matrix[0].resize(init.size());
	rows = 1;
	columns = init.size();

	for (int i = 0; i < columns; i++)matrix[0][i] = init[i];
	matrixid = matrix_counter;
	matrix_counter++;
}

Matrix2d::Matrix2d(int r, int c, double initVal)
{
	rows = r;
	columns = c;
	matrix = vector<vector<double>>(rows, vector<double>(columns, initVal));
	matrixid = matrix_counter;
	matrix_counter++;
}

Matrix2d::~Matrix2d()
{
	matrix_counter--;
}

void Matrix2d::Resize(int r, int c, double initVal)
{
	rows = r;
	columns = c;
	matrix = vector<vector<double>>(rows, vector<double>(columns, initVal));
}

double& Matrix2d::operator()(int row, int column)
{
	// TODO: вставьте здесь оператор return
	return matrix[row][column];
}

Matrix2d Matrix2d::operator*(const Matrix2d& rhs)
{
	Matrix2d res(rows, rhs.columns);

	for (int i = 0; i < res.rows; i++)
	{
		for (int j = 0; j < res.columns; j++)
		{
			res(i, j) = 0;
			for (int k = 0; k < columns; k++)
			{
				res(i, j) += matrix[i][k] * rhs.matrix[k][j];
			}
		}
	}
	return res;
}

Matrix2d Matrix2d::operator*(const double rhs)
{
	Matrix2d res;
	res.Resize(rows, columns);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			res(i, j) = matrix[i][j] * rhs;
		}
	}
	return res;
}

Matrix2d Matrix2d::operator+(const Matrix2d& rhs)
{
	Matrix2d res(rows, columns);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			res(i, j) = matrix[i][j] + rhs.matrix[i][j];
		}
	}
	return res;
}

Matrix2d Matrix2d::operator-(const Matrix2d& rhs)
{
	Matrix2d res(rows, columns);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			res(i, j) = matrix[i][j] - rhs.matrix[i][j];
		}
	}
	return res;
}

Matrix2d Matrix2d::Transform(OneArgFuncType predicate)
{
	Matrix2d res(rows, columns);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			res(i, j) = predicate(matrix[i][j]);
		}
	}
	return res;
}

Matrix2d Matrix2d::Transpose()
{
	Matrix2d res;
	res.Resize(columns, rows);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			res(j, i) = matrix[i][j];
		}
	}
	return res;
}

Matrix2d Matrix2d::HadamardProduct(const Matrix2d& rhs)
{
	Matrix2d res;
	res.Resize(rows, columns);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			res(i, j) = matrix[i][j] * rhs.matrix[i][j];
		}
	}
	return res;
}

double Matrix2d::fold(double acc, TwoArgFuncType predicate)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			acc = predicate(acc, matrix[i][j]);
		}
	}
	return acc;
}

Matrix2d& Matrix2d::apply(OneArgFuncType predicate)
{
	// TODO: вставьте здесь оператор return
	for (auto& row : matrix)
	{
		for (auto& item : row)
		{
			item = predicate(item);
		}
	}
	return *this;
}

CString Matrix2d::ToString(CString label)
{
	CString res;
	res.Format(L"[Matrix]:[id][%d]:[label][%s]\n", matrixid);
	CString temp;
	for (int i = 0; i < rows; i++)
	{
		res += L"[";
		for (int j = 0; j < columns - 1; j++)
		{
			temp.Format(L"%f, ", matrix[i][j]);
			res += temp;
		}
		temp.Format(L"%f]\n", matrix[i][columns - 1]);
		res += temp;
	}
	return res;
}

CString Matrix2d::ToStringData()
{
	CString res = L"";
	CString temp;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			temp.Format(L"%f ", matrix[i][j]);
			res += temp;
		}
		res += L"\n";
	}
	return res;
}

