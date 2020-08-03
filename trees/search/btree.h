#ifndef FONTUS_BTREE_H
#define FONTUS_BTREE_H

#include <bits/stdc++.h>

template <typename T, size_t N>
struct btree_node_t;

template <typename T, size_t N>
struct btree_insert_response_t {
  bool inserted;
  btree_node_t<T, N> *first;
  btree_node_t<T, N> *second;
  T pivot;

  btree_insert_response_t() : inserted{false}, first{}, second{} {}
  operator bool() const {
    return inserted;
  }
};

template <typename T, size_t N>
struct btree_node_t {
  int height;
  int count;
  T data[N-1];
  btree_node_t<T, N> *children[N];

  btree_node_t() : height{0}, count{0}, data{}, children{} {}

  ~btree_node_t() {
    if (height == 0 || count == 0) {
      return;
    }
    std::for_each(children, children + count + 1, [](auto child) {
          delete child;
        });
  }

  btree_insert_response_t<T, N> insert_in_place(T key,
      btree_node_t<T, N> *left = nullptr,
      btree_node_t<T, N> *right = nullptr) {
    assert(count < N-1); // pre-condition: there must be room
    if (height == 0) {
      assert(!left);  // pre-condition: no child ptrs for leaf nodes
      assert(!right); // pre-condition: no child ptrs for leaf nodes
    }

    btree_insert_response_t<T, N> result;
    result.first = this;
    auto insert_at = std::lower_bound(data, data + count, key);
    if (insert_at < data + count) {
      if (*insert_at == key) {
        result.inserted = false;
        return result;
      }
      std::copy(insert_at, data + count, insert_at + 1);
    }
    *insert_at = std::move(key);
    ++count;
    if (height > 0) {
      auto index = insert_at - data;
      std::copy(children + index + 1, children + count, children + index + 2);
      children[index] = left;
      children[index + 1] = right;
    }
    result.inserted = true;
    return result;
  }

  btree_insert_response_t<T, N> split_and_insert(T key,
      btree_node_t<T, N> *left = nullptr,
      btree_node_t<T, N> *right = nullptr) {
    btree_insert_response_t<T, N> result;
    result.first = this;
    assert(count == N-1);
    if (height == 0) {
      assert(!left);  // pre-condition: no child ptrs for leaf nodes
      assert(!right); // pre-condition: no child ptrs for leaf nodes
    }

    auto size = count;
    auto insert_at = std::lower_bound(data, data + size, key);
    if (insert_at < data + size) {
      if (*insert_at == key) {
        result.inserted = false;
        return result;
      }
    }
    auto index = insert_at - data;
    auto pivot_index = N/2; // N (not N-1) because we count the new key
    btree_node_t<T, N> *new_node = new btree_node_t<T, N>();
    new_node->height = height;
    auto original_count = count;
    auto actual_pivot_index = pivot_index - (index < pivot_index?1:0);
    bool inserted = false;
    if (actual_pivot_index == index) {
      result.pivot = std::move(key);
      inserted = true;
    } else {
      result.pivot = std::move(data[actual_pivot_index]);
      ++actual_pivot_index;
      --count;
    }

    for (int i = actual_pivot_index, j = 0; i < size; ++i) {
      if (i == index) {
        if (inserted) {
          new_node->children[j] = right; // not children[i]?
          children[i] = nullptr;
        } else {
          new_node->data[j] = std::move(key);
          new_node->children[j] = left;
          new_node->children[j+1] = right;
          ++new_node->count;
          ++j;
          inserted = true;
          children[i] = nullptr;
        }
      }
      new_node->data[j] = std::move(data[i]);
      if (i != index) {
        new_node->children[j] = children[i];
        children[i] = nullptr;
      }
      ++j;
      ++new_node->count;
      --count;
    }
    if (!inserted && index == size) {
      new_node->data[new_node->count] = std::move(key);
      new_node->children[new_node->count] = left;
      new_node->children[++new_node->count] = right;
      inserted = true;
    } else {
      new_node->children[new_node->count] = children[size];
      children[size] = nullptr;
    }

    if (!inserted) {
      insert_in_place(std::move(key), left, right);
    } else if (actual_pivot_index == index) {
      children[count] = left;
    }

    result.second = new_node;
    result.inserted = true;
    return result;
  }

  btree_insert_response_t<T, N> insert(T key) {
    if (height == 0) {
      if (count < N-1) {
        return insert_in_place(std::move(key));
      }
      return split_and_insert(std::move(key));
    }

    auto it = std::lower_bound(data, data + count, key);
    if (it < data + count && *it == key) {
      // TODO: add the .first
      return btree_insert_response_t<T, N>{};
    }

    auto index = it - data;
    auto result = children[index]->insert(std::move(key));
    if (!result.second) {
      // no split happened
      return result;
    }
    if (count < N-1) {
      return insert_in_place(std::move(result.pivot), result.first,
          result.second);
    }
    return split_and_insert(std::move(result.pivot), result.first,
        result.second);
  }

  void print(int max_height) {
    for (int i = 0; i < (max_height - height); ++i) {
      std::cout << "  ";
    }
    std::cout << "h:[" << height << "] ";
    std::ostream_iterator<int> os(std::cout, ", ");
    std::copy(data, data + count, os);
    std::cout << '\n';
    for (int i = 0; i < count + 1; ++i) {
      if (!children[i]) {
        break;
      }
      children[i]->print(max_height);
    }
  }
};

template <typename T, size_t N>
class BTree {
public:
  BTree() : root{} {}

  ~BTree() {
    delete root;
  }

  void insert(T key) {
    if (!root) {
      root = new btree_node_t<T, N>();
    }
    auto result = root->insert(std::move(key));
    if (result.second) {
      auto new_root = new btree_node_t<T, N>();
      new_root->data[0] = std::move(result.pivot);
      new_root->children[0] = result.first;
      new_root->children[1] = result.second;
      new_root->count = 1;
      new_root->height = root->height + 1;
      root = new_root;
    }
  }

  void remove(T key) {
  }

  std::pair<std::weak_ptr<T>, size_t> find(const T& key) const {
    return std::make_pair(std::weak_ptr<T>(), 0u);
  }

  int height() const {
    return root->height;
  }

  void print() {
    root->print(height());
    std::cout << '\n';
  }

private:
  btree_node_t<T, N> *root;
};

#endif /* FONTUS_BTREE_H */
