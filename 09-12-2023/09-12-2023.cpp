#include "../lib/io.hpp"
#include <algorithm>
#include <charconv>
#include <cstdint>
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

int main(int argc, char *argv[]) {
  auto f = io::readFile("input.txt", []() { exit(1); });
  auto lines = getLines(std::move(f));
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

  // for (auto &triangle : triangles) {
  //   for (auto &line : triangle) {
  //     for (auto &number : line) {
  //       std::cout << number << " ";
  //     }
  //     std::cout << std::endl;
  //   }
  //   std::cout << std::endl;
  // }

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
  std::cout << "Sum: " << sum << std::endl;
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
