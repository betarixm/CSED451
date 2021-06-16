#include "Tree.h"

Node *Tree::root() const {
    return this->_root;
}

Tree::~Tree() {
    delete _root->child();
    delete _root->sibling();
    delete _root;
}
