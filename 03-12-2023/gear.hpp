#include <charconv>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

enum direction { UP, DOWN, LEFT, RIGHT, DDLEFT, DULEFT, DDRIGHT, DURIGHT };
std::map<direction, std::pair<int, int>> directions{
    {UP, {-1, 0}},     {DOWN, {1, 0}},     {LEFT, {0, -1}},
    {RIGHT, {0, 1}},   {DDLEFT, {1, -1}},  {DULEFT, {-1, -1}},
    {DDRIGHT, {1, 1}}, {DURIGHT, {-1, 1}},
};

struct Point {
  int X, Y;
  Point(int x, int y) : X(x), Y(y) {}
};

struct PartNumber {
  int y, x1, x2;
  long unsigned int id;
  std::string value;
  PartNumber(int y, int x1, int x2, long unsigned int id)
      : y(y), x1(x1), x2(x2), id(id) {}
  bool isValidPartNumber(std::vector<std::string> &lines) {
    for (auto &[direction, _] : directions) {
      auto &[dy, dx] = directions[direction];
      for (int x = x1; x <= x2; x++) {
        Point scanPos{x + dx, y + dy};
        if (scanPos.X >= 0 && scanPos.X < lines[scanPos.Y].length() &&
            scanPos.Y >= 0 && scanPos.Y < lines.size()) {
          auto scanChar = lines[scanPos.Y][scanPos.X];
          if (!isdigit(scanChar) && scanChar != '.') {
            return true;
          }
        }
      }
    }
    return false;
  }
};

struct Gear {
  std::vector<PartNumber> adjacentPartNumbers{};

public:
  auto ratio() {
    uint64_t ratio = 1;
    uint64_t value{};
    for (auto &partNumber : adjacentPartNumbers) {
      std::from_chars(partNumber.value.data(),
                      partNumber.value.data() + partNumber.value.size(), value);
      ratio *= value;
    }
    return ratio;
  }
};