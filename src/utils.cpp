#include "utils.hpp"
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>

std::default_random_engine generator;

double getRandom()
{
  return static_cast<double>(rand()) / RAND_MAX;
}

int getRandomInt(int min, int max)
{
  return rand() % (max - min) + min;
}

double getRandomGaussian(double mu, double sigma)
{
  std::normal_distribution<double> distribution(mu, sigma);
  return distribution(generator);
}

std::vector<int> findClass(const std::vector<std::vector<int>>& classes, int v)
{
  for (const auto& c: classes)
  {
    if (std::find(c.begin(), c.end(), v) != c.end())
    {
      return c;
    }
  }
  return {};
}

// Helper function to tokenize a string based on a delimiter
std::vector<std::string> tokenize(std::string line, std::string del)
{
  std::vector<std::string> tokens;
  size_t pos = 0;
  std::string token;
  while ((pos = line.find(del)) != std::string::npos)
  {
    token = line.substr(0, pos);
    tokens.push_back(token);
    line.erase(0, pos + del.size());
  }
  tokens.push_back(line);

  return tokens;
}

