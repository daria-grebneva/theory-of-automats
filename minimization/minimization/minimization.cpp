#include "stdafx.h"

using namespace std;

void ShowVector(vector<int>& a)
{
	for (vector<int>::iterator it = a.begin(); it != a.end(); ++it)
		cout << *it;
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

int FindElemInClasses(map<int, vector<int>> classes, vector<int> elem)
{
	map<int, vector<int>>::iterator it;
	for (it = classes.begin(); it != classes.end(); it++)
	{
		if (it->second == elem)
		{
			return it->first;
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
		transitionsToOutputs.insert(pair<int, vector<int>>(q[j] + 1, state));
	}

	return transitionsToOutputs;
}

map<int, vector<int>> GetEquivalenceСlasses(multimap<int, vector<int>> transitionsToOutputs, vector<int> q)
{
	map<int, vector<int>> equivalenceСlasses;

	int k = 0;
	for (int j = 0; j < q.size(); ++j)
	{
		auto elem = transitionsToOutputs.find(q[j] + 1)->second;
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

int** SubstituteNewClasses(map<int, vector<int>> equivalenceСlasses, int x, int q, int** transitions)
{
	auto newClasses = InitializeArray(x, q);
	map<int, vector<int>>::iterator it;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		auto A = it->second;
		cout << "vector A: " << A.size() << endl;
		ShowVector(A);
		for (int i = 0; i < A.size(); ++i)
		{
			for (int j = 0; j < x; ++j)
			{
				int k = A[i] - 1;
				int elem = transitions[j][k];
				cout << elem << endl;
				newClasses[j][k] = FindElemInClasses(equivalenceСlasses, TransitionsToVector(elem));
			}
		}
	}

	//for (int j = 0; j < q; ++j)
	//{
	//	cout << j + 1;
	//}
	//cout << endl;
	//for (int i = 0; i < x; ++i)
	//{
	//	for (int j = 0; j < q; ++j)
	//	{
	//		cout << newClasses[i][j];
	//	}
	//	cout << endl;
	//}

	return newClasses;
}

void MinimizateMilly(int x, int q, istream& fin)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);

	FillTransitionsAndOutputs(x, q, fin, transitions, outputs);

	vector<int> strQ = TransitionsToVector(q);
	auto transitionsToOutputs = TransitionsToOutputs(x, strQ, outputs);
	auto equivalenceСlasses = GetEquivalenceСlasses(transitionsToOutputs, strQ);

	cout << "equivalentStates" << endl;
	map<int, vector<int>>::iterator it;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		cout << it->first << " ";
		ShowVector(it->second);
		cout << endl;
	}

	cout << endl
		 << "transitions" << endl;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			cout << transitions[i][j];
		}
		cout << endl;
	}
	cout << endl
		 << "outputs" << endl;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			cout << outputs[i][j];
		}
		cout << endl;
	}

	cout << endl
		 << "podstavlyaem A" << endl;
	auto newClasses = SubstituteNewClasses(equivalenceСlasses, x, q, transitions);

	cout << endl
		 << "sravnivaem classes and vosvrashaem nado rascheplyat' or not" << endl;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		auto elem = it->second;
		auto transitionsToOutputs = TransitionsToOutputs(x, elem, outputs);
		auto equivalenceСlasses = GetEquivalenceСlasses(transitionsToOutputs, elem);

		cout << "equivalentStates" << endl;
		map<int, vector<int>>::iterator it;
		for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
		{
			cout << it->first << " ";
			ShowVector(it->second);
			cout << endl;
		}
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
