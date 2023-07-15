#pragma once

#include <utility>
#include "Hashtable.h"

template<typename T>
struct Equal {
	bool operator()(const T& a, const T& b) const { return a == b; }
};

template<typename Pair>
struct Select1st {
	typedef typename Pair::first_type first_type;
	const first_type& operator()(const Pair& p) const { return p.first; }
};


template<typename Key, typename Data, typename HashFunc = std::hash<Key>>
class Unordered_Map {
public:
	typedef Key key_type;
	typedef Data data_type;
	typedef std::pair<Key, Data> Pair;
	typedef Pair value_type;
	typedef Pair& reference;
	typedef Pair* pointer;

	typedef Select1st<value_type> ExtractKey;
	typedef Equal<key_type> EqualKey;
	typedef __hashtable_iterator<value_type, key_type, ExtractKey, HashFunc, EqualKey> Iterator;
private:
	typedef hashtable<value_type, key_type, ExtractKey, HashFunc, EqualKey> Hashtable;
	Hashtable ht{};

public:
	Unordered_Map() {}

	data_type& operator[](const key_type& k) {
		Iterator it = ht.find(k);
		if (it.curr) return (*it).second;
		return ht.find_or_insert(Pair{ k, data_type{} }).second;
	}
};