#include "GraphGenerator.hpp"
#include "Graph.hpp"
#include "RhoGenerator.hpp"
#include "PerformanceEvaluation.hpp"
#include "utils.hpp"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <fstream>

using namespace std;

/**
 * There must be all the following argument
 * 0 : (int) number of nodes in the graph
 * 1 : (str) path to a .txt file containing a edge representation of the graph
 * 2 : (str) path to a .txt file containing the rho for every vertex of the graph
 * 3 : (str) path to a .txt file containing the radio channel for every vertex of the graph
 * 4 : (str) path to the output file
 */
int main(int argc, char *argv[]) {
  srand(time(nullptr));

  if (argc != 5) return 1;
  int number_of_nodes = stoi(argv[0]);

  // Graph initialization
  Graph G = fromEdgeFile(argv[1], number_of_nodes);
  vector<double> rhos = fromRhoFile(argv[2], number_of_nodes);
  G.setRhos(rhos);

  // Channel allocation (every AP on the same radio channel)
  std::ifstream allocFile(argv[3]);
  string line;
  std::vector<std::vector<int>> channel_allocation (1, std::vector<int>{});
  while (getline(allocFile, line))
  {
    // tokenize line
    std::vector<std::string> tokens = tokenize(line, " ");
    channel_allocation[std::stoi(tokens[1])].push_back(std::stoi(tokens[0]));
  }

  // Compute performances
  std::vector<double> performance = performanceModel(G, channel_allocation);

  // Save performance evaluation
  std::ofstream results_file(argv[4]);
  for (int u=0; u < number_of_nodes+1; u++)
  {
    results_file << u << " " << performance[u] << std::endl;
  }
  results_file.close();

  return 0;
}
