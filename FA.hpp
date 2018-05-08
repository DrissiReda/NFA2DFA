#pragma once

#include <vector>
#include <iostream>
#include <list>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
class Det_Table
{
	public:
		Det_Table()
		{}
		Det_Table(const Det_Table &dt)
		{
			this->state=dt.state;
			this->table=dt.table;
		}

		int state;
		vector<vector<int> > table;
};
class FA
{
public:
	explicit FA(int initial, bool isfinal);
	FA(const FA &fa);
	~FA(void);
	FA& operator=(const FA &fa);
	void add_state(int s, bool isfinal);
	void add_transition(int src, char input, int dest);
	int  duplicate_trans(int src,char input);

	bool is_accepting();
	void reset();
	int input(char inp);
	int state();

	//determinizing
	bool is_in_col(std::vector<int> tab,std::list<Det_Table> dt);
	bool is_in_curr_col(std::vector<int> tab, int index, std::list<Det_Table>::iterator curr_col);
	void add_col(std::list<Det_Table> dt, std::vector<int> tab,int* st);
	FA build_dfa(std::list<Det_Table> dt);

	FA starring(); // changes exp to (exp)*
	FA asmTo(FA); // changes exp1 to (exp1)|(exp2)
	FA conTo(FA); // changes exp1 to exp1exp2
	FA determinize(int*);


//members
	int m_initial; //handy to keep around, if you don't follow the convention of 0
	int m_state;  //current state
	set<int> m_states; // all of our states
	set<char> m_alpbt; //lists the alphabet
	set<int> m_final_states; // boolean set to check which are final might turn into a bitset
	vector<pair< pair<int,char>, int> > m_transitions; // mapping the transitions, the pair contains the state before
                                       // we transit and the object to insert, the map contains the
                                       // pair and the resulting state
};
