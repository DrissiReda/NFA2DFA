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
	//update alphabet
	if(!m_alpbt.count(input))
		m_alpbt.insert(input);
}
int FA::duplicate_trans(int src,char input)
{
	int count=0;
	for(int i=0;i<m_transitions.size();i++)
		if(m_transitions[i].first== std::make_pair(src,input))
			count++;
	return count;
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
	return m_final_states.count(m_state);
}

//this requires no comments whatsoever

int FA::state()
{
	return m_state;
}

FA FA::starring()
{

	//To add the looping part we must get add clone all transitions from an accepting state
	//but change the source of the transition into the intial state
	std::vector<std::pair<std::pair<int,char>, int> >::iterator itr=m_transitions.begin();
	for(;itr!=m_transitions.end();++itr)
	{
		for(std::set<int>::iterator it=m_final_states.begin();it!=m_final_states.end();it++)
		{
			if((itr->first).first==*(it)) // set the before value of the transition if it was acceptant
			{
				//add a transition starting from the initial state instead of an accepting state
				add_transition(m_initial,(itr->first).second,itr->second);
			}
		}
	}
	//if the initial state is not acceptant we should
	//make it so
	if(m_final_states.find(m_initial)== m_final_states.end())
		m_final_states.insert(m_initial);
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
				add_transition(*(it), (itr->first).second, itr->second);
			}
			//Adding other transitions
			else
				m_transitions.push_back(*itr);
		}
	}


	FA FA::determinize(int *state)
	{
		std::list<Det_Table> dt;
		std::list<Det_Table>::iterator curr_col,tmp_col;
		std::vector<std::pair<std::pair<int,char>, int> > trans;
		dt.emplace_back(Det_Table());
		(dt.begin())->state=(*state)++;

		(dt.begin())->table.resize(m_alpbt.size());
		(dt.begin())->table[0].push_back(m_initial); // 0.0 should have the initial state

		curr_col=dt.begin();

		int transcount=0;
		do
		{
			//for every element of the alphabet
			int index=0;
			for(std::set<char>::iterator input=m_alpbt.begin(); input!=m_alpbt.end();input++)
			{
				transcount=0;
				//for every state in the current column
				for(int src=0;src<curr_col->table[0].size();src++)
				{
					//how many go from state [0][src] using the character input
					transcount+= duplicate_trans(src,*input);
				}
				if(!transcount)
				{
					//inserting -1 into table[input][0] because we found no transitions
					curr_col->table[index].push_back(-1);
					//the rest is only if we found transitions
					continue;
				}
				std::vector<int> tmp(transcount),tmp2;
				int cnt=0;
				//again for every state in the current column
				for(int src=0;src<curr_col->table[0].size();src++)
				{
						for(int j=0;j<m_transitions.size();j++)
						{
							//if found we should add it
							if(m_transitions[j].first == std::make_pair(curr_col->table[0][src],*input))
								tmp[cnt++]=m_transitions[j].second;
						}
				}
				//tmp2=remove_duplicates();
				//this duplicate free set is what we're gonna use for our final version
				for(int j=0;j<curr_col->table[index].size();j++)
				{
					curr_col->table[index][j]=tmp2[j];
				}
				++index;
			}

			index=0;
			for(std::set<char>::iterator input=m_alpbt.begin(); input!=m_alpbt.end();input++)
			{
				//checking if it's in the column
				if(!is_in_col(curr_col->table[index],dt))
					//checking if it's in the current column
					if(!is_in_curr_col(curr_col->table[index],index,curr_col))
					{
						//new column should be added if its header is legal
						if(curr_col->table[index][0]!= -1)
							add_col(dt,curr_col->table[index],state);
					}
				++index;
			}
			//if no more columns are added we can break
		} while (curr_col == dt.end());
		build_dfa(dt);
		//change the table into a finite automata
		return *this;
	}
	bool FA::is_in_col(std::vector<int> tab,std::list<Det_Table> dt)
	{
			if (dt.size()<1)
				return true;
			std::list<Det_Table>::iterator it=dt.begin();
			for(;it!=dt.end();it++)
			{
				if(tab.size() != it->table[0].size())
					continue;
				for(int i=0;i<tab.size();i++)
				{
					if(it->table[0][i] != tab[i])
						continue;
					if(i == tab.size()-1)
						return true;
				}
			}
			return false;
	}
	bool FA::is_in_curr_col(std::vector<int> tab, int index, std::list<Det_Table>::iterator curr_col)
	{
		if(curr_col->table.size()<1)
			return true;
		for(int i=0;i<index;i++)
		{
			if(curr_col->table[i].size() != tab.size())
				continue;
			for(int j=0;j<tab.size();j++)
			{
				if(tab[j] != curr_col->table[i][j])
					break;
				if(j == tab.size()-1)
					return 1;
			}
		}
		return 0;
	}
	void FA::add_col(std::list<Det_Table> dt, std::vector<int> tab,int* st)
	{
		//inserting new column
		dt.emplace_back(Det_Table());
		std::list<Det_Table>::iterator it=dt.end();it--;
		it->state=(*st)++;
		//making sure we have enough room
		it->table.resize(m_alpbt.size());

		for(int i=0;i<tab.size();i++)
			it->table[0][i]=tab[i];
		return;
	}
	FA FA::build_dfa(std::list<Det_Table> dt)
	{
		FA *ret;
		std::list<Det_Table>::iterator it=dt.begin();
		//the initial state is the state of the first element
		m_initial=it->state;
		//copy the alphabet, it didn't change
		ret->m_alpbt.insert(m_alpbt.begin(),m_alpbt.end());
		//copying states
		for(;it!=dt.end();it++)
		{
			ret->m_states.insert(it->state);
		}
		//finding new accepting states
		for(it=dt.begin();it!=dt.end();it++)
		{
			for(int i=0;i<it->table[0].size();i++)
			{
				if(m_final_states.count(it->table[0][i]))
					{ret->m_final_states.insert(it->state);break;}
			}
		}
		// mapping transitions
		for(it=dt.begin();it!=dt.end();it++) // each column
		{
			int index=0;
			for(std::set<char>::iterator input=m_alpbt.begin(); input!=m_alpbt.end();input++)
			{
				//We will add the transition if it's valid (invalid is -1)
				if(it->table[index][0]!=-1)
				{
					//find destination
					int dest;
					for(std::list<Det_Table>::iterator itr=dt.begin();itr!=dt.end();itr++)
						for(int i=0;i<it->table[index].size();i++)
						{
							if(itr->table[0][i]!=it->table[index][i])
								break;
							if(i==it->table[index].size())
								dest=itr->state;
						}
					ret->add_transition(it->state,*input,dest);
				}
			}
		}
		return *ret;
	}
