#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>

/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to uss global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
char mine_map[30][30];
char visit_map[30][30];
int none_mine_count = 0;
int step_count = -1;
int visit_count = 0;
int rows; // The count of rows of the game map
int columns;  // The count of columns of the game map
int game_state = 0; // The state of the game, 0 for continuing, 1 for winning, -1 for losing

/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (2, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map would
 * be initialized, with all the blocks unvisited.
 */
void InitMap() {
  std::cin >> rows >> columns;
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < columns; j++) {
      std::cin >> mine_map[i][j];
      visit_map[i][j] = '?';
      if(mine_map[i][j] == '.') {
        mine_map[i][j] = '0';
        none_mine_count++;
      }
    }
  }
  for(int i = 0; i < rows; i++) {
    for(int j = 0;j < columns; j++) {
      visit_map[i][j] = '?';
      if(mine_map[i][j] != 'X') {
        if(i > 0) {
          if(mine_map[i - 1][j] == 'X') {
            mine_map[i][j]++;
          }
          if(j > 0) {
            if(mine_map[i - 1][j - 1] == 'X') {
              mine_map[i][j]++;
            }
          }
          if(j < columns - 1) {
            if(mine_map[i - 1][j + 1] == 'X') {
              mine_map[i][j]++;
            }
          }
        }
        if(i < rows - 1) {
          if(mine_map[i + 1][j] == 'X') {
            mine_map[i][j]++;
          }
          if(j > 0) {
            if(mine_map[i + 1][j - 1] == 'X') {
              mine_map[i][j]++;
            }
          }
          if(j < columns - 1) {
            if(mine_map[i + 1][j + 1] == 'X') {
              mine_map[i][j]++;
            }
          }
        }
        if(j > 0) {
          if(mine_map[i][j - 1] == 'X') {
            mine_map[i][j]++;
          }
        }
        if(j < columns - 1) {
          if(mine_map[i][j + 1] == 'X') {
            mine_map[i][j]++;
          }
        }
      }
    }
  }
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param row The row coordinate (0-based) of the block to be visited.
 * @param column The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 */
void VisitBlock(int row, int column) {
  if(visit_map[row][column] == '?') {
    if(mine_map[row][column] == 'X') {
      game_state = -1;
      visit_map[row][column] = 'X';
    } else if(mine_map[row][column] == '0') {
      visit_count++;
      visit_map[row][column] = '0';
      if(row > 0) {
        VisitBlock(row - 1, column);
        if(column  > 0) {
          VisitBlock(row - 1, column - 1);
        }
        if(column < columns - 1) {
          VisitBlock(row - 1, column + 1);
        }
      }
      if(row < rows - 1) {
        VisitBlock(row + 1, column);
        if(column > 0) {
          VisitBlock(row + 1, column - 1);
        }
        if(column < columns - 1) {
          VisitBlock(row + 1, column + 1);
        }
      }
      if(column > 0) {
        VisitBlock(row, column - 1);
      }
      if(column < columns - 1) {
        VisitBlock(row, column + 1);
      }
    } else {
      visit_count++;
      visit_map[row][column] = mine_map[row][column];
    }
    if(none_mine_count == visit_count) {
      game_state = 1;
    }
  }
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */
void PrintMap() {
  step_count++;
  if(game_state == 1) {
    for(int i = 0; i < rows; i++) {
      for(int j = 0; j < columns; j++) {
        if(visit_map[i][j] == '?') {
          std::cout << '@';
        } else {
          std::cout << visit_map[i][j];
        }
        if(j == columns - 1) {
          std::cout << std::endl;
        }
      }
    }
  } else {
    for(int i = 0; i < rows; i++) {
      for(int j = 0; j < columns; j++) {
        std::cout << visit_map[i][j];
        if(j == columns - 1) {
          std::cout << std::endl;
        }
      }
    }
  }
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game. 
 * It outputs a line according to the result, and a line of two integers, visit_count and step_count,
 * representing the number of blocks visited and the number of steps taken respectively.
 */
void ExitGame() {
  if(game_state == 1) {
    std::cout << "YOU WIN!" << std::endl;
    std::cout << visit_count << ' ' << step_count << std::endl;
  } else {
    std::cout << "GAME OVER!" << std::endl;
    std::cout << visit_count << ' ' << step_count << std::endl;
  }
  exit(0); // Exit the game immediately
}

#endif
