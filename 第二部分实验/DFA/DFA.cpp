#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
using namespace std;

/*MODEΪ1��ʾʹ�ü������룬MODEΪ2��ʾ�ļ���д*/
#define MODE 1

string charSet;				//�ַ���
int stateNum;				//״̬����С
int startState;				//��ʼ״̬
vector<int> acceptStateSet;	//����״̬
vector<vector<int>> trans;	//״̬ת���� 

unordered_map<char, size_t> getCharNum;		//�ַ�->�ַ����

/*�Ӽ�������DFA*/
void inputDFA()
{
	charSet.clear();
	acceptStateSet.clear();
	trans.clear();
	getCharNum.clear();

	int n, state;
	cout << "�������ַ���(o�������ǰ���ַ��������ַ�)��";
	getline(cin, charSet);
	for (size_t i = 0; i < charSet.size(); ++i)
		getCharNum[charSet[i]] = i;

	cout << "������״̬���Ĵ�С��״̬Ĭ�ϴ�0��ʼ����";
	cin >> stateNum;

	cout << "�����뿪ʼ״̬��";
	cin >> startState;

	cout << "���������״̬�ĸ�����";
	cin >> n;

	cout << "���������״̬����";
	for (int i = 0; i < n; ++i)
	{
		cin >> state;
		acceptStateSet.push_back(state);
	}

	cout << "������״̬ת����" << endl;
	trans.assign(stateNum, vector<int>());
	for (int i = 0; i < stateNum; ++i)
		for (size_t j = 0; j < charSet.size(); ++j)
		{
			cin >> state;
			trans[i].push_back(state);
		}
}

/*���ļ�����DFA*/
void readDFA(const string& path)
{
	charSet.clear();
	acceptStateSet.clear();
	trans.clear();
	getCharNum.clear();

	ifstream in(path);
	int n, state;

	getline(in, charSet);
	for (size_t i = 0; i < charSet.size(); ++i)
		getCharNum[charSet[i]] = i;

	in >> stateNum>> startState >> n;

	for (int i = 0; i < n; ++i)
	{
		in >> state;
		acceptStateSet.push_back(state);
	}

	trans.assign(stateNum, vector<int>());
	for (int i = 0; i < stateNum; ++i)
		for (size_t j = 0; j < charSet.size(); ++j)
		{
			in >> state;
			trans[i].push_back(state);
		}
	
	in.close();
}

/*��DFA������ļ���*/
void writeDFA(const string& path)
{
	ofstream out(path);
	out << charSet << endl << stateNum << endl << acceptStateSet.size() << endl;

	for (size_t i = 0; i < acceptStateSet.size(); ++i)
		out << acceptStateSet[i] << " ";

	out << endl;
	for (int i = 0; i < stateNum; ++i)
	{
		for (size_t j = 0; j < charSet.size(); ++j)
			out << trans[i][j] << " ";
		out << endl;
	}

	out.close();
}

/*�ж��Ƿ�Ϊ����״̬*/
bool isAcceptState(int state)
{
	for (size_t i = 0; i < acceptStateSet.size(); ++i)
		if (state == acceptStateSet[i]) return true;

	return false;
}

/*�ж�DFA�Ƿ�Ϸ�*/
bool isLegalDFA()
{
	//�ַ����ظ�Ԫ�ؼ��
	for (size_t i = 0; i < charSet.size(); ++i)
		for (size_t j = i + 1; j < charSet.size(); ++j)
			if (charSet[i] == charSet[j]) return false;

	//����ʼ״̬�Ƿ�����״̬��
	if (startState < 0 || startState >= stateNum) return false;

	//������̬�Ƿ�Ϊ�ռ�
	if (acceptStateSet.empty()) return false;

	//������״̬���Ƿ�Ϊ״̬�����Ӽ�
	for (size_t i = 0; i < acceptStateSet.size(); ++i)
		if (acceptStateSet[i] < 0 || acceptStateSet[i] >= stateNum) return false;

	//��״̬ת�Ʊ�ļ��
	for (size_t i = 0; i < trans.size(); ++i)
	{
		for (size_t j = 0; j < charSet.size(); ++j)
		{
			if (trans[i][j] < 0) continue;
			//���ܵ���һ�������ڵ�״̬
			if (trans[i][j] < 0 || trans[i][j] >= stateNum) return false;
		}
	}

	return true;
}

/*�ж��ַ����Ƿ��ܱ�DFA����*/
bool isLegalString(string str)
{
	//����������ַ�����������ַ����滻Ϊ'o'
	for (size_t i = 0; i < str.length(); ++i)
		if (charSet.find(str[i]) == string::npos)
			str[i] = 'o';

	//״̬ת��
	int curState = startState;
	cout << "״̬ת�������" << curState;
	for (size_t i = 0; i < str.length(); ++i)
	{
		//��������ַ������ַ����У����������״̬-1
		if (getCharNum.find(str[i]) == getCharNum.end()) curState = -1;
		else curState = trans[curState][getCharNum[str[i]]];
		cout << "->" << curState;
		if (curState == -1) return false;
	}

	return isAcceptState(curState);
}

/*���ѣ��ҵ�����ΪmaxLength�Ŀɽ����ַ���*/
void dfs(int curState, int curLength, int maxLength, string& str)
{
	if (curLength == maxLength)
	{
		if (isAcceptState(curState))
			cout << (str.empty() ? ("��") : str) << endl;
		return;
	}

	for (int j = 0; j < charSet.size(); ++j)
	{
		if (trans[curState][j] != -1)
		{
			str += charSet[j];
			dfs(trans[curState][j], curLength + 1, maxLength, str);
			str.pop_back();
		}
	}
}

/*��ӡ�����г��Ȳ�����N�Ŀɽ����ַ���*/
void getString(int N)
{
	string str = "";
	for (int maxLength = 0; maxLength < N + 1; ++maxLength)
		dfs(startState, 0, maxLength, str);
}

int main()
{
	if (MODE == 1)
	{
		inputDFA();
		writeDFA("out.dfa");
	}
	else readDFA("3.dfa");

	if (!isLegalDFA())
	{
		cout << "��DFA�ǷǷ���" << endl;
		return 0;
	}
	else cout << "��DFA�ǺϷ���" << endl;

	int N;
	cout << "�������ַ�������󳤶�N��";
	cin >> N;
	getString(N);

	cout << "������������ַ�����" << endl;
	string str;
	while (cin >> str)
	{
		if (isLegalString(str))
			cout << "���ɽ���" << endl;
		else cout << "�����ɽ���" << endl;
	}

	system("pause");
	return 0;
}
