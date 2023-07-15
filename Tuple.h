#pragma once
#include <iostream>

template<typename... Values> class Tuple; // 通用模板声明
template<> class Tuple<> {}; // End of recursion

template<typename Head, typename... Tail>
class Tuple<Head, Tail...> {

	typedef Tuple<Tail...> TailTuple;

protected:
	Head m_head;
	TailTuple m_tail;

public:
	Tuple() {}
	Tuple(Head v, Tail... vtail)
		: m_head(v), m_tail(vtail...) {} // 初始化head,并递归调用tail的构造器

	Head head() const { return m_head; }
	TailTuple& tail() { return m_tail; }
	const TailTuple& tail() const { return m_tail; }
};

// My Tuple Print
template<typename Head, typename... Tail>
void PrintTuple(std::ostream& os, const Tuple<Head, Tail...>& tuple) {
	os << tuple.head() << (sizeof...(Tail) == 0 ? "" : ", ");
	PrintTuple(os, tuple.tail());
}

void PrintTuple(std::ostream& os, const Tuple<>& tuple) {}


// My Tuple Get
template <size_t Index, typename Head, typename... Tail>
struct TupleGet {
	static auto get(const Tuple<Head, Tail...>& tuple) {
		Tuple<Tail...> tail = tuple.tail();
		return TupleGet<Index - 1, Tail...>::get(tail);
	}
};

template <typename Head, typename... Tail>
struct TupleGet<0, Head, Tail...> {
	static auto get(const Tuple<Head, Tail...>& tuple) {
		return tuple.head();
	}
};

template <size_t Index, typename... Types>
auto get(const Tuple<Types...>& tuple) {
	return TupleGet<Index, Types...>::get(tuple);
}

template<typename... Types>
std::ostream& operator<<(std::ostream& os, const Tuple<Types...>& tuple) {
	os << "[";
	PrintTuple(os, tuple);
	os << "]";
	return os;
}
