#include <iostream>
#include "FA.hpp"

using namespace std;


FA constructFA()
{
	int st;
	bool acc;
	char inp;
	cout << "Choose your initial state " << endl;
	cin >> st;
	cout << "Is it accepting? [y/N]" << endl;
	cin >> inp;
	if(inp=='y' || inp=='Y')
		acc=true;
	else if (inp=='n' || inp=='N')
		acc=false;
	else
	{
		cout << "Did not quite get that, but I'll take it as a no" << endl;
	}
	FA fa(inp,acc);
	cout << "Start entering your states" << endl;
	while(true)
	{
		cin >> st;
		if(cin.eof())
		 	break;
		cout << "Is it accepting? [y/N]" << endl;
		cin >> inp;
		if(inp=='y' || inp=='Y')
			acc=true;
		else if (inp=='n' || inp=='N')
			acc=false;
		else
		{
			cout << "Did not quite get that, but I'll take it as a no" << endl;
		}
		fa.add_state(inp,acc);
		cout << "Next state :[You can exit with Ctrl+D]"<<endl;
	}
	cin.clear();
	//Transitions
	cout << "Enter your transitions" << endl;
	int dst;
	while(true)
	{
		cout << "Source of the transition " << endl;
		cin >> st;
		if(cin.eof())
		 	break;
		cout << "Element to consume " << endl;
		cin >> inp;
		cout << "Destination of the transition" << endl;
		cin >> dst;
		fa.add_transition(st,inp,dst);
		cout << "Next transition :[You can exit with Ctrl+D]"<<endl;
	}
	cin.clear();
	return fa;
}
void simulate()
{
	char inp;
	cout << "Construct your automaton" << endl;
	FA fa=constructFA();
	while(true)
	{
		cout << "State: " << fa.state() << "  " << fa.is_accepting()?"true":"false";
		cout << endl;
		cin >> inp;
		if(cin.eof()) break;
		fa.input(inp);
	}
	cin.clear();
}
void concatenate()
{
	cout << "construct your first automaton" << endl;
	FA fa1=constructFA();
	cout << "construct your second automaton " << endl;
	FA fa2=constructFA();
	cout << "Watch them concatenate" << endl;
	fa1.conTo(fa2);
}
void starring()
{
	cout << "construct your automaton" << endl;
	FA fa=constructFA();
	cout << "Applying a kleene star on your automaton" << endl;
	fa.starring();
}
void assmble()
{
	cout << "construct your first automaton" << endl;
	FA fa1=constructFA();
	cout << "construct your second automaton " << endl;
	FA fa2=constructFA();
	cout << "Watch them combine" << endl;
	fa1.asmTo(fa2);
}
void NFA2DFA()
{
	cout << "construct your automaton" << endl;
	FA fa=constructFA();
	cout << "Converting your automaton to a DFA" << endl;
	int i=0;
	fa.determinize(&i);
}
void disp_menu()
{
	cout << "Choose an option " << endl;
	cout << "--------------------" << endl;
	cout << "1.for simulating"    <<endl;
	cout << "2.for a Kleene Star" <<endl;
	cout << "3.for a Kleene |"    <<endl;
	cout << "4.for concatenation" <<endl;
	cout << "5.for converting to DFA" << endl;
	cout << "Enter any other key to quit" << endl;
}
int main(int argc, char* argv[])
{
	FA fa(0, false);
	fa.add_state(1, false);
	fa.add_state(2, true);
	fa.add_state(3, true);
	fa.add_transition(0,'a',1);
	fa.add_transition(1,'b',2);
	fa.add_transition(1,'c',3);
	fa.add_transition(3,'a',0);
	fa.add_transition(3,'b',0);
	//fa.conTo(fa);
	disp_menu();
	int in=0;
	while(true)
	{
		if(in>0 && in<6)
		{
			disp_menu();
		}
		cin>>in;
		switch(in)
		{
			case 1 : simulate(); break;
			case 2 : starring(); break;
			case 3 : assmble(); break;
			case 4 : concatenate(); break;
			case 5 : NFA2DFA(); break;
			default: return 0;
		}
		if(cin.eof())
			break;
	}
	return 0;
}
