#ifndef UTILS_HPP
#define UTILS_HPP

#include "Graph.hpp"
#include <vector>
#include <string>

/**
 * Generate a random floating-point number between 0 and 1 using a uniform law
 */
double getRandom();

/**
 * Generate a random int between min (included) and max (excluded)
 */
int getRandomInt(int, int);

/**
 * Generate a random floating number using a Gaussian law
 * @param mu, sigma parameter of the Gaussian law
 */
double getRandomGaussian(double mu, double sigma);

/**
 * Find the first class containing an object
 * @param classes list of classes containing int
 * @param v object to find
 * @return the first vector containting v, a empty vector instead
 */
std::vector<int> findClass(const std::vector<std::vector<int>>& classes, int v);

std::vector<std::string> tokenize(std::string line, std::string del);

std::vector<std::vector<int>> fromAllocationFile(const std::string filename, int nbColor);

bool canAddToClique(int v, const std::vector<int> &clique, const Graph &G);

void canAddToColorClass(int v, const std::vector<std::vector<int>> colorClass, const Graph &G, bool &createP3, int &candidate);

bool isValidSubcoloring(const Graph &G, const std::vector<std::vector<std::vector<int>>> &subcoloring);

std::vector<std::vector<int>> flatten(const std::vector<std::vector<std::vector<int>>> &subcoloring);




#endif