#include "stdafx.h"

using namespace std;

//заполняем массив переходов и массив выходов
void FillTransitionsAndOutputs(int x, int q, istream & fin, int** transitions, int** outputs)
{
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			fin >> transitions[i][j];
			fin >> outputs[i][j];
			//cout << transitions[i][j] << outputs[i][j] << endl;
		}
	}
}

int** InitializeArray(int x, int q)
{
	int** arr = new int *[x];
	for (int i = 0; i < x; ++i)
	{
		arr[i] = new int[q];
	}
	
	return arr;
}

bool FindInClasses(map<int,string> classes, string elem)
{
	map<int, string>::iterator it;
	for (it = classes.begin(); it != classes.end(); it++)
	{
		if (it->second == elem)
		{
			return true;
		}
	}
	return false;
}

int FindElemInClasses(map<int, string> classes, string elem)
{
	map<int, string>::iterator it;
	for (it = classes.begin(); it != classes.end(); it++)
	{
		string str = it->second;
		size_t pos = str.find(elem);
		if (pos != std::string::npos)
		{
			return it->first;
		}
	}
	return -1;
}

multimap< int, string > TransitionsToOutputs(int x, int q, int** outputs)
{
	multimap< int, string > transitionsToOutputs;
	for (int j = 0; j < q; ++j)
	{
		string state = "";
		for (int i = 0; i < x; ++i)
		{
			state += to_string(outputs[i][j]);
		}
		//cout << j + 1 << " " << state << endl;
 		transitionsToOutputs.insert(pair< int, string >(j + 1, state));
	}

	return transitionsToOutputs;
}

map<int, string> GetEquivalenceСlasses(multimap< int, string > transitionsToOutputs, int q)
{
	map<int, string> equivalenceСlasses;

	int k = 0;
	for (int j = 0; j < q; ++j)
	{
		auto elem = transitionsToOutputs.find(j + 1)->second;
		//cout << elem << endl;
		int i = 0;
		string arr = "";

		multimap<int, string>::iterator it;
		for (it = transitionsToOutputs.begin(); it != transitionsToOutputs.end(); it++)
		{
			if (elem == it->second)
			{
				arr += to_string(it->first);
				i++;
			}
		}

		if (!FindInClasses(equivalenceСlasses, arr))
		{
			equivalenceСlasses.insert(pair<int, string>(k, arr));
			++k;
		}
	}

	return equivalenceСlasses;
}

int** SubstituteNewClasses(map<int, string> equivalenceСlasses, int x, int q, int** transitions)
{
	auto newClasses = InitializeArray(x, q);
	map<int, string>::iterator it;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		cout << "NEW" << endl;
		auto A = it->second;
		for (int i = 0; i < A.length(); ++i)
		{
			//cout << "q: " << q << endl;
			for (int j = 0; j < x; ++j)
			{
				//cout << " A[i]: " << A[i] << endl;
				int k = A[i] - 48 - 1;
				string elem = to_string(transitions[j][k]);
				//cout << " k: " << k << " " << j << endl;
				//cout << " elem: " << elem << endl;
				newClasses[i][j] = FindElemInClasses(equivalenceСlasses, elem);
				cout << FindElemInClasses(equivalenceСlasses, elem) << endl;
				//cout << elem << endl;
			}
			//cout << j << endl;
			//cout << endl;
		}
		//cout << "SECOND A" << endl;
	}

	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			//cout << newClasses[i][j];
		}
		cout << endl;
	}

	return newClasses;
}

void MinimizateMilly(int x, int q, istream &fin)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);

	FillTransitionsAndOutputs(x, q, fin, transitions, outputs);

	auto transitionsToOutputs = TransitionsToOutputs(x, q, outputs);
	auto equivalenceСlasses = GetEquivalenceСlasses(transitionsToOutputs, q);

	cout << "equivalentStates" << endl;
	map<int, string>::iterator it;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		cout << it->first << " " << it->second << endl;
	}

	cout << endl << "transitions" << endl;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			cout << transitions[i][j];
		}
		cout << endl;
	}
	cout << endl << "outputs" << endl;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			cout << outputs[i][j];
		}
		cout << endl;
	}

	cout << endl << "podstavlyaem A" << endl;
	auto newClasses = SubstituteNewClasses(equivalenceСlasses, x, q, transitions);
}

void MinimizateMur()
{

}

int main()
{
	string inputFileName = "in.txt";
	string outputFileName = "out.txt";

	//ifstream fin(inputFileName);
	//ofstream fout(outputFileName);
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

