#include <iostream>
#include <fstream>
#include "grid.hpp"

void test() {
  std::string str = "240007000008095200000003500000509007080000600000060018950000000107000000000004060";
  str = "000000000000000001000002340000030400003560000054010020020780600080045200390006000";
  Grid::solve(str);
}


int main(int argc, char **argv) {
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
}
