#include "FA.hpp"

//Creating a finite automaton from
//a state

FA::FA(int initial, bool isfinal)
{
	this->m_initial = this->m_state = initial;
	add_state(initial, isfinal);
	add_state(-1, false); // invalid dead state
}

FA::FA(const FA &fa)
{
	this->m_initial= this->m_state=fa.m_initial;
	this->m_states.insert(fa.m_states.begin(),fa.m_states.end());
	this->m_final_states.insert(fa.m_final_states.begin(),fa.m_final_states.end());
	this->m_transitions.insert(fa.m_transitions.begin(),fa.m_transitions.end());
}
FA& FA::operator=(const FA &fa)
{
	this->m_initial= this->m_state=fa.m_initial;
	this->m_states.insert(fa.m_states.begin(),fa.m_states.end());
}
FA::~FA(void)
{
}
//adds a state to the automaton

void FA::add_state(int s, bool isfinal)
{
	m_states.insert(s);
	if(isfinal) m_final_states.insert(s);
}
//adds a transition to the automaton

void FA::add_transition(int src, char input, int dest)
{
	m_transitions.insert(pair<pair<int,char>, int>(pair<int,char>(src, input), dest));
}
//useful if you wanna loop

void FA::reset()
{
	m_state = m_initial;
}


int FA::input(char inp)
{
	auto tr = make_pair(m_state, inp);
  //find if the transition exists or not
	if(m_transitions.count(tr)>0)
	{
		auto it = m_transitions.find(tr);
		return m_state = it->second;
	}
	else
	{
    //this transition is a fraud so we return -1
    //for the sake of violence we could raise a SIGTERM
    //or throw an exception, make them panic.
		return m_state = -1;
	}
}

//checking the boolean value of acceptance

bool FA::is_accepting()
{
	return m_final_states.count(m_state)!=0;
}

//this requires no comments whatsoever

int FA::state()
{
	return m_state;
}

FA FA::starring()
{
	//if the initial state is not acceptant we should
	//make it so
	//The final state
	if(m_final_states.find(m_initial)== m_final_states.end())
		m_final_states.insert(m_initial);
	//To add the looping part we must get rid of the final state in transitions
	//and overwrite it with the original state that way we ensure we can return to the start
	int last;
	for(auto i: m_states)
	{
		last=i;
	}
	std::map<std::pair<int,char>, int>::iterator it=m_transitions.begin();
	for(;it!=m_transitions.end();++it)
	{
		if(it->second==last) // set the after value of the transition
			it->second=m_initial;
		if((it->first).first==last) // set the before value of the transition
		{
			//add the updated transition
			m_transitions.erase(it);
			m_transitions.insert(pair<pair<int,char>, int>(pair<int,char>(m_initial,(it->first).second ), it->second));
		}
	}
	return *this;
}
