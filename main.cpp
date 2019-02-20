#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

//ユニット内二択テスト
void alt_test() {
  std::string str = "000000000000000001001023040000500020002041600070000000004036702060050030800900060";
  Grid g(str);
  mbit mtwo = g.find_two(g.remained_row_mask);
  std::cout << mtwo << std::endl;
  while (mtwo) {
    mbit v = mtwo & -mtwo;
    int n = bitpos(v) / 9;
    int r = bitpos(v) % 9;
    mbit mpos = g.cell_mask[n] & Grid::unit_mask[r];
    mbit mpos1 = mpos & -mpos;
    mbit mpos2 = mpos ^ mpos1;
    int pos1 = bitpos(mpos1);
    int pos2 = bitpos(mpos2);
    printf("%d on %d or %d\n", n + 1, pos1, pos2);
    mtwo ^= v;
  }
  //Grid::solve(str);
}

int main(int argc, char **argv) {
  //alt_test();
  //return 0;
  //stopwatch::timer<> timer("all");
  //timer.start();
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
  //timer.stop();
}
