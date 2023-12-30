#include "../lib/io.hpp"
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

using std::all_of;
using std::string;
using std::vector;

vector<int> getNumbersInLine(string line);
vector<string> getLines(std::stringstream &&ss);
vector<int> getDifferences(vector<int> numbers);
void partOne(auto lines);
void partTwo(auto lines);

int main(int argc, char *argv[]) {
  auto f = io::readFile("input.txt", []() { exit(1); });
  auto lines = getLines(std::move(f));
  // Horrible code because my datastructure didn't include the initial line
  partOne(lines);
  partTwo(lines);
  return 0;
}

void partOne(auto lines) {
  vector<vector<vector<int>>> triangles{};
  for (auto &line : lines) {
    vector<vector<int>> triangle{};
    bool canBeSolved = false;
    auto numbersInLine = getNumbersInLine(line);
    do {
      numbersInLine = getDifferences(numbersInLine);
      triangle.push_back(numbersInLine);
      canBeSolved =
          all_of(numbersInLine.begin(), numbersInLine.end(),
                 [&](int number) { return number == numbersInLine.front(); });
    } while (!canBeSolved);
    triangles.push_back(triangle);
  }

  vector<int> sums{};
  for (auto &triangle : triangles) {
    int sum{};
    for (auto &line : triangle) {
      sum += line.back();
    }
    sums.push_back(sum);
  }

  int64_t sum{};
  for (int i = 0; i < lines.size(); i++) {
    sum += (getNumbersInLine(lines[i]).back() + sums[i]);
  }
  std::cout << "Part One: " << sum << std::endl;
}

void partTwo(auto lines) {
  vector<vector<vector<int>>> triangles{};
  for (auto &line : lines) {
    vector<vector<int>> triangle{};
    bool canBeSolved = false;
    auto numbersInLine = getNumbersInLine(line);
    do {
      numbersInLine = getDifferences(numbersInLine);
      triangle.push_back(numbersInLine);
      canBeSolved =
          all_of(numbersInLine.begin(), numbersInLine.end(),
                 [&](int number) { return number == numbersInLine.front(); });
    } while (!canBeSolved);
    triangles.push_back(triangle);
  }

  int lineCount = 0;
  int64_t sum{};
  for (auto &triangle : triangles) {
    int current = 0;
    int count = 0;
    for (auto &line : triangle | std::views::reverse) {
      auto front = line.front();
      current = front - current;
      if (count++ == triangle.size() - 1) {
        auto lineFront = getNumbersInLine(lines[lineCount]).front();
        auto leftMost = lineFront - current;
        sum += leftMost;
      }
    }
    lineCount++;
  }
  std::cout << "Part Two: " << sum << std::endl;
}

vector<int> getDifferences(vector<int> numbers) {
  vector<int> differences{};
  auto scan = numbers.front();
  for (auto &number : numbers | std::views::drop(1)) {
    differences.push_back(number - scan);
    scan = number;
  }
  return differences;
}

vector<int> getNumbersInLine(string line) {
  vector<int> numbers{};
  // I had a bug here because I assumed that all numbers in input were positive
  std::regex re("-?\\d+");
  std::sregex_iterator it(line.begin(), line.end(), re);
  std::sregex_iterator reg_end;

  for (auto m = it; m != reg_end; ++m) {
    int value{};
    auto s = (*m).str();
    std::from_chars(s.data(), s.data() + s.size(), value);
    numbers.push_back(value);
  }
  return numbers;
}

vector<string> getLines(std::stringstream &&ss) {
  vector<string> lines{};
  do {
    string line = io::input<string>("", 0, '\n', ss);
    lines.push_back(line);
  } while (lines.back() != "");
  return {lines.begin(), lines.end() - 1};
}
