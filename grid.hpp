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

inline mbit str2mbit(const std::string &str) {
  mbit t = 0;
  for (unsigned int i = 0; i < 81 && i < str.size(); i++) {
    t ^= mbit((str[i] != '0')) << i;
  }
  return t;
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

    // 場所とボックスインデックス
    constexpr static int box_index[81] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 3, 3, 3, 4, 4, 4, 5, 5, 5, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8};

    // 行、列、ボックスの位置を表すマスク
    static mbit unit_mask[27];

    //そのマスに置かれたら、どのcell_maskを消すべきかANDするマスク
    static mbit kill_cell_mask[81];

    //そのマスに置かれたら、どのpos_maskを消すべきかANDするマスク
    static mbit kill_row_mask[81]; //行
    static mbit kill_column_mask[81]; //列
    static mbit kill_box_mask[81]; //ボックス

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
    mbit cell_mask[9]; // 各数字ごとにおける可能性のあるセル
    mbit pos_row_mask[9];  // 各数字ごとにどの行ユニットに置けるかマスク
    mbit pos_column_mask[9];  // 各数字ごとにどの列ユニットに置けるかのマスク

    // その場所にその数字がおけるか
    bool can_put(const int index, const int n) {
      return (cell_mask[n - 1] & (mbit(1) << index));
    }


    // 数字をマスに置き、マスクの対応するビットを削除
    void put(int i, int n) {
      cell_mask[n - 1] &= kill_cell_mask[i];
      mbit mm = mask81 ^ (mbit(1) << i);
      for (auto &m : cell_mask) {
        m &= mm;
      }
      data[i] = n;
      /*
      for (int j = 0; j < 9; j++) {
        pos_row_mask[j] &= (mask81 ^ (mbit(1) << i));
        pos_column_mask[j] &= (mask81 ^ (mbit(1) << i));
      }
      pos_row_mask[n - 1] &= kill_row_mask[i];
      pos_column_mask[n - 1] &= kill_column_mask[i];
      */
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
      for (auto &m : cell_mask) {
        r |= m;
      }
      return r == data_mask;
    }

  public:
    static void init(void);
    Grid(const std::string &str) {
      std::fill(&cell_mask[0], &cell_mask[9], mask81);
      std::fill(&pos_row_mask[0], &pos_row_mask[9], mask81);
      std::fill(&pos_column_mask[0], &pos_column_mask[9], mask81);
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
    // セル内二択を探す
    mbit find_two(void) {
      mbit *b = cell_mask;
      // 0..3が1ビット
      mbit b0_3a = (b[0] ^ b[1]) & ~(b[2] | b[3]);
      mbit b0_3b = (b[2] ^ b[3]) & ~(b[0] | b[1]);
      mbit b0_3_1 = b0_3a | b0_3b;

      // 0..3が2ビット
      mbit b0_3_0 = ~(b[0] | b[1] | b[2] | b[3]);//0-3が0
      mbit b0_3c = ~(b[0] ^ b[1] ^ b[2] ^ b[3]); //0か2か4
      mbit b0_3d = (b[0] & b[1] & b[2] & b[3]); //4
      mbit b0_3_2 = b0_3c & (~b0_3d) & (~b0_3_0);

      // 4..7が1ビット
      mbit b4_7a = (b[4] ^ b[5]) & ~(b[6] | b[7]);
      mbit b4_7b = (b[6] ^ b[7]) & ~(b[4] | b[5]);
      mbit b4_7_1 = b4_7a | b4_7b;

      // 4..7が2ビット
      mbit b4_7_0 = ~(b[4] | b[5] | b[6] | b[7]);//0-3が0
      mbit b4_7c = ~(b[4] ^ b[5] ^ b[6] ^ b[7]); //0か2か4
      mbit b4_7d = (b[4] & b[5] & b[6] & b[7]); //4
      mbit b4_7_2 = b4_7c & (~b4_7d) & (~b4_7_0);

      mbit b2 = (b0_3_2 & b4_7_0 & ~b[8]) // 2, 0, 0
                | (b0_3_1 & b4_7_1 & ~b[8]) // 1,1,0
                | (b0_3_1 & b4_7_0 & b[8]) // 1,0,1
                | (b0_3_0 & b4_7_1 & b[8]) // 0,1,1
                | (b0_3_0 & b4_7_2 & ~b[8]); // 0,2,0

      return b2;
    }

    // 現在のマスクの表示
    void show_mask() {
      for (int i = 0; i < 9; i++) {
        std::cout << cell_mask[i] << std::endl;
      }
      std::cout << std::endl;
    }
    unsigned int solve_internal(std::string &answer);
    unsigned int solve_unit(std::string &answer);
    static void solve(std::string &str);
};
