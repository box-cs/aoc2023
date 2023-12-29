#include "../lib/io.hpp"
#include <cstdint>
#include <map>
#include <numeric>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#define ACCESS_NODE(ROUTE) ((ROUTE == 'L' ? 0 : 1))

using std::map;
using std::string;
using std::vector;

typedef map<string, vector<string>> NavigationGraph;

void partOne(auto lines);
void partTwo(auto lines);
NavigationGraph getNavigationGraphs(auto lines, string pattern);
vector<string> getLines(std::stringstream &&ss);

int main(int argc, char *argv[]) {
  auto f = io::readFile("input.txt", []() { exit(1); });
  auto lines = getLines(std::move(f));
  partOne(lines);
  partTwo(lines);
  return 0;
}

void partOne(auto lines) {
  auto instructions = lines.front();
  string pattern("([A-Z]{3}) = \\(([A-Z]{3}),\\s([A-Z]{3})\\)");
  auto graph = getNavigationGraphs(lines, pattern);
  string route = "AAA";
  auto location = graph[route];
  int steps = 0;
  while (route != "ZZZ") {
    for (auto routeInstruction : instructions) {
      route = location[ACCESS_NODE(routeInstruction)];
      location = graph[route];
      steps++;
    }
  }
  std::cout << "Part 1: " << steps << std::endl;
}

void partTwo(auto lines) {
  auto instructions = lines.front();
  vector<string> startingRoutes{};
  for (auto line : lines | std::views::drop(1)) {
    string route = {line.begin(), line.begin() + 3};
    if (route.back() == 'A') {
      startingRoutes.push_back(route);
    }
  }
  string pattern("(.{3}) = \\((.{3}),\\s(.{3})\\)");
  auto graph = getNavigationGraphs(lines, pattern);
  vector<int> stepsTaken{};
  for (auto startingRoute : startingRoutes) {
    int steps = 0;
    auto location = graph[startingRoute];
    auto route = startingRoute;
    while (route.back() != 'Z') {
      for (auto routeInstruction : instructions) {
        route = location[ACCESS_NODE(routeInstruction)];
        location = graph[route];
        // std::cout << "Route: " << route << std::endl;
        steps++;
      }
    }
    stepsTaken.push_back(steps);
    uint64_t lcm = stepsTaken[0];
    for (auto stepTaken : stepsTaken | std::views::drop(1)) {
      lcm = std::lcm(lcm, stepTaken);
    }

    std::cout << "Part 2: " << lcm << '\n';
  }
}

NavigationGraph getNavigationGraphs(auto lines, string pattern) {
  NavigationGraph graph{};
  string startRoute{};
  std::regex match(pattern);
  for (auto line : lines | std::views::drop(1)) {
    std::smatch matches;
    if (std::regex_search(line, matches, match)) {
      std::ssub_match route = matches[1];
      std::ssub_match pathA = matches[2];
      std::ssub_match pathB = matches[3];
      graph[route.str()] = {pathA.str(), pathB.str()};
    }
  }
  return graph;
}

vector<string> getLines(std::stringstream &&ss) {
  vector<string> lines{};
  do {
    string line = io::input<string>("", 0, '\n', ss);
    lines.push_back(line);
  } while (lines.back() != "");
  return {lines.begin(), lines.end() - 1};
}
