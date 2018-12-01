#include <iostream>
#include <fstream>
#include "grid.hpp"

void test() {
  std::string str = "000000000000000001002003450000030060003700002560002000006005070080900000105007024";
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
