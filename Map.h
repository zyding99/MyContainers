#pragma once

#include "RBTree.h"

template<typename Key, typename Data, typename Compare>
class Map {
public:
	typedef Key key_type;
	typedef Data data_type;
	typedef Compare key_compare;
protected:
	typedef RBTree<const Key, Data, Compare> rep_type;
	rep_type t;
public:
	typedef typename rep_type::Node Node;
	typedef typename rep_type::Iterator Iterator;

	Map() {}
	
	data_type& operator[](const key_type& k) {
		Node* node = t.find(k);
		if (!node) {
			node = t.insert_unique(k, data_type{});
		}
		return node->data;
	}
};