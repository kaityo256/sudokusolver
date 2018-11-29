#pragma once
#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif


typedef unsigned __int128 mbit;

const static mbit mask81 = (mbit(1) << 81) - 1;
inline std::ostream &operator<<(std::ostream &dest, mbit v) {
  char buffer[81];
  for (int i = 0; i < 81; i++) {
    buffer[i] = (v & 1) + '0';
    v = v >> 1;
  }
  dest.rdbuf()->sputn(buffer, 81);
  return dest;
}

inline int popcnt_u128(const mbit &s) {
  const uint64_t *m = (uint64_t *)&s;
  return _mm_popcnt_u64(m[0]) + _mm_popcnt_u64(m[1]);
}

inline int bitpos(mbit s) {
  return popcnt_u128(s - 1);
}

class Grid {
  private:
    // 行、列、ボックスの位置を表すマスク
    static mbit unit_mask[27];

    //そのマスに置かれたら、どのnum_maskを消すべきかANDするマスク
    static mbit cell_mask[81];

    // マスクの自動初期化用クラス
    class GridInitializer {
      public:
        GridInitializer() {
          Grid::init();
        };
    };
    static GridInitializer si;

    int _rest; // 残りマス
    bool _valid; // 正常な状態かどうか
    int data[81]; //現在決定している数字
    mbit num_mask[9]; // 各数字ごとにおける可能性のある場所

    // その場所にその数字がおけるか
    bool can_put(const int index, const int n) {
      return (num_mask[n - 1] & (mbit(1) << index));
    }
    // 数字をマスに置き、マスクの対応するビットを削除
    void put(int i, int n) {
      num_mask[n - 1] &= cell_mask[i];
      mbit mm = mask81 ^ (mbit(1) << i);
      for (auto &m : num_mask) {
        m &= mm;
      }
      data[i] = n;
      _rest--;
    }
    bool solved_squares(void); //Naked Singles
    bool hidden_singles(void); //Hidden Singles
    // 現在の状態が正常かどうか
    bool is_valid(void) {
      if (!_valid)return false;
      mbit data_mask = mbit(0);
      for (int i = 0; i < 81; i++) {
        if (data[i] == 0) {
          data_mask |= mbit(1) << i;
        }
      }
      mbit r = mbit(0);
      for (auto &m : num_mask) {
        r |= m;
      }
      return r == data_mask;
    }

  public:
    static void init(void);
    Grid(const std::string &str) {
      std::fill(&num_mask[0], &num_mask[9], mask81);
      std::fill(&data[0], &data[81], 0);
      _rest = 81;
      _valid = true;
      for (int i = 0; i < 81; i++) {
        const int n = str[i] - '0';
        if (n == 0)continue;
        if (!can_put(i, n)) {
          _valid = false;
        }
        put(i, n);
      }
    }
    unsigned int solve_internal(std::string &answer);
    static void solve(std::string &str);
};
