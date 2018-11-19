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

void ShowVector(vector<int> v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i];
	}
	cout << endl;
}

map<vector<int>, vector<vector<int>>> GetNonDeterministicMatrix(fstream& fin, int z, int x)
{
	map<vector<int>, vector<vector<int>>> nonDeterministic;
	bool firstIteration = true;
	
	string str; 
	int j = 0;
	while (getline(fin, str))
	{	
		auto temp = GetVectorFromString(str, x);
		if (!firstIteration)
		{
			vector<int> start;
			start.push_back(j - 1);
			nonDeterministic.insert(pair<vector<int>, vector<vector<int>>>(start, temp));
		}	
		firstIteration = false;	
		++j;
	}
	
	return nonDeterministic;
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

void ShowMap(map<vector<int>, vector<vector<int>>> newStates)
{
	for (auto it = newStates.begin(); it != newStates.end(); ++it) ///вывод на экран
	{
		auto v = it->first;
		for (int i = 0; i < v.size(); ++i)
		{
			cout << v[i];
		}
		auto vec = it->second;
		for (int i = 0; i < vec.size(); ++i)
		{
			if (i != 0)
				cout << " ";
			cout << " |" << i << " |";
			ShowVector(vec[i]);
		}
	}
}

void Determitation(vector<vector<vector<int>>>& v, int x)
{
	map<vector<int>, vector<vector<int>>> newStates;
	vector<int> start;
	start.push_back(0);
	newStates.insert(pair<vector<int>, vector<vector<int>>>(start, v[0]));
	cout << "Starting map" << endl;
	ShowMap(newStates);
	cout << "Starting map" << endl;
	for (int i = 0; i < v.size(); ++i)
	{
		auto h = v[i];
		cout << i << " ";

		for (int j = 0; j < h.size(); ++j)
		{
			if (h[j].size() > 0)
			{
				cout << j << " ";
				ShowVector(h[j]);
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


	map<vector<int>, vector<vector<int>>> nonDeterministic = GetNonDeterministicMatrix(fin, z, x);
	ShowMap(nonDeterministic);
	//Determitation(nonDeterministic, x);
}
