#include "grid.hpp"

Grid::GridInitializer Grid::si;//マスクの初期化
mbit Grid::unit_mask[27];
mbit Grid::cell_mask[81];

// マスクの初期化
void Grid::init(void) {
  mbit s = (1 << 9) - 1;
  mbit c = 1;
  mbit b = 7 | (mbit(7) << 9) | (mbit(7) << 18);
  for (int i = 0; i < 8; i++) {
    c = c << 9;
    c = c | 1;
  }
  for (int i = 0; i < 9; i++) {
    unit_mask[i] = s;
    unit_mask[i + 9] = c;
    s = s << 9;
    c = c << 1;
  }
  for (int i = 0; i < 9; i++) {
    unit_mask[i + 18] = b << (((i % 3) * 3) + (i / 3) * 27);
  }
  for (int i = 0; i < 81; i++) {
    const int ri = i / 9;
    const int ci = i % 9;
    const int bi = (ci / 3) + (ri / 3) * 3;
    const mbit m = unit_mask[ri] | unit_mask[ci + 9] | unit_mask[bi + 18];
    cell_mask[i] = ((mbit(1) << 81) - 1) & (~m);
  }
}

bool Grid::solved_squares(void) {
  const mbit *g = num_mask;
  const mbit x1 = (g[0] ^ g[1] ^ g[2]);
  const mbit x2 = (g[3] ^ g[4] ^ g[5]);
  const mbit x3 = (g[6] ^ g[7] ^ g[8]);
  const mbit a1 = (g[0] & g[1] & g[2]);
  const mbit a2 = (g[3] & g[4] & g[5]);
  const mbit a3 = (g[6] & g[7] & g[8]);
  const mbit o1 = (g[0] | g[1] | g[2]);
  const mbit o2 = (g[3] | g[4] | g[5]);
  const mbit o3 = (g[6] | g[7] | g[8]);
  const mbit b1 = x1 ^ a1;
  const mbit b2 = x2 ^ a2;
  const mbit b3 = x3 ^ a3;
  const mbit c1 = b1 & (mask81 ^ (o2 | o3));
  const mbit c2 = b2 & (mask81 ^ (o3 | o1));
  const mbit c3 = b3 & (mask81 ^ (o1 | o2));
  mbit b = c1 | c2 | c3;
  bool flag = false;
  while (b) {
    const mbit p = (b & -b);
    for (int i = 0; i < 9; i++) {
      if (p & num_mask[i]) {
        put(bitpos(p), i + 1);
        flag = true;
      }
    }
    b ^= p;
  }
  return flag;
}

bool Grid::hidden_singles(void) {
  static const mbit mzero = mbit(0);
  for (int i = 0; i < 9; i++) {
    if (num_mask[i] == mzero)continue;
    for (const auto &m : unit_mask) {
      const mbit p = num_mask[i] & m;
      if ((popcnt_u128(p) == 1)) {
        put(bitpos(p), i + 1);
        return true;
      }
    }
  }
  return false;
}

void Grid::solve(std::string &str) {
  Grid g(str);
  std::string answer;
  int n = g.solve_internal(answer);
  if (n == 0) {
    std::cout << "no solution" << std::endl;
  } else if (n > 1) {
    std::cout << "multiple solutions" << std::endl;
  } else {
    std::cout << answer << std::endl;
  }
}

// 解の数を返す
// 0: 解なし
// 1: 唯一解あり
// 2以上: 複数解あり
unsigned int Grid::solve_internal(std::string &answer) {
  bool flag = true;
  // Naked/Hidden singlesで解けるだけ解く
  while (flag) {
    flag = false;
    if (solved_squares())flag = true;
    if (hidden_singles())flag = true;
  }
  if (!is_valid())return 0;

  if (_rest == 0) {
    // 解けたので解答をセット
    answer.resize(81);
    for (int i = 0; i < 81; i++) {
      answer[i] = '0' + data[i];
    }
    return 1;
  }
  int min = 9;
  int min_index = -1;
  mbit um = 0;
  for (int i = 0; i < 9; i++) {
    const mbit nm = num_mask[i];
    if (nm == mbit(0))continue;
    for (const auto &m : unit_mask) {
      const int n = popcnt_u128(nm & m);
      //assert(n!=1);
      if (n != 0 && n < min) {
        min_index = i;
        um = m;
        if (n <= 2) {
          goto break_loop;
        }
      }
    }
  }
break_loop:
  mbit v = (num_mask[min_index] & um);
  int sum = 0;
  while (v) {
    const mbit p = (v & -v);
    const int n = bitpos(p);
    Grid g2 = (*this);
    g2.put(n, min_index + 1);
    sum = sum + g2.solve_internal(answer);
    if (sum > 1)return sum;
    v ^= p;
  }
  return sum;
}
