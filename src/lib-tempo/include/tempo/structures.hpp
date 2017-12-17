////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file Application.hpp
/// \author Raef Coles
/// \date 2017/11/20
/// \Useful datastructures
//////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_STRUCTURE_HPP
#define TEMPO_STRUCTURE_HPP

#include <mutex>
#include <queue>

#include <SFML/Network.hpp>

namespace tempo
{

template<class T>
class Queue {

	std::queue<T>* q;
	std::mutex m;
	
	public:
	
	Queue();
	~Queue();
	
	void push(T elem);
	void pop();
	T front();
	bool empty();
};

}

#endif
