#ifndef BOARD_H
#define BOARD_H

#include <vector>

using namespace std;

#define red "\033[31m"
#define green "\033[32m"
#define blue "\033[33m"
#define reset "\033[0m"

#define dbg() cerr << __LINE__ << endl;
#define dbgv(x) cerr << #x << " = " << x << endl;

void SetCursorPos(int i, int j);

class Board
{
    public:
        Board(int n, int m);
        virtual ~Board();

        void show();
        void place(int i, int j);
        void setCurrentPlayer(bool krestik);

    protected:
        const int m_value[6] = {0, 1, 10, 100, 1000, 10000};
        int m_n, m_m, m_real_n, m_real_m;
        int last_i, last_j;
        bool m_krestik;
        vector<int> m_board, m_score;

        inline int get(int i, int j);
    private:
};

#endif // BOARD_H
