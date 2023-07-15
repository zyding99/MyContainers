#pragma once

#include "Deque.h"

template<typename T, typename Sequence = Deque<T>>
class Queue {
public:
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;
public:
	Queue() {}
	Queue(size_t sz) : c(sz) {}

	bool empty() const { return c.empty(); }
	size_t size() const { return c.size(); }

	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }

	void push(const T& data) { c.push_back(data); }
	void pop() { c.pop_front(); }
};