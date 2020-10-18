#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "mechinecode.h"
using namespace std;

class riscv {
public:
	struct inform {
		string format;
		string opcode;
	};
	void parseInsts(vector<string>);
	void print();
	void setFormat(string, string, string);
private:
	string itob(int a)
	{
		string binary = "";
		int mask = 1;
		for (int i = 0; i < 31; i++)
		{
			if ((mask & a) >= 1)
				binary = "1" + binary;
			else
				binary = "0" + binary;
			mask <<= 1;
		}
		return binary;
	}
	int getValue(map<string, int>& label, string str, int p);
	vector<mechinecode> mcodes;
	map<string, inform> inst_format;
};

void riscv::parseInsts(vector<string> text)
{
	size_t pos;
	int p = 0;
	map<string, int> label;
	for (vector<string>::iterator it = text.begin(); it != text.end();) {
		for (int j = 0; j < (*it).size(); ++j) if ((*it)[j] == '\t') (*it)[j] = ' '; //remove tab

		pos = (*it).find(':');
		if (pos != string::npos) {//if find label, put position into [label]
			label[(*it).substr((*it).find_first_not_of(' '), pos)] = p;
		}
		if ((*it)[(*it).find_last_not_of(' ')] == ':') { //if not find instruction, erase current line
			it = text.erase(it);
		}
		else {
			if (pos == string::npos)
				pos = 0;
			++pos;
			
			*it = (*it).substr(pos);
			++it;
			++p;
		}
	}

	string str;
	string str1;
	string str2;
	
	stringstream ss1, ss2;
	stringstream ss1_, ss2_;
	map<string, int> val;
	mechinecode mc;
	for (int i = 0; i < text.size(); ++i) {
		mc.clear();
		str = text[i];
		for (int j = 0; j < str.size(); ++j) { //convert ',' into ' '
			if (str[j] == ',') str[j] = ' ';
			if (str[j] == '\t') str[j] = ' ';
		}
		ss1 << str;
		ss1 >> str; //inst
		str2 = inst_format[str].format;
		for (int j = 0; j < str2.size(); ++j) { //convert ',' into ' '
			if (str2[j] == ',') str2[j] = ' ';
			if (str2[j] == '\t') str2[j] = ' ';
		}
		ss2 << str2;
		while (ss1 >> str1) {
			ss2 >> str2;
			if (str2 == "offset(rs1)") {
				for (int i = 0; i < str1.size(); ++i) if (str1[i] == '(' || str1[i] == ')') str1[i] = ' ';
				for (int i = 0; i < str2.size(); ++i) if (str2[i] == '(' || str2[i] == ')') str2[i] = ' ';

				ss1_ << str1;
				ss1_ << str2;

				ss1_ >> str1;
				ss2_ >> str2;
				val[str2] = getValue(label, str1, i);

				ss1_ >> str1;
				ss2_ >> str2;
				val[str2] = getValue(label, str1, i);
			}
			else {
				val[str2] = getValue(label, str1, i);
			}
		}

		
		ss1.clear();
		ss2.clear();
		ss1 << inst_format[str].opcode;
		while (ss1 >> str1) {
			if (str1.find("[") != string::npos) {
				str2 = str1.substr(str1.find('[') + 1, str1.find(']') - str1.find('[') - 1);
				for (int i = 0; i < str2.size(); ++i) if (str2[i] == '|') str2[i] = ' ';
				ss2.clear();
				ss2 << str2;

				string bi;
				if (val.find("imm") != val.end()) {
					bi = itob(val["imm"]);
				}
				else {
					bi = itob(val["offset"]);
				}
				reverse(bi.begin(), bi.end());
				while (ss2 >> str2) {
					if (str2.find(':') != string::npos) {
						str1 = str2.substr(0, str2.find(':'));
						str2 = str2.substr(str2.find(':') + 1);
						int begin = atoi(str1.c_str());
						int end = atoi(str2.c_str());
						str1.clear();
						for (int i = begin; i >= end; --i) {
							str1.push_back(bi[i]);
						}
					}
					else {
						str1.clear();
						str1.push_back(bi[atoi(str2.c_str())]);
					}
					mc.set(str1);
					str1.clear();
				}
			}
			else if (str1.find("=") != string::npos) {
				str2 = str1.substr(str1.find('=') + 1);
				mc.set(str2);
			}
			else {
				str2.clear();
				string bi;
				bi = itob(val[str1]);
				for (int i = 26; i <= 30; ++i) {
					str2.push_back(bi[i]);
				}
				mc.set(str2);
			}
		}
		ss1.clear();
		ss2.clear();
		ss1_.clear();
		ss2_.clear();
		val.clear();
		mcodes.push_back(mc);
	}
}



void riscv::print()
{
	for (vector<mechinecode>::iterator it = mcodes.begin(); it != mcodes.end(); ++it)
		(*it).print();
}

void riscv::setFormat(string inst, string format, string opcode) {
	inst_format[inst] = inform{ format, opcode };
}

inline int riscv::getValue(map<string, int>& label, string str, int p)
{
	if (label.find(str) != label.end())
		return label[str] - p;
	else if (str[0] == 'x')
		str.erase(str.begin());
	return atoi(str.c_str());
}
