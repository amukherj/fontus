#ifndef AVL_H
#define AVL_H

#include <queue>
#include <tuple>

template <class E>
struct AvlNode {
  using ElemType = E;
  ElemType elem;
  AvlNode<E> *left, *right;
  int balance;

  AvlNode(const E& elem) : elem(elem), left{}, right{}, balance{0} {}
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

    int height_change = 0;
    std::tie(root, height_change) = remove(root, elem);
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

  int insert(AvlNode<E>* base, const E& elem) {
    if (base->elem > elem) {
      if (base->left) {
        auto old_bal = base->balance;
        auto height_change = insert(base->left, elem);

        if (base->left->balance > 1 || base->left->balance < -1) {
          base->left = rotate(base->left);
        } else {
          /* adjust balance */
          base->balance += height_change;
        }
        return (base->balance - old_bal);
      }
      base->left = new AvlNode<E>{elem};

      /* adjust balance */
      auto old_bal = base->balance;
      base->balance += 1;
      return (base->balance - old_bal);
    } else if (base->elem < elem) {
      if (base->right) {
        auto old_bal = base->balance;
        auto height_change = insert(base->right, elem);

        if (base->right->balance > 1 || base->right->balance < -1) {
          base->right = rotate(base->right);
        } else {
          /* adjust balance */
          base->balance -= height_change;
        }
        return (old_bal - base->balance);
      }
      base->right = new AvlNode<E>{elem};

      /* adjust balance */
      auto old_bal = base->balance;
      base->balance -= 1;
      return (old_bal - base->balance);
    }

    return 0;
  }

  std::tuple<AvlNode<E>*, int> remove(AvlNode<E>* base, const E& elem) {
    if (!base) {
      return std::make_tuple(base, 0);
    }

    if (base->elem == elem) {
      if (base->left == nullptr) {
        auto new_base = base->right;
        delete base;
        return std::make_tuple(new_base, 1);
      }
      if (base->right == nullptr) {
        auto new_base = base->left;
        delete base;
        return std::make_tuple(new_base, 1);
      }

      AvlNode<E>* new_base = nullptr;
      auto result = pluck_largest(base->left, new_base);
      auto *new_left = std::get<0>(result);
      auto height_change = std::get<1>(result);

      new_base->balance = base->balance - height_change;
      new_base->left = new_left;
      new_base->right = base->right;
      delete base;
      return std::make_tuple(new_base,
        height_change > 0 && new_base->balance == 0 ? 1 : 0);
    }

    int height_change = 0;
    if (base->elem > elem) {
      std::tie(base->left, height_change) = remove(base->left, elem);
      base->balance -= height_change;
    } else /* (base->elem < elem) */ {
      std::tie(base->right, height_change) = remove(base->right, elem);
      base->balance += height_change;
    }

    return std::make_tuple(base, height_change > 0 && base->balance == 0 ? 1 : 0);
  }

private:
  AvlNode<E>* rotate(AvlNode<E>* base) {
    if (base->balance <= 1 && base->balance >= -1) {
      return base;
    }

    if (base->balance > 1) {
      auto new_base = base->left;
      if (new_base->balance > 0) {
        base->left = new_base->right;
        new_base->right = base;
        base->balance -= 1;
      } else if (new_base->balance < 0) {
        new_base = new_base->right;
        base->left->right = new_base->left;
        new_base->left = base->left;
        base->left = new_base->right;
        new_base->right = base;
      }
      return new_base;
    }

    /* base->balance < -1 */
    auto new_base = base->right;
    if (new_base->balance < 0) {
      base->right = new_base->left;
      new_base->left = base;
      base->balance += 1;
    } else if (new_base->balance > 0) {
      new_base = new_base->left;
      base->right->left = new_base->right;
      new_base->right = base->right;
      base->right = new_base->left;
      new_base->left = base;
    }
    return new_base;
  }

  std::tuple<AvlNode<E>*, int> pluck_largest(AvlNode<E>* base, AvlNode<E>*& plucked) {
    if (base->right) {
      auto result = pluck_largest(base->right, plucked);
      auto *sub_tree = std::get<0>(result);
      auto height_change = std::get<1>(result);

      base->right = sub_tree;
      base->balance += height_change;
      height_change = (height_change > 0 && base->balance == 0) ? 1 : 0;
      return std::make_tuple(base, height_change);
    }

    plucked = base;
    return std::make_tuple(base->left, 1);
  }
};

#endif /* AVL_H */
