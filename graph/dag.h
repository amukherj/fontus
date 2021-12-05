#ifndef FONTUS_DAG_H
#define FONTUS_DAG_H

#include <bits/stdc++.h>
#include "common/double.h"

namespace fontus {

typedef unsigned int vertex_type;
typedef std::pair<vertex_type, vertex_type> edge_type;
typedef std::vector<std::set<vertex_type>> adjacency_list;

class DirectedAcyclicGraph {
public:
  DirectedAcyclicGraph(unsigned int vertex_count, bool weighted) :
    weighted_(weighted) {

    // Reserve and initialize storage for adjacency list.
    adj_list_.reserve(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
      adj_list_.emplace_back();
    }
  }

  // Returning a reference to this allows chaining add_edge
  // calls and creates a fluent API.
  DirectedAcyclicGraph& add_edge(vertex_type start, vertex_type end,
                                 double weight = 1.0) {
    assert(fontus::equals(weight, 1.0) || weighted_);
    assert(start < adj_list_.size() && end < adj_list_.size());
    // create or update edge
    adj_list_[start].insert(end);
    edge_weights_[std::make_pair(start, end)] = weight;
    return *this;
  }

  template <typename T, typename... U>
  void dfs(vertex_type vertex, std::set<vertex_type>& visited,
           T action, U&... args) const {
    auto result = visited.insert(vertex);
    if (!result.second) {
      return;
    }

    for (vertex_type v: adj_list_[vertex]) {
      dfs(v, visited, action, args...);
    }
    action(vertex, args...);
  }

  std::vector<vertex_type> topsort() const {
    std::set<vertex_type> visited;
    std::vector<vertex_type> result(adj_list_.size(), 0);
    int n = adj_list_.size() - 1;

    auto action = [](const vertex_type& v, int& next_index, std::vector<vertex_type>& seq) {
      seq[next_index--] = v;
    };

    for (int i = 0; i < adj_list_.size(); ++i) {
      dfs(i, visited, action, n, result);
    }
    return result;
  }

  std::vector<size_t> ss_shortest_path(vertex_type source) {
    assert(source < adj_list_.size());
    auto MAX_DIST = (2<<28);
    std::vector<size_t> dist_vec(adj_list_.size(), MAX_DIST);
    dist_vec[source] = 0;

    std::vector<vertex_type> sorted_nodes = topsort();
    size_t num_nodes = sorted_nodes.size();
    size_t start_index = num_nodes + 1;

    for (int i = 0; i < num_nodes; ++i) {
      if (sorted_nodes[i] == source) {
        start_index = i;
        break;
      }
    }

    for (int i = start_index; i < num_nodes; ++i) {
      if (dist_vec[sorted_nodes[i]] == MAX_DIST) {
        continue;
      }

      for (auto v: adj_list_[sorted_nodes[i]]) {
        if (dist_vec[sorted_nodes[i]] +
            edge_weights_[std::make_pair(sorted_nodes[i], v)] < dist_vec[v]) {
          dist_vec[v] = dist_vec[sorted_nodes[i]] +
            edge_weights_[std::make_pair(sorted_nodes[i], v)];
        }
      }
    }

    return dist_vec;
  }

private:
  adjacency_list adj_list_;
  bool weighted_;
  std::map<edge_type, double> edge_weights_;
};

} // namespace fontus

#endif /* FONTUS_DAG_H */
