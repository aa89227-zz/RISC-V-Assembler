#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "RISCV.h"
using namespace std;

int main() {
	riscv a;
	string str;
	string str1;
	string str2;
	string str3;
	ifstream inFile("opcode.txt", ios::in);
	while (getline(inFile, str)) {
		if (str == "") continue;
		str1 = str.substr(0, str.find_first_of(' '));
		str2 = str.substr(str.find_first_of(' ') + 1, str.find_first_of('=') - str.find_first_of(' ') - 1);
		str3 = str.substr(str.find_first_of('=') + 1);
		a.setFormat(str1, str2, str3);
	}
	inFile.close();
	cin >> str;
	//str = "text.txt";
	inFile.open(str, ios::in);
	vector<string> prog;
	while (getline(inFile, str)) {
		prog.push_back(str);
	}
	a.parseInsts(prog);
	a.print();
}

