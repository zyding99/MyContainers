#pragma once

#include<cstring>

template<typename T>
class Vector {
public:
	typedef T* Iterator;
	typedef T& Ref;
protected:
	Iterator start = nullptr;
	Iterator finish = nullptr;
	Iterator end_of_storage = nullptr;
	bool initialized = false;
public:
	Vector() {}
	Vector(size_t sz)
		: start(new T[sz]), finish(start + sz), end_of_storage(finish), initialized(true) {}

	Vector(size_t sz, const T& v) : Vector(sz) {
		Iterator it = begin();
		for (; it != end(); ++it) {
			*it = v;
		}
	}

	Vector(const Vector<T>& v) {
		if (initialized) {
			delete[] start;
		}
		if (v.size() != 0) {
			size_t sz = v.size();
			size_t cap = v.capacity();
			start = new T[sz];
			finish = start + sz;
			end_of_storage = start + cap;
			memcpy(start, v.begin(), v.size() * sizeof(T));
		}
	}

	~Vector() {
		delete[] start;
		// end_of_storage = finish = nullptr;
	}

	Iterator begin() const { return start; } // trivial if empty Vector
	Iterator end() const { return finish; } // trivial if empty Vector
	size_t size() const { return initialized ? (end() - begin()) : 0; }
	size_t capacity() const { return initialized ? (end_of_storage - begin()) : 0; }
	bool empty() const { return !initialized || begin() == end(); }
	Ref operator[](size_t pos) { return *(begin() + pos); }
	Ref front() const { return *begin(); } // trivial if empty Vector
	Ref back() const { return *(end() - 1); } // trivial if empty Vector
protected:
	void expand() {
		size_t old_size = size();
		size_t new_size = old_size == 0 ? 2 * old_size : 1;
		Iterator new_start = new T[new_size];
		memcpy(new_start, start, old_size * sizeof(T));
		delete[] start;
		start = new_start;
		finish = new_start + old_size;
		end_of_storage = new_start + new_size;
	}
public:
	void push_back(const T& data) {
		if (end_of_storage == end()) expand();
		*(finish++) = data;
	}

	void pop_back() {
		*(--finish) = T{};
	}

	void insert(size_t pos, const T& data) {
		if (end_of_storage == end()) expand();
		for (size_t i = size(); i > pos; --i) {
			operator[](i) = operator[](i - 1);
		}
		operator[](pos) = data;
	}

	void remove(size_t pos, const T& data) {
		for (size_t i = pos; i < size(); ++i) {
			operator[](i) = operator[](i + 1);
		}
		*(--finish) = T{};
	}

	void reserve(size_t sz) {
		if (sz < size()) return;
		size_t old_size = size();
		Iterator new_start = new T[sz];
		memcpy(new_start, start, old_size * sizeof(T));
		delete[] start;
		start = new_start;
		finish = new_start + old_size;
		end_of_storage = new_start + sz;
	}

	// void resize(size_t sz) {}
};
