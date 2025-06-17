// src/GraphGenerator.cpp
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "GraphGenerator.hpp"
#include "utils.hpp"


// Function to generate a random unit disk graph
Graph generateUnitDiskGraph(int n, double r)
{
  Graph G(n);
  std::vector<std::pair<double, double>> points(n);

  // Generate random points
  for (int i = 0; i < n; ++i)
  {
    points[i] = {getRandom(), getRandom()};
    G.setPosition(i, {points[i].first, points[i].second, 0.0});
  }

  double numberEdges = 0;
  // Connect points based on the distance threshold (r)
  for (int i = 0; i < n; ++i)
  {
    for (int j = i + 1; j < n; ++j)
    {
      double distance = sqrt(pow(points[i].first - points[j].first, 2) +
                             pow(points[i].second - points[j].second, 2));
      if (distance <= 2 * r)
      {
        G.addEdge(i, j);
        numberEdges = numberEdges + 1;
      }
    }
  }
  // double density = 2*numberEdges /(n*(n-1));
  // std::cout <<  "Density : " << density << std::endl ;
  return G;
}

Graph generateUnitDiskGraph(int n, double r, std::vector<double> rhos)
{
  Graph G = generateUnitDiskGraph(n, r);
  G.setRhos(rhos);
  return G;
}

Graph fromEdgeFile(const std::string filename, int nbNodes)
{
  std::ifstream edgeFile(filename);
  std::string line;

  Graph G(nbNodes);

  while (getline(edgeFile, line))
  {
    // tokenize line
    std::vector<std::string> tokens = tokenize(line, " ");
    G.addEdge(std::stoi(tokens[0]), std::stoi(tokens[1]));
  }
  return G;
}

