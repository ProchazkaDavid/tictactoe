# Tic-Tac-Toe

> "Infinite" 3D Tic-Tac-Toe with an arbitrary number of players and configurable length of a winning line

## Example

```cpp
#include "gttt.hpp"
#include <cassert>

using namespace tictactoe;

gttt uut(2, 2);
uut.play(0, 0, 0);
uut.play(-10, -10, -10);
uut.play(1, 1, 1);

assert(uut.finished() && uut.winner() == 0);
```
