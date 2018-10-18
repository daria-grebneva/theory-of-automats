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
		//cout << elem << endl;
		//ShowVector(temp);


		for (int j = 0; j < temp.size(); ++j)
		{
			if (temp[j] == (elem))
			{
				return it->first;
			}
		}
	}
	//cout << endl << "return -1" << endl;
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
	//cout << "Razmer: " << q.size();
	//cout << endl;
	map<int, vector<int>>::iterator it;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		auto temp = it->second;
		//cout << it->first << " ";
		//ShowVector(temp);
		//cout << "elem" << endl;
		for (int i = 0; i < temp.size(); ++i)
		{
			for (int j = 0; j < x; ++j)
			{
				int k = temp[i];
				int elem = transitions[j][k];
				//cout << elem;
				auto temp = FindElemInClasses(equivalenceСlasses, elem);
				newClasses[j][k] = temp;
			}
		}
		//cout << endl;
	}
	//cout << endl;

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
	map<int, vector<int>> allClasses;
	vector<map<int, vector<int>>> allNewClasses;
	vector<int> arrayOfQ;
	vector<vector<int>> arrayOfQNew;
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

		
		map<int, vector<int>>::iterator it1;

		for (it1 = equivalenceСlasses.begin(); it1 != equivalenceСlasses.end(); it1++)
		{
			auto elem = it1->second;
			ShowVector(elem);
			allClasses.insert(pair<int, vector<int>>(it1->first, elem));
		}
		allNewClasses.push_back(equivalenceСlasses);
		for (int i = 0; i < elem.size(); ++i)
		{
			arrayOfQ.push_back(elem[i]);
		}
		arrayOfQNew.push_back(elem);
	}

	if (isNeedToContinue)
	{
		//cout << "allClasses: " << allClasses.size() << endl;
		for (int i = 0; i < allNewClasses.size(); ++i)
		{
			auto newClasses = SubstituteNewClasses(allClasses, x, arrayOfQNew[i], transitions);
			cout << "new classes" << endl;
			for (int i = 0; i < x; ++i)
			{
				for (int j = 0; j < allClasses[i].size(); ++j)
				{
					cout << newClasses[i][j];
				}
				cout << endl;
			}
			SplitClasses(allClasses, x, newClasses, fissionableClasses);
		}			
	}
	cout << "EQUIVALENTNOST " << endl;
	for (int i = 0; i < allClasses.size(); ++i)
	{
		cout << i << " ";
		ShowVector(allClasses[i]);
	}

	for (int i = 0; i < allClasses.size(); ++i)
	{
		for (it = allClasses.begin(); it != allClasses.end(); it++)
		{
			if (find(fissionableClasses.begin(), fissionableClasses.end(), it->second) == fissionableClasses.end())
			{
				fissionableClasses.push_back(it->second);
			}
		}
	}

	return fissionableClasses;
}

vector<vector<int>> GetStates(vector<int> arrOfStates, int** transitions, int x)
{
	vector<vector<int>> arrStates;
	for (int i = 0; i < arrOfStates.size(); ++i)
	{
		vector<int> temp;
		for (int j = 0; j < x; ++j)
		{
			temp.push_back(transitions[j][arrOfStates[i]]);
		}
		arrStates.push_back(temp);
	}

	return arrStates;
}

vector<int> GetOneNumberFromEachState(vector<vector<int>> fissionableClasses)
{
	vector<int> arrOfStates;
	for (int i = 0; i < fissionableClasses.size(); ++i)
	{
		arrOfStates.push_back(fissionableClasses[i][0]);
	}

	return arrOfStates;
}

int FindRenameState(int number, vector<vector<int>> classes, int x)
{
	for (int j = 0; j < x; ++j)
	{
		for (int i = 0; i < classes.size(); ++i)
		{
			//cout << number << " " << classes[i][j] << endl; 
			if (number == classes[i][j])
			{
				return i;
			}
		}
	}

	return 1;
}

vector<vector<int>> СhangeFinalStates(vector<vector<int>> classes, vector<vector<int>> oldStates, int x)
{
	vector<vector<int>> finalStates;
	for (int j = 0; j < classes.size(); ++j)
	{
		vector<int> temp;
		for (int i = 0; i < x; ++i)
		{
			int s = FindRenameState(oldStates[j][i], classes, x);
			temp.push_back(s);
		}
		finalStates.push_back(temp);
	}
	
	return finalStates;
}

void MinimizateAuthomate(int x, int q, vector<vector<int>>& states, vector<vector<int>>& finalOutputs, int** transitions, int** outputs)
{
	vector<int> strQ = TransitionsToVector(q);
	auto transitionsToOutputs = TransitionsToOutputs(x, strQ, outputs);
	auto equivalenceСlasses = GetEquivalenceСlasses(transitionsToOutputs, strQ);

	/*cout << "EQUIVALENTNOST " << endl;
	for (int i = 0; i < equivalenceСlasses.size(); ++i)
	{
		ShowVector(equivalenceСlasses[i]);
	}*/

	bool isNeedToContinue = false;

	cout << "transitions" << endl;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < strQ.size(); ++j)
		{
			cout << transitions[i][j];
		}
		cout << endl;
	}
	cout << endl;

	cout << "outputs" << endl;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < strQ.size(); ++j)
		{
			cout << outputs[i][j];
		}
		cout << endl;
	}
	cout << endl;

	auto newClasses = SubstituteNewClasses(equivalenceСlasses, x, strQ, transitions);


	cout << "new classes" << endl;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < strQ.size(); ++j)
		{
			cout << newClasses[i][j];
		}
		cout << endl;
	}

	vector<vector<int>> temp;
	SplitClasses(equivalenceСlasses, x, newClasses, temp);

	//выбрать первое число (массив с первыми числами)
	vector<int> statesS = GetOneNumberFromEachState(temp);

	//вернуть состояния
	auto statesFromCondition = GetStates(statesS, transitions, x);
	finalOutputs = GetStates(statesS, outputs, x);
	states = СhangeFinalStates(temp, statesFromCondition, x);
}


void MinimizateMilly(int x, int q, istream& fin, ostream& fout)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);

	FillTransitionsAndOutputs(x, q, fin, transitions, outputs);
	vector<vector<int>> finalStates;
	vector<vector<int>> outputsFromCondition;
	MinimizateAuthomate(x, q, finalStates, outputsFromCondition, transitions, outputs);
	fout << finalStates.size() << endl;
	for (int j = 0; j < x; ++j)
	{
		for (int i = 0; i < finalStates.size(); ++i)
		{
			fout << finalStates[i][j];
			fout << outputsFromCondition[i][j];
		}
		fout << endl;
	}


}

void MinimizateMur()
{
}

int main()
{
	string inputFileName = "in1.txt";
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

	fout << automatonType << endl;
	fout << x << endl;
	fout << y << endl;

	switch (automatonType)
	{
	case 1:
		MinimizateMur();
	case 2:
		MinimizateMilly(x, q, fin, fout);
	}

	return 0;
}
