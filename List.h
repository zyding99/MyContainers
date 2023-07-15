#pragma once

#include <iterator>

template<typename T>
struct _list_node { 
	_list_node* prev = nullptr;
	_list_node* next = nullptr;
	T data{};
	_list_node() {}
	_list_node(const T& _data) : data(_data) {}
	~_list_node() {
		_list_node* prev = nullptr;
		_list_node* next = nullptr;
	}
};

template<typename T>
struct _list_iterator {
	typedef _list_iterator<T> self;
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T	value_type;
	typedef T*	pointer;
	typedef T&	reference;
	typedef std::ptrdiff_t difference_type;

	typedef _list_node<T> ListNode;

	_list_iterator(ListNode* _node) : node(_node) {}
	_list_iterator(const self& x) : node(x.node) {} // copy ctor

	ListNode* node;

	reference operator*() const { return (*node).data; }
	pointer operator->() const { return &(operator*()); }
	bool operator==(const self& rhs) {
		return this->node == rhs.node;
	}
	bool operator!=(const self& rhs) {
		return !operator==(rhs);
	}

	self& operator++() { // ++it
		node = node->next;
		return *this;
	}
	self operator++(int) { // it++
		self temp = *this;
		++ *this;
		return temp;
	}
	self& operator--() { // --it
		node = node->prev;
		return *this;
	}
	self operator--(int) { // it--
		self temp = *this;
		-- *this;
		return temp;
	}
};

template<typename T>
class List {
protected:
	typedef _list_node<T> ListNode;
public:
	typedef _list_iterator<T> Iterator;
protected:
	ListNode* node; // end of List
	size_t m_size = 0;

public:
	List() : node(new ListNode) {}
	List(const T& data) : List() {
		node->next = new ListNode(data);
		node->next->prev = node;
		this->m_size = 1;
	}
	List(const List<T>& list2) : List() {
		if(!list2.empty()){
			Iterator it2 = list2.begin();
			for (; it2 != list2.end(); ++it2) {
				T data = *it2;
				this->push_back(data);
			}
		}
	}
	~List() {
		while (!this->empty()) { remove(0); }
		delete node;
	}

	List& operator=(const List<T>& list2) {
		while (!this->empty()) { remove(0); }
		if (!list2.empty()) {
			Iterator it2 = list2.begin();
			for (; it2 != list2.end(); ++it2) {
				T data = *it2;
				this->push_back(data);
			}
		}
		return *this;
	}

	size_t size() const { return this->m_size; }
	bool empty() const { return this->m_size == 0; }

	T& front() const { return *(node->next).data; }
	T& back() const { return *(node->prev).data; }

	Iterator end() const { return Iterator(node); }
	Iterator begin() const { return empty() ? end() : Iterator(node->next); }
	Iterator rbegin() const { return empty() ? end() : --end(); }
	
	void insert(Iterator& it, const T& data) {
		ListNode* newNode = new ListNode(data);
		if (empty()) {
			this->node->next = newNode;
			this->node->prev = newNode;
			newNode->prev = this->node;
			newNode->next = this->node;
			++this->m_size;
		}
		else {
			ListNode* tmp = it.node->next;
			it.node->next = newNode;
			newNode->prev = it.node;
			tmp->prev = newNode;
			newNode->next = tmp;
			++this->m_size;
		}
	}
	void insert(size_t pos, const T& data) {
		if (pos != 0 && pos >= this->m_size) return;
		Iterator it = begin();
		for (size_t i = 0; i < pos; ++i) {
			++it;
		}
		insert(it, data);
	}

	void push_front(const T& data) { Iterator it = begin(); insert(it, data); }
	void push_back(const T& data) { Iterator it = rbegin(); insert(it, data); }
		
	void remove(Iterator& it) {
		if (empty()) return;
		it.node->prev->next = it.node->next;
		it.node->next->prev = it.node->prev;
		delete it.node;
		--this->m_size;
	}
	void remove(size_t pos) {
		if (empty()) return;
		if (pos >= this->m_size) return;

		if (this->m_size == 1) { // pos == 0
			delete node->next;
			node->next = nullptr;
			node->prev = nullptr;
			this->m_size = 0;
		}
		else {
			Iterator it = begin();
			for (size_t i = 0; i < pos; ++i) {
				++it;
			}
			remove(it);
		}
	}

	void pop_front() { Iterator it = begin(); remove(it); }
	void pop_back() { Iterator it = rbegin(); remove(it); }
};