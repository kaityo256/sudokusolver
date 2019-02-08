#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

void test() {
  std::string str =
      "061007003092003000000000000008530000000000504500008000040000001000160800600000000";
  // str =
  //   "000000000000000000000000000000000000000000000000000000000000000000100000000000000";
  Grid g(str);
  g.show_unit_mask();
}

int main(int argc, char **argv) {
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
