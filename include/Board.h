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
        bool rightTurn(int i, int j);
        void showx();
        void showy();
        bool place(int i, int j, bool who);
        bool place(int pos, bool who);
        bool place(int pos, bool who, vector<int> &board, vector<int> &scorex, vector<int> &scorey);
        void setCurrentPlayer(bool krestik);
        int turnOfAI();

    protected:
        const int m_value[6] = {0, 1, 10, 100, 1000, 10000};
        int m_n, m_m, m_real_n, m_real_m;
        int last_i, last_j;
        bool m_krestik;
        bool m_pause;
        bool m_win_krestik;
        vector<int> m_board, m_scorex, m_scorey;

        inline int get(int i, int j);
        int getAILogic(
            vector<int> board,
            vector<int> scorex,
            vector<int> scorey,
            bool krestik, bool &isWon, int MAX_DEPTH = 4
        );
    private:
};

#endif // BOARD_H
