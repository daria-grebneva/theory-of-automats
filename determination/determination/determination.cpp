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

int VectorToInt(vector<int> v)
{
	reverse(v.begin(), v.end());
	int decimal = 1;
	int total = 0;
	for (auto& it : v)
	{
		total += it * decimal;
		decimal *= 10;
	}
	return total;
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
			auto size = to_string(VectorToInt(vec[i])).size();
			//cout << "size: " << size << endl;
			auto need = 10 - (size / 2 );
			for (int j = 0; j < need; ++j)
			{
				cout << " ";
			}
			cout << "|" << i << "    |";
			ShowVector(vec[i]);
		}
	}
	cout << endl;
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

pair<vector<int>, vector<vector<int>>> GlueVertex(map<vector<int>, vector<vector<int>>> v, vector<int> vertex, int x)
{
	vector<int> vertexes;
	vector<vector<int>> vertexesValues(x);
	for (int i = 0; i < vertex.size(); ++i)
	{
		//cout << vertex[i] << " ";
		vector<int> f;
		f.push_back(vertex[i]);
		auto t = ReturnValueInMap(v, f).first;
		vertexes.insert(vertexes.end(), t.begin(), t.end());

		auto k = ReturnValueInMap(v, f).second;
		for (int j = 0; j < k.size(); ++j)
		{
			vertexesValues[j].insert(vertexesValues[j].end(), k[j].begin(), k[j].end());
		}
	}

	return pair<vector<int>, vector<vector<int>>>(vertexes, vertexesValues);
}

map<vector<int>, vector<vector<int>>> Determitation(map<vector<int>, vector<vector<int>>> v, int x)
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
						//cout << "insert" << endl;
						newStates.insert(ReturnValueInMap(v, vec[i]));
					}
					else
					{
						//склеиваем
						newStates.insert(GlueVertex(v, vec[i], x));
					}
					//ShowVector(vec[i]);
				}
			}
		}
	}
	return newStates;
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
	auto determized = Determitation(nonDeterministic, x);
	ShowMap(determized);
}
