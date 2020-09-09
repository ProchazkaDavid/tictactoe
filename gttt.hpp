#include <map>
#include <set>
#include <unordered_map>

namespace tictactoe {

using Direction = std::tuple<short, short, short>;
using Point = std::tuple<int, int, int>;
using Player = int;

const Player UNDEFINED_PLAYER = -1;

/* This class, ‹gttt› (short for generalized tic-tac-toe), provides
 * the entire interface of the game. */
class gttt {
    std::map<Point, Player> grid;
    std::set<Direction> directions;
    std::unordered_map<Player, std::set<Point>> possible_wins;

    Player current_player, game_winner;
    int number_of_players, winning_streak;

    // Computes all the consecutive winning possibilities containing point
    std::set<Point> calculate_possible_wins(Point point) const;
    // Can the player win by placing point
    bool is_possible_win(Player player, Point point);

public:
    /* Start a new game with a given number of players. The
     * ‹to_win› argument specifies how many consecutive symbols need
     * to be placed by the same player in order to win the game. The
     * number of players must be between 2 and 1000000 (inclusive).
     * Same limits apply to the length of the winning sequence. */
    gttt(int players = 2, int to_win = 2);
    gttt()
        : gttt(2, 2) {};

    /* Which player plays next. Players are numbered from 0 and play
     * in ascending order (player 0 goes first). */
    int next() const;

    /* Play the active player's round by placing a mark on the cell
     * given by (x, y, z). Returns true if the move was valid.
     * Returns false if the cell was already occupied (and does not
     * change the state in this case and the player's round is not
     * completed).  */
    bool play(int x, int y, int z);

    /* Return true if the specified cell is empty, false otherwise. */
    bool empty(int x, int y, int z) const;

    /* Return the identifier of the player who owns (placed their
     * mark on) the given cell. The behaviour is unspecified if the
     * cell is empty. */
    int owner(int x, int y, int z) const;

    /* True iff there is a winning sequence on the grid. */
    bool finished() const;

    /* The player who won. Undefined unless finished() is true. */
    int winner() const;

    /* Returns true if the next player can immediately win the game
     * by placing their mark strategically. */
    bool may_win() const;
};

}