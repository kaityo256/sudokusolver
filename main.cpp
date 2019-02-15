#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

//セル内二択テスト
void alt_test() {
  std::string str = "000000000000000001001023040000500020002041600070000000004036702060050030800900060";
  Grid::solve(str);
}

int main(int argc, char **argv) {
  //alt_test();
  //return 0;
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
