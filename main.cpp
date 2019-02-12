#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

void hidden_singles_row() {
  // TODO:この入力でhidden_singles_rowが間違える
  std::string str = "0060025070000900000000064305000036000380207000200000000019"
                    "00008400001050080050000";
  // Grid g(str);
  // g.hidden_singles_row();
  Grid::solve(str);
}

void hidden_singles_column() {
  std::string str = "4000200905000802000090000000103000600806000070500009000000"
                    "40700608730000000106500";
  Grid g(str);
  // TODO:
}

int main(int argc, char **argv) {
  // hidden_singles_row();
  // return 0;
  stopwatch::timer<> timer("all");
  timer.start();
  if (argc < 2) {
    std::cout << "Usage: ./a.out filename" << std::endl;
    return 0;
  }
  std::string filename = argv[1];
  std::ifstream ifs(filename);
  if (!ifs) {
    std::cerr << "Cannot open file " << filename << std::endl;
    return -1;
  }
  std::string line;
  while (getline(ifs, line)) {
    Grid::solve(line);
  }
  timer.stop();
}
