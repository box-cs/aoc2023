#include "../lib/io.hpp"
#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;
/* types **********************************************************************/
enum HandRank {
  FiveOfAKind = 6,
  FourOfAKind = 5,
  FullHouse = 4,
  ThreeOfAKind = 3,
  TwoPair = 2,
  OnePair = 1,
  HighCard = 0,
};

struct Hand {
  std::string cards;
  int bid;
  HandRank handRank;
};
/* prototypes *****************************************************************/
void partOne(auto lines);
vector<string> getLines(std::stringstream &&ss);
HandRank getHandRank(string hand);
HandRank pairToHandRank(std::pair<int, int> kinds);

static map<const char, const int> faceCards = {
    {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};

int main(int argc, char *argv[]) {
  auto f = io::readFile("input.txt", []() { exit(1); });
  auto lines = getLines(std::move(f));
  partOne(lines);
  return 0;
}

void partOne(auto lines) {
  vector<Hand> hands{};
  for (auto &line : lines) {
    auto spacePos = line.find(" ");
    auto cards = line.substr(0, spacePos);
    auto bid = line.substr(spacePos + 1);
    auto handRank = getHandRank(cards);
    hands.push_back({cards, std::stoi(bid), handRank});
  }

  std::sort(hands.begin(), hands.end(), [](Hand &a, Hand &b) {
    if (a.handRank == b.handRank) {
      for (int i = 0; i < a.cards.size(); i++) {
        char aCard = a.cards[i];
        char bCard = b.cards[i];
        if (aCard == bCard) {
          continue;
        }
        auto aCardValue = faceCards[aCard];
        auto bCardValue = faceCards[bCard];
        return aCardValue < bCardValue;
      }
    }
    return a.handRank < b.handRank;
  });

  uint64_t winnings{};
  for (int i = 0; i < hands.size(); i++) {
    auto hand = hands[i];
    winnings += hand.bid * (i + 1);
  }
  std::cout << "Winnings: " << winnings << '\n';
}

HandRank pairToHandRank(std::pair<int, int> kinds) {
  auto &[first, second] = kinds;
  if (second == 2) {
    if (first == 3) {
      return HandRank::FullHouse;
    };
    if (first == 2) {
      return HandRank::TwoPair;
    };
  }
  if (first == 5)
    return HandRank::FiveOfAKind;
  else if (first == 4)
    return HandRank::FourOfAKind;
  else if (first == 3)
    return HandRank::ThreeOfAKind;
  else if (first == 2)
    return HandRank::OnePair;
  else
    return HandRank::HighCard;
}

HandRank getHandRank(string hand) {
  pair<char, int> highKindCount{};
  pair<char, int> lowKindCount{};
  map<char, int> cardCounts{};
  for (auto card : hand) {
    cardCounts[card]++;
  }

  for (auto [card, count] : cardCounts) {
    if (count > highKindCount.second) {
      lowKindCount = highKindCount;
      highKindCount = {card, count};
    } else if (count > lowKindCount.second) {
      lowKindCount = {card, count};
    }
  }
  return pairToHandRank({highKindCount.second, lowKindCount.second});
}

vector<string> getLines(std::stringstream &&ss) {
  vector<string> lines{};
  do {
    string line = io::input<string>("", 0, '\n', ss);
    lines.push_back(line);
  } while (lines.back() != "");
  return {lines.begin(), lines.end() - 1};
}
