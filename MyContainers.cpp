// MyContainers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


#include "Tuple.h"
#include <string>
#include <complex>
void MyTupleTest() {
    Tuple<int, float, std::string> t{ 0, 1.0f, "Hello" };

    std::cout << t.head() << std::endl;
    std::cout << t.tail().head() << std::endl;
    std::cout << t.tail().tail().head() << std::endl;

    std::cout << t << std::endl;
    std::cout << get<2>(t) << std::endl;

    std::cout << "-------------------------------" << std::endl;

    Tuple<int, float, std::complex<double>, std::string>
        tuple{ 0, 1.0f, std::complex<double>(1, 1), "Hello" };
    std::cout << get<2>(tuple) << std::endl;
}

#include "List.h"
void MyListTest() {
    using std::cout;
    using std::endl;

    List<int> list;
    List<int> list2(1);
    cout << "ctor ok" << endl;
    for (int i = 0; i < 100; ++i) {
        list.push_back(i);
        cout << ".";
    }
    cout << endl << list.size() << endl;

    for (int i = 0; i < 50; ++i) {
        list.pop_front();
        cout << ":";
    }
    cout << endl << list.size() << endl;

    auto it = list.begin();
    for (int i = 0; i < 20; ++i) {
        it++;
    }
    cout << *it << endl;

    cout << "copy start" << endl;
    list2 = list;
    cout << list2.size() << endl;

    auto it2 = list2.begin();
    for (int i = 0; i < 20; ++i) {
        cout << *it2 << ",";
        it2++;
    }
}

#include "Vector.h"
void MyVectorTest() {
    using std::cout;
    using std::endl;

    Vector<int> v0;
    cout << v0.size() << endl;
    cout << v0.empty() << endl;

    Vector<int> v(100, 3);
    cout << v.size() << endl;
    
    for (int i = 0; i < 50; ++i) {
        v.pop_back();
    }
    cout << v.size() << endl;

    v.insert(20, -1);
    cout << v[20] << endl;

    v.reserve(100);
    cout << v.capacity() << endl;
}

#include "Deque.h"
void MyDequeTest() {
    using std::cout;
    using std::endl;

    Deque<int, 50> deque;
    cout << "ctor" << endl;

    for (int i = 0; i < 100; ++i) {
        deque.push_back(i);
        deque.push_front(i);
    }
    cout << deque.size() << endl;
    for (int i = 0; i < 50;++i) {
        deque.pop_back();
        deque.pop_front();
    }
    cout << deque.size() << endl;
}

#include "RBTree.h"
void MyRBTreeTest() {
    typedef RBTree<int, int, std::less<int>> T;
    T tree;
    for (int i = 0; i < 10; ++i) {
        tree.insert_unique(10, i);
    }
    for (int i = 0; i < 10; ++i) {
        tree.insert_equal(10, i);
    }
    std::cout << tree.size() << std::endl;
    auto it = T::Iterator(tree.begin());
    for (int i = 0; i < tree.size() / 2; ++i) {
        it++;
    }
    tree.inorder();
    std::cout << std::endl;
    std::cout << (*it).data << std::endl;
}

#include "Map.h"
#include <string>
void MyMapTest() {
    typedef Map<int, std::string, std::less<int>> M;
    M map;
    map[1] = "hello";
    map[3] = "world";
    std::cout << map[1] << " " << map[3] << std::endl;
}

#include "Unordered_Map.h"
#include <utility>
void MyHashMapTest() {
    // hash table test
    {
        using Pair = std::pair<int, std::string>;
        hashtable<Pair, int, Select1st<Pair>, std::hash<int>, Equal<int>> ht;
        Pair p = ht.find_or_insert(Pair{ 1, "hello" });
        std::cout << p.first << " " << p.second << std::endl;
    }

    Unordered_Map<int, std::string> umap;
    umap[1] = "hello";
    umap[3] = "world";
    std::cout << umap[1] << " " << umap[3] << std::endl;
}


int main()
{
    //... Put your use case here
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
