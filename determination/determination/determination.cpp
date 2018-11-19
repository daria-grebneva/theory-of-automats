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


vector<vector<vector<int>>> GetNonDeterministicMatrix(fstream& fin, int z, int x)
{
	vector<vector<vector<int>>> nonDeterministic;
	bool firstIteration = true;
	
	for (int j = 0; j < z; ++j)
	{
		vector<vector<int>> temp(x);

		//while (((fin.peek() != '\n') || (fin.peek() != EOF)) || firstIteration)
		while (fin.peek() != '\n')
		{
			int i;
			int l;
			fin >> i;
			fin >> l;
			cout << i << " " << l << " ";
			//temp[l].push_back(i);
			//firstIteration = false;
		}

		//string str; 
		//while (getline(fin, str))
		//{
		//	for (int i = 0; i < str.size(); ++i)
		//	{
		//		std::cout << str[i] << " "; //1,2,3,
		//	}
		//}
		//cout << endl;

		nonDeterministic.push_back(temp);
	}

	
	return nonDeterministic;
}


void ShowVector(vector<int> v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		cout << v[i];
	}	
	cout << endl;
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

void Determitation(vector<vector<vector<int>>>& v, int x)
{
	for (int i = 0; i < v.size(); ++i)
	{
		auto h = v[i];
		for (int j = 0; j < h.size(); ++j)
		{
			if (h[j].size() > 1)
			{
				//determization
				//GetNewDetermitationVector(v, h[j], j, x);
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


	vector<vector<vector<int>>> nonDeterministic = GetNonDeterministicMatrix(fin, z, x);
	//Determitation(nonDeterministic, x);
	//ShowBigVector(nonDeterministic);
}
