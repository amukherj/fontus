#include "dag.h"

int main() {
  fontus::DirectedAcyclicGraph g(9, true);

  g.add_edge(8, 7, 3);
  g.add_edge(8, 5, 6);
  g.add_edge(7, 5, 2);
  g.add_edge(7, 6, 3);
  g.add_edge(5, 4, 4);
  g.add_edge(5, 6, 2);
  g.add_edge(6, 3, 5);
  g.add_edge(5, 3, 3);
  g.add_edge(6, 2, 2);
  g.add_edge(4, 2, 2);
  g.add_edge(1, 3, 1);
  g.add_edge(2, 1, 1);
  g.add_edge(1, 0, 2);

  auto sorted = g.topsort();
  for (const auto& v: sorted) {
    std::cout << v << ',';
  }
  std::cout << '\n';

  auto dist_vec = g.ss_shortest_path(5);
  fontus::vertex_type v = 0;
  for (auto dist: dist_vec) {
    std::cout << "dist[" << v++ << "] = " << dist << '\n';
  }
}
