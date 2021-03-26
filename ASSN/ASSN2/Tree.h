#ifndef CSED451_TREE_H
#define CSED451_TREE_H

#include "Node.h"

class Tree {
private:
    Node* _root = new GroupNode();
protected:
public:
    Tree() = default;

    Node* root() const;

    ~Tree();
};


#endif //CSED451_TREE_H
