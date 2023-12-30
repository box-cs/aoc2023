#include "../lib/io.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
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
  string cards;
  int bid;
  HandRank handRank;
  pair<char, int> high; // part2
  pair<char, int> low;  // part2

  string getPartTwoCards() {
    auto jPositions = [&]() {
      vector<int> positions{};
      for (int i = 0; i < cards.size(); i++) {
        if (cards[i] == 'J') {
          positions.push_back(i);
        }
      }
      return positions;
    }();
    if (jPositions.size() > 0) {
      string copy{};
      for (int i = 0; i < cards.size(); i++) {
        copy += cards[i];
      }
      for (auto pos : jPositions) {
        auto highCard = high.first == 'J' ? low.first : high.first;
        auto lowCard = low.first == 'J' ? high.first : low.first;
        copy[pos] = highCard;
      }
      return copy;
    } else {
      return cards;
    }
  };
};
/* prototypes *****************************************************************/
void partOne(auto lines);
void partTwo(auto lines);
HandRank pairToHandRank(pair<int, int> kinds);
pair<pair<char, int>, pair<char, int>> getPair(string hand);

static map<char, int> faceCards = {
    {'2', 2}, {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7}, {'8', 8},
    {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}};

int main(int argc, char *argv[]) {
  auto f = io::readFile("input.txt");
  auto lines = io::readLines<string>(
      f, [](auto &ss) { return io::input<string>("", 0, '\n', ss); });
  partOne(lines);
  partTwo(lines);
  return 0;
}

void partOne(auto lines) {
  vector<Hand> hands{};
  for (auto &line : lines) {
    auto spacePos = line.find(" ");
    auto cards = line.substr(0, spacePos);
    auto bid = line.substr(spacePos + 1);
    auto [high, low] = getPair(cards);
    auto handRank = pairToHandRank({high.second, low.second});
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
  std::cout << "Part One: " << winnings << '\n';
}

void partTwo(auto lines) {
  vector<Hand> hands{};
  auto faceCardsWithJoker = map<char, int>();
  for (auto &[k, v] : faceCards) {
    if (k == 'J') {
      faceCardsWithJoker['J'] = 1;
    } else {
      faceCardsWithJoker[k] = v;
    }
  }
  for (auto &line : lines) {
    auto spacePos = line.find(" ");
    auto cards = line.substr(0, spacePos);
    auto bid = line.substr(spacePos + 1);
    auto [high, low] = getPair(cards);
    auto handRank = pairToHandRank({high.second, low.second});
    hands.push_back({cards, std::stoi(bid), handRank, high, low});
  }

  std::sort(hands.begin(), hands.end(), [&](Hand &a, Hand &b) {
    auto aCards = a.getPartTwoCards();
    auto bCards = b.getPartTwoCards();
    auto [aHigh, aLow] = getPair(aCards);
    auto [bHigh, bLow] = getPair(bCards);
    auto aHandRank = pairToHandRank({aHigh.second, aLow.second});
    auto bHandRank = pairToHandRank({bHigh.second, bLow.second});

    if (aHandRank == bHandRank) {
      for (int i = 0; i < aCards.size(); i++) {
        char aCard = a.cards[i];
        char bCard = b.cards[i];
        if (aCard == bCard) {
          continue;
        }
        auto aCardValue = faceCardsWithJoker[aCard];
        auto bCardValue = faceCardsWithJoker[bCard];
        return aCardValue < bCardValue;
      }
    }
    return aHandRank < bHandRank;
  });

  uint64_t winnings{};
  for (int i = 0; i < hands.size(); i++) {
    auto hand = hands[i];
    winnings += hand.bid * (i + 1);
  }
  std::cout << "Part Two: " << winnings << '\n';
}

HandRank pairToHandRank(pair<int, int> kinds) {
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

pair<pair<char, int>, pair<char, int>> getPair(string hand) {
  pair<char, int> highKindCount{};
  pair<char, int> secondHighCount{};
  map<char, int> cardCounts{};
  for (auto card : hand) {
    cardCounts[card]++;
  }
  vector<pair<char, int>> sorted{};
  for (auto &it : cardCounts) {
    sorted.push_back(it);
  }
  sort(sorted.begin(), sorted.end(),
       [](pair<char, int> &a, pair<char, int> &b) {
         return a.second < b.second;
       });
  auto last = sorted.size() - 1;
  highKindCount = pair{sorted[last].first, sorted[last].second};
  secondHighCount = pair{sorted[last - 1].first, sorted[last - 1].second};
  return pair{highKindCount, secondHighCount};
}