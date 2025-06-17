#include "PerformanceEvaluation.hpp"
#include "utils.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <functional> 
#include <iostream>
#include <map>
#include <time.h>

// helper functions
void dfs(const Graph &G, int v, std::vector<int> &component, std::vector<int> colorClass, std::vector<bool> &visited)
{
  visited[v] = true;      // Mark the vertex as visited
  component.push_back(v); // Add to the current component

  // Visit all neighbors of the current vertex
  for (int neighbor : G.getColoredNeighbors(v, colorClass))
  {
    if (!visited[neighbor])
    {
      dfs(G, neighbor, component, colorClass, visited); // Recursive DFS call
    }
  }
}

int pickRandomVertex(const std::vector<int> &vertices)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, vertices.size() - 1);
  return vertices[dist(gen)];
}

std::vector<std::vector<int>> connectedComponentsPartition(const Graph &G, const std::vector<int> &colorClass)
{
  std::vector<bool> visited(G.getNumVertices(), false); // Track visited vertices

  std::vector<std::vector<int>> colorClassComponents; // Store components for the current color class
  for (int vertex : colorClass)
  {
    if (!visited[vertex])
    {
      std::vector<int> component;                     // Store the current component
      dfs(G, vertex, component, colorClass, visited); // Start DFS from the current vertex in the induced graph
      colorClassComponents.push_back(component);      // Add the found component
    }
  }
  return colorClassComponents;
}

void evaluatePerformanceArbitrarySaturated(const Graph &G, const std::vector<std::vector<int>> &colorClass, int X, int T, double lambda, std::vector<double> &performance)
{
  std::vector<int> independentSet; // Current independent set, initially empty
  for (int iteration=0; iteration<X; iteration++){
    
    for (const auto &connectedComponent : colorClass)
    {

      independentSet.clear();
      // Run the Markov chain for T steps
      for (int step = 0; step < T; ++step)
      {
        int u = pickRandomVertex(connectedComponent); // Pick a random vertex from the color class

        // Delete ↓: If u is in the independent set, remove it with probability 1 / (1 + lambda)
        if (std::find(independentSet.begin(), independentSet.end(), u) != independentSet.end())
        {
          double prob = 1.0 / (1.0 + lambda);
          if (((double)rand() / RAND_MAX) < prob)
          {
            independentSet.erase(std::remove(independentSet.begin(), independentSet.end(), u), independentSet.end());
          }
        }
        else
        {
          // Insert ↑: If u is not in the independent set and none of its neighbors are in the set
          bool canInsert = true;
          for (int neighbor : G.getNeighbors(u))
          {
            if (std::find(independentSet.begin(), independentSet.end(), neighbor) != independentSet.end())
            {
              canInsert = false;
              break;
            }
          }

          // If we can insert, do so with probability λ / (1 + λ)
          if (canInsert)
          {
            double prob = lambda / (1.0 + lambda);
            if (((double)rand() / RAND_MAX) < prob)
            {
              independentSet.push_back(u);
            }
          }
          else
          {
            // Drag ↔: If u has a unique neighbor v in the independent set, add u and remove v
            int uniqueNeighborInSet = -1;
            int neighborCountInSet = 0;
            for (int neighbor : G.getNeighbors(u))
            {
              if (std::find(independentSet.begin(), independentSet.end(), neighbor) != independentSet.end())
              {
                uniqueNeighborInSet = neighbor;
                neighborCountInSet++;
                if (neighborCountInSet > 1)
                  break; // More than 1 neighbor in the set, break
              }
            }
            if (neighborCountInSet == 1)
            {
              independentSet.erase(std::remove(independentSet.begin(), independentSet.end(), uniqueNeighborInSet), independentSet.end());
              independentSet.push_back(u);
            } 
          }
        }
      }
      // Update performance for all vertices in the independent set
      for (int v : independentSet)
      {
        performance[v]+= 1.0;
      }
    }
  }
}


std::vector<double> modelForOneColor(Graph G, const std::vector<int> &subgraph)
{
  double timeRatio = 1.0;
  double epsilon = 0.001;
  int T = 5*std::pow(2, std::sqrt(G.getNumVertices()));
  double lambda = 10.0;
  int X = 1000;

  std::vector<double> performance(G.getNumVertices(), 0.0);


  while (timeRatio > epsilon)
  {
    // Find subgraph of active nodes (rho != 0)
    std::vector<int> activeSubgraph;
    for (int u : subgraph)
    {
      if ((G.getRho(u)) > epsilon)
        activeSubgraph.push_back(u);
    }

    if (activeSubgraph.size() == 0)
      return performance;

    // Find MIS
    std::vector<double> MIS(G.getNumVertices(), 0.0);
    evaluatePerformanceArbitrarySaturated(G, connectedComponentsPartition(G, activeSubgraph), X, T, lambda, MIS);
    // Normalize performance values by the number of steps
    for (int i = 0; (unsigned)i < MIS.size(); i++)
    {
      MIS[i] = MIS[i] / X;
    }

    // Compute Rho/MIS
    std::vector<double> capacite(G.getNumVertices(), std::numeric_limits<double>::infinity());
    for (int u : activeSubgraph)
    {
      if (MIS[u] > epsilon)
        capacite[u] = G.getRho(u) / MIS[u];
      else
        MIS[u] = 0.0;
    }

    // Find minimum Rho/MIS
    double capacite_min = std::numeric_limits<double>::infinity();
    for (double c : capacite)
    {
      if (c < capacite_min)
        capacite_min = c;
    }
    if (timeRatio < capacite_min)
      capacite_min = timeRatio; // can not exceed the remaining time ratio

    // Run for *capacite_min* ratio of time
    for (int u : activeSubgraph)
    {
      performance[u] += capacite_min * MIS[u];
      G.setRho(u, G.getRho(u) - capacite_min * MIS[u]);
      // G.setRho(u, G.getRho(u) - performance[u]);
    }

    // Update remaining time ratio
    timeRatio -= capacite_min;
  }

  return performance;
}

std::vector<double> performanceModel(const Graph &G, const std::vector<std::vector<int>> &allocation)
{
  std::vector<double> performance(G.getNumVertices(), 0.0);
  for (const auto &colorClass : allocation)
  {
    std::vector<double> perf = modelForOneColor(G, colorClass);
    for (int u : colorClass)
      performance[u] = perf[u];
  }

  return performance;
}
