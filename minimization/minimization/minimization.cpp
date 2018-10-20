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
void FillTransitionsAndOutputsMilly(int x, int q, istream& fin, int** transitions, int** outputs)
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

void FillTransitionsAndOutputsMur(int x, int q, istream& fin, int** transitions, int** outputs)
{
	vector<int> temp;
	int k;

	for (int j = 0; j < q; ++j)
	{
		fin >> k;
		temp.push_back(k);
	}

	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			transitions[i][j] = temp[j];
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
			if (temp[j] == (elem))
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
int** SubstituteNewClasses(map<int, vector<int>> equivalenceСlasses, int x, int q, int** transitions)
{
	auto newClasses = InitializeArray(x, q);
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
				auto temp = FindElemInClasses(equivalenceСlasses, elem);
				newClasses[j][k] = temp;
			}
		}
	}

	return newClasses;
}

bool IsFissionable(multimap<int, vector<int>> transitionsToOutputs1)
{
	vector<int> temp = transitionsToOutputs1.begin()->second;
	int repeats = count_if(transitionsToOutputs1.begin(), transitionsToOutputs1.end(), [temp](const pair<int, vector<int>>& p) { return p.second == temp; });

	return (repeats != transitionsToOutputs1.size());
}

vector<vector<int>> SplitClasses(map<int, vector<int>> equivalenceСlasses, int x, int q, int** transitions, int** transitionsConst, vector<vector<int>>& fissionableClasses)
{
	bool isNeedToContinue = false;
	map<int, vector<int>>::iterator it;
	map<int, vector<int>> allClasses;
	vector<int> arrayOfQ;

	int i = 0;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		auto elem = it->second;
		auto transitionsToOutputs1 = TransitionsToOutputs(x, elem, transitions);
		auto equivalenceСlasses1 = GetEquivalenceСlasses(transitionsToOutputs1, elem);

		map<int, vector<int>>::iterator it1;
		for (it1 = equivalenceСlasses1.begin(); it1 != equivalenceСlasses1.end(); it1++)
		{
			allClasses.insert(pair<int, vector<int>>(i, it1->second));
			++i;
		}

		if (IsFissionable(transitionsToOutputs1))
		{
			isNeedToContinue = true;
		}

		arrayOfQ.push_back(i);
	}

	if (isNeedToContinue)
	{
		//передаем классы эквивалентности, x, q и переходы
		auto newClasses = SubstituteNewClasses(allClasses, x, q, transitionsConst);
		//передаем классы эквивалентности, x, переходы и то куда засунем все классы
		SplitClasses(allClasses, x, q, newClasses, transitionsConst, fissionableClasses);
	}
	else
	{
		for (it = allClasses.begin(); it != allClasses.end(); it++)
		{
			fissionableClasses.push_back(it->second);
		}
	}

	return fissionableClasses;
}

vector<vector<int>> GetStates(vector<int> arrOfStates, int** transitions, int x)
{
	//cout << "GetStates" << endl;
	vector<vector<int>> arrStates;
	for (int i = 0; i < arrOfStates.size(); ++i)
	{
		//cout << arrOfStates[i] << " ";
		vector<int> temp;
		for (int j = 0; j < x; ++j)
		{
			//cout << transitions[j][arrOfStates[i]];
			temp.push_back(transitions[j][arrOfStates[i]]);
		}
		//cout << endl;
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
	for (int j = 0; j < classes.size(); ++j)
	{
		for (int i = 0; i < classes[j].size(); ++i)
		{
			if (number == classes[j][i])
			{
				return j;
			}
		}
	}

	return 1;
}

vector<vector<int>> СhangeFinalStates(vector<vector<int>> classes, vector<vector<int>> oldStates, int x)
{
	cout << "ChangeFinalStates" << endl;
	vector<vector<int>> finalStates;
	for (int j = 0; j < oldStates.size(); ++j)
	{
		vector<int> temp;
		for (int i = 0; i < oldStates[j].size(); ++i)
		{
			int s = FindRenameState(oldStates[j][i], classes, x);
			//cout << oldStates[j][i] << " " << s << endl;
			temp.push_back(s);
		}
		//ShowVector(temp);
		//ShowVector(oldStates[j]);
		finalStates.push_back(temp);
	}

	return finalStates;
}

void MinimizateAuthomate(int x, int q, vector<vector<int>>& states, vector<vector<int>>& finalOutputs, int** transitions, int** outputs)
{
	vector<int> strQ = TransitionsToVector(q);
	auto transitionsToOutputs = TransitionsToOutputs(x, strQ, outputs);
	auto equivalenceСlasses = GetEquivalenceСlasses(transitionsToOutputs, strQ);

	cout << "Equiv" << endl;
	multimap<int, vector<int>>::iterator it;
	for (it = equivalenceСlasses.begin(); it != equivalenceСlasses.end(); it++)
	{
		cout << it->first << "  ";
		ShowVector(it->second);
	}

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

	auto newClasses = SubstituteNewClasses(equivalenceСlasses, x, q, transitions);

	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < strQ.size(); ++j)
		{
			cout << newClasses[i][j];
		}
		cout << endl;
	}
	cout << endl;

	vector<vector<int>> temp;
	SplitClasses(equivalenceСlasses, x, q, newClasses, transitions, temp);
	cout << "Temp: " << temp.size() << endl;
	for (int i = 0; i < temp.size(); ++i)
	{
		ShowVector(temp[i]);
	}

	//выбрать первое число (массив с первыми числами)
	vector<int> statesS = GetOneNumberFromEachState(temp);

	//вернуть состояния
	auto statesFromCondition = GetStates(statesS, transitions, x);
	//cout << "States from condition" << endl;
	//for (int i = 0; i < statesFromCondition.size(); ++i)
	//{
	//	ShowVector(statesFromCondition[i]);
	//}
	finalOutputs = GetStates(statesS, outputs, x);
	states = СhangeFinalStates(temp, statesFromCondition, x);
	//cout << "Size: " << states.size() << endl;
}

void MinimizateMilly(int x, int q, istream& fin, ostream& fout)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);

	FillTransitionsAndOutputsMilly(x, q, fin, transitions, outputs);
	vector<vector<int>> finalStates;
	vector<vector<int>> outputsFromCondition;
	MinimizateAuthomate(x, q, finalStates, outputsFromCondition, transitions, outputs);
	fout << finalStates.size() << endl;
	for (int j = 0; j < x; ++j)
	{
		for (int i = 0; i < finalStates.size(); ++i)
		{
			fout << finalStates[i][j] << " " << outputsFromCondition[i][j] << " ";
		}
		fout << endl;
	}
}

void MinimizateMur(int x, int q, istream& fin, ostream& fout)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);

	FillTransitionsAndOutputsMur(x, q, fin, transitions, outputs);
	vector<vector<int>> finalStates;
	vector<vector<int>> outputsFromCondition;
	MinimizateAuthomate(x, q, finalStates, outputsFromCondition, transitions, outputs);
	fout << finalStates.size() << endl;
	for (int j = 0; j < x; ++j)
	{
		for (int i = 0; i < finalStates.size(); ++i)
		{
			fout << finalStates[i][j] << " " << outputsFromCondition[i][j] << " ";
		}
		fout << endl;
	}
}

int main()
{
	string inputFileName = "in8.txt";
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

	if (automatonType == 1)
	{
		MinimizateMur(x, q, fin, fout);
	}
	else if (automatonType == 2)
	{
		MinimizateMilly(x, q, fin, fout);
	}

	return 0;
}
