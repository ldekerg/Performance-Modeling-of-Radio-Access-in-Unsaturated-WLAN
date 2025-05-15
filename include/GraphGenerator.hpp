#ifndef GRAPH_GENERATOR_HPP
#define GRAPH_GENERATOR_HPP

#include "Graph.hpp"
#include <string>
#include <vector>

/**
 * Generate a random disk graph in a 1*1 space
 * @param n number of nodes
 * @param r radius of the disks
 */
Graph generateUnitDiskGraph(int n, double r);

/**
 * Generate a random disk graph in a 1*1 space
 * @param n number of nodes
 * @param radius of the disks
 * @param rhos list of rhos to use in the graph
 */
Graph generateUnitDiskGraph(int n, double r, std::vector<double> rhos);

#endif // GRAPH_GENERATOR_HPP
