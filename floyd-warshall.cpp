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

  const int num_edges = 11;

  int edges[] = { 1,     2,
                  1,     3,
                  1,     4,
                  2,     5,
                  3,     4,
                  3,     6,
                  4,     5,
                  4,     6,
                  4,     7,
                  5,     7,
                  6,     7 };

  t_weight weight[] = {  4,
                        10,
                         3,
                         1,
                        12,
                        20,
                         6,
                         3,
                         0,
                         3,
                         9 };

  for (std::size_t k = 0; k < num_edges; ++k)
    boost::add_edge(edges[k*2]-1, edges[k*2+1]-1, weight[k], g);

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
