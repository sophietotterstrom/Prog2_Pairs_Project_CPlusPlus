/* Program author
* Name: Sophie Tötterström
* Student number: 050102822
* UserID: knsoto
* E-Mail: sophie.totterstrom@tuni.fi */

/* Class: Player
 * -------------
 * Represents a single player in pairs (memory) game.
 *
 * COMP.CS.110 K2021
 * */

#ifndef PLAYER_HH
#define PLAYER_HH

#include "card.hh"
#include <string>

using namespace std;

class Player
{
public:

    // Constructor: creates a player with the given name.
    Player(const std::string& name);

    // Returns the name of the player.
    std::string get_name() const;

    // Returns the number of pairs collected by the player so far.
    unsigned int number_of_pairs() const;

    // Method not implemented. I used add_pair instead.
    void add_card(Card);

    // Used instead of add_card. Moves two given cards from the game board to the player,
    // increments the player's score and removes them from the game board.
    void add_pair(Card &card_1, Card &card_2);

    // Prints the game status of the player: name and collected pairs so far.
    void print() const;

private:
    string name_;
    unsigned int number_of_pairs_;
};

#endif // PLAYER_HH
