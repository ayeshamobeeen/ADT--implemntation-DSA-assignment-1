#include "UNO.h"
#include <list>
#include <stack>
#include <map>
#include <vector>
#include <random>
#include <sstream>
#include <algorithm>

using namespace std;

/*
   HiddenState struct:
   
   Stores all the internal data for one UNOGame object.
   Users of the class never see this; it's completely hidden.

   Members:
   - drawPile : stack of Cards representing the deck (LIFO)
   - discardPile : linked list representing the discard pile (allows easy insertion at back)
   - hands : vector of linked lists, each representing a player's hand
   - currentPlayer : index of the player whose turn it is
   - clockwise : direction of play (true = clockwise, false = counter-clockwise)
   - winnerIndex : index of winner (-1 if no winner yet)
*/
struct HiddenState {
    stack<Card> drawPile;
    list<Card> discardPile;
    vector<list<Card>> hands;
    int currentPlayer;
    bool clockwise;
    int winnerIndex;
};

// Global map to associate each UNOGame* with its HiddenState
// This hides the internal implementation, mimicking a private member
static map<const UNOGame*, HiddenState*> hiddenGames;

// Helper function to get or create HiddenState for a given UNOGame
static HiddenState* getHiddenState(const UNOGame* game) {
    if (hiddenGames.find(game) == hiddenGames.end()) {
        hiddenGames[game] = new HiddenState();
    }
    return hiddenGames[game];
}

/*
   Constructor: UNOGame(int numPlayers)
   
   Initializes the UNOGame object with the given number of players.
   - Resizes hands vector to hold 'numPlayers' lists
   - Sets default turn to player 0
   - Sets play direction to clockwise
   - No winner yet (-1)
*/
UNOGame::UNOGame(int numPlayers) {
    HiddenState* state = getHiddenState(this);
    state->hands.resize(numPlayers);
    state->currentPlayer = 0;
    state->clockwise = true;
    state->winnerIndex = -1;
}

/*
   Helper function: createFullDeck()
   
   Builds a standard UNO deck (without wild cards) using colors and numbers/specials.
   - Numbers: 0–9, two of each (except 0)
   - Action cards: Skip, Reverse, Draw Two, two of each
   Returns a vector of Card structs
*/
static vector<Card> createFullDeck() {
    vector<Card> cards;
    vector<string> colors = { "Red", "Green", "Blue", "Yellow" };
    vector<string> specials = { "Skip", "Reverse", "Draw Two" };

    for (auto& col : colors) {
        cards.push_back({ col, "0" }); // Only one zero per color
        for (int i = 1; i <= 9; i++) {
            cards.push_back({ col, to_string(i) });
            cards.push_back({ col, to_string(i) });
        }
        for (auto& sp : specials) {
            cards.push_back({ col, sp });
            cards.push_back({ col, sp });
        }
    }
    return cards;
}

/*
   Function: initialize()
  
   Prepares the game for play:
   1. Creates and shuffles the deck using fixed seed 1234
   2. Deals 7 cards to each player
   3. Places the top card of the deck in the discard pile
   4. Resets currentPlayer, clockwise direction, and winnerIndex
*/
void UNOGame::initialize() {
    HiddenState* state = getHiddenState(this);
    vector<Card> deck = createFullDeck();
    mt19937 rng(1234); // Fixed seed for reproducibility
    shuffle(deck.begin(), deck.end(), rng);

    // Load deck into stack (drawPile)
    while (!state->drawPile.empty()) state->drawPile.pop();
    for (auto& c : deck) state->drawPile.push(c);

    // Deal 7 cards per player
    for (auto& hand : state->hands) {
        hand.clear();
        for (int i = 0; i < 7 && !state->drawPile.empty(); i++) {
            hand.push_back(state->drawPile.top());
            state->drawPile.pop();
        }
    }

    // Place first card in discard pile to start game
    state->discardPile.clear();
    if (!state->drawPile.empty()) {
        state->discardPile.push_back(state->drawPile.top());
        state->drawPile.pop();
    }

    state->currentPlayer = 0;
    state->clockwise = true;
    state->winnerIndex = -1;
}

/*
   Helper function: cardMatches(top, c)
   
   Checks if a card can be played on top of the discard pile
   - Matches if color is same OR value is same
*/
static bool cardMatches(const Card& top, const Card& c) {
    return (top.color == c.color || top.value == c.value);
}

/*
   Helper function: executeAction(state, card)
   
   Applies effects of special action cards:
   - Skip: skips the next player
   - Reverse: reverses the play direction
   - Draw Two: next player draws 2 cards and is skipped
*/
static void executeAction(HiddenState* state, const Card& card) {
    int numPlayers = state->hands.size();

    if (card.value == "Skip") {
        state->currentPlayer = state->clockwise ?
            (state->currentPlayer + 1) % numPlayers :
            (state->currentPlayer - 1 + numPlayers) % numPlayers;
    }
    else if (card.value == "Reverse") {
        state->clockwise = !state->clockwise;
    }
    else if (card.value == "Draw Two") {
        int next = state->clockwise ?
            (state->currentPlayer + 1) % numPlayers :
            (state->currentPlayer - 1 + numPlayers) % numPlayers;
        for (int i = 0; i < 2 && !state->drawPile.empty(); i++) {
            state->hands[next].push_back(state->drawPile.top());
            state->drawPile.pop();
        }
        state->currentPlayer = next; // skip next player
    }
}

/*
   Function: playTurn()
   
   Executes a single turn for the current player:
   1. Checks hand for playable cards (color match > value match > action card)
   2. If no playable card, draws one card from deck
   3. Plays the card if possible and applies any action
   4. Updates winner if hand is empty
   5. Advances currentPlayer according to direction
*/
void UNOGame::playTurn() {
    HiddenState* state = getHiddenState(this);
    if (state->winnerIndex != -1) return; // game already over

    int pid = state->currentPlayer;
    auto& hand = state->hands[pid];
    Card top = state->discardPile.back();

    // Step 1: Find playable card in hand
    auto it = hand.end();
    for (auto h = hand.begin(); h != hand.end(); ++h) {
        if (h->color == top.color) { it = h; break; }
    }
    if (it == hand.end()) {
        for (auto h = hand.begin(); h != hand.end(); ++h) {
            if (h->value == top.value) { it = h; break; }
        }
    }
    if (it == hand.end()) {
        for (string act : {"Skip", "Reverse", "Draw Two"}) {
            for (auto h = hand.begin(); h != hand.end(); ++h) {
                if (h->value == act && cardMatches(top, *h)) { it = h; break; }
            }
            if (it != hand.end()) break;
        }
    }

    // Step 2: If no card, draw one from deck
    if (it == hand.end()) {
        if (!state->drawPile.empty()) {
            Card drawn = state->drawPile.top(); state->drawPile.pop();
            if (cardMatches(top, drawn)) {
                state->discardPile.push_back(drawn);
                executeAction(state, drawn);
            }
            else {
                hand.push_back(drawn);
            }
        }
    }
    else {
        // Step 3: Play selected card and apply action
        Card played = *it;
        state->discardPile.push_back(played);
        hand.erase(it);
        executeAction(state, played);
    }

    // Step 4: Check if current player won
    if (hand.empty()) state->winnerIndex = pid;

    // Step 5: Advance turn if game not over
    if (state->winnerIndex == -1) {
        int n = state->hands.size();
        state->currentPlayer = state->clockwise ?
            (state->currentPlayer + 1) % n :
            (state->currentPlayer - 1 + n) % n;
    }
}

/*
   Function: isGameOver()
   
   Returns true if any player has won (hand is empty)
*/
bool UNOGame::isGameOver() const {
    return getHiddenState(this)->winnerIndex != -1;
}

/*
   Function: getWinner()
   
   Returns the index of the winning player, or -1 if no winner yet
*/
int UNOGame::getWinner() const {
    return getHiddenState(this)->winnerIndex;
}

/*
   Function: getState()
  
   Returns a formatted string representing the current game state:
   - Player's turn
   - Direction of play
   - Top card on discard pile
   - Number of cards each player holds
*/
string UNOGame::getState() const {
    HiddenState* state = getHiddenState(this);
    stringstream ss;

    ss << "Player " << state->currentPlayer
        << "'s turn, Direction: " << (state->clockwise ? "Clockwise" : "Counter-clockwise")
        << ", Top: " << state->discardPile.back().color
        << " " << state->discardPile.back().value
        << ", Players cards: ";

    for (size_t i = 0; i < state->hands.size(); i++) {
        ss << "P" << i << ":" << state->hands[i].size();
        if (i != state->hands.size() - 1) ss << ", ";
    }

    return ss.str();
}
