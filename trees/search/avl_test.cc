#include "avl.h"
#include <iostream>

int main() {
  AvlTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(6);
  tree.insert(9);
  tree.insert(12);
  tree.insert(17);
  tree.bfs([](const AvlNode<int>* p){ std::cout << p->elem << '\n'; });
  tree.remove(5);
  tree.bfs([](const AvlNode<int>* p){ std::cout << p->elem << '\n'; });
}
