#include "sudoku_lib/grid.hpp"
#include <fstream>
#include <iostream>

void hidden_singles_row() {
  std::string str = "006002507000090000000006430500003600038020700020000000001900008400001050080050000";
  Grid g(str);
  mbit m_row[9];
  g.get_hidden_singles_row(m_row);
  for (auto m : m_row) {
    std::cout << m << std::endl;
  }
}

void hidden_singles_column() {
  // hidden_singles_rowの入力を転置したもの。
  // 同じ結果を出力しなければならない
  //std::string str = "000500040000032008600080100000000900090020005206300010504670000003000050700000800";
}

void make_unit_mask() {
  std::string str = "000000000000000001000012340000500010003006700026000030001050604008790000500008007";
  Grid g(str);
  mbit m_row[9] = {};
  mbit m_column[9] = {};
  mbit m_box[9] = {};

  for (int n = 0; n < 9; n++) {
    for (int i = 0; i < 81; i++) {
      if (g.cell_mask[n] & (mbit(1) << i)) {
        int r = i % 9;
        int c = i / 9;
        int br = r % 3;
        int bc = c % 3;
        int b = br + bc * 3;            //ボックス内インデックス
        int bi = (r / 3) + (c / 3) * 3; //ボックスのインデックス
        m_row[r] |= mbit(1) << (c + n * 9);
        m_column[c] |= mbit(1) << (r + n * 9);
        m_box[b] |= mbit(1) << (bi + n * 9);
      }
    }
  }
  // Hidden singles in rows
  mbit gs;
  gs = Grid::find_single(m_row);
  while (gs) {
    mbit v = gs & -gs;
    int n = bitpos(v) / 9 + 1;
    int r = bitpos(v) % 9;
    for (int i = 0; i < 9; i++) {
      if (m_row[i] & v) {
        printf("%d on %d (row)\n", n, i + r * 9);
      }
    }
    gs ^= v;
  }

  // Hidden singles in columns
  gs = Grid::find_single(m_column);
  while (gs) {
    mbit v = gs & -gs;
    int n = bitpos(v) / 9 + 1;
    int c = bitpos(v) % 9;
    for (int i = 0; i < 9; i++) {
      if (m_column[i] & v) {
        printf("%d on %d (column)\n", n, c + i * 9);
      }
    }
    gs ^= v;
  }
  // Hidden singles in boxes
  gs = Grid::find_single(m_box);
  while (gs) {
    mbit v = gs & -gs;
    int n = bitpos(v) / 9 + 1;  //どの数字か
    int bindex = bitpos(v) % 9; //どのボックスか
    for (int i = 0; i < 9; i++) {
      if (m_box[i] & v) {
        int br = (bindex / 3) * 3 + (i / 3);
        int bc = (bindex % 3) * 3 + (i % 3);
        int pos = bc + br * 9;
        printf("%d on %d (box)\n", n, pos);
      }
    }
    gs ^= v;
  }
}

int main(int argc, char **argv) {
  make_unit_mask();
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
