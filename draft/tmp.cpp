#include <bits/stdc++.h>

using namespace std;

int numCells = 0;
string line, cellname, expression;
set<string> setCell;
map<string, string> cells;
map<string, double> cellValues;
map<string, list<string>> topo;
unordered_map<string, int> numPrevNode;
unordered_map<string, string> nextNode;

// Report Error and exit to OS
void Error(const string & MsgA, const string & MsgB){
   	cerr << "Error: " << MsgA << MsgB << endl;
   	// If error, return error code 1 to Operating System
	exit(1);
}

bool checkCell(string operand)
{
	for(auto c : operand)
	{
		if(!isdigit(c) && c != '.')
		{
			return true;
		}
	}
	return false;
}
// vector<string> find(string cellName, string expression)
void find(string cellname, string expression)
{
	// vector<string> res;
	string operand = "";
	for(int i = 0;i < expression.size(); ++i)
	{
		if(expression[i] == ' ') continue;
		if(expression[i] != '+' && expression[i] != '-' && expression[i] != '*' && expression[i] != '/')
		{
			operand += expression[i];
		}
		else
		{
			if(checkCell(operand))
			{
				// res.push_back(operand);
				topo[operand].push_back(cellname);
				numPrevNode[cellname]++;
			}
			operand = "";
		}
	}
	if(checkCell(operand)) 
	{
		topo[operand].push_back(cellname);
		numPrevNode[cellname]++;
	}
	// return res;
}

string inFixToPostFix(string expression)
{
	expression += '#';
	string operand = "";
	string res;
	stack<char> myStack;
	myStack.push('$');
	map<char, int> myOperator;
	myOperator['$'] = 0;
	myOperator['+'] = 1;
	myOperator['-'] = 1;
	myOperator['*'] = 2;
	myOperator['/'] = 2;
	for(auto c : expression)
	{
		if(c == ' ') continue;
		if(c != '+' && c != '-' && c != '*' && c != '/' && c != '#')
		{
			operand += c;
		}
		else
		{
			res = res + operand + ' ';
			operand = "";
			if(c == '#') break;
			while(myOperator[myStack.top()] >= myOperator[c])
			{
				char c0 = myStack.top();
				myStack.pop();
				res = res + c0;// + ' ';
			}
			myStack.push(c);
		}
	}

	while(myStack.size() > 1)
	{
		res = res + myStack.top();// + ' ';
		myStack.pop();
	}
	// cout << res.size() << '\n';
	return res;
}

double calculate(string expression)
{
	string operand = "";
	stack<double> myStack;
	// cout << "value of B1" << cells["B1"].size() << '\n';

	for(char c : expression)
	{
		// cout << "YES\n";
		// cout << c << ' ';
		if(c != '+' && c != '-' && c != '*' && c != '/' && c != ' ')
		{
			operand += c;
		}
		else
		{
			if(c == ' ')
			{
				double val;
				if(cells.find(operand) != cells.end()) val = stod(cells[operand]);
				else val = stod(operand);
				// cout << operand << ' ' << cells[operand] << '\n';
				
				// double val = stod(operand);
				myStack.push(val);
				operand = "";
			}
			else
			{
				// double val1 = stod(cells[operand]);
				// double val1 = stod(operand);
				double val1 = myStack.top();
				myStack.pop();
				double val2 = myStack.top();
				myStack.pop();
				double res;
				switch(c)
				{
					case '+': res = val1 + val2; break;
					case '-': res = val2 - val1; break;
					case '*': res = val1 * val2; break;
					case '/': res = val2 / val1; break;
					default: break;
				}
				myStack.push(res);
				operand = "";
			}
		}
	}
	return myStack.top();
	// return 1;
}



int main()
{
	// inputData();
	string inputFileName = "testcase.txt";
	ifstream inputFile(inputFileName.c_str(), ifstream::in | ifstream::binary);
	if(!inputFile.is_open())
	{
		Error("Counldn't open the file named", inputFileName);
	}

	getline(inputFile, line);
	numCells = stoi(line);

	while(getline(inputFile, cellname))
	{
		getline(inputFile, expression);
		setCell.insert(cellname);
		cells[cellname] = expression;
		// vector<string> S = find(expression);
		find(cellname, expression);
		// list<string> tmpList = {};
		// for(auto str : S)
		// {
		// 	tmpList.push_back(str);
		// 	numPrevNode[str]++;
		// }
		// topo[cellname] = tmpList;


		// cout << cellname << ' ';
		// for(auto val : setCell)
		// {
		// 	cout << val << ' ' << numPrevNode[val] << ' ';
		// }
		// cout << '\n';

		// for(int i = 0;i < S.size(); ++i)
		// {
		// 	cout << S[i] << ' ';
		// }
		// cout << '\n';
	}

	stack<string> Stk;// {"0"};
	Stk.push("0");
	for(auto cellname : setCell)
	{
		if(numPrevNode[cellname] == 0)
		{
			// cout << cellname << ' ';
			nextNode[cellname] = Stk.top();
			Stk.push(cellname);
		}
	}

	// for(auto k = 0;k < Stk.size(); ++k)
	// {
	// 	cout << Stk.top();
	// 	Stk.pop();
	// }

	for(auto cellname : setCell)
	{
		if(Stk.top() == "0")
		{
			Error("Circular dependencies", "\n");
		}
		auto curCell = Stk.top();
		Stk.pop();
		// cout << curCell << "\n";
		auto tmp = inFixToPostFix(cells[curCell]);
		// cout << tmp << ' ' << tmp.size() << '\n';
		// cout << calculate(tmp) << '\n';
		cellValues[curCell] = calculate(tmp);
		cells[curCell] = to_string(calculate(tmp));
		auto ptr = topo[curCell].begin();
		// cout << "ptr " << *ptr << ' ';
		int i = 0;
		while(ptr != topo[curCell].end())
		{
			// cout << "i = " << i << ' ';
			// i++;
			// cout << *ptr << ' ' << numPrevNode[*ptr] << ' ';
			numPrevNode[*ptr]--;
			// cout << *ptr << ' ' << numPrevNode[*ptr];
			// if(numPrevNode[*ptr] == 0);
			// {
			// 	cout << *ptr << ' ' << numPrevNode[*ptr];
			// 	nextNode[*ptr] = Stk.top();
			// 	Stk.push(*ptr);
			// }
			// ptr++;
			if(numPrevNode[*ptr] == 0)
			{
				nextNode[*ptr] = Stk.top();
				Stk.push(*ptr);
			}
			ptr++;
		}
	}

	for(auto cell : cellValues)
	{
		cout << cell.first << "\n";
		cout << cell.second << "\n";
	}

	// string exp = " 9 + 5 - 4 * 3";
	// exp = inFixToPostFix(exp);
	// cout << exp << '\n';
	// cout << calculate(exp) << '\n';

	// for(auto s : topo)
	// {
	// 	cout << s.first << ' ';
	// 	for(auto val : s.second)
	// 	{
	// 		cout << val << ' ' << numPrevNode[val] << ' ';
	// 	}
	// 	cout << '\n';
	// }
}