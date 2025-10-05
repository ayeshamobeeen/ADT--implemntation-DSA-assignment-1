// UNO.cpp
#include "UNO.h"
#include <vector>
#include <list>
#include <random>
#include <algorithm>
#include <sstream>
#include <iostream>

// Concrete implementation (hidden from the header)
class UNOGameImpl : public UNOGame {
public:
    UNOGameImpl(int n)
        : numPlayers(n), currentPlayer(0), direction(1), winner(-1)
    {
        hands.assign(numPlayers, std::list<Card>{});
    }

    void initialize() override {
        buildDeck();
        std::mt19937 rng(1234);
        std::shuffle(deck.begin(), deck.end(), rng);

        // Deal 7 cards to each player
        for (int i = 0; i < 7; ++i) {
            for (int p = 0; p < numPlayers; ++p) {
                hands[p].push_back(deck.back());
                deck.pop_back();
            }
        }

        // Start discard pile with top card
        if (!deck.empty()) {
            discard.push_back(deck.back());
            deck.pop_back();
        }
    }

    void playTurn() override {
        if (isGameOver()) return;

        Card top = discard.back();
        Card playedCard;
        bool played = tryPlayFromHand(currentPlayer, top, playedCard);

        // If didn't play from hand, draw one and try to play it
        if (!played) {
            if (!deck.empty()) {
                Card drawn = deck.back(); deck.pop_back();
                hands[currentPlayer].push_back(drawn);
                // check playable immediately (with same priority rules)
                played = tryPlayFromHand(currentPlayer, top, playedCard);
            }
        }

        // If a card was played, it's already placed on discard by tryPlayFromHand
        // Apply action effects (we'll decide how many times to advance)
        int advances = 1; // normally move to next player once at end

        if (played) {
            const std::string& v = playedCard.value;
            if (v == "Skip") {
                advances += 1; // skip next
            }
            else if (v == "Reverse") {
                direction *= -1;
                // With 2 players Reverse acts like Skip in UNO
                if (numPlayers == 2) advances += 1;
            }
            else if (v == "Draw Two") {
                // Next player draws 2 and gets skipped
                int nxt = nextPlayer();
                for (int i = 0; i < 2 && !deck.empty(); ++i) {
                    hands[nxt].push_back(deck.back());
                    deck.pop_back();
                }
                advances += 1; // skip the drawn player
            }
        }

        // Check winner before advancing (if someone emptied their hand)
        for (int p = 0; p < numPlayers; ++p) {
            if (hands[p].empty()) {
                winner = p;
                break;
            }
        }

        // Advance turn 'advances' times (handles skips)
        for (int i = 0; i < advances; ++i) advancePlayer();
    }

    bool isGameOver() const override {
        return winner != -1;
    }

    int getWinner() const override {
        return winner;
    }

    std::string getState() const override {
        std::ostringstream oss;
        oss << "Player " << currentPlayer << "'s turn, Direction: "
            << (direction == 1 ? "Clockwise" : "Counter-clockwise")
            << ", Top: " << (discard.empty() ? "None" : discard.back().color + std::string(" ") + discard.back().value)
            << ", Players cards: ";

        for (int i = 0; i < numPlayers; ++i) {
            if (i) oss << ", ";
            oss << "P" << i << ":" << hands[i].size();
        }
        return oss.str();
    }

private:
    int numPlayers;
    int currentPlayer;      // index of current player
    int direction;          // 1 = clockwise, -1 = counter-clockwise
    int winner;             // -1 if none, else player index

    std::vector<Card> deck;             // draw deck (vector for easy shuffle/pop_back)
    std::vector<std::list<Card>> hands; // each player's hand as linked list (std::list)
    std::list<Card> discard;            // discard pile (we only need back())

    void buildDeck() {
        deck.clear();
        const std::vector<std::string> colors = { "Red","Green","Blue","Yellow" };
        const std::vector<std::string> actions = { "Skip","Reverse","Draw Two" };

        for (const auto& c : colors) {
            // 0 once
            deck.push_back({ c, "0" });
            // 1-9 twice each
            for (int i = 1; i <= 9; ++i) {
                deck.push_back({ c, std::to_string(i) });
                deck.push_back({ c, std::to_string(i) });
            }
            // action cards: 2 of each per color
            for (const auto& a : actions) {
                deck.push_back({ c, a });
                deck.push_back({ c, a });
            }
        }
    }

    // returns true if a card was played; if true, playedCard is set (and put onto discard)
    bool tryPlayFromHand(int player, const Card& top, Card& playedCard) {
        auto& hand = hands[player];

        // 1) color match (first found)
        for (auto it = hand.begin(); it != hand.end(); ++it) {
            if (it->color == top.color) {
                playedCard = *it;
                discard.push_back(playedCard);
                hand.erase(it);
                return true;
            }
        }

        // 2) value match (first found)
        for (auto it = hand.begin(); it != hand.end(); ++it) {
            if (it->value == top.value) {
                playedCard = *it;
                discard.push_back(playedCard);
                hand.erase(it);
                return true;
            }
        }

        // 3) action cards priority (Skip, Reverse, Draw Two) among playable cards
        const std::vector<std::string> pri = { "Skip", "Reverse", "Draw Two" };
        for (const auto& action : pri) {
            for (auto it = hand.begin(); it != hand.end(); ++it) {
                if (it->value == action) {
                    // ensure it's actually playable (should match color or value)
                    if (it->color == top.color || it->value == top.value) {
                        playedCard = *it;
                        discard.push_back(playedCard);
                        hand.erase(it);
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void advancePlayer() {
        currentPlayer = (currentPlayer + direction + numPlayers) % numPlayers;
    }

    int nextPlayer() const {
        return (currentPlayer + direction + numPlayers) % numPlayers;
    }
};

// Factory function (defined in UNO.cpp, NOT in header)
UNOGame* createUNOGame(int numPlayers) {
    return new UNOGameImpl(numPlayers);
}
