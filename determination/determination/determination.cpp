#include "pch.h"

using namespace std;

namespace
{

using arr = vector<int>;
using matrix = vector<pair<arr, vector<arr>>>;

auto comparator(arr v)
{
	return [=](const pair<arr, vector<arr>> lhs) {
		return lhs.first == v;
	};
}
} // namespace

arr GetFinalStatesArray(fstream& fin)
{
	arr K;
	int i;

	do
	{
		fin >> i;
		K.push_back(i);
	} while (fin.peek() != '\n');

	return K;
}

vector<arr> GetVectorFromString(const string& str, int x)
{
	vector<arr> temp(x);
	vector<arr> temp1(x);
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

	for (int i = 0; i < temp.size(); ++i)
	{
		auto k = temp[i];
		sort(k.begin(), k.end());
		for (int j = 0; j < k.size(); ++j)
		{
			temp1[i].push_back(k[j]);
		}
	}

	return temp1;
}

void PrintFinalStates(vector<arr> v, ostream& fout)
{
	for (size_t i = 0; i < v.size(); ++i)
	{
		auto k = v[i];
		for (size_t j = 0; j < k.size(); ++j)
		{
			fout << k[j];
		}
		fout << " ";
	}
	fout << endl;
}

void PrintVector(arr v, ostream& fout)
{
	for (size_t i = 0; i < v.size(); ++i)
	{
		fout << v[i];
		if (i + 1 != v.size())
		{
			fout << ".";
		}
	}
}

matrix GetNonDeterministicMatrix(fstream& fin, int z, int x)
{
	matrix nonDeterministic;
	bool firstIteration = true;

	string str;
	int j = 0;
	while (getline(fin, str))
	{
		auto temp = GetVectorFromString(str, x);
		if (!firstIteration)
		{
			arr start;
			start.push_back(j - 1);
			nonDeterministic.emplace_back(start, temp);
		}
		firstIteration = false;
		++j;
	}

	return nonDeterministic;
}

int VectorToInt(arr v)
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

void PrintDeterministicAutomaton(matrix newStates, ostream& fout)
{
	for (auto [first, second] : newStates)
	{
		auto v = second;
		for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i].size() > 0)
			{
				PrintVector(v[i], fout);
				fout << " " << i << " ";
			}
		}
		fout << endl;
	}
}

matrix InsertStartVertex(matrix v)
{
	matrix newStates;
	arr start = { 0 };
	auto [first, second] = *find_if(v.begin(), v.end(), comparator(start));
	newStates.emplace_back(first, second);

	return newStates;
}

bool IsInMap(matrix m, arr elem)
{
	auto it = find_if(m.begin(), m.end(), comparator(elem));
	return (it != m.end());
}

bool IsInQueue(vector<arr> m, arr elem)
{
	auto it = find(m.begin(), m.end(), elem);
	return (it != m.end());
}

pair<arr, vector<arr>> ReturnValueInMap(matrix m, arr elem)
{
	auto it = find_if(m.begin(), m.end(), comparator(elem));
	if (it != m.end())
	{
		return { it->first, it->second };
	}

	throw "The value not found in map!";
}

arr MergeVectors(arr& v1, arr v2)
{
	for (auto k : v2)
	{
		if (std::find(v1.begin(), v1.end(), k) == v1.end())
		{
			v1.push_back(k);
		}
	}

	sort(v1.rbegin(), v1.rend());
	reverse(v1.begin(), v1.end());

	return v1;
}

pair<arr, vector<arr>> GlueVertex(matrix v, arr vertex, int x)
{
	arr vertexes;
	vector<arr> vertexesValues(x);
	try
	{
		for (size_t i = 0; i < vertex.size(); ++i)
		{
			arr f;
			f.push_back(vertex[i]);
			auto t = ReturnValueInMap(v, f).first;

			vertexes = MergeVectors(vertexes, t);

			auto k = ReturnValueInMap(v, f).second;
			for (size_t j = 0; j < k.size(); ++j)
			{
				vertexesValues[j] = MergeVectors(vertexesValues[j], k[j]);
			}
		}
	}
	catch (const char* msg)
	{
		cerr << msg << endl;
	}

	return pair<arr, vector<arr>>(vertexes, vertexesValues);
}

void AddToQueue(vector<arr>& queue, vector<arr> vertex)
{
	for (auto k : vertex)
	{
		if (k.size() > 0 && !IsInQueue(queue, k))
		{
			queue.insert(queue.begin(), k);
		}
	}
}

void PopFront(vector<arr>& vec)
{
	assert(!vec.empty());
	vec.front() = std::move(vec.back());
	vec.pop_back();
}

matrix Determitation(matrix v, int x)
{
	auto newStates = InsertStartVertex(v);
	vector<arr> queue = {};
	AddToQueue(queue, newStates[0].second);

	while (queue.size() != 0)
	{
		auto elem = queue.back();
		queue.pop_back();
		try
		{
			if (!IsInMap(newStates, elem))
			{
				if (elem.size() == 1)
				{
					newStates.push_back(ReturnValueInMap(v, elem));
				}
				else
				{
					newStates.push_back(GlueVertex(v, elem, x));
				}

				auto t = ReturnValueInMap(newStates, elem);
				AddToQueue(queue, t.second);
			}
		}
		catch (const char* msg)
		{
			cerr << msg << endl;
		}
	}

	return newStates;
}

bool IsThereSuchState(const arr& v, int state)
{
	return (find(v.begin(), v.end(), state) != v.end());
}

bool IsThereSuchState(const vector<arr>& v, arr state)
{
	return (find(v.begin(), v.end(), state) != v.end());
}

vector<arr> GetFinalStatesArray(const matrix& m, const arr& K)
{
	vector<arr> finalStates;
	for (auto [first, second] : m)
	{
		auto v = first;
		for (auto j : K)
		{
			if (IsThereSuchState(v, j) && !IsThereSuchState(finalStates, v))
			{
				finalStates.push_back(v);
			}
		}
	}
	return finalStates;
}

string ConvertVecToString(const arr& my_vector)
{
	std::stringstream result;
	std::copy(my_vector.begin(), my_vector.end(), std::ostream_iterator<int>(result, "."));
	string s = result.str();
	s.erase(s.rbegin().base() - 1);
	string s1 = "\"";
	s1 = s1 + s + s1;
	return s1;
}

void Visualization(matrix newStates, vector<arr> finalStates)
{
	ofstream fout;
	fout.open("out.dot");
	fout << "digraph G{" << endl;

	for (size_t i = 0; i < finalStates.size(); i++)
	{
		fout << ConvertVecToString(finalStates[i]) << " [color=red]" << std::endl;
	}

	for (auto [first, second] : newStates)
	{
		auto v = second;
		string from = ConvertVecToString(first);
		for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i].size() > 0)
			{
				string to = ConvertVecToString(v[i]);
				fout << "	" << from << "->" << to << "[label=" << i << ']' << endl;
			}
		}
	}

	fout << "}" << endl;
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

	int x, z, k;
	arr K;
	fin >> x >> z >> k;
	fout << x << endl;
	K = GetFinalStatesArray(fin);

	auto nonDeterministic = GetNonDeterministicMatrix(fin, z, x);
	auto determized = Determitation(nonDeterministic, x);
	fout << determized.size() << endl;
	auto finalStates = GetFinalStatesArray(determized, K);
	fout << finalStates.size() << endl;
	PrintFinalStates(finalStates, fout);
	PrintDeterministicAutomaton(determized, fout);
	Visualization(determized, finalStates);
}
