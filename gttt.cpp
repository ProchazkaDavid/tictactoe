#include "gttt.hpp"

using std::set;
using namespace tictactoe;

set<Direction> generate_directions()
{
    set<Direction> directions;

    // go through all possible directions
    for (auto i = 0; i < 8; i++) {
        // go through all different signs in each axis
        for (auto j = 0; j < 8; j++) {
            for (auto sign : { -1, 1 }) {
                short x = sign * ((i & 4) >> 2);
                short y = sign * ((i & 2) >> 1);
                short z = sign * (i & 1);

                directions.insert({
                    (j & 4) ? x : -x,
                    (j & 2) ? y : -y,
                    (j & 1) ? z : -z,
                });
            }
        }
    }
    return directions;
}

Point calculate_point_position(Point point, int difference, Direction direction)
{
    auto [x, y, z] = point;

    return {
        x + (difference * std::get<0>(direction)),
        y + (difference * std::get<1>(direction)),
        z + (difference * std::get<2>(direction))
    };
}

gttt::gttt(int players, int to_win)
    : number_of_players(players)
    , winning_streak(to_win)
{
    current_player = 0;
    game_winner = UNDEFINED_PLAYER;
    directions = generate_directions();

    for (Player player = 0; player < number_of_players; player++)
        possible_wins[player] = {};
}

bool gttt::empty(int x, int y, int z) const { return owner(x, y, z) == UNDEFINED_PLAYER; }

bool gttt::finished() const { return game_winner != UNDEFINED_PLAYER; }

int gttt::winner() const { return game_winner; }

bool gttt::may_win() const { return !possible_wins.find(current_player)->second.empty(); }

int gttt::next() const { return current_player; }

bool gttt::is_possible_win(Player player, Point point) { return possible_wins[player].count({ point }); }

Player gttt::owner(int x, int y, int z) const
{
    if (auto grid_point = grid.find({ x, y, z }); grid_point != grid.end())
        return grid_point->second;
    return UNDEFINED_PLAYER;
}

bool gttt::play(int x, int y, int z)
{
    Point point = { x, y, z };
    if (grid.count(point))
        return false;

    grid[point] = current_player;

    if (is_possible_win(current_player, point)) {
        game_winner = current_player;
        return true;
    }

    for (Player player = 0; player < number_of_players; player++)
        if (is_possible_win(player, point))
            possible_wins[player].erase({ point });

    possible_wins[current_player].merge(calculate_possible_wins(point));
    current_player = (current_player + 1) % number_of_players;

    return true;
}

set<Point> gttt::calculate_possible_wins(Point point) const
{
    set<Point> wins;

    for (auto direction : directions) {
        // go through all consecutive winning possibilities containing point
        for (auto i = -winning_streak + 1; i < 1; i++) {
            Point winning_move;
            size_t empty_points = 0;
            size_t enemy_points = 0;

            for (auto j = 0; j < winning_streak; j++) {
                auto [x, y, z] = calculate_point_position(point, i + j, direction);
                auto point_owner = owner(x, y, z);

                if (point_owner == UNDEFINED_PLAYER) {
                    empty_points++;
                    winning_move = { x, y, z };
                } else if (point_owner != current_player) {
                    enemy_points++;
                }
            }

            // check if there possibility to win by placing a point on winning_move
            if (empty_points == 1 && enemy_points == 0)
                wins.insert({ winning_move });
        }
    }

    return wins;
}
