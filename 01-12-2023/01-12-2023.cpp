#include "../lib/io.hpp"
#include <charconv>
#include <iostream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::tuple;
using std::vector;

void partOne(const auto &lines);
void partTwo(const auto &lines);

int main(int argc, char *argv[]) {
  auto f = io::readFile("input.txt");
  const auto lines = io::readLines<string>(
      f, [](auto &ss) { return io::input<string>("", ' ', '\n', ss); });
  partOne(lines);
  partTwo(lines);
}

void partOne(const auto &lines) {
  int sum = 0;
  for (const auto line : lines) {
    string buffer;
    auto nums =
        line | std::views::filter([](char num) { return isdigit(num); });

    buffer += nums.front();
    buffer += nums.back();
    int value{};
    std::from_chars(buffer.data(), buffer.data() + buffer.size(), value);
    sum += value;
  }
  std::cout << "Part One: " << sum << '\n';
}

void partTwo(const auto &lines) {
  int sum = 0;
  map<string, char> digits{{"one", 1},   {"two", 2},   {"three", 3},
                           {"four", 4},  {"five", 5},  {"six", 6},
                           {"seven", 7}, {"eight", 8}, {"nine", 9}};

  for (auto line : lines) {
    // std::tuple<value, pos>
    vector<tuple<string, char>> occurrences{};
    for (auto [digit, value] : digits) {
      auto pos = line.find(digit);
      while (pos != string::npos) { // if we found a digit in the line
        occurrences.push_back(
            // add that digit and it's position to the vector
            std::make_tuple(string{static_cast<char>(value + '0')}, pos));
        // keep looping until we can't find any more digits
        pos = line.find(digit, pos + 1);
      }
    }
    // find digit as char
    for (int i = 0; i < line.size(); i++) {
      if (isdigit(line[i])) {
        occurrences.push_back(std::make_tuple(string{line[i]}, i));
      }
    }

    sort(occurrences.begin(), occurrences.end(),
         [](auto &&a, auto &&b) { return std::get<1>(a) < std::get<1>(b); });
    auto front = get<0>(occurrences.front());
    auto back = get<0>(occurrences.back());
    sum += (stoi(front + back));
  }
  std::cout << "Part Two: " << sum << '\n';
}
