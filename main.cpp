#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

void hidden_singles_test() {
  std::string str = "006002507000090000000006430500003600038020700020000000001900008400001050080050000";
  Grid::solve(str);
}

int main(int argc, char **argv) {
  return 0;
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
