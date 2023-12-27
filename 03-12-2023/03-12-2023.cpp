#include "../lib/io.hpp"
#include "gear.hpp"
#include <string>
#include <vector>

#define UNSET_POSTION (-1)
using std::string;
using std::vector;

void partOne(std::vector<PartNumber> &partNumbers, std::vector<string> &lines);
void partTwo(vector<PartNumber> &partNumbers, vector<string> &lines);
vector<PartNumber> getPartNumbers(vector<string> &lines);
vector<string> getLines(std::stringstream &&ss);

int main(int argc, char *argv[]) {
  auto file = io::readFile("input.txt", []() { exit(1); });
  auto lines = getLines(std::move(file));
  auto partNumbers = getPartNumbers(lines);
  partOne(partNumbers, lines);
  partTwo(partNumbers, lines);
}

void partOne(std::vector<PartNumber> &partNumbers, std::vector<string> &lines) {
  int32_t sum = 0;
  std::ranges::for_each(partNumbers, [&](auto &partNumber) {
    if (partNumber.isValidPartNumber(lines)) {
      sum += std::stoi(partNumber.value);
    }
  });
  std::cout << "Sum: " << sum << '\n';
}

void partTwo(vector<PartNumber> &partNumbers, vector<string> &lines) {
  vector<Gear> gears{};
  for (int y = 0; y < lines.size(); y++) {
    auto line = lines[y];
    for (int x = 0; x < line.length(); x++) {
      auto ch = line[x];
      if (ch != '*') continue;
      Gear gear{};

      for (auto &[_, direction] : directions) {
        auto &[dy, dx] = direction;
        Point scanPos{x + dx, y + dy};
        auto inBounds = scanPos.X >= 0 &&
                        scanPos.X < lines[scanPos.Y].length() &&
                        scanPos.Y >= 0 && scanPos.Y < lines.size();
        if (!inBounds) continue;
        auto scanChar = lines[scanPos.Y][scanPos.X];
        if (!isdigit(scanChar)) continue;
        auto isScanPartNumber = [&](auto &partNumber) {
          return partNumber.y == scanPos.Y && partNumber.x1 <= scanPos.X &&
                 partNumber.x2 >= scanPos.X;
        };
        auto partNumber = std::find_if(partNumbers.begin(), partNumbers.end(),
                                       isScanPartNumber);
        if (partNumber != partNumbers.end()) {
          auto match = std::find_if(
              gear.adjacentPartNumbers.begin(), gear.adjacentPartNumbers.end(),
              [&](auto &pN) { return pN.id == partNumber->id; });
          if (match == gear.adjacentPartNumbers.end())
            gear.adjacentPartNumbers.push_back(*partNumber);
        }
      }
      gears.push_back(gear);
    }
  }
  long unsigned int sum{};
  for (Gear &gear : gears) {
    if (gear.adjacentPartNumbers.size() == 2) {
      auto ratio = gear.ratio();
      sum += ratio;
    }
  }
  std::cout << "Sum of gear ratios: " << sum << '\n';
}

vector<PartNumber> getPartNumbers(vector<string> &lines) {
  vector<PartNumber> partNumbers{};
  for (int y = 0; y < lines.size(); y++) { // for each line
    auto line = lines[y];
    int x1 = UNSET_POSTION;
    int x2 = UNSET_POSTION;
    for (int x = 0; x <= line.length(); x++) { // for each character
      if (std::isdigit(line[x])) {
        if (x1 == UNSET_POSTION) {
          x1 = x;
        }
        x2 = x;
      } else {
        if (x1 != UNSET_POSTION && x2 != UNSET_POSTION) {
          auto partNumber = PartNumber{y, x1, x2, partNumbers.size()};
          partNumber.value = line.substr(x1, x2 - x1 + 1);
          partNumbers.push_back(partNumber);
        }
        x1 = UNSET_POSTION;
        x2 = UNSET_POSTION;
      }
    }
  }
  return partNumbers;
}

vector<string> getLines(std::stringstream &&ss) {
  vector<string> lines{};
  string line = io::input<string>("", ' ', '\n', ss);
  while (line != "") {
    lines.push_back(line);
    line = io::input<string>("", ' ', '\n', ss);
  }
  return lines;
}
