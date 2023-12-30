#include "../lib/io.hpp"
#include <cctype>
#include <charconv>
#include <cmath>
#include <format>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

typedef long long int lli;

using std::string;
using std::stringstream;
using std::vector;

void partOne(auto lines);
void partTwo(auto lines);
lli getWaysToWin(lli raceTime, lli recordDistance);
auto solveQuadratic(lli a, lli b, lli c);

int main(int argc, char *argv[]) {
  auto f = io::readFile("input.txt");
  auto lines = io::readLines<string>(
      f, [](auto &ss) { return io::input<string>("", 0, '\n', ss); });
  partOne(lines);
  partTwo(lines);
  return 0;
}

void partOne(auto lines) {
  auto range = lines | std::views::transform([](auto &&line) {
                 return line | std::views::split(':') | std::views::drop(1) |
                        std::views::transform([](auto &&range) {
                          auto str = string{std::begin(range), std::end(range)};
                          auto ss = stringstream{str};
                          return io::tokenize<int>("", 0, ss);
                        });
               }) |
               std::views::join;

  auto raceTimes = *(range.begin());
  auto recordDistances = *(++range.begin());

  int race = 0;
  int answer = 0;
  for (int i = 0; i < raceTimes.size(); i++) {
    int waysToWin = getWaysToWin(raceTimes[i], recordDistances[i]);
    if (answer == 0) {
      answer = waysToWin;
    } else {
      answer *= waysToWin;
    }
  }
  std::cout << "Answer: " << answer << '\n';
}

void partTwo(auto lines) {
  auto range = lines | std::views::transform([](auto &&line) {
                 return line | std::views::split(':') | std::views::drop(1) |
                        std::views::transform([](auto &&range) {
                          lli num{};
                          auto str = range | std::views::filter([](auto &&c) {
                                       return !std::isspace(c);
                                     });
                          auto value = string{std::begin(str), std::end(str)};
                          std::from_chars(value.data(),
                                          value.data() + value.size(), num);
                          return num;
                        });
               }) |
               std::views::join;
  auto time = *(range.begin());
  auto distance = *(++range.begin());
  auto [x1, x2] = solveQuadratic(-1, +time, -distance);
  // This measly round cost me like 20 minutes
  std::cout << "Part Two: " << std::format("{}", round(x2 - x1)) << '\n';
}

lli getWaysToWin(lli raceTime, lli recordDistance) {
  lli waysToWin = 0;
  for (lli heldDuration = 0; heldDuration <= raceTime; heldDuration++) {
    lli releasedDuration = raceTime - heldDuration;
    lli millimeters = releasedDuration * heldDuration;
    if (millimeters > recordDistance) {
      waysToWin++;
    }
  }
  return waysToWin;
}

auto solveQuadratic(lli a, lli b, lli c) {
  return std::pair{(-b + sqrt(b * b - 4 * a * c)) / (2 * a),
                   (-b - sqrt(b * b - 4 * a * c)) / (2 * a)};
}