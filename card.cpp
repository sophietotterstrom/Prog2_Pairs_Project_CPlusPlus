/* Program author
* Name: Sophie Tötterström
* Student number: 050102822
* UserID: knsoto
* E-Mail: sophie.totterstrom@tuni.fi */


#include "card.hh"
#include <iostream>

using namespace std;

Card::Card():
    letter_(EMPTY_CHAR), visibility_(EMPTY)
{

}

Card::Card(const char c):
    letter_(c), visibility_(HIDDEN)
{

}

void Card::set_letter(const char c)
{
    letter_ = c;
}

void Card::set_visibility(const Visibility_type visibility)
{
    visibility_ = visibility;
}

char Card::get_letter() const
{
    return letter_;
}

Visibility_type Card::get_visibility() const
{
    return visibility_;
}

void Card::turn()
{
    if(visibility_ == HIDDEN)
    {
        visibility_ = OPEN;
    }
    else if(visibility_ == OPEN)
    {
        visibility_ = HIDDEN;
    }
    else if(visibility_ == EMPTY)
    {
        std::cout << "Cannot turn an empty place." << std::endl;
    }
}

void Card::print() const
// Prints a character on the game board for a card, depends on the status of the card.
{

    if(visibility_ == HIDDEN)
    {
        cout << "#";
    }
    else if(visibility_ == OPEN)
    {
        cout << letter_;
    }
    else if(visibility_ == EMPTY)
    {
        cout << ".";
    }
}

void Card::remove_from_game_board()
{
    visibility_ = EMPTY;
}
