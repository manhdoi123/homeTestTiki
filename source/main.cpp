#include <bits/stdc++.h>

using namespace std;

int numCells = 0;
string line, cellname, expression;
//	set of cells
set<string> setCell;
//	cells and their expression in input file
map<string, string> cells;
//	cells and value after calculate
map<string, double> cellValues;
//	topo structure
map<string, list<string>> topo;
unordered_map<string, int> numPrevNode;
unordered_map<string, string> nextNode;

// Report Error and exit to OS
void Error(const string & MsgA, const string & MsgB){
   	cerr << "Error: " << MsgA << MsgB << endl;
   	// If error, return error code 1 to Operating System
	exit(1);
}

//	check a operand is a name of cell or not
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

//	find order of cells in the topo
void find(string cellname, string expression)
{
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
}

//	convert infix expression to postfix expression
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
				res = res + c0;
			}
			myStack.push(c);
		}
	}

	while(myStack.size() > 1)
	{
		res = res + myStack.top();
		myStack.pop();
	}
	return res;
}

//	calculate value from postfix expression
double calculate(string expression)
{
	string operand = "";
	stack<double> myStack;

	for(char c : expression)
	{
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
				myStack.push(val);
				operand = "";
			}
			else
			{
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
}



int main()
{
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
		find(cellname, expression);
	}

	inputFile.close();

	//	find nodes that doesn't have a previous node
	stack<string> Stk;
	Stk.push("0");
	for(auto cellname : setCell)
	{
		if(numPrevNode[cellname] == 0)
		{
			nextNode[cellname] = Stk.top();
			Stk.push(cellname);
		}
	}

	//	topo sort algorithms
	for(auto cellname : setCell)
	{
		if(Stk.top() == "0")
		{
			Error("Circular dependencies", "\n");
		}
		auto curCell = Stk.top();
		Stk.pop();
		auto tmp = inFixToPostFix(cells[curCell]);
		cellValues[curCell] = calculate(tmp);
		cells[curCell] = to_string(calculate(tmp));
		auto it = topo[curCell].begin();
		int i = 0;
		while(it != topo[curCell].end())
		{
			numPrevNode[*it]--;
			if(numPrevNode[*it] == 0)
			{
				nextNode[*it] = Stk.top();
				Stk.push(*it);
			}
			it++;
		}
	}

	// cout << "Result: \n";
	ofstream outputFile("output.txt", ofstream :: out);
	for(auto cell : cellValues)
	{
		outputFile << cell.first << "\n";
		outputFile << cell.second << "\n";
	}

	outputFile.close();

	return 0;
}