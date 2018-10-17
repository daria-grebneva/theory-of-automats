#include "stdafx.h"

using namespace std;

void ShowVector(vector<int> a)
{
	for (int i = 0; i < a.size(); ++i)
	{
		cout << a[i];
	}
	cout << endl;
}

//заполняем массив переходов и массив выходов
void FillTransitionsAndOutputs(int x, int q, istream& fin, int** transitions, int** outputs)
{
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			fin >> transitions[i][j];
			fin >> outputs[i][j];
		}
	}
}

int** InitializeArray(int x, int q)
{
	int** arr = new int*[x];
	for (int i = 0; i < x; ++i)
	{
		arr[i] = new int[q];
	}

	return arr;
}

int** GetOutputsByClasses(int x, vector<int> q, int** outputsPrev)
{
	auto outputs = InitializeArray(x, q.size());
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q.size(); ++j)
		{
			outputs[i][j] = outputsPrev[i][q[j]];
		}
	}

	return outputs;
}

bool FindInClasses(map<int, vector<int>> classes, vector<int> elem)
{
	map<int, vector<int>>::iterator it;
	for (it = classes.begin(); it != classes.end(); it++)
	{
		if (it->second == elem)
		{
			return true;
		}
	}
	return false;
}

int FindElemInClasses(map<int, vector<int>> classes, int elem)
{
	map<int, vector<int>>::iterator it;
	for (it = classes.begin(); it != classes.end(); it++)
	{
		auto temp = it->second;

		for (int j = 0; j < temp.size(); ++j)
		{
			if (temp[j] == (elem - 1))
			{
				return it->first;
			}
		}
	}
	return -1;
}

multimap<int, vector<int>> TransitionsToOutputs(int x, vector<int> q, int** outputs)
{
	multimap<int, vector<int>> transitionsToOutputs;
	for (int j = 0; j < q.size(); ++j)
	{
		vector<int> state;
		for (int i = 0; i < x; ++i)
		{
			state.push_back(outputs[i][q[j]]);
		}
		transitionsToOutputs.insert(pair<int, vector<int>>(q[j], state));
	}

	return transitionsToOutputs;
}

//получаем классы эквивалентности
map<int, vector<int>> GetEquivalenceСlasses(multimap<int, vector<int>> transitionsToOutputs, vector<int> q)
{
	map<int, vector<int>> equivalenceСlasses;

	int k = 0;
	for (int j = 0; j < q.size(); ++j)
	{
		auto elem = transitionsToOutputs.find(q[j])->second;
		int i = 0;
		vector<int> arr;

		multimap<int, vector<int>>::iterator it;
		for (it = transitionsToOutputs.begin(); it != transitionsToOutputs.end(); it++)
		{
			if (elem == it->second)
			{
				arr.push_back(it->first);
				i++;
			}
		}

		if (!FindInClasses(equivalenceСlasses, arr))
		{
			equivalenceСlasses.insert(pair<int, vector<int>>(k, arr));
			++k;
		}
	}

	return equivalenceСlasses;
}

vector<int> TransitionsToVector(int q)
{
	vector<int> vecQ;
	for (int i = 0; i < q; ++i)
	{
		vecQ.push_back(i);
	}

	return vecQ;
}

//подставляем новые классы эквивалентности
int** SubstituteNewClasses(map<int, vector<int>> equivalenceСlasses, int x, vector<int> q, int** transitions)
{
	auto newClasses = InitializeArray(x, q.size());
	map<int, vector<int>>::iterator it;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		auto temp = it->second;

		for (int i = 0; i < temp.size(); ++i)
		{
			for (int j = 0; j < x; ++j)
			{
				int k = temp[i];
				int elem = transitions[j][k];
				newClasses[j][k] = FindElemInClasses(equivalenceСlasses, elem);
			}
		}
	}

	return newClasses;
}

bool IsFissionable(map<int, vector<int>> equivalenceСlass)
{
	return (equivalenceСlass.size() != 1);
}

vector<vector<int>> SplitClasses(map<int, vector<int>> equivalenceСlasses, int x, int** transitions, vector<vector<int>>& fissionableClasses)
{
	bool isNeedToContinue = false;
	map<int, vector<int>>::iterator it;
	vector<map<int, vector<int>>> allClasses;
	vector<vector<int>> arrayOfQ;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		auto elem = it->second;
		auto transitionsToOutputs1 = TransitionsToOutputs(x, elem, transitions);
		auto equivalenceСlasses1 = GetEquivalenceСlasses(transitionsToOutputs1, elem);
		map<int, vector<int>>::iterator it;
		if (IsFissionable(equivalenceСlasses1))
		{
			isNeedToContinue = true;
		}
		allClasses.push_back(equivalenceСlasses1);
		arrayOfQ.push_back(elem);
	}

	if (isNeedToContinue)
	{
		for (int i = 0; i < allClasses.size(); ++i)
		{
			auto newClasses = SubstituteNewClasses(allClasses[i], x, arrayOfQ[i], transitions);
			SplitClasses(allClasses[i], x, newClasses, fissionableClasses);
		}
	}

	for (int i = 0; i < allClasses.size(); ++i)
	{
		for (it = allClasses[i].begin(); it != allClasses[i].end(); it++)
		{
			if (find(fissionableClasses.begin(), fissionableClasses.end(), it->second) == fissionableClasses.end())
			{
				fissionableClasses.push_back(it->second);
			}
		}
	}

	return fissionableClasses;
}

void MinimizateMilly(int x, int q, istream& fin)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);
	vector<vector<int>> fissionableClasses;

	FillTransitionsAndOutputs(x, q, fin, transitions, outputs);

	vector<int> strQ = TransitionsToVector(q);
	auto transitionsToOutputs = TransitionsToOutputs(x, strQ, outputs);
	auto equivalenceСlasses = GetEquivalenceСlasses(transitionsToOutputs, strQ);

	bool isNeedToContinue = false;

	auto newClasses = SubstituteNewClasses(equivalenceСlasses, x, strQ, transitions);
	fissionableClasses = SplitClasses(equivalenceСlasses, x, newClasses, fissionableClasses);
	for (int i = 0; i < fissionableClasses.size(); ++i)
	{
		ShowVector(fissionableClasses[i]);
	}
}

void MinimizateMur()
{
}

int main()
{
	string inputFileName = "in.txt";
	string outputFileName = "out.txt";

	fstream fin(inputFileName, istream::in);
	fstream fout(outputFileName, ostream::out);

	if (!fin.is_open())
	{
		return false;
	}

	int automatonType;
	int x;
	int y;
	int q;

	fin >> automatonType;
	fin >> x;
	fin >> y;
	fin >> q;

	switch (automatonType)
	{
	case 1:
		MinimizateMur();
	case 2:
		MinimizateMilly(x, q, fin);
	default:
		fout << "Uncorrect input";
	}

	fout << automatonType;

	return 0;
}
