#include <tempo/structures.hpp>

namespace tempo
{

template<class T>
Queue<T>::Queue() {
	q = new std::queue<T>;
	m = new std::mutex;
}

template<class T>
Queue<T>::~Queue() {
	delete q;
	delete m;
}

template<class T>
void Queue<T>::push(T elem) {
	m->lock();
	q->push(elem);
	m->unlock();
}

template<class T>
void Queue<T>::pop() {
	m->lock();
	q->pop();
	m->unlock();
}

template<class T>
T Queue<T>::front() {
	m->lock();
	T &ref = q->front();
	m->unlock();
	return ref;
}

template<class T>
bool Queue<T>::empty() {
	m->lock();
	bool res = q->empty();
	m->unlock();
	return res;
}

template class Queue<int>;
template class Queue<sf::Packet>;

}
