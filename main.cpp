#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

void hidden_singles_test() {
  std::string str = "0060025070000900000000064305000036000380207000200000000019"
                    "00008400001050080050000";
  // Grid::solve(str);
  Grid g(str);
  mbit m_row[9] = {};
  mbit m_column[9] = {};
  mbit m_box[9] = {};
  g.get_kill_mask(m_row, m_column, m_box);
  for (int i = 0; i < 9; i++) {
    std::cout << (m_row[i] == g.remained_row_mask[i]) << std::endl;
    std::cout << (m_column[i] == g.remained_column_mask[i]) << std::endl;
    std::cout << (m_box[i] == g.remained_box_mask[i]) << std::endl;
  }
}

int main(int argc, char **argv) {
  hidden_singles_test();
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
