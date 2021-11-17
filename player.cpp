/* Program author
* Name: Sophie Tötterström
* Student number: 050102822
* UserID: knsoto
* E-Mail: sophie.totterstrom@tuni.fi */


#include <iostream>
#include <string>
#include "player.hh"

using namespace std;


Player::Player(const std::string &name):
    name_(name), number_of_pairs_(0)
{
}

std::string Player::get_name() const
// Getter method, returns the name
{
    return name_;
}

unsigned int Player::number_of_pairs() const
// Getter method, returns the number of pairs for a single player
{
    return number_of_pairs_;
}

void Player::add_card(Card) {}
// I chose not to use this method and instead implemented the add_pair method.


void Player::add_pair(Card &card_1, Card &card_2)
// Setter method, takes two Card objects, changes the status to EMPTY and increments the
// players number_of_pairs by one.
{
    card_1.remove_from_game_board();
    card_2.remove_from_game_board();

    number_of_pairs_ += 1;
}

void Player::print() const
// Prints out the information of a player.
{
    cout << "*** " << name_ << " has " << number_of_pairs_ << " pair(s)." << endl;

}
