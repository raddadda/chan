// 2016603042 최영찬
// visual studio 2019 version
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

char operation1[3][3];
char operation2[3][3];
char operation3[3][3];

using namespace std;



void Operation_setting() {
	ifstream read("operations.txt");
	char operands;

	//  @ operator
	read >> operands;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			read >> operands;
			operation1[i][j] = operands;
		}
	}

	//  # operator
	read >> operands;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			read >> operands;
			operation2[i][j] = operands;
		}
	}

	//  & operator
	read >> operands;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			read >> operands;
			operation3[i][j] = operands;
		}
	}

}


bool read_and_evaluate(const string temp) {
	stack<char> Stack;
	int lengths = (int)temp.length();
	
	char nextelement, prevelement, pair;

	for (int i = 0; i < lengths; i++) {
		switch (temp.at(i)) {
		
		case '(':
		case '{':
		case '[':
			Stack.push(temp.at(i)); break;
		case ')':
		case '}':
		case ']':
			if (Stack.empty()) return false;
			switch (temp.at(i)) {
			case ')': pair = '('; break;
			case '}': pair = '{'; break;
			case ']': pair = '['; break;
			}
			
			if (Stack.top() != pair) return false;
			Stack.pop(); break;
		case '@':
		case '#':
		case '&':
			if (i < 1 || i + 1 >= lengths) return false; 
			prevelement = temp.at(i - 1);
			nextelement = temp.at(i + 1);
			
			if (prevelement == '(' || prevelement == '{' || prevelement == '[') return false; 
			else if (prevelement == '@' || prevelement == '#' || prevelement == '&') return false; 
			else if (nextelement == ')' || nextelement == '}' || nextelement == ']') return false; 
			break;
		}
	}

	
	if (Stack.empty()) return true;
	
	else return false;
}

void New_Calculate(stack<char>& numStack, char op) {
	int left, right;
	right = (int)(numStack.top() - 'a');
	numStack.pop();
	left = (int)(numStack.top() - 'a');
	numStack.pop();

	switch (op) {
	case '@':
		numStack.push(operation1[left][right]);
		break;
	case '#':
		numStack.push(operation2[left][right]);
		break;
	case '&':
		numStack.push(operation3[left][right]);
		break;
	}
}

void make_Postfix(const string temp, char* buf) {
	stack<char> oper;
	char now;
	int iter = 0;
	for (int i = 0; i < temp.length(); i++) {
		now = temp.at(i);
		if (now >= 'a' && now <= 'c') { buf[iter] = now; iter++; }
		else if (now == '@' || now == '#' || now == '&') {
			if (oper.empty() || oper.top() == '('
				|| oper.top() == '{' || oper.top() == '[') {
				oper.push(now);
			}
			else if (now == '&') { 
				if (oper.top() == '&') {
					buf[iter] = now;
					iter++;
					oper.pop();
					oper.push(now);
				}
				else oper.push(now);
			}
			else {
				buf[iter] = oper.top();
				iter++;
				oper.pop();
				oper.push(now);
			}
		}
		else if (now == '(' || now == '{' || now == '[') oper.push(now);
		else if (now == ')' || now == '}' || now == ']') {
			char pair = 0;
			switch (now) {
			case ')': pair = '('; break;
			case '}': pair = '{'; break;
			case ']': pair = '['; break;
			}
			for (;;) {
				if ((char)oper.top() == pair) { oper.pop(); break; }
				buf[iter] = oper.top();
				iter++;
				oper.pop();
			}
		}
	}
	if (oper.empty()) return;
	for (; oper.empty() != true; iter++) {
		buf[iter] = oper.top();
		oper.pop();
	}
}

void calcul_Postfix(const char* temp, int lengths) {
	stack<char> num;
	char now;
	for (int i = 0; i < lengths; i++) {
		now = temp[i];
		switch (now) {
		case 'a':
		case 'b':
		case 'c':
			num.push(now); break;
		case '@':
		case '#':
		case '&':
			New_Calculate(num, now);
			break;
		}
	}
	cout << num.top() << endl;
}

void Calculate(const string temp) {

	if (!read_and_evaluate(temp)) 
	{
		cout << "입력문자를 확인해주세요." << endl;
	}
	else {
		char* postfix = (char*)malloc(temp.length());

		for (int i = 0; i < temp.length(); i++)
			postfix[i] = 0;


		make_Postfix(temp, postfix);
		


		calcul_Postfix(postfix, (int)strlen(postfix));
		free(postfix);
	}
}


int main(int argc, const char* argv[]) {
	vector<string> exp;
	string tmp;
	Operation_setting();

	
	while (1) {
		cin >> tmp;
		if (tmp == "end") break;
		exp.push_back(tmp);
	}

	
	for (int i = 0; i < exp.size(); i++)
		Calculate(exp[i]);

	return 0;
}

