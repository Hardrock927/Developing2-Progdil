#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<iomanip>
//#include<algorithm>
#include<map>
using namespace std;

int NumberOfLines();
string * Lines(int NumberOfLines);
vector<string> produceListFromALine(string aLine, char split);
bool checkForString(vector<string>::iterator begin, vector<string>::iterator end, string val);
bool checkForChar(string look, char val);
void executeLines(string * Lines);

//int main() {
//	try {
//		executeLines(Lines(NumberOfLines()));
//		system("PAUSE");
//	}
//	catch (runtime_error &ex) {
//		cout << ex.what() << endl;
//		system("PAUSE");
//	}
//}

int NumberOfLines() {
	int numberOfLines = 0;
	string line;
	ofstream file("C:\\Users\\A. SERTKAYA\\Desktop\\Codes.txt");

	if (file.is_open()) {

		while (getline(cin, line)) {
			file << line << '\n';
			numberOfLines++;
		}
		file << "stop";
		file.close();
	} else throw runtime_error(" An error occured while opening file!\n");

	return ++numberOfLines;
}

string * Lines(int numberOfLines) {
	string * lines = new string[numberOfLines];
	string take;
	ifstream file("C:\\Users\\A. SERTKAYA\\Desktop\\Codes.txt",ios::beg);

	int i = 0;
	if (file.is_open()) {

		while (getline(file, take)) {
			lines[i] = take;
			i++;
		}

		file.close();
	} else throw runtime_error(" An error occured while opening file!\n");
	

	return lines;
}

vector<string> produceListFromALine(string aLine, char split) {
	vector<string> list;
	vector<char> word;
	for (size_t i = 0; i < aLine.length(); i++)
	{
		if (i == aLine.length() - 1) {
			word.push_back(aLine[i]);
			string temp(word.begin(), word.end());
			list.push_back(temp);
		}
		else if (aLine[i] != split) {
			word.push_back(aLine[i]);
		}
		else {
			string temp(word.begin(), word.end());
			if (!temp.empty()) list.push_back(temp);
			word.clear();
		}
	}
	return list;
}

bool checkForString(vector<string>::iterator begin, vector<string>::iterator end,string val) {
	while (begin != end) {
		if (*begin == val) return true;
		begin++;
	}
	return false;
}

bool checkForChar(string look, char val) {
	size_t take = look.find(val);
	if (take != string::npos) return true;
	return false;
}

char checkForOperation(string look) {
	char operations[5] = { '+','-','*','/','%' };
		for (size_t i = 0; i < 5; i++) {
			size_t take = look.find(operations[i]);
			if (take != string::npos) return operations[i];
		}
	return 'N';
}

bool checkForVariable(string look, map <string, int> variableNames ) {
	if (variableNames.find(look) != variableNames.end())
		return true;
	return false;
}

int numberOfVariable(string variable, map<string, int> variableNames) {
	if (variableNames[variable] != NULL) return variableNames[variable];
	else return INT_MAX;
}

void executeLines(string * lines) {
	string reservedKeywords[2] = { "svar", "print" };
	vector<string> listOfWords; int count = 0;
	map<string, int> variableNames;
	for (; *lines != "stop"; lines++)
	{
		count++;
		listOfWords = produceListFromALine(*lines, ' ');

		//Comma checkForString
		string CommaCheck = listOfWords[listOfWords.size() - 1];
		string take(listOfWords[listOfWords.size() - 1].begin(), listOfWords[listOfWords.size() - 1].end() - 1);
		listOfWords[listOfWords.size() - 1] = take;
		if (CommaCheck[CommaCheck.length() - 1] != ';') throw runtime_error(" An error occured: Expected ';' at the end of the statement " + count);
		//Svar variable definition
		if (checkForString(listOfWords.begin(),listOfWords.end(),"svar") ) {
			if (listOfWords.size() == 2) {
				variableNames.insert(make_pair(listOfWords[listOfWords.size() - 1], NULL));
				continue;
			}
			else
			{
				string error = " An error occured: Wrong definition of a variable in statement " + count;
				throw runtime_error(error);
			}
		}else if(listOfWords[0] == "svar") throw runtime_error(" An error occured: Wrong definition of a variable in statement " + count + '\n');

		//Print a variable
		if(checkForString(listOfWords.begin(),listOfWords.end(),"print")){
			if (listOfWords.size() == 2)
			{
				if (checkForVariable(listOfWords[1], variableNames))
				{
					if (variableNames[listOfWords[1]] != NULL) cout << variableNames[listOfWords[1]] << endl;
					else cout << "0x" << setw(6) << setfill('3') << rand() % 100000 << endl;
				}
				else throw runtime_error(" An error occured: Variable name couldn't find!");
			}
			else throw runtime_error(" An error occured: Wrong definition of a variable in statement " + count + '\n');
		}
		else if(listOfWords[0] == "print") throw runtime_error(" An error occured: Wrong style print in statement " + count + '\n');

		//Equality things
		for (size_t i = 0; i < listOfWords.size(); i++)
		{
			if (checkForChar(listOfWords[i], '=')) {
				//a = a*10
					if (listOfWords.size() == 3 && listOfWords[1] == "=") {
						vector<string> split = produceListFromALine(listOfWords[2], checkForOperation(listOfWords[2]));
						try {
							if (stoi(split[0]) && stoi(split[1])) {
								switch (checkForOperation(listOfWords[2]))
								{
								case '+':
									variableNames[listOfWords[0]] = stoi(split[0]) + stoi(split[1]);
									break;
								case '%':
									variableNames[listOfWords[0]] = stoi(split[0]) % stoi(split[1]);
									break;
								case '/':
									variableNames[listOfWords[0]] = stoi(split[0]) / stoi(split[1]);
									break;
								case '*':
									variableNames[listOfWords[0]] = stoi(split[0]) * stoi(split[1]);
									break;
								case '-':
									variableNames[listOfWords[0]] = stoi(split[0]) - stoi(split[1]);
									break;
								}
							}
							if (checkForVariable(split[0], variableNames) && checkForVariable(split[1], variableNames)) {
								switch (checkForOperation(listOfWords[2]))
								{
								case '+':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) + numberOfVariable(split[1], variableNames);
									break;
								case '%':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) % numberOfVariable(split[1], variableNames);
									break;
								case '/':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) / numberOfVariable(split[1], variableNames);
									break;
								case '*':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) * numberOfVariable(split[1], variableNames);
									break;
								case '-':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) - numberOfVariable(split[1], variableNames);
									break;
								}
							}
							
							if (checkForVariable(split[0], variableNames) && stoi(split[1]))
							{
								switch (checkForOperation(listOfWords[2]))
								{
								case '+':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) +stoi(split[1]);
									break;
								case '%':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) % stoi(split[1]);
									break;
								case '/':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) / stoi(split[1]);
									break;
								case '*':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) * stoi(split[1]);
									break;
								case '-':
									variableNames[listOfWords[0]] = numberOfVariable(split[0], variableNames) - stoi(split[1]);
									break;
								}
							}

							if (stoi(split[0]) && checkForVariable(split[1], variableNames)) {
								switch (checkForOperation(listOfWords[2]))
								{
								case '+':
									variableNames[listOfWords[0]] = stoi(split[0]) + numberOfVariable(split[1], variableNames);
									break;
								case '%':
									variableNames[listOfWords[0]] = stoi(split[0]) % numberOfVariable(split[1], variableNames);
									break;
								case '/':
									variableNames[listOfWords[0]] = stoi(split[0]) / numberOfVariable(split[1], variableNames);
									break;
								case '*':
									variableNames[listOfWords[0]] = stoi(split[0]) * numberOfVariable(split[1], variableNames);
									break;
								case '-':
									variableNames[listOfWords[0]] = stoi(split[0]) - numberOfVariable(split[1], variableNames);
									break;
								}
							}
						}
						catch (invalid_argument &ex) {
							cout << " An error occured: Invalid argument at operation in statement " << count << endl;
						}
					}
				}
				else if (listOfWords.size() == 2 ){
					vector<string> withEqual = produceListFromALine(listOfWords[i], '=');
					string check = listOfWords[i];
					// a= a*10
					if (check[check.length() - 1] == '=') {

					}// a =a*10
					else if (check[0] == '=') {

					}else throw runtime_error(" An error occured: Wrong assignment in statement " + count + '\n');
				}
				else if(1 == 2){// a=a*10
					vector<string> take = produceListFromALine(listOfWords[i], '=');
					if (take.size() == 2) {

					}
					else throw runtime_error(" An error occured: Wrong assignment in statement " + count + '\n');
				}
			}
		}
	}
