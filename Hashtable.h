#pragma once

#include <algorithm>
#include <vector>

// hard code hash table size constants
enum { num_primes = 28 };

static const unsigned long prime_list[num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long next_prime(unsigned long n)
{
	const unsigned long* first = prime_list;
	const unsigned long* last = prime_list + (int)num_primes;
	const unsigned long* pos = std::lower_bound(first, last, n);
	return pos == last ? *(last - 1) : *pos;
}


template<typename Value>
struct __hashtable_node {
	__hashtable_node* next{};
	Value val;

	__hashtable_node(const Value& v) : val(v) {}
};

template<typename Pair, typename Key, typename ExtractKey, typename HashFunc, typename EqualKey>
class hashtable;

template<typename Pair, typename Key, typename ExtractKey, typename HashFunc, typename EqualKey>
struct __hashtable_iterator {
	typedef __hashtable_node<Pair> Node;
	typedef __hashtable_iterator<Pair, Key, ExtractKey, HashFunc, EqualKey> Iterator;

	typedef std::forward_iterator_tag iterator_category;
	typedef Pair value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Pair& reference;
	typedef Pair* pointer;
	typedef hashtable<Pair, Key, ExtractKey, HashFunc, EqualKey> Hashtable;

	Node* curr;
	Hashtable* table;

	__hashtable_iterator(Node* node, Hashtable* ht) : curr(node), table(ht) {}

	reference operator*() const { return (*curr).val; }
	pointer operator->() const { return &(operator*()); }

	Iterator& operator++() {
		const Node* old = curr;
		curr = curr->next;
		if (!curr) {
			size_t bucket = table->bucket_num(old->val);
			while (!curr && ++bucket < table->buckets.size())
				curr = table->buckets[bucket];
		}
		return *this;
	}
	Iterator operator++(int) {
		Iterator* temp = this;
		++ *this;
		return temp;
	}
	bool operator==(const Iterator& it) const {
		return this->curr == it.curr;
	}
	bool operator!=(const Iterator& it) const {
		return this->curr != it.curr;
	}
};

//                Pair<Key, Data>
template<typename Pair, typename Key, typename ExtractKey, typename HashFunc, typename EqualKey>
class hashtable {
public:
	typedef Key key_type;
	typedef Pair value_type;
	typedef Pair& reference;
	typedef Pair* pointer;

	typedef HashFunc hasher;
	typedef EqualKey key_equal;
	typedef __hashtable_iterator<Pair, Key, ExtractKey, HashFunc, EqualKey> Iterator;
private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	typedef __hashtable_node<Pair> Node;

	std::vector<Node*> buckets{};
	size_t num_elements{};

	void resize(size_t new_size_hint) {
		const size_t old_size = buckets.size();
		if (new_size_hint > old_size) {
			const size_t new_size = next_prime(new_size_hint);
			if (new_size > old_size) {
				std::vector<Node*> temp(new_size, (Node*)(0));
				for (size_t bucket = 0; bucket < old_size; ++bucket) {
					Node* first = buckets[bucket];
					while (first) {
						size_t new_bucket = bucket_num(first->val, new_size);
						buckets[bucket] = first->next;
						first->next = temp[new_bucket];
						temp[new_bucket] = first;
						first = buckets[bucket];
					}
				}
				buckets.swap(temp);
			}
		}
	}

public:
	hashtable() {
		resize(next_prime(0));
	}

	size_t bucket_count() const { return buckets.size(); }

	size_t bucket_num(const key_type& k, size_t n) const {
		return hash(k) % n;
	}
	size_t bucket_num(const key_type& k) const {
		return bucket_num(k, buckets.size());
	}
	size_t bucket_num(const Pair& p, size_t n) const {
		return bucket_num(get_key(p), n);
	}
	size_t bucket_num(const Pair& p) const {
		return bucket_num(get_key(p), buckets.size());
	}

	Iterator find(const key_type& k)
	{
		size_t n = bucket_num(k);
		Node* first;
		for (first = buckets[n];
			first && !equals(get_key(first->val), k);
			first = first->next)
		{
		}
		return Iterator(first, this);
	}

	size_t count(const key_type& k) const
	{
		const size_t n = bucket_num(k);
		size_t result = 0;

		for (const Node* curr = buckets[n]; curr; curr = curr->next) {
			if (equals(get_key(curr->val), k)) ++result;
		}

		return result;
	}

	reference find_or_insert(const Pair& p){
		resize(num_elements + 1);

		const size_t n = bucket_num(p);
		Node* first = buckets[n];

		for (Node* curr = first; curr; curr = curr->next) {
			if (equals(get_key(curr->val), get_key(p))) return curr->val;
		}

		Node* temp = new Node(p);
		temp->next = first;
		buckets[n] = temp;
		++num_elements;
		return temp->val;
	}
};

