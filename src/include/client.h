#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>
#include <ctime>
#include <random>

extern int rows;     // The count of rows of the game map
extern int columns;  // The count of columns of the game map

// You can not use any other external variables except for rows and columns.

char client_map[30][30];
bool is_mine_map[30][30] = {};
int search_around_row[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int search_around_column[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

/**
 * @brief The definition of function Execute(int, int)
 *
 * @details This function is designed to take a step when player the client's (or player's) role, and the implementation
 * of it has been finished by TA. (I hope my comments in code would be easy to understand T_T) If you do not understand
 * the contents, please ask TA for help immediately!!!
 *
 * @param row The row coordinate (0-based) of the block to be visited.
 * @param column The column coordinate (0-based) of the block to be visited.
 */
void Execute(int row, int column);

/**
 * @brief The definition of function InitGame()
 *
 * @details This function is designed to initialize the game. It should be called at the beginning of the game, which
 * will read the scale of the game map and the first step taken by the server (see README).
 */
void InitGame() {
  int first_row, first_column;
  std::cin >> first_row >> first_column;
  Execute(first_row, first_column);
}

/**
 * @brief The definition of function ReadMap()
 *
 * @details This function is designed to read the game map from stdin when playing the client's (or player's) role.
 * Since the client (or player) can only get the limited information of the game map, so if there is a 3 * 3 map as
 * above and only the block (2, 0) has been visited, the stdin would be
 *     ???
 *     12?
 *     01?
 */
void ReadMap() {
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < columns; j++) {
      std::cin >> client_map[i][j];
      if(isdigit(client_map[i][j])) {
        is_mine_map[i][j] = false;
      }
    }
  }
}

/**
 * @brief The definition of function Decide()
 *
 * @details This function is designed to decide the next step when playing the client's (or player's) role. Open up your
 * mind and make your decision here!
 */
void Decide() {
  bool need_guess = true;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      if (client_map[i][j] >= '1' && client_map[i][j] <= '9') {
        int mine_counter = 0, not_visited_counter = 0;
        bool not_visited_map[8] = {};
        for (int k = 0; k < 8; k++) {
          int search_i = i + search_around_row[k], search_j = j + search_around_column[k];
          if (search_i >= 0 && search_i < columns && search_j >= 0 && search_j < columns) {
            if (is_mine_map[search_i][search_j]) {
              mine_counter++;
            } else if (client_map[search_i][search_j] == '?') {
              not_visited_counter++;
              not_visited_map[k] = true;
            }
          }
        }
        if (mine_counter == client_map[i][j] - '0') {  // visit all the blocks that have no mines
          for (int k = 0; k < 8; k++) {
            if (not_visited_map[k]) {
              int search_i = i + search_around_row[k], search_j = j + search_around_column[k];
              Execute(search_i, search_j);
              need_guess = false;
            }
          }
        } else if (mine_counter + not_visited_counter == client_map[i][j] - '0') {
          for (int k = 0; k < 8; k++) {
            if (not_visited_map[k]) {
              int search_i = i + search_around_row[k], search_j = j + search_around_column[k];
              is_mine_map[search_i][search_j] = true;
              need_guess = false;
            }
          }
        }
      }
      if (client_map[i][j] >= '1' && client_map[i][j] <= '9' && i < columns - 1) {
        if (client_map[i + 1][j] >= '1' && client_map[i + 1][j] <= '9') {
          int upper_not_visited_counter = 0, upper_mine_counter = 0;
          int right_not_visited_counter = 0, right_mine_counter = 0;
          bool not_visited_map[8] = {};
          for (int k = 0; k < 3; k++) {
            int search_i = i + search_around_row[k], search_j = j + search_around_column[k];
            if (search_i >= 0 && search_i < columns && search_j >= 0 && search_j < columns) {
              if (is_mine_map[search_i][search_j]) {
                upper_mine_counter++;
              } else if (client_map[search_i][search_j] == '?') {
                upper_not_visited_counter++;
                not_visited_map[k] = true;
              }
            }
          }
          for (int k = 5; k < 8; k++) {
            int search_i = i + 1 + search_around_row[k], search_j = j + search_around_column[k];
            if (search_i >= 0 && search_i < columns && search_j >= 0 && search_j < columns) {
              if (is_mine_map[search_i][search_j]) {
                right_mine_counter++;
              } else if (client_map[search_i][search_j] == '?') {
                right_not_visited_counter++;
                not_visited_map[k] = true;
              }
            }
          }
          if (client_map[i][j] == client_map[i + 1][j]) {
            if (right_not_visited_counter == 0 && upper_not_visited_counter != 0 &&
                right_mine_counter == upper_mine_counter + upper_not_visited_counter) {
              need_guess = false;
              for (int k = 0; k < 3; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + search_around_row[k], search_j = j + search_around_column[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            } else if (upper_not_visited_counter == 0 && right_not_visited_counter != 0 &&
                       upper_mine_counter == right_mine_counter + right_not_visited_counter) {
              need_guess = false;
              for (int k = 5; k < 8; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + 1 + search_around_row[k], search_j = j + search_around_column[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            }
          } else {
            int difference = client_map[i][j] - client_map[i + 1][j];
            if (upper_mine_counter - difference - right_mine_counter == 0) {
              if (right_not_visited_counter == 0 && upper_not_visited_counter != 0) {
                need_guess = false;
                for (int k = 0; k < 3; k++) {
                  if (not_visited_map[k]) {
                    int search_i = i + search_around_row[k], search_j = j + search_around_column[k];
                    Execute(search_i, search_j);
                  }
                }
              } else if (right_not_visited_counter != 0 && upper_not_visited_counter == 0) {
                need_guess = false;
                for (int k = 5; k < 8; k++) {
                  if (not_visited_map[k]) {
                    int search_i = i + 1 + search_around_row[k], search_j = j + search_around_column[k];
                    Execute(search_i, search_j);
                  }
                }
              }
            } else if (upper_mine_counter - difference - right_mine_counter == right_not_visited_counter) {
              need_guess = false;
              for (int k = 5; k < 8; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + 1 + search_around_row[k], search_j = j + search_around_column[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            } else if (difference + right_mine_counter - upper_mine_counter == upper_not_visited_counter) {
              need_guess = false;
              for (int k = 0; k < 3; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + search_around_row[k], search_j = j + search_around_column[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            }
          }
        }
      }
      if (client_map[i][j] >= '1' && client_map[i][j] <= '9' && j < columns - 1) {
        if (client_map[i][j + 1] >= '1' && client_map[i][j + 1] <= '9') {
          int left_not_visited_counter = 0, left_mine_counter = 0;
          int right_not_visited_counter = 0, right_mine_counter = 0;
          bool not_visited_map[8] = {};
          for (int k = 0; k < 3; k++) {
            int search_i = i + search_around_column[k], search_j = j + search_around_row[k];
            if (search_i >= 0 && search_i < columns && search_j >= 0 && search_j < columns) {
              if (is_mine_map[search_i][search_j]) {
                left_mine_counter++;
              } else if (client_map[search_i][search_j] == '?') {
                left_not_visited_counter++;
                not_visited_map[k] = true;
              }
            }
          }
          for (int k = 5; k < 8; k++) {
            int search_i = i + search_around_column[k], search_j = j + 1 + search_around_row[k];
            if (search_i >= 0 && search_i < columns && search_j >= 0 && search_j < columns) {
              if (is_mine_map[search_i][search_j]) {
                right_mine_counter++;
              } else if (client_map[search_i][search_j] == '?') {
                right_not_visited_counter++;
                not_visited_map[k] = true;
              }
            }
          }
          if (client_map[i][j] == client_map[i][j + 1]) {
            if (right_not_visited_counter == 0 && left_not_visited_counter != 0 &&
                right_mine_counter == left_mine_counter + left_not_visited_counter) {
              need_guess = false;
              for (int k = 0; k < 3; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + search_around_column[k], search_j = j + search_around_row[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            } else if (left_not_visited_counter == 0 && right_not_visited_counter != 0 &&
                       left_mine_counter == right_mine_counter + right_not_visited_counter) {
              need_guess = false;
              for (int k = 5; k < 8; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + search_around_column[k], search_j = j + 1 + search_around_row[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            }
          } else {
            int difference = client_map[i][j] - client_map[i][j + 1];
            if (left_mine_counter - difference - right_mine_counter == 0) {
              if (right_not_visited_counter == 0 && left_not_visited_counter != 0) {
                need_guess = false;
                for (int k = 0; k < 3; k++) {
                  if (not_visited_map[k]) {
                    int search_i = i + search_around_column[k], search_j = j + search_around_row[k];
                    Execute(search_i, search_j);
                  }
                }
              } else if (right_not_visited_counter != 0 && left_not_visited_counter == 0) {
                need_guess = false;
                for (int k = 5; k < 8; k++) {
                  if (not_visited_map[k]) {
                    int search_i = i + search_around_column[k], search_j = j + 1 + search_around_row[k];
                    Execute(search_i, search_j);
                  }
                }
              }
            } else if (left_mine_counter - difference - right_mine_counter == right_not_visited_counter) {
              need_guess = false;
              for (int k = 5; k < 8; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + search_around_column[k], search_j = j + 1 + search_around_row[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            } else if (difference + right_mine_counter - left_mine_counter == left_not_visited_counter) {
              need_guess = false;
              for (int k = 0; k < 3; k++) {
                if (not_visited_map[k]) {
                  int search_i = i + search_around_column[k], search_j = j + search_around_row[k];
                  is_mine_map[search_i][search_j] = true;
                }
              }
            }
          }
        }
      }
    }
  }
  if (need_guess) {
//    std::cout << "guess" << std::endl;
    int uncertain_row[900], uncertain_column[900], uncertain_counter = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        if (!is_mine_map[i][j] && client_map[i][j] == '?') {
          uncertain_row[uncertain_counter] = i;
          uncertain_column[uncertain_counter] = j;
          uncertain_counter++;
        }
      }
    }
    std::mt19937 myrand(std::time(nullptr));
    int index = myrand() % uncertain_counter;
    Execute(uncertain_row[index], uncertain_column[index]);
    return;
  }
  // while (true) {
  //   Execute(0, 0);
  // }
}

#endif