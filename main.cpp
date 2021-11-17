/* Pairs
 *
 * Desc:
 *  This program generates a pairs (memory) game. The game has a variant
 * number of cards and players. At the beginning, the program also asks for a
 * seed value, since the cards will be set randomly in the game board.
 *  On each round, the player in turn gives the coordinates of two cards
 * (totally four numbers). After that the given cards will be turned as
 * visible and told if they are pairs or not. If they are pairs, they are
 * removed from the game board, the score of the player is increased, and
 * a new turn is given for the player. If the cards are not pairs, they
 * will be  turned hidden again, and the next player will be in turn.
 *  The program checks if the user-given coordinates are legal. The cards
 * determined by the coordinates must be found in the game board.
 *  After each change, the game board is printed again. The cards are
 * described as letters, starting from A and lasting so far as there are
 * cards. In printing the game board, a visible card is shown as its letter,
 * a hidden one as the number sign (#), and a removed one as a dot.
 *  Game will end when all pairs have been found, and the game board is
 * empty. The program tells who has/have won, i.e. collected most pairs.
 *
 * Program author
 * Name: Sophie Tötterström
 * Student number: 050102822
 * UserID: knsoto
 * E-Mail: sophie.totterstrom@tuni.fi
 *
 * Notes about the program and it's implementation:
 *
 * */


#include <player.hh>
#include <card.hh>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

const string INPUT_AMOUNT_OF_CARDS = "Enter the amount of cards (an even number): ";
const string INPUT_SEED = "Enter a seed value: ";
const string INPUT_AMOUNT_OF_PLAYERS = "Enter the amount of players (one or more): ";
const string INPUT_CARDS = ": Enter two cards (x1, y1, x2, y2), or q to quit: ";
const string INVALID_CARD = "Invalid card.";
const string FOUND = "Pairs found.";
const string NOT_FOUND = "Pairs not found.";
const string GIVING_UP = "Why on earth you are giving up the game?";
const string GAME_OVER = "Game over!";

using Game_row_type = vector<Card>;
using Game_board_type = vector<vector<Card>>;

// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns 0
// (which leads to an invalid card later).
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
    {
        return stoi(str);
    }
    else
    {
        return 0;
    }
}

// Fills the game board, the size of which is rows * columns, with empty cards.
void init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns)
{
    g_board.clear();
    Game_row_type row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        g_board.push_back(row);
    }
}

// Finds the next free position in the game board (g_board), starting from the
// given position start and continuing from the beginning if needed.
// (Called only by the function init_with_cards.)
unsigned int next_free(Game_board_type& g_board, unsigned int start)
{

    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Starting from the given value
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY) // vaihdettu
        {
            return i;
        }
    }

    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }

    // You should never reach this
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Initializes the given game board (g_board) with randomly generated cards,
// based on the given seed value.
void init_with_cards(Game_board_type& g_board, int seed)
{

    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);

    // Wiping out the first random number (that is always the lower bound of the distribution)
    distr(randomEng);

    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {

        // Adding two identical cards (pairs) in the game board
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(g_board, cell);
            g_board.at(cell / columns).at(cell % columns).set_letter(c);
            g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
        }
    }
}

// Prints a line consisting of the given character c.
// The length of the line is given in the parameter line_length.
// (Called only by the function print.)
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length * 2 + 7; ++i)
    {
        cout << c;
    }
    cout << endl;
}

// Prints a variable-length game board with borders.
void print(const Game_board_type& g_board)
{
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    print_line_with_char('=', columns);
    cout << "|   | ";
    for(unsigned int i = 0; i < columns; ++i)
    {
        cout << i + 1 << " ";
    }
    cout << "|" << endl;
    print_line_with_char('-', columns);
    for(unsigned int i = 0; i < rows; ++i)
    {
        cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < columns; ++j)
        {
            g_board.at(i).at(j).print();
            cout << " ";
        }
        cout << "|" << endl;
    }
    print_line_with_char('=', columns);
}

// Asks the desired product from the user, and calculates the factors of
// the product such that the factor as near to each other as possible.
void ask_product_and_calculate_factors(unsigned int& smaller_factor, unsigned int& bigger_factor)
{
    unsigned int product = 0;
    while(not (product > 0 and product % 2 == 0))
    {
        std::cout << INPUT_AMOUNT_OF_CARDS;
        string product_str = "";
        std::getline(std::cin, product_str);
        product = stoi_with_check(product_str);
    }

    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}

// Asks for user input on the number of players and their names.
void ask_amount_and_names_of_players(vector<Player>& players)
{
    // Initializing and asking for input.
    string number_of_players;
    cout << INPUT_AMOUNT_OF_PLAYERS;
    cin >> number_of_players;

    // Asking for input (number of players) until the input is valid.
    while (stoi_with_check(number_of_players) <= 0)
    {
        cout << INPUT_AMOUNT_OF_PLAYERS;
        cin >> number_of_players;
    }

    // Initializing and asking for the names of the players.
    int num_of_players = stoi_with_check(number_of_players);
    cout << "List " << num_of_players << " players: ";

    // Making sure there are enough players.
    // Initializing a Player object of each player.
    int count = 0;
    while (count < num_of_players)
    {
        string player_name;
        cin >> player_name;

        Player player_object = Player(player_name);
        players.push_back(player_object);

        count += 1;
    }
}

// Function returns true if the user given coordinates are the same and input is invalid.
// Used as an assistive function for the input checker function.
bool are_the_coordinates_the_same(vector<int> coordinates)
{
    // Checks if the x-coordinates or the y-coordinates are the same.
    if ((coordinates[0] == coordinates[2]) and (coordinates[1] == coordinates[3]))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Checks user input, if it is valid, returns true.
bool is_input_valid(vector<string> input, Game_board_type& g_board)
{
    // Initializing
    string character;
    vector<int> list_of_characters_in_input;

    // Finding out the number of rows and columns of the game board
    unsigned int max_rows = g_board.size();
    unsigned int max_columns = g_board.at(0).size();

    // Goes through each character in the user input vector.
    for (int index = 0; index < 4; index++)
    {
        character = input[index];
        unsigned int number = stoi_with_check(character);

        // Checks if the character is a letter or number, and takes care of coordinates that are less than 1.
        // If it is a character or an int which is less than one, returns false.
        if (number < 1)
        {
            return false;
        }

        // Because user inputs x-coordinates first, we can get those and check them:
        if (index % 2 == 0) // Character is a column coordinate
        {
            if (number > max_columns)
            {
                return false;
            }
        }
        else // character is a row coordinate
        {
            if (number > max_rows)
            {
                return false;
            }
        }

        // Now we know the coordinate is a number and on the game board. Now we can
        // add it to a vector of coordinates, which is used for the last input checking.
        list_of_characters_in_input.push_back(number);
    }

    // Here we do the final input checking by using a separate function.
    // If that function returns true, the input is invalid.
    if (are_the_coordinates_the_same(list_of_characters_in_input) == true)
    {
        return false;
    }

    // This checks if a card spot in the game board is empty.

    // Initializing coordinates. Substracting 1 as index of game boards starts at 0.
    int xcoordinate1 = (int)stoi_with_check(input[0])-1;
    int ycoordinate1 = (int)stoi_with_check(input[1])-1;
    int xcoordinate2 = (int)stoi_with_check(input[2])-1;
    int ycoordinate2 = (int)stoi_with_check(input[3])-1;

    // At specific coordinates of the game board we can find a Card object.
    // First g_board.at() calls the row vector at the given index, finds a column vector.
    // Second .at() calls a specific Card object from a column vector.
    // To this Card object we call Card class method get_visibility. If the
    // card is empty, it cannot be turned again and thus is invalid input.
    if (g_board.at(ycoordinate1).at(xcoordinate1).get_visibility() == EMPTY)
    {
        return false;
    }
    else if (g_board.at(ycoordinate2).at(xcoordinate2).get_visibility() == EMPTY)
    {
        return false;
    }

    // In all other occurances, we have determined the input is valid, thus we return true.
    return true;
}

// Prints the status of all players using a Player class method print in a loop.
void printing_score_status(vector<Player> players)
{
    for (Player object : players)
    {
        object.print();
    }
}

// Turns two cards. Checks if the cards are a match and returns true if they are.
bool turning_two_cards(Game_board_type& g_board, Player& player, vector<string> coordinates)
{

    // Initializing coordinates. Substracting 1 as index of game boards starts at 0.
    int xcoordinate1 = (int)stoi_with_check(coordinates[0])-1;
    int ycoordinate1 = (int)stoi_with_check(coordinates[1])-1;
    int xcoordinate2 = (int)stoi_with_check(coordinates[2])-1;
    int ycoordinate2 = (int)stoi_with_check(coordinates[3])-1;

    // Same way of finding card objects as in the input checker:
    // At specific coordinates of the game board we can find a Card object.
    // First g_board.at() calls the row vector at the given index, finds a column vector.
    // Second .at() calls a specific Card object from a column vector.

    // Then call a Card method to to Card class object.
    g_board.at(ycoordinate1).at(xcoordinate1).turn();
    g_board.at(ycoordinate2).at(xcoordinate2).turn();

    // Printing the game board when the cards have been turned.
    print(g_board);

    // Now we check if the turned cards are a pair. First we find the
    // values/letters of the two turned cards.
    char letter1 = g_board.at(ycoordinate1).at(xcoordinate1).get_letter();
    char letter2 = g_board.at(ycoordinate2).at(xcoordinate2).get_letter();

    // If the letters are the same, cards are a pair.
    // We call the Player class method add_pair to increment player's score
    // and remove the cards from the board.
    if (letter1 == letter2)
    {
        player.add_pair(g_board.at(ycoordinate1).at(xcoordinate1), g_board.at(ycoordinate2).at(xcoordinate2));
        return true;
    }
    else // Cards are not a pair. Turn them back and return false.
    {
        g_board.at(ycoordinate1).at(xcoordinate1).turn();
        g_board.at(ycoordinate2).at(xcoordinate2).turn();
        return false;
    }
}

// Checks after each turn to see if the game is over. Returns true when it is.
bool is_game_over(unsigned int fac1, unsigned int fac2, vector<Player>& players)
{
    // We can get the total number of pairs by dividing the total number of
    // cards on the game board by two.
    int total_number_of_pairs = (fac1 * fac2)/2;
    int total_score_of_pairs = 0;

    // Going through every playe and finding the scores.
    for (Player player : players)
    {
        total_score_of_pairs += player.number_of_pairs();
    }

    if (total_score_of_pairs == total_number_of_pairs)
    {
        // Game is over
        return true;
    }
    return false;
}

// Checks the winner of the game. This function is called when game is over.
void who_won(vector<Player> players)
{
    // Initializing
    unsigned int highest_score = 0;
    string winners_name;
    int number_of_players_who_have_tied = 1;

    // Loops through all players.
    for (Player player : players)
    {
        // If the player has the most pairs thus far, we name them the
        // winner (until someone else has more pairs) and reset the
        // number of ties.
        if (highest_score < player.number_of_pairs())
        {
            highest_score = player.number_of_pairs();
            winners_name = player.get_name();
            number_of_players_who_have_tied = 1;
        }
        // In this case, all players thus far have tied. Incrementing the
        // number of ties and resetting the name of the winner.
        else if (highest_score == player.number_of_pairs())
        {
            number_of_players_who_have_tied += 1;
            winners_name = "";
        }
    }

    // Necessary print for when a game is over. If winner's name is an empty string,
    // the game was a tie. Else print the winner and their score.
    cout << GAME_OVER << endl;

    if (winners_name == "")
    {
        cout << "Tie of " << number_of_players_who_have_tied << " players with " << highest_score << " pairs." << endl;
    }
    else
    {
        cout << winners_name << " has won with " << highest_score << " pairs." << endl;
    }
}

int main()
{
    Game_board_type game_board;

    unsigned int factor1 = 1;
    unsigned int factor2 = 1;
    ask_product_and_calculate_factors(factor1, factor2);
    init_with_empties(game_board, factor1, factor2);

    string seed_str = "";
    std::cout << INPUT_SEED;
    std::getline(std::cin, seed_str);
    int seed = stoi_with_check(seed_str);
    init_with_cards(game_board, seed);


    // Calling a function to ask for input and add the given players
    // to a vector of Player objects.
    vector<Player> players;
    ask_amount_and_names_of_players(players);

    // Main game loop.
    while (true)
    {
        // For loop loops through the players, takes care of turns.
        for (int index = 0; index < (int)players.size(); index++)
        {
            print(game_board);

            // Initializing user coordinates.
            string x1, x2, x3, x4;

            // Asking for user coordinate input, first only cin the first character
            // to use the "q" quit command.
            cout << players[index].get_name() << INPUT_CARDS;
            cin >> x1;

            if (x1 == "q")
            {
                cout << GIVING_UP << endl;
                return EXIT_SUCCESS;
            }

            // If player is not trying to quit, cin the rest of the coordinates.
            // Creating a vector of strings of the given coordinates for
            // efficient input checking.
            cin >> x2 >> x3 >> x4;
            vector<string> input = {x1, x2, x3, x4};

            // Keeps asking for input until coordinates are valid.
            // Same structure as before.
            while (is_input_valid(input, game_board) == false)
            {
                cout << INVALID_CARD << endl;
                cout << players[index].get_name() << INPUT_CARDS;
                cin >> x1;

                if (x1 == "q")
                {
                    cout << GIVING_UP << endl;
                    return EXIT_SUCCESS;
                }

                cin >> x2 >> x3 >> x4;
                input = {x1, x2, x3, x4};
            }

            // Initializing. This function takes valid input coordinates and
            // checks if the cards at those coordinates are a match.
            bool did_player_get_a_pair;
            did_player_get_a_pair = turning_two_cards(game_board, players[index], input);

            // If cards are a match/pair, function returns true. We give the
            // correct message and decrese the index to reset the players turn
            // (loop through the for loop with the same index).
            if (did_player_get_a_pair == true)
            {
                cout << FOUND << endl;
                index--;
            }
            else // Pair not found, print correct message and continue to the next player.
            {
                cout << NOT_FOUND << endl;
            }

            printing_score_status(players);

            // At the end of each turn check to see if the game is over.
            // Else continue onto the next player.
            if (is_game_over(factor1, factor2, players) == true)
            {
                print(game_board);

                // Using function to find the winner and print necessary messages.
                who_won(players);

                // When game is over, return EXIT_SUCCESS.
                return EXIT_SUCCESS;
            }
        }
    }
}

