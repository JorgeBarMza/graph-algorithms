#include <iostream>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <chrono>

using namespace std::chrono;

typedef double t_weight;

typedef boost::property<boost::edge_weight_t, t_weight> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty> Graph;

typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

typedef boost::exterior_vertex_property<Graph, t_weight> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;


int main()
{
  Graph g;
  // const int num_edges = 11;
  //
  // // define edges
  // int edges[] = { 1,     2,
  //                 1,     3,
  //                 1,     4,
  //                 2,     5,
  //                 3,     4,
  //                 3,     6,
  //                 4,     5,
  //                 4,     6,
  //                 4,     7,
  //                 5,     7,
  //                 6,     7 };
  //
  // // define the weight on edges
  // t_weight weight[] = {  4,
  //                       10,
  //                        3,
  //                        1,
  //                       12,
  //                       20,
  //                        6,
  //                        3,
  //                        0,
  //                        3,
  //                        9 };
  const int num_edges = 14;

  int edges[] = { 0,     1,
                  0,     7,
                  1,     7,
                  1,     2,
                  7,     6,
                  7,     8,
                  2,     8,
                  2,     5,
                  2,     3,
                  6,     5,
                  3,     5,
                  3,     4,
                  5,     4,
                  5,     6 };

  t_weight weight[] = {  4,
                         8,
                        11,
                         8,
                         1,
                         7,
                         2,
                         4,
                         7,
                         2,
                        14,
                         9,
                        10,
                         6 };

  for (std::size_t k = 0; k < num_edges; ++k)
    boost::add_edge(edges[k*2], edges[k*2+1], weight[k], g);
  WeightMap weight_pmap = boost::get(boost::edge_weight, g);

  DistanceMatrix distances(num_vertices(g));
  DistanceMatrixMap dm(distances, g);

  std::cout << "Floyd Warshall:\n";
  auto t1 = high_resolution_clock::now();
  bool valid = floyd_warshall_all_pairs_shortest_paths(g, dm,
                                            boost::weight_map(weight_pmap));
  auto t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();

  if (!valid) {
    std::cerr << "Error - Negative cycle in matrix" << std::endl;
    return -1;
  }

  std::cout << "Distance matrix: " << std::endl;
  for (std::size_t i = 0; i < num_vertices(g); ++i) {
    for (std::size_t j = i; j < num_vertices(g); ++j) {
      std::cout << "From vertex " << i+1 << " to " << j+1 << " : ";
      if(distances[i][j] == std::numeric_limits<t_weight>::max())
        std::cout << "inf" << std::endl;
      else
        std::cout << distances[i][j] << std::endl;
    }
    std::cout << std::endl;
  }
  std::cout << "duration: " << duration << " μs\n";
  return 0;
}
