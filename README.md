# Blacksmith puzzle solver

This project solves the blacksmithing puzzle from the game Puzzle Pirates by simulating a lot of attempts to clear the whole layer.
Once it finds a combination that clears it whole, it will stop and output the combination in a txt file to replicate it in game.
This approach is not perfect since after every stroke of the hammer a new piece may appear which might help clearing the board, which cannot be taken into account to solve the whole game because the incoming piece is selected randomly.

## Build instructions

### Prerequisites

1. A C++ compiler using the c++20 standard. The build is tested in Windows via [MSVC 2022](https://visualstudio.microsoft.com/downloads/) and in Linux via [gcc](https://gcc.gnu.org/).
2. [CMake](https://cmake.org/).

### Building steps

1. Clone the git repository to your folder of choice: `git clone https://github.com/Xaunther/Blacksmith.git`.
2. Go the build folder and from there configure the build from the terminal: `cmake ..`.
3. Build, while still in the build folder: `cmake --build .`. This should produce the blacksmith executable (check the build/Release folder if using `MSVC` or the build folder if using `gcc`).

## Running instructions

The following instructions are valid either if you downloaded the executable directly or you built it yourself.

First, create a text file which will contain the information of the blacksmith game board. In the file, you should create a 6x6 grid like this:

```txt
3 4 R Q 3 2
Q Q 1 B K E
Q R 3 B Q 4
Q 3 2 R Q 2
1 K B 2 R K
Q W Q 3 1 3
```

Corresponding to each of the pieces in the board

- `1`: **1** piece.
- `2`: **2** piece.
- `3`: **3** piece.
- `4`: **4** piece.
- `Q`: **Queen** piece.
- `K`: **Knight** piece.
- `R`: **Rook** piece.
- `B`: **Bishop** piece.
- `W`: **Rum** (wildcard) piece.
- `E`: **Empty** (unstrokable) piece.

Once saved, run the executable and after (hopefully) a short time it will return the best combination it found to try and hit every piece of the board given. If it takes longer, it probably means that it is having a hard time to clear it whole. There are a number of options you can set like the number of simulations, the random seed or the input and output files. To get a full list, run `blacksmith -h`.

Once it's finished, a `Best-pattern.txt` file (or the file you specified with the `-o` option) will have been created containing the stroke order needed to clear from the board as many pieces as possible, as well as any remaining pieces:

```txt
Step #   (i,j)   Piece
----------------------
   0     (4,3)     2
   1     (2,3)     B
   2     (4,5)     K
   3     (2,4)     Q
   4     (0,2)     R
   5     (5,2)     Q
   6     (3,0)     Q
   7     (5,0)     Q
   8     (0,5)     2
   9     (0,3)     Q
  10     (5,3)     3
  11     (2,0)     Q
  12     (0,0)     3
  13     (3,3)     R
  14     (3,5)     2
  15     (1,3)     B
  16     (0,4)     3
  17     (3,1)     3
  18     (3,4)     Q
  19     (0,1)     4
  20     (4,1)     K
  21     (2,2)     3
  22     (5,5)     3
  23     (2,5)     4
  24     (2,1)     R
  25     (5,1)     W
  26     (3,2)     2
  27     (1,2)     1
  28     (1,1)     Q
  29     (1,0)     Q
  30     (5,4)     1
  31     (4,4)     R
  32     (4,0)     1
----------------------
   Remaining Pieces   
----------------------
(1,4): K
(4,2): B
```

The position of each piece is given by the `(i,j)` indices, $0\leq i,j \leq 5$. Take into account that the top-left piece corresponds to the position `(0,0)` while the bottom-right is the `(5,5)`.

If you're lucky, repeating this process 3 times will clear the 3 layers flawlessly, resulting in a perfect game. If the program is unable to perfectly clear a given board, I suggest you personally watch the board while following the stroke sequence and see if you can get that sneaky remaining piece by hitting a previously hit position!
