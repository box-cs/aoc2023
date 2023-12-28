#include "../lib/io.hpp"
#include <iostream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;
namespace ranges = std::ranges;
namespace views = std::views;

typedef std::map<int, int> CardDict;

vector<string> getLines(stringstream &&ss);
int getMatches(auto numbersIter, auto winningNumbersIter);
void partOne(vector<string> lines);
void partTwo(vector<string> lines);

int main(int argc, char *argv[]) {
  auto file = io::readFile("input.txt", []() { exit(1); });
  auto lines = getLines(std::move(file));
  partOne(lines);
  partTwo(lines);
}

void partOne(vector<string> lines) {
  int sumOfPoints = 0;
  for (auto line : lines) {
    auto range = line | views::drop_while([](char c) { return c != ':'; }) |
                 views::drop(1) | views::split('|');

    auto [numbersIter, winningNumbersIter] =
        std::pair{*(range.begin()), *(++range.begin())};

    auto matches = getMatches(numbersIter, winningNumbersIter);
    if (matches > 0) {
      sumOfPoints += 1 << (matches - 1);
    }
  }
  std::cout << "Points: " << sumOfPoints << '\n';
}

void partTwo(vector<string> lines) {
  int totalCards = 0;
  int cardNumber = 1;
  CardDict cards{};
  for (auto line : lines) {
    cards[cardNumber] += 1;
    auto range = line | views::drop_while([](char c) { return c != ':'; }) |
                 views::drop(1) | views::split('|');
    auto [numbersIter, winningNumbersIter] =
        std::pair{*(range.begin()), *(++range.begin())};

    auto matches = getMatches(numbersIter, winningNumbersIter);
    for (int i = 1; i <= matches; i++) {
      cards[cardNumber + i] += cards[cardNumber];
    }
    cardNumber++;
  }
  for (auto [cardNumber, count] : cards) {
    totalCards += count;
  }
  std::cout << "Total cards: " << totalCards << '\n';
}

int getMatches(auto numbersIter, auto winningNumbersIter) {
  auto ss = stringstream{string{numbersIter.begin(), numbersIter.end()}};
  auto numbers = io::tokenize<int>("", 0, ss);

  ss = stringstream{
      string{winningNumbersIter.begin(), winningNumbersIter.end()}};
  auto winningNumbers = io::tokenize<int>("", 0, ss);

  return ranges::count_if(numbers, [&](int number) {
    return ranges::find(winningNumbers, number) != winningNumbers.end();
  });
}

vector<string> getLines(std::stringstream &&ss) {
  vector<string> lines{};
  do {
    string line = io::input<string>("", 0, '\n', ss);
    lines.push_back(line);
  } while (lines.back() != "");
  return {lines.begin(), lines.end() - 1};
}
