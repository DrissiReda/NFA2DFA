#include "FA.hpp"

//Creating a finite automaton from
//a state
template<typename T>
FA<T>::FA(int initial, bool isfinal)
{
	this->m_initial = this->m_state = initial;
	add_state(initial, isfinal);
	add_state(-1, false); // invalid dead state
}

template<typename T>
FA<T>::~FA(void)
{
}
//adds a state to the automaton
template<typename T>
void FA<T>::add_state(int s, bool isfinal)
{
	m_states.insert(s);
	if(isfinal) m_final_states.insert(s);
}
//adds a transition to the automaton
template<typename T>
void FA<T>::add_transition(int src, T input, int dest)
{
	m_transitions.insert(pair<pair<int,T>, int>(pair<int,T>(src, input), dest));
}

template<typename T>
void FA<T>::reset()
{
	m_state = m_initial;
}

template<typename T>
int FA<T>::input(T inp)
{
	auto tr = make_pair(m_state, inp);
  //find if the transition already exists 
	if(m_transitions.count(tr)>0)
	{
		auto it = m_transitions.find(tr);
		return m_state = it->second;
	}
	else
	{
		return m_state = -1;
	}
}

template<typename T>
bool FA<T>::is_accepting()
{
	return m_final_states.count(m_state)!=0;
}

template<typename T>
int FA<T>::state()
{
	return m_state;
}


template class FA<char>;
