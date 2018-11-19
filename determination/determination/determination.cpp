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
	} while (fin.peek() != '\n');

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

void ShowMap(map<vector<int>, vector<vector<int>>> newStates)
{
	for (auto it = newStates.begin(); it != newStates.end(); ++it)
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

map<vector<int>, vector<vector<int>>> InsertStartVertex(map<vector<int>, vector<vector<int>>> v)
{
	map<vector<int>, vector<vector<int>>> newStates;
	vector<int> start;
	start.push_back(0);
	newStates.insert(pair<vector<int>, vector<vector<int>>>(v.find(start)->first, v.find(start)->second));

	return newStates;
}

bool IsInMap(map<vector<int>, vector<vector<int>>> m, vector<int> elem)
{
	map<vector<int>, vector<vector<int>>>::iterator it = m.find(elem);
	return (it != m.end());
}

pair<vector<int>, vector<vector<int>>> ReturnValueInMap(map<vector<int>, vector<vector<int>>> m, vector<int> elem)
{
	map<vector<int>, vector<vector<int>>>::iterator it = m.find(elem);
	if (it != m.end())
	{
		return pair<vector<int>, vector<vector<int>>>(it->first, it->second);
	}
}

void Determitation(map<vector<int>, vector<vector<int>>> v, int x)
{
	auto newStates = InsertStartVertex(v);
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		auto vec = it->second;
		for (int i = 0; i < vec.size(); ++i)
		{
			if (vec[i].size() > 0)
			{
				//если не нашли в новой map, то надо вставить
				if (!IsInMap(newStates, vec[i]))
				{
					if (vec[i].size() == 1)
					{
						cout << "insert" << endl;
						newStates.insert(ReturnValueInMap(v, vec[i]));
					}
					else
					{
						//склеиваем
					}
					ShowVector(vec[i]);
				}
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
	Determitation(nonDeterministic, x);
}
