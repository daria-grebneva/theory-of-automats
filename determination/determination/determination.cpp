#include "pch.h"

using namespace std;

vector<int> GetFinalStatesArray(fstream& fin)
{
	vector<int> K;
	int i;

	do 
	{
		fin >> i;
		K.push_back(i);
	}
	while (fin.peek() != '\n');

	return K;
}

vector<vector<int>> GetVectorFromString(const string& str, int x)
{
	vector<vector<int>> temp(x);
	if (!empty(str))
	{
		stringstream ss;
		int i, l;

		ss << str;
		while (ss >> i && ss >> l)
		{
			temp[l].push_back(i);
		}
	}

	return temp;
}

vector<vector<vector<int>>> GetNonDeterministicMatrix(fstream& fin, int z, int x)
{
	vector<vector<vector<int>>> nonDeterministic;
	bool firstIteration = true;
	
	for (int j = 0; j < z; ++j)
	{
		string str; 
		while (getline(fin, str))
		{	
			auto temp = GetVectorFromString(str, x);
			if (!firstIteration)
			{
				nonDeterministic.push_back(temp);
			}	
			firstIteration = false;		
		}
	}
	
	return nonDeterministic;
}


void ShowVector(vector<int> v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i];
	}	
	cout << endl;
}

void ShowBigVector(vector<vector<vector<int>>> v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		auto h = v[i];
		for (int j = 0; j < h.size(); ++j)
		{
			cout << j << "   ";
			ShowVector(h[j]);
		}
		cout << endl;
	}
}

void Determitation(vector<vector<vector<int>>>& v, int x)
{
	for (int i = 0; i < v.size(); ++i)
	{
		auto h = v[i];
		for (int j = 0; j < h.size(); ++j)
		{
			if (h[j].size() > 1)
			{

			}
		}
	}
}

int main()
{

	string inputFileName = "in2.txt";
	string outputFileName = "out2.txt";

	fstream fin(inputFileName, istream::in);
	fstream fout(outputFileName, ostream::out);

	if (!fin.is_open())
	{
		return false;
	}

	int x, z, k;
	vector<int> K;
	fin >> x >> z >> k;
	cout << x << z << k << endl;
	K = GetFinalStatesArray(fin);


	vector<vector<vector<int>>> nonDeterministic = GetNonDeterministicMatrix(fin, z, x);
	ShowBigVector(nonDeterministic);
	//Determitation(nonDeterministic, x);
}
