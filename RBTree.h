#pragma once

#include <iostream>

enum Color { RED, BLACK };

template<typename Key, typename Data>
struct _Tree_Node {
    Key key;
    Data data;
    Color color;
    _Tree_Node *left, *right, *parent;
    typedef _Tree_Node<Key, Data> Node;

    // Constructor
    explicit _Tree_Node(const Key& k, const Data& v)
        : key(k), data(v), color(RED), left(nullptr), right(nullptr), parent(nullptr)
    {}
    
    // find min from current node
    Node* minValue(Node* node)
    {
        Node* current = node;

        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    // find successor
    Node* successor(Node* n)
    {
        if (n->right != nullptr)
            return minValue(n->right);

        Node* p = n->parent;
        while (p != nullptr && n == p->right)
        {
            n = p;
            p = p->parent;
        }
        return p;
    }

    // find max from current node
    Node* maxValue(Node* node)
    {
        Node* current = node;

        while (current && current->right != nullptr)
            current = current->right;

        return current;
    }

    // 
    Node* predecessor(Node* n)
    {
        if (n->left != nullptr)
            return maxValue(n->left);

        Node* p = n->parent;
        while (p != nullptr && n == p->left)
        {
            n = p;
            p = p->parent;
        }
        return p;
    }

};


template<typename Key, typename Data>
struct _Tree_Iterator {
    typedef _Tree_Node<Key, Data> Node;
    typedef _Tree_Iterator<Key, Data> self;
    typedef Node* pointer;
    typedef Node& reference;

    Node* node;

    _Tree_Iterator(Node* _node) : node(_node) {}
    _Tree_Iterator(const self& x) : node(x.node) {} // copy ctor
    bool operator==(const self& rhs) {
        return this->node == rhs.node;
    }
    bool operator!=(const self& rhs) {
        return !operator==(rhs);
    }
    reference operator*() const { return *node; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() { // ++it
        node = node->successor(node);
        return *this;
    }
    self operator++(int) { // it++
        self temp = *this;
        ++* this;
        return temp;
    }
    self& operator--() { // --it
        node = node->predecessor(node);
        return *this;
    }
    self operator--(int) { // it--
        self temp = *this;
        --* this;
        return temp;
    }
};

// Value = Pair<Key, Data>
template<typename Key, typename Data, typename Compare>
class RBTree {
public:
    typedef _Tree_Node<Key, Data> Node;
    typedef _Tree_Iterator<Key, Data> Iterator;
protected:
    Node* root{};
    Node* start{};
    Node* finish{};
    size_t node_count = 0;
    Compare key_compare;

protected:
    void updateBegin() {
        Node* temp = root;
        while (temp->left) {
            temp = temp->left;
        }
        start = temp;
    }
    void updateEnd() {
        Node* temp = root;
        while (temp->right) {
            temp = temp->right;
        }
        finish = temp;
    }

    void rotateLeft(Node*&, Node*&);
    void rotateRight(Node*&, Node*&);
    void fixInsertRBTree(Node*&, Node*&);
    //void fixDeleteRBTree(Node*&);
    void inorderBST(Node*);
    //void preorderBST(Node*&);
    int getColor(Node*& node) const { return node->color; }
    void setColor(Node*& node, Color color) { node->color = color; };
    bool insertBST(bool, Node*&, Node*&);
    //void deleteBST(Node*&, Key&);
    //int getBlackHeight(Node*);

public:
    // Constructor
    RBTree() {}
    RBTree(Node* node) : root(node) {
        root->color = BLACK;
        node_count = 1;
    }
    RBTree(const Key& k, const Data& v) : root(new Node(k, v)) {
        root->color = BLACK;
        node_count = 1;
    }

    Node* getRoot() const { return root; }
    Node* begin() const { return start; }
    Node* end() const { return finish; }
    size_t size() const { return node_count; }

    void inorder();

    Node* find(const Key& k) {
        Node* temp = root;
        while (temp) {
            if (k == temp->key) return temp;
            if (key_compare(k, temp->key)) {
                temp = temp->left;
            }
            else {
                temp = temp->right;
            }
        }
        return temp;
    }

    Node* insert_unique(const Key& k, const Data& v)
    {
        Node* node = new Node(k, v);
        if (root == nullptr) {
            root = node;
            node->color = BLACK;
            node_count = 1;
            start = finish = node;
        }
        else {
            if (insertBST(true, root, node)) {
                fixInsertRBTree(root, node);
                updateBegin();
                updateEnd();
            }
            else {
                return nullptr;
            }
        }
        return node;
    }


    Node* insert_equal(const Key& k, const Data& v)
    {
        Node* node = new Node(k, v);
        if (root == nullptr) {
            root = node;
            node->color = BLACK;
            node_count = 1;
            start = finish = node;
        }
        else {
            insertBST(false, root, node);
            fixInsertRBTree(root, node);
            updateBegin();
            updateEnd();
        }
        return node;
    }

};

template<typename Key, typename Data, typename Compare>
void RBTree<Key, Data, Compare>::rotateLeft(Node*& root, Node*& pt)
{
    Node* pt_right = pt->right;

    pt->right = pt_right->left;

    if (pt->right != nullptr)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

template<typename Key, typename Data, typename Compare>
void RBTree<Key, Data, Compare>::rotateRight(Node*& root, Node*& pt)
{
    Node* pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != nullptr)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == nullptr)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}

// This function fixes violations caused by BST insertion
template<typename Key, typename Data, typename Compare>
void RBTree<Key, Data, Compare>::fixInsertRBTree(Node*& root, Node*& pt)
{
    Node* parent_pt = nullptr;
    Node* grand_parent_pt = nullptr;

    while ((pt != root) && (getColor(pt) == RED) &&
        (getColor(pt->parent) == RED))
    {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {
            Node* uncle_pt = grand_parent_pt->right;

            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (getColor(uncle_pt) == RED)
            {
                setColor(uncle_pt, BLACK);
                setColor(parent_pt, BLACK);
                setColor(grand_parent_pt, RED);
                pt = grand_parent_pt;
            }

            else
            {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                rotateRight(root, grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        /* Case : B
           Parent of pt is right child of Grand-parent of pt */
        else
        {
            Node* uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != nullptr) && (getColor(uncle_pt) == RED))
            {
                setColor(uncle_pt, BLACK);
                setColor(parent_pt, BLACK);
                setColor(grand_parent_pt, RED);
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                rotateLeft(root, grand_parent_pt);
                std::swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    setColor(root, BLACK);
}

// Function to do inorder and level order traversals
template<typename Key, typename Data, typename Compare>
void RBTree<Key, Data, Compare>::inorder()
{
    inorderBST(getRoot());
}

template<typename Key, typename Data, typename Compare>
void RBTree<Key, Data, Compare>::inorderBST(Node* root)
{
    if (root == nullptr)
        return;

    inorderBST(root->left);
    std::cout << root->data << " ";
    inorderBST(root->right);
}

template<typename Key, typename Data, typename Compare>
bool RBTree<Key, Data, Compare>::insertBST(bool unique, Node*& root, Node*& node) {
    Node* temp = root;
    while (true) {
        if (unique && node->key == temp->key) return false;
        if (key_compare(node->key, temp->key)) {
            if (temp->left) {
                temp = temp->left;
            }
            else {
                temp->left = node;
                node->parent = temp;
                break;
            }
        }
        else {
            if (temp->right) {
                temp = temp->right;
            }
            else {
                temp->right = node;
                node->parent = temp;
                break;
            }
        }
    }
    ++node_count;
    return true;
}
