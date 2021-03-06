#include "stdafx.h"

using namespace std;

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
			outputs[i][j] = temp[j];
			fin >> transitions[i][j];
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
				++i;
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
		auto newClasses = SubstituteNewClasses(allClasses, x, q, transitionsConst);
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
	vector<vector<int>> finalStates;
	for (int j = 0; j < oldStates.size(); ++j)
	{
		vector<int> temp;
		for (int i = 0; i < oldStates[j].size(); ++i)
		{
			int s = FindRenameState(oldStates[j][i], classes, x);
			temp.push_back(s);
		}
		finalStates.push_back(temp);
	}

	return finalStates;
}

void ConversionForMur(vector<vector<int>>& statesFromCondition, vector<vector<int>>& finalOutputs, vector<vector<int>>& temp, int x)
{
	multimap<int, vector<int>> mapWithTransitions;
	multimap<int, vector<int>> mapWithStates;

	for (int i = 0; i < statesFromCondition.size(); ++i)
	{
		mapWithTransitions.insert(pair<int, vector<int>>(finalOutputs[i][0], statesFromCondition[i]));
		mapWithStates.insert(pair<int, vector<int>>(finalOutputs[i][0], temp[i]));
	}
	finalOutputs.clear();
	statesFromCondition.clear();
	temp.clear();
	map<int, vector<int>>::iterator it;
	for (it = mapWithTransitions.begin(); it != mapWithTransitions.end(); it++)
	{
		vector<int> k;
		for (int i = 0; i < x; ++i)
		{
			k.push_back(it->first);
		}
		finalOutputs.push_back(k);
		statesFromCondition.push_back(it->second);
	}

	for (it = mapWithStates.begin(); it != mapWithStates.end(); it++)
	{
		temp.push_back(it->second);
	}
}

void MinimizateAuthomate(int x, int q, vector<vector<int>>& states, vector<vector<int>>& finalOutputs, int** transitions, int** outputs, bool isMur)
{
	vector<vector<int>> temp;
	bool isNeedToContinue = false;

	vector<int> strQ = TransitionsToVector(q);
	auto transitionsToOutputs = TransitionsToOutputs(x, strQ, outputs);
	auto equivalenceСlasses = GetEquivalenceСlasses(transitionsToOutputs, strQ);
	auto newClasses = SubstituteNewClasses(equivalenceСlasses, x, q, transitions);

	SplitClasses(equivalenceСlasses, x, q, newClasses, transitions, temp);

	vector<int> statesS = GetOneNumberFromEachState(temp);

	auto statesFromCondition = GetStates(statesS, transitions, x);
	finalOutputs = GetStates(statesS, outputs, x);

	if (isMur)
	{
		ConversionForMur(statesFromCondition, finalOutputs, temp, x);
	}

	states = СhangeFinalStates(temp, statesFromCondition, x);
}

void Visualization(vector<vector<int>> transitions, vector<vector<int>> outputs)
{
	ofstream fout;
	fout.open("out.dot");

	fout << "digraph G{" << endl;

	for (int j = 0; j < transitions.size(); j++)
	{
		for (int i = 0; i < transitions[j].size(); i++)
		{
			fout << "\"" << j << "\""
				 << "->";
			fout << "\"" << transitions[j][i] << "\"";
			fout << "[label=\"" << i + 1 << "/" << outputs[j][i] << "\"];" << endl;
		}
	}

	fout << "}" << endl;
}

void MinimizateMilly(int x, int q, istream& fin, ostream& fout)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);

	FillTransitionsAndOutputsMilly(x, q, fin, transitions, outputs);
	vector<vector<int>> finalStates;
	vector<vector<int>> finalOutputs;
	MinimizateAuthomate(x, q, finalStates, finalOutputs, transitions, outputs, false);
	fout << finalStates.size() << endl;

	for (int j = 0; j < x; ++j)
	{
		for (int i = 0; i < finalStates.size(); ++i)
		{
			fout << finalStates[i][j] << " " << finalOutputs[i][j] << " ";
		}
		fout << endl;
	}

	Visualization(finalStates, finalOutputs);
}

void MinimizateMur(int x, int q, istream& fin, ostream& fout)
{
	auto transitions = InitializeArray(x, q);
	auto outputs = InitializeArray(x, q);

	FillTransitionsAndOutputsMur(x, q, fin, transitions, outputs);
	vector<vector<int>> finalStates;
	vector<vector<int>> finalOutputs;
	MinimizateAuthomate(x, q, finalStates, finalOutputs, transitions, outputs, true);
	fout << finalStates.size() << endl;

	for (int j = 0; j < finalOutputs.size(); ++j)
	{
		fout << finalOutputs[j][0] << " ";
		for (int i = 0; i < finalOutputs[j].size(); ++i)
		{
			finalOutputs[j][i] += 1;
		}
	}
	fout << endl;

	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < finalStates.size(); ++j)
		{
			fout << finalStates[j][i] << " ";
		}
		fout << endl;
	}

	Visualization(finalStates, finalOutputs);
}

int main()
{
	string inputFileName = "in5.txt";
	string outputFileName = "out5.txt";

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
