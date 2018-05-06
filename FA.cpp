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
	this->m_transitions.insert(this->m_transitions.end(),fa.m_transitions.begin(),fa.m_transitions.end());
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
	m_transitions.push_back(pair<pair<int,char>, int>(pair<int,char>(src, input), dest));
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
	for(int i=0;i<m_transitions.size();i++)
	{
		if(m_transitions[i].first==tr)
			return m_state = m_transitions[i].second;
	}
    //this transition is a fraud so we return -1
    //for the sake of violence we could raise a SIGTERM
    //or throw an exception, make them panic.
		return m_state = -1;
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
	int fstlst= *(std::max_element(m_states.begin(),m_states.end()));
	if(m_final_states.find(m_initial)== m_final_states.end())
		m_final_states.insert(m_initial);
	std::cout << fstlst << std::endl;
	//To add the looping part we must get rid of the final state in transitions
	//and overwrite it with the original state that way we ensure we can return to the start
	int last;
	for(auto i: m_states)
	{
		last=i;
	}
	std::vector<std::pair<std::pair<int,char>, int> >::iterator it=m_transitions.begin();
	for(;it!=m_transitions.end();++it)
	{
		if(it->second==last) // set the after value of the transition
			it->second=m_initial;
		if((it->first).first==last) // set the before value of the transition
		{
			//add the updated transition
			m_transitions.erase(it);
			m_transitions.push_back(pair<pair<int,char>, int>(pair<int,char>(m_initial, (it->first).second), it->second));
		}
	}
	return *this;
}
FA FA::asmTo(FA fa)
{
	//Adding the second FA's states to the first FAs states
	int fstlst= *(std::max_element(m_states.begin(),m_states.end()));
	std::set<int>::iterator it=fa.m_states.begin();
	for(;it!=fa.m_states.end();it++)
	{
		if(*(it)!=fa.m_initial)
			m_states.insert(*(it)+fstlst);
	}
	//We add accepting states of the second FA to the first, then the initial state of the first will
	//be accepting if at least one of the FA's initial states are accepting
	if (fa.m_final_states.count(fa.m_initial) && !m_final_states.count(m_initial))
		m_final_states.insert(m_initial);

	for(it=fa.m_final_states.begin();it!=fa.m_final_states.end();it++)
	{
		if(*(it)!=fa.m_initial)
			m_final_states.insert(*(it)+fstlst);
	}
	//adding all transitions of the second FA to the first FA except the transitions
	//from the second FA's initial state, these will be from the first FA's initial state instead
	std::vector<std::pair<std::pair<int,char>, int> >::iterator itr=fa.m_transitions.begin();
	for(;itr!=fa.m_transitions.end();itr++)
	{
		//if from initial state of second change to initial state of first
		if((itr->first).first==fa.m_initial)
			m_transitions.push_back(pair<pair<int,char>, int>(pair<int,char>(m_initial, (itr->first).second), itr->second));
		else
			m_transitions.push_back(*itr);
	}
	return *this;
}
	FA FA::conTo(FA fa)
	{
		//Adding the second FA's states to the first FAs states
		//Adding 1 to account for the initial state being a 0
		int fstlst= *(std::max_element(m_states.begin(),m_states.end()))+1;
		std::set<int>::iterator it=fa.m_states.begin();
		for(;it!=fa.m_states.end();it++)
		{
				m_states.insert(*(it)+fstlst);
		}
		//Get rid of the final states of the first automaton
		m_final_states.clear();
		//We add accepting states of the second FA to the first, then the initial state of the first will
		//be accepting if at least one of the FA's initial states are accepting
		for(it=fa.m_final_states.begin();it!=fa.m_final_states.end();it++)
		{
				m_final_states.insert(*(it)+fstlst);
		}
		//First FA's transitions to an accepting state should instead be to second FA's initial state
		std::vector<std::pair<std::pair<int,char>, int> >::iterator itr=m_transitions.begin();
		for(;itr!=m_transitions.end();itr++)
		{
			if(m_final_states.count(itr->second))//if it is a final state
				itr->second=fa.m_initial;
		}
		//Second FA's transitions from the initial state should also work from first FA's accepting states
		itr=fa.m_transitions.begin();
		for(;itr!=fa.m_transitions.end();itr++)
		{
			//Adding transitions that are from second FA's initial state
			if((itr->first).first==fa.m_initial)
			{
			it=m_final_states.begin();
			for(;it!=m_final_states.end();it++)
				m_transitions.push_back(pair<pair<int,char>, int>(pair<int,char>(*(it), (itr->first).second), itr->second));
			}
			//Adding other transitions
			else
				m_transitions.push_back(*itr);
		}
	}
