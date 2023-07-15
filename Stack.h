#pragma once

#include "Deque.h"

template<typename T, typename Sequence = Deque<T>>
class Stack {
public:
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;
public:
	Stack() {}
	Stack(size_t sz) : c(sz) {}

	bool empty() const { return c.empty(); }
	size_t size() const { return c.size(); }

	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }

	void push(const T& data) { c.push_back(data); }
	void pop() { c.pop_back(); }
};