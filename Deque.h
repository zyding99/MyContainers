#pragma once

#include <iterator>

template<typename T, size_t buffer_size = 512 / sizeof(T)>
struct _deque_iterator {
	typedef std::random_access_iterator_tag iterator_category;
	typedef T  value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef std::ptrdiff_t difference_type;
	typedef T** map_pointer;
	typedef _deque_iterator<T, buffer_size> self;

	pointer curr{};
	pointer first{};
	pointer last{};
	map_pointer node{};

	_deque_iterator() {}
	_deque_iterator(map_pointer _node) {
		node = _node;
		first = &(node[0][0]);
		curr = first;
		last = first + buffer_size;
	}
	~_deque_iterator() {}

	reference operator*() const { return *curr; }
	pointer operator->() const { return &(operator*()); }

	difference_type operator-(const self& rhs) const {
		difference_type diff = difference_type(buffer_size) * (node - rhs.node - 1) + (curr - first) + (rhs.last - rhs.curr);
		return diff;
	}

	bool operator==(const self& rhs) const {
		if (node != rhs.node) return false;
		if (first != rhs.first) return false;
		if (last != rhs.last) return false;
		if (curr != rhs.curr) return false;
		return true;
	}
	bool operator!=(const self& rhs) const { return operator==(rhs); }

	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size);
	}

	self& operator++() { // ++it
		++curr;
		if (curr == last) {
			set_node(node + 1);
			curr = first;
		}
		return *this;
	}
	self operator++(int) { // it++
		self temp = *this;
		++ *this;
		return temp;
	}
	self& operator--() { // --it
		if (curr == first) {
			set_node(node - 1);
			curr = last;
		}
		--curr;
		return *this;
	}
	self operator--(int) { // it--
		self temp = *this;
		-- *this;
		return temp;
	}

	self& operator+=(difference_type n) {
		difference_type offset = n + (curr - first);
		if (offset >= 0 && offset < difference_type(buffer_size)) {
			curr += n;
		}
		else {
			difference_type node_offset = offset > 0 ?
				offset / difference_type(buffer_size) : -difference_type((-offset - 1) / buffer_size) - 1;

			set_node(node + node_offset);
			curr = first + (offset - node_offset * difference_type(buffer_size));
		}
		return *this;
	}
	self operator+(difference_type n) const {
		self temp = *this;
		return temp += n;
	}
	self& operator-=(difference_type n) {
		return *this += -n;
	}
	self operator-(difference_type n) const {
		self temp = *this;
		return temp -= n;
	}
	reference operator[](difference_type n) const { return *(*this + n); }
};

template<typename T, size_t buffer_size = 512 / sizeof(T)>
class Deque {
public:
	typedef _deque_iterator<T, buffer_size> Iterator;
	typedef T* pointer;
	typedef T& reference;
	typedef const T& const_reference;
protected:
	typedef T** map_pointer;
protected:
	Iterator start;
	Iterator finish;
	map_pointer map;
	size_t map_size;
public:
	Deque() : map_size(1) {
		map = new pointer[1];
		map[0] = new T[buffer_size];
		start = Iterator(map);
		finish = Iterator(map);
	}
	Deque(const size_t& sz) {
		map_size = (sz - 1) / buffer_size + 2;
		map = new pointer[map_size];
		for (size_t i = 0; i < map_size; ++i) {
			map[i] = new T[buffer_size];
		}
		start = Iterator(map);
		finish = start + sz;
	}
	~Deque() {
		for (size_t i = 0; i < map_size; ++i) {
			delete[] map[i];
		}
		delete[] map;
	}

	map_pointer getMap() { return map; }

	Iterator begin() const { return start; }
	Iterator end() const { return finish; }
	size_t size() const { return finish - start; }
	bool empty() const { return finish == start; }

	reference front() { return *start; }
	const_reference front() const { return *start; }
	reference back() { return *(finish - 1); }
	const_reference back() const { return *(finish - 1); }
	reference operator[](size_t n) { return start[n]; }
	const_reference operator[](size_t n) const { return start[n]; }
protected:
	void map_expand(int flag) {
		size_t old_size = 0, left_space = 0, right_space = 0;
		if (flag == 0) { // expand to left
			for (size_t i = 0; i < map_size; ++i) {
				if (finish.node == &map[i]) {
					old_size = i + 1;
					right_space = map_size - old_size;
					break;
				}
			}
		}
		else { // expand to right
			for (size_t i = 0; i < map_size; ++i) {
				if (start.node == &map[i]) {
					old_size = map_size - i;
					left_space = i;
					break;
				}
			}
		}
		
		size_t new_size = old_size < 2 ? 4 : 2 * old_size;
		if (new_size < map_size) new_size = map_size + left_space + right_space;
		map_pointer new_map = new pointer[new_size];
		size_t new_start = new_size / 4 > left_space ? new_size / 4 - left_space : 0;

		for (size_t i = 0; i < map_size; ++i) {
			new_map[i + new_start] = map[i];
		}
		for (size_t i = 0; i < new_start; ++i) {
			new_map[i] = new T[buffer_size];
		}
		for (size_t i = new_start + map_size; i < new_size; ++i) {
			new_map[i] = new T[buffer_size];
		}
		delete[] map;
		map = new_map;
		start.node = &map[new_start];
		finish.node = &map[new_start + old_size - 1];
		map_size = new_size;
	}

	void map_expand() {
		size_t new_size = map_size < 2 ? 4 : map_size * 2;
		map_pointer new_map = new pointer[new_size];
		size_t new_start = new_size / 4;

		size_t finish_offset = 0;
		for (size_t i = 0; i < map_size; ++i) {
			if (finish.node == &map[i]) {
				finish_offset = i;
				break;
			}
		}

		for (size_t i = 0; i < map_size; ++i) {
			new_map[i + new_start] = map[i];
		}
		for (size_t i = 0; i < new_start; ++i) {
			new_map[i] = new T[buffer_size];
		}
		for (size_t i = new_start + map_size; i < new_size; ++i) {
			new_map[i] = new T[buffer_size];
		}
		delete[] map;
		map = new_map;
		start.node = &map[new_start];
		finish.node = &map[new_start + finish_offset];
		map_size = new_size;
	}

public:
	void push_front(const T& data) {
		if (empty()) { // start from mid of map
			size_t new_start = map_size / 2;
			start = Iterator(&map[new_start]);
			finish = Iterator(&map[new_start]);
			*start = data;
			finish++;
			return;
		}
		if (start.node == map && start.curr == start.first) {
			map_expand();
		}
		--start;
		*start = data;
	}

	void push_back(const T& data) {
		if (empty()) { // start from mid of map
			size_t new_start = map_size / 2;
			start = Iterator(&map[new_start]);
			finish = Iterator(&map[new_start]);
			*start = data;
			finish++;
			return;
		}
		if (finish.node == &map[map_size - 1] && finish.curr == finish.last - 1) {
			map_expand();
		}
		*finish = data;
		finish++;
	}

	void pop_front() {
		if (empty()) return;
		*start = T{};
		++start;
	}

	void pop_back() {
		if (empty()) return;
		--finish;
		*finish = T{};
	}
};
