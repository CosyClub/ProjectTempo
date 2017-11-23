#include <tempo/config.hpp>
#include <tempo/structures.hpp>

namespace tempo
{
	void Queue::push(T elem) {
		m.lock();
		q.push(elem);
		m.unlock();
	}
	
	void Queue::pop() {
		m.lock();
		q.pop();
		m.unlock();
	}
	T Queue::front() {
		m.lock();
		T &ref = q.front();
		m.unlock();
	}
	void Queue::pop() {
		m.lock();
		q.pop();
		m.unlock();
	}
}
