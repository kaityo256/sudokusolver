#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

void test() {
  std::string str =
      "054000600000185000900000800000360710082000304000000060020009057009600000000070000";
  Grid g(str);
  mbit m = g.cell_mask[4];
  std::cout << m << std::endl;
  mbit mm[9] = {};
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      mm[i] |= (mbit(1) << (j * 9 + i));
    }
  }
  mbit gm[9] = {};
  for (int i = 0; i < 9; i++) {
    gm[i] = ((m & mm[i]) >> i);
  }
  mbit gs = Grid::find_single(gm);
  std::cout << gs << std::endl;
}

void test3() {
  std::string str =
      "450000060000185000009000080000360071280000430000000006020009705900600000000070000";
  Grid::solve(str);
}

void test2() {
  std::string str =
      "450000060000185000009000080000360071280000430000000006020009705900600000000070000";
  Grid g(str);
  mbit mm[9] = {};
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      mm[i] |= (mbit(1) << (j * 9 + i));
    }
  }
  mbit m_row[9] = {};
  for (int n = 0; n < 9; n++) {
    mbit m = g.cell_mask[n];
    for (int i = 0; i < 9; i++) {
      m_row[i] |= (((m & mm[i]) >> i) << n);
    }
  }
  for (int i = 0; i < 9; i++) {
    std::cout << m_row[i] << std::endl;
  }
  mbit gs = Grid::find_single(m_row);
  while (gs) {
    mbit v = gs & -gs;
    int n = bitpos(v) % 9;
    int r = bitpos(v) / 9;
    for (int i = 0; i < 9; i++) {
      if (v & m_row[i]) {
        std::cout << (n + 1) << ":" << (r * 9 + i) << std::endl;
      }
    }
    gs ^= v;
  }
}

int main(int argc, char **argv) {
  // test3();
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
