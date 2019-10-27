#include "avl.h"
#include <iostream>

int main() {
  AvlTree<int> tree;
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(7);
  tree.insert(6);
  // tree.insert(9);
  tree.insert(12);
  tree.insert(17);
  tree.bfs([](const AvlNode<int>* p){ std::cout << p->elem << '(' << p->balance << ')' << '\n'; });
  std::cout << "------\n";
  tree.remove(10);
  tree.remove(5);
  tree.remove(17);
  tree.remove(6);
  tree.bfs([](const AvlNode<int>* p){ std::cout << p->elem << '(' << p->balance << ')' << '\n'; });
}
