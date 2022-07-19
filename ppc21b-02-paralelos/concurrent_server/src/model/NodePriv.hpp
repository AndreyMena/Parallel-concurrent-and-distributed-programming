#pragma once
#include "Node.hpp"

using namespace std;

class NodePriv
{
private:
    /* data */
    // Node* nodo;
    // int index;
    // int64_t num;
public:
    NodePriv(Node* nodo_c, int index_c, int64_t num_c);
    ~NodePriv();
    NodePriv();
    Node* nodo;
    int index;
    int64_t num;
};
