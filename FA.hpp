#pragma once

#include <vector>
#include <set>
#include <map>

using namespace std;

template<typename T>
class FA
{
public:
	explicit FA(int initial, bool isfinal);
	~FA(void);
	void add_state(int s, bool isfinal);
	void add_transition(int src, T input, int dest);
	bool is_accepting();
	void reset();
	int input(T inp);
	int state();


private:
	int m_initial; //handy to keep around, if you don't follow the convention of 0
	int m_state;  //current state
	set<int> m_states; // all of our states
	set<int> m_final_states; // boolean set to check which are final might turn into a bitset
	map<pair<int,T>, int> m_transitions; // mapping the transitions, the pair contains the state before
                                       // we transit and the object to insert, the map contains the
                                       // pair and the resulting state 
};
