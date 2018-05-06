#pragma once

#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

class FA
{
public:
	explicit FA(int initial, bool isfinal);
	FA(const FA &fa);
	~FA(void);
	FA& operator=(const FA &fa);
	void add_state(int s, bool isfinal);
	void add_transition(int src, char input, int dest);
	bool is_accepting();
	void reset();
	int input(char inp);
	int state();

	FA starring(); // changes exp to (exp)*
	FA asmTo(FA); // changes exp1 to (exp1)|(exp2)
	FA conTo(FA); // changes exp1 to exp1exp2


//members
	int m_initial; //handy to keep around, if you don't follow the convention of 0
	int m_state;  //current state
	set<int> m_states; // all of our states
	set<int> m_final_states; // boolean set to check which are final might turn into a bitset
	vector<pair< pair<int,char>, int> > m_transitions; // mapping the transitions, the pair contains the state before
                                       // we transit and the object to insert, the map contains the
                                       // pair and the resulting state
};
