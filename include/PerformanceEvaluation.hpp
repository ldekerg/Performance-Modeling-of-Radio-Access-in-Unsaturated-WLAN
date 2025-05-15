#ifndef PERFORMANCE_EVALUATION_HPP
#define PERFORMANCE_EVALUATION_HPP

#include <vector>
#include <unordered_set>
#include "Graph.hpp"

/**
 * Compute the u_v's of the given graph with the given channel allocation
 * @param G Graph with loads \rho_v's
 * @param allocation Channel allocation on the form of a list of vertices for every channel
 * @return A list of u_v's, in the order of the vertices of the graph
 */
std::vector<double> performanceModel (const Graph& G, const std::vector<std::vector<int>>& allocation);


#endif
