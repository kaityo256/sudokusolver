#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

void test() {
  std::string str =
      "054000600000185000900000800000360710082000304000000060020009057009600000000070000";
  Grid g(str);
  for(auto m: g.cell_mask){
    std::cout << m << std::endl;
  }
}

int main(int argc, char **argv) {
  test();
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
