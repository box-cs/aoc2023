#include "../lib/io.hpp"
#include <iomanip>
#include <map>
#include <ostream>
#include <ranges>

using std::map;
using std::string;
using std::vector;

struct SubGame {
  static const int MAX_RED_CUBES = 12;
  static const int MAX_GREEN_CUBES = 13;
  static const int MAX_BLUE_CUBES = 14;
  map<string, int> cubeColorMap{{"red", 0}, {"green", 0}, {"blue", 0}};

  bool isValid() {
    return cubeColorMap["red"] <= MAX_RED_CUBES &&
           cubeColorMap["green"] <= MAX_GREEN_CUBES &&
           cubeColorMap["blue"] <= MAX_BLUE_CUBES;
  }
};

struct Game {
  int gameId{};
  vector<SubGame> subGames{};
  Game(int game_id) : gameId(game_id) {}
};

void partOne(vector<Game> &games);
void partTwo(vector<Game> &games);
vector<Game> getGames(std::stringstream &&ss);

int main(int argc, char *argv[]) {
  auto file = io::readFile("input.txt", []() { exit(1); });
  auto games = getGames(std::move(file));
  partOne(games);
  partTwo(games);
}

void partOne(vector<Game> &games) {
  int sum = 0;
  auto validGames =
      games | std::views::filter([](Game &game) {
        return std::ranges::all_of(
            game.subGames, [](SubGame &subGame) { return subGame.isValid(); });
      });
  io::for_each(validGames, [&](auto &game) { sum += game.gameId; });
  std::cout << "Sum: " << sum << '\n';
}

void partTwo(vector<Game> &games) {
  long long int sumOfPowers = 0;
  std::ranges::for_each(games, [&](Game &game) {
    map<string, int> cubeColorMap{{"red", 0}, {"green", 0}, {"blue", 0}};

    std::ranges::for_each(game.subGames, [&](SubGame &subGame) {
      for (auto &[color, count] : subGame.cubeColorMap) {
        if (count > cubeColorMap[color]) {
          cubeColorMap[color] = count;
        }
      }
    });

    int power = 1;
    for (auto &[color, count] : cubeColorMap) {
      power *= count;
    }
    sumOfPowers += power;
  });
  std::cout << "Sum of powers: " << sumOfPowers << '\n';
}

vector<Game> getGames(std::stringstream &&ss) {
  vector<Game> games{};
  string _buffer; // stores tokens that are not needed
  int gameId{};
  do {
    ss >> _buffer >> gameId >> _buffer;
    Game game(gameId);
    auto line =
        io::tokenize<string>("", '\n', ss) |
        std::views::transform([](auto str) {
          str.erase(std::remove(str.begin(), str.end(), ','), str.end());
          return str;
        });

    for (auto it = line.begin(); it != line.end(); it += 2) {
      SubGame subGame{};
      auto cubeCount = std::stoi(*it);
      auto cubeColor = *(it + 1);
      if (cubeColor.contains(';')) {
        cubeColor.erase(std::remove(cubeColor.begin(), cubeColor.end(), ';'),
                        cubeColor.end());
      }
      subGame.cubeColorMap[cubeColor] = cubeCount;
      game.subGames.push_back(subGame);
    }
    games.push_back(game);
  } while (ss.peek() != EOF);
  return games;
}