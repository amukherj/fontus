#ifndef AVL_H
#define AVL_H

#include <queue>

template <class E>
struct AvlNode {
  using ElemType = E;
  ElemType elem;
  AvlNode<E> *left, *right;
  int balance;

  AvlNode(const E& elem) : elem(elem), left{}, right{}, balance{} {}
};

template <class E>
class AvlTree {
public:
  typedef void (*Visitor)(const AvlNode<E>*);
  AvlTree() : root{} {}

  void insert(const E& elem) {
    if (!root) {
      root = new AvlNode<E>{elem};
      return;
    }

    insert(root, elem);
  }

  void remove(const E& elem) {
    if (!root) {
      return;
    }

    root = remove(root, elem);
  }

  void bfs(Visitor v) {
    if (!root) {
      return;
    }

    std::queue<AvlNode<E>*> q;
    q.push(root);

    while (!q.empty()) {
      auto next = q.front();
      q.pop();
      if (next->left) {
        q.push(next->left);
      }
      if (next->right) {
        q.push(next->right);
      }
      v(next);
    }
  }

private: 
  AvlNode<E> *root;

  void insert(AvlNode<E>* base, const E& elem) {
    if (base->elem > elem) {
      if (base->left) {
        return insert(base->left, elem);
      }
      base->left = new AvlNode<E>{elem};
    } else if (base->elem < elem) {
      if (base->right) {
        return insert(base->right, elem);
      }
      base->right = new AvlNode<E>{elem};
    }
  }

  AvlNode<E>* remove(AvlNode<E>* base, const E& elem) {
    if (!base) {
      return base;
    }

    if (base->elem == elem) {
      if (base->left == nullptr) {
        delete base;
        return base->right;
      }
      if (base->right == nullptr) {
        delete base;
        return base->left;
      }

      AvlNode<E> *smaller = base->left, *parent = base;
      while (smaller->right) {
        parent = smaller;
        smaller = smaller->right;
      }
      if (parent == base) {
        parent->left = smaller->left;
      } else {
        parent->right = smaller->left;
      }
      smaller->left = base->left;
      smaller->right = base->right;
      base->left = base->right = nullptr;
      delete base;
      return smaller;
    }

    if (base->elem > elem) {
      base->left = remove(base->left, elem);
    } else /* (base->elem < elem) */ {
      base->right = remove(base->right, elem);
    }
  }
};

#endif /* AVL_H */
