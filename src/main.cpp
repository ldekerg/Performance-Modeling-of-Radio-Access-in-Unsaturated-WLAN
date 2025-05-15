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

int main() {
  srand(time(nullptr));

  // Graph parameters
  int graphSize = 25;
  double radius = 0.144;      //corresponds to a density of roughly 0.2

  // Graph initialization
  Graph G = generateUnitDiskGraph(graphSize, radius);
  std::vector<double> rhos = generateUniformRhos(graphSize, 0.1, 0.6);
  G.setRhos(rhos);

  // Channel allocation (every AP on the same radio channel)
  std::vector<std::vector<int>> channel_allocation;
  std::vector<int> list;
  for(int u = 0; u < graphSize; u++) list.push_back(u);
  channel_allocation.push_back(list);

  // Compute performances
  std::vector<double> performance = performanceModel(G, channel_allocation);

  // Save graph configuration
  std::ofstream graph_file("./results/exemple_graph.txt");
  for (int u=0; u < graphSize-1; u++)
  {
    for (int v = u+1; v < graphSize; v++){
      if (G.areAdjacent(u,v)) graph_file << u << " " << v << "\n";
    }
  }
  graph_file.close();

  std::ofstream rho_file("./results/exemple_rhos.txt");
  for (int u=0; u < graphSize-1; u++)
  {
    rho_file << u << " " << rhos[u] << std::endl;
  }
  rho_file.close();

  std::ofstream allocation_file("./results/exemple_allocation.txt");
  for (int u=0; u < graphSize-1; u++)
  {
    allocation_file << u << " " << 0 << std::endl;
  }
  allocation_file.close();

  // Save performance evaluation
  std::ofstream results_file("./results/exemple_model_performances.txt");
  for (int u=0; u < graphSize-1; u++)
  {
    results_file << u << " " << performance[u] << std::endl;
  }
  results_file.close();

  return 0;
}