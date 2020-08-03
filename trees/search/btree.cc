#include <bits/stdc++.h>
#include "btree.h"

int main() {
  BTree<int, 4> tree;
  tree.insert(10);
  tree.print();
  tree.insert(2);
  tree.print();
  tree.insert(7);
  tree.print();
  tree.insert(4);
  tree.print();
  tree.insert(1);
  tree.print();
  tree.insert(3);
  tree.print();
  tree.insert(14);
  tree.print();
  tree.insert(5);
  tree.print();
  tree.insert(11);
  tree.print();
  tree.insert(19);
  tree.print();
  tree.insert(15);
  tree.print();
  tree.insert(8);
  tree.print();
  tree.insert(12);
  tree.print();
  tree.insert(21);
  tree.print();
  tree.insert(25);
  tree.print();
  // -----
  tree.insert(6);
  tree.print();
}
