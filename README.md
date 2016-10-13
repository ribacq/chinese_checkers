#Chinese Checkers
A board game written in C for the GNU/Linux CLI using `ncurses.h`. I decided to
code this game after having bought a material, wooden one, because CLI games are
cool, and because I had no idea how a program could represent a hexagon grid. A
fantastic resource about this subject is [this page on Red Blob Games][1].

## Installation
In order to play, download this repository or clone it using git, open the
folder in a command line, type in the command `make`, and run it with
`./chinese-checkers`. You may copy or link this binary file to one of your
`$PATH` directories for easy access.

#Rules
It’s actually from Germany, and was originally called, ‘Stern-Halma’, at the
time of its creation in 1892.

## Setup
Chinese checkers is played on a 6-branch star where every cell has 6 neighbors.
The game can be played by _2, 3, 4 or 6 players_. Each player owns _10 pieces_
of their own color, placed on a star corner of the board. The camps are
simmetrically set around the board. When there are 2, 4 or 6 players, their
camps must face one another, but in the case of 3 players, they are all placed
opposite an empty space.

## Goal
A player wins when all of their pieces get to the star corner opposite their
starting camp.

## Game
The game is played in turns. On their turn, a player has to move _a single one_
of their pieces on the board. For such a move their are two choices:

* Move a single piece from its current location to an adjacent cell, or
* Proceed to a series of jumps:
  * All jumps of a series use the same piece;
  * A jump can be made by a piece from its current location over a directly  
    adjacent piece, whose color is indifferent;
  * The jumping piece lands directly behind the jumped piece, in the opposite  
    direction where there has to be a free cell;
  * The jumped piece remains unchanged.

That’s it!

[1]: http://www.redblobgames.com/grids/hexagons/

