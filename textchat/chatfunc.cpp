#include "chatfunc.h"
#include <iostream>

using namespace std;


string getUser()
{
	cin.sync();
	string line;
	getline(cin, line);

	return line;
}

string getAddress()
{
	cout << "enter the adress: " << flush;
	string input = getUser();

	return input;

}

int getPort()
{
	cout << "enter the port: " << flush;
	string input=getUser();

	return stoi(input);
}

string getMessage()
{
	return getUser();
}
