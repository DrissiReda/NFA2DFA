#include <iostream>
#include "FA.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	FA<char> fa(0, false);
	fa.add_state(1, false);
	fa.add_state(2, true);
	fa.add_state(3, true);
	fa.add_transition(0,'a',1);
	fa.add_transition(1,'b',2);
	fa.add_transition(1,'c',3);

	char inp;
	while(true)
	{
		cout << "State: " << fa.state() << "  " << fa.is_accepting()?"true":"false";
		cout << endl;
		cin >> inp;
		if(cin.eof()) break;
		fa.input(inp);
	}

	return 0;
}
