#pragma once
#include <iostream>
#include <string>
using namespace std;

class mechinecode {
public:
	mechinecode& operator=(const mechinecode&);
	void clear() {
		code.clear();
	}
	void print();
	bool set(string&);
private:
	string code;
};

mechinecode& mechinecode::operator=(const mechinecode& rhs)
{
	code = rhs.code;
	return *this;
	// TODO: �󦹳B���J return ���z��
}

void mechinecode::print()
{
	cout << code << endl;
}

bool mechinecode::set(string& str) {
	if (code.size() + str.size() > 32) return false;
	code.append(str);
	return true;
}