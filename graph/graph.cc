#include <bits/stdc++.h>
using namespace std;

class UndirectedGraph;

// Directed graph algorithms
class DirectedGraph {
public:
  friend class UndirectedGraph;

  DirectedGraph(int size) :
    vertices(size),
    adj_list(max(vertices, 0), set<int>()),
    edges(0) {} 

  bool add_edge(int u, int v) {
    if (u >= vertices || v >= vertices) {
      throw runtime_error("vertex not in graph");
    }

    auto result = adj_list[u].insert(v);
    if (result.second) {
      ++edges;
    }
    return result.second;
  }

  bool remove_edge(int u, int v) {
    return adj_list[u].erase(v) > 0;
  }

  int vertex_count() const {
    return vertices;
  }

  int edge_count() const {
    return edges;
  }

  // Depth first search on a directed graph
  void dfs(void (visit)(int vertex)) {
    vector<bool> visited(vertices, false);
    int current = 0;

    function<void(int)> dfs_impl = [&](int u) {
      visited[u] = true;
      for (auto& v: adj_list[u]) {
        if (!visited[v]) {
          dfs_impl(v);
        }
      }
      visit(u);
    };

    for (int i = 0; i < vertices; ++i) {
      if (!visited[i]) {
        dfs_impl(i);
      }
    }
  }

  set<vector<int>> strongly_connected_components() {
    // Start the traversal from any node.
    // Assign a traversal id based on traversal order
    // to each node. Also assign a low-link value equal
    // to this id.
    // Push each node onto a seen stack, also mark it visited.
    // Once the traversal hits a node which is already visited,
    // start backtracking.
    // On the path back, if the last node be on the stack then
    // set the current node's low-link value equal to the minimum
    // of the last node's and the current node's. **Don't pop the
    // stack yet!**
    // If on the return path we encounter a node whose traversal id
    // and low link value are same, and all of whose neighbors have
    // been explored, then pop all previous nodes from the stack.
    // In this case we have completed one SC component.

    int current_id = -1;
    vector<int> traversal_id(vertices, -1);
    vector<int> low_link_value(vertices, -1);
    stack<int> seen;
    vector<bool> visited(vertices, false);
    vector<bool> on_stack(vertices, false);

    set<vector<int>> result;

    function<void(int)> dfs_impl = [&](int u) {
      if (visited[u]) {
        return;
      }

      // set traversal state before recursing depth first.
      visited[u] = true;
      traversal_id[u] = low_link_value[u] = ++current_id;
      seen.push(u);
      on_stack[u] = true;

      // recurse depth first.
      for (auto& v: adj_list[u]) {
        dfs_impl(v);
        if (on_stack[v]) {
          low_link_value[u] = min(low_link_value[u], low_link_value[v]);
        }
      }

      // Did we find the start of a strongly-connected component.
      if (low_link_value[u] == u) {
        assert(on_stack[u]);
        if (seen.top() != u) {
          // there are multiple vertices in this strongly
          // connected component. Put them in a vector.
          vector<int> strong_component;
          while (true) {
            auto val = seen.top();
            on_stack[val] = false;
            seen.pop();
            if (low_link_value[val] == u) {
              strong_component.push_back(val);
            }
            if (val == u) {
              break;
            }
          }
          // add this to the list of strongly-connected
          // components.
          result.insert(strong_component);
        } else {
          on_stack[u] = false;
          seen.pop();
        }
      }
    };

    for (int i = 0; i < vertices; ++i) {
      if (visited[i]) {
        continue;
      }
      dfs_impl(i);
    }

    return result;
  }

private:
  const int vertices;
  vector<set<int>> adj_list;
  int edges;
};

// Undirected graph algorithms
class UndirectedGraph {
public:
  typedef pair<pair<int, int>, int> edge_type;

  UndirectedGraph(int size, bool weighted = false)
    : dgraph(size), weighted(weighted) {}

  bool add_edge(int u, int v, int weight = 1) {
    assert(weighted || weight == 1);
    if (dgraph.add_edge(u, v)) {
      if (dgraph.add_edge(v, u)) {
        if (weighted) {
          set_weight(u, v, weight);
        }
        return true;
      }
      dgraph.remove_edge(u, v);
    }
    return false;
  }

  int vertex_count() const {
    return dgraph.vertex_count();
  }

  int edge_count() const {
    auto edges = dgraph.edge_count();
    assert(edges % 2 == 0);
    return edges/2;
  }

  // Depth first search on an undirected graph
  bool dfs(void (visit)(int vertex)) {
    vector<bool> visited(vertex_count(), false);
    bool has_cycle = false;
    vector<bool> on_stack(vertex_count(), false);

    function<void(int, int)> dfs_impl = [&](int u, int p) {
      if (visited[u]) {
        has_cycle = has_cycle || on_stack[u];
        return;
      }

      visited[u] = true;
      on_stack[u] = true;
      for (auto& next: dgraph.adj_list[u]) {
        if (next == p) {
          // don't trace back to where we came from
          // because all edges are bidirectional.
          continue;
        }
        dfs_impl(next, u);
      }
      visit(u);
      on_stack[u] = false;
    };

    for (int i = 0; i < vertex_count(); ++i) {
      if (!visited[i]) {
        dfs_impl(i, -1);  // no parent if we start the dfs from here.
      }
    }

    return has_cycle;
  }

  bool is_tree() {
    return !dfs([](int) {});
  }

  pair<int, int> center() {
    if (!is_tree()) {
      return make_pair(-1, -1);
    }

    vector<bool> remaining(vertex_count(), true);
    auto adj_list = dgraph.adj_list;
    int remnant = vertex_count();
    int target = 2;

    while (remnant > target) {
      set<int> toRemove;
      for (int i = 0; i < vertex_count(); ++i) {
        if (remaining[i] && adj_list[i].size() == 1) {
          toRemove.insert(i);
        }
      }

      for (auto v: toRemove) {
        for (auto& neigh: adj_list[v]) {
          adj_list[neigh].erase(v);
        }
        remaining[v] = false;
        adj_list[v].clear();
        --remnant;
        cout << "Removed " << v << '\n';
      }
    }

    int centers[2] = {-1, -1};
    for (int i = 0, j = 0; i < vertex_count(); ++i) {
       if (remaining[i]) {
         centers[j++] = i;
         remaining[i] = false;
       }
    }
    return make_pair(centers[0], centers[1]);
  }

  UndirectedGraph mst_prim() {
    UndirectedGraph result(vertex_count(), true);

    auto comparator = [](edge_type& e1, edge_type& e2) -> bool {
      return e1.second > e2.second;
    };

    priority_queue<edge_type,
                   vector<edge_type>,
                   decltype(comparator)> pq(comparator);

    vector<bool> visited(vertex_count(), false);
    int current = 0;
    int done_count = 1;
    visited[current] = true;

    int min_cost = 0;

    while (done_count < vertex_count()) {
      for (int next: dgraph.adj_list[current]) {
        if (!visited[next]) {
          pq.push(make_pair(make_pair(current, next), get_weight(current, next)));
        }
      }
      if (pq.empty()) {
        // This condition can be used to find a minimum spanning
        // forest instead. Find the next unvisited node, set
        // current's value to you, and start a fresh iteration
        // instead of breaking.
        break;
      }

      edge_type next_edge = pq.top();
      pq.pop();
      int next;

      if (!visited[next_edge.first.first]) {
        current = next_edge.first.second;
        next = next_edge.first.first;
      } else if (!visited[next_edge.first.second]) {
        current = next_edge.first.first;
        next = next_edge.first.second;
      } else {
        continue;
      }

      result.add_edge(current, next, next_edge.second);
      cout << "Adding edge (" << current << ", " << next << ")\n";
      current = next;
      visited[current] = true;
      ++done_count;
      min_cost += next_edge.second;
    }

    cout << "Min cost: " << min_cost << '\n';
    return result;
  }

  friend bool operator==(UndirectedGraph& ug1, UndirectedGraph& ug2) {
    return true;
  }

  bool is_isomorphic(UndirectedGraph& that) {
    if (!is_tree() || !that.is_tree()) {
      return false;
    }

    if (vertex_count() != that.vertex_count()) {
      return false;
    }

    auto centers1 = center();
    auto centers2 = that.center();

    if ((centers1.second < 0 && centers2.second >= 0) ||
        (centers2.second < 0 && centers1.second >= 0)) {
      return false;
    }

    string tree1str = encode_tree(centers1.first);
    string tree2str = that.encode_tree(centers2.first);
    cout << tree1str << '\n' << tree2str << '\n';
    if (tree1str == tree2str) {
      return true;
    }
    if (centers1.second >= 0) {
      tree2str = that.encode_tree(centers2.second);
      cout << tree2str << '\n';
      return tree1str == tree2str;
    }
    return false;
  }

  string encode_tree(int root = 0) {
    assert(root < vertex_count());

    vector<bool> visited(vertex_count(), false);

    function<string(int)> encode_impl = [&](int subtreeRoot) {
      vector<string> encoded_children;
      visited[subtreeRoot] = true;

      for (auto next: dgraph.adj_list[subtreeRoot]) {
        if (visited[next]) {
          continue;
        }
        encoded_children.push_back(encode_impl(next));
      }
      sort(encoded_children.begin(), encoded_children.end());
      stringstream sout;
      copy(encoded_children.begin(), encoded_children.end(),
           ostream_iterator<string>(sout, ""));
      return "(" + sout.str() + ")";
    };
    return encode_impl(root);
  }

private:
  DirectedGraph dgraph;
  bool weighted;
  map<pair<int, int>, int> weights;

  void set_weight(int u, int v, int weight) {
    pair<int, int> key = (u < v) ? make_pair(u, v) : make_pair(v, u);
    weights[key] = weight;
  }

  int get_weight(int u, int v) {
    pair<int, int> key = (u < v) ? make_pair(u, v) : make_pair(v, u);
    return weights[key];
  }

  edge_type get_edge(int u, int v) {
    pair<int, int> key = (u < v) ? make_pair(u, v) : make_pair(v, u);
    auto it = weights.find(key);
    if (it == weights.end()) {
      return make_pair(make_pair(-1, -1), numeric_limits<int>::max());
    }
    return *it;
  }
};

int main() {
  DirectedGraph dg(5);
  try {
    dg.add_edge(0, 1);
    dg.add_edge(0, 2);
    dg.add_edge(1, 3);
    dg.add_edge(2, 4);
    dg.add_edge(3, 4);
  } catch(exception& e) {
    cout << e.what() << '\n';
  }

  cout << dg.vertex_count() << " vertices and "
       << dg.edge_count() << " edges\n";

  dg.dfs([](int v) {
    cout << '[' << v << "]\n"; 
  });
  cout << '\n';

  auto components = dg.strongly_connected_components();
  for (auto& component: components) {
    cout << "Component(dg1):\n========\n";
    ostream_iterator<int> os(cout, " ");
    copy(component.begin(), component.end(), os);
    cout << '\n';
  }

  DirectedGraph dg1(10);
  dg1.add_edge(0, 1);
  dg1.add_edge(1, 2);
  dg1.add_edge(2, 0);
  dg1.add_edge(1, 3);
  dg1.add_edge(3, 4);
  dg1.add_edge(4, 5);
  dg1.add_edge(5, 3);
  dg1.add_edge(4, 6);
  dg1.add_edge(6, 7);
  dg1.add_edge(7, 8);
  dg1.add_edge(8, 6);
  dg1.add_edge(7, 9);

  dg1.dfs([](int v) {
    cout << '[' << v << "]\n"; 
  });

  components = dg1.strongly_connected_components();
  for (auto& component: components) {
    cout << "Component(dg1):\n========\n";
    ostream_iterator<int> os(cout, " ");
    copy(component.begin(), component.end(), os);
    cout << '\n';
  }

  DirectedGraph dg2(9);
  dg2.add_edge(0, 1);
  dg2.add_edge(1, 2);
  dg2.add_edge(2, 0);
  dg2.add_edge(1, 3);
  dg2.add_edge(3, 4);
  dg2.add_edge(4, 5);
  dg2.add_edge(5, 6);
  dg2.add_edge(6, 7);
  dg2.add_edge(7, 8);
  dg2.add_edge(8, 6);
  dg2.add_edge(6, 5);
  dg2.add_edge(5, 3);

  components = dg2.strongly_connected_components();
  for (auto& component: components) {
    cout << "Component(dg2):\n========\n";
    ostream_iterator<int> os(cout, " ");
    copy(component.begin(), component.end(), os);
    cout << '\n';
  }

  UndirectedGraph udg(8);
  udg.add_edge(0, 1);
  udg.add_edge(0, 2);
  udg.add_edge(1, 3);
  udg.add_edge(2, 4);
  udg.add_edge(1, 5);
  udg.add_edge(2, 6);
  udg.add_edge(5, 7);
  // udg.add_edge(4, 5);

  udg.dfs([](int v) {
    cout << '[' << v << "]\n"; 
  });
  cout << '\n';
  if (udg.is_tree()) {
    cout << "Is tree.\n";
  } else {
    cout << "Is not tree.\n";
  }
  int center1, center2;
  tie(center1, center2) = udg.center();
  cout << "Centers: " << center1 << ", " << center2 << '\n';

  UndirectedGraph udg1(7, true);

  udg1.add_edge(0, 1, 3);
  udg1.add_edge(0, 2, 6);
  udg1.add_edge(1, 3, 2);
  udg1.add_edge(2, 3, 7);
  udg1.add_edge(2, 4, 3);
  udg1.add_edge(4, 5, 5);
  udg1.add_edge(3, 5, 3);
  udg1.add_edge(3, 6, 4);
  udg1.add_edge(5, 6, 6);

  auto mst = udg1.mst_prim();

  /*        3
        0 ----- 1              2 ----- 1
     6 /        | 2                    |
      /   7     |                      |
     2 -------- 3           6          0
      \      3 | \ 4         \         |\
     3 \  5    |  \  **       \        | \
        4 -----5 - 6           5 ----- 4  3 
                 6                     

    Adding edge (0, 1)
    Adding edge (1, 3)
    Adding edge (3, 5)
    Adding edge (3, 6)
    Adding edge (5, 4)
    Adding edge (4, 2)

  */

  UndirectedGraph udg_tree(7, false);
  udg_tree.add_edge(0, 1);
  udg_tree.add_edge(1, 3);
  udg_tree.add_edge(3, 5);
  udg_tree.add_edge(3, 6);
  udg_tree.add_edge(5, 4);
  udg_tree.add_edge(4, 2);

  UndirectedGraph udg2_tree(7, false);
  udg2_tree.add_edge(0, 1);
  udg2_tree.add_edge(1, 2);
  udg2_tree.add_edge(0, 3);
  udg2_tree.add_edge(0, 4);
  udg2_tree.add_edge(4, 5);
  udg2_tree.add_edge(5, 6);

  auto c1 = udg_tree.center();
  cout << "Center: [" << c1.first << ", " << c1.second << "]\n";

  auto c2 = udg2_tree.center();
  cout << "Center: [" << c2.first << ", " << c2.second << "]\n";
  cout << '\n';

  cout << udg_tree.is_isomorphic(udg2_tree) << '\n';
}
