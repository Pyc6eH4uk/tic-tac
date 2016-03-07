#include "Board.h"

#include <iostream>
#include <iomanip>
#include <cstdio>

void SetCursorPos(int XPos, int YPos)
{
    printf("\033[%d;%dH", YPos+1, XPos+1);
}

Board::Board(int n, int m)
    : m_n(n), m_m(m), m_real_n(n+2), m_real_m(m+2)
{
    m_board.assign(m_real_n * m_real_m, 0);
    m_score.assign(m_real_n * m_real_m, 0);

    for (int j = 0; j < m_real_m; j++) m_board[get(0, j)] = 3, m_board[get(m_real_n-1, j)] = 3;
    for (int i = 0; i < m_real_n; i++) m_board[get(i, 0)] = 3, m_board[get(i, m_real_m-1)] = 3;
}

Board::~Board()
{
}

void Board::show()
{
    system("clear");
    SetCursorPos(0, 0);
    for (int i = 0; i < m_real_n; i++) {
        for (int j = 0; j < m_real_m; j++) {
            cout << m_board[get(i, j)] << " ";
        }
        cout << endl;
    }

    SetCursorPos(40, 0);
    for (int i = 1; i <= m_n; i++) {
        SetCursorPos(40, i - 1);
        for (int j = 1; j <= m_m; j++) {
            if (i == last_i && j == last_j)
                cout << blue;
            if (m_board[get(i, j)] == 1)
                    cout << red << setw(6) << 'X' << " ";
            else if (m_board[get(i, j)] == 2)
                    cout << green << setw(6) << 'O' << " ";
            else {
                cout << setw(6) << m_score[get(i, j)] << " ";
            }
            cout << reset;
        }
        cout << endl;
    }
}

void Board::place(int i, int j)
{
    m_board[get(i, j)] = m_krestik ? 1 : 2;

    static int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    static int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    last_i = i;
    last_j = j;

    auto goodIndex = [&] (int i, int j) {
        bool ok = i >= 0 && i < m_real_n && j >= 0 && j < m_real_m;
        if (!ok) return false;
        return true;
    };

    for (int w = 0; w < 4; w++) {
        for (int k = 0; k < 5; k++) { // po pyaterkam
            vector<int> kol(4, 0);
            int is = i - dy[w] * (4 - k);
            int js = j - dx[w] * (4 - k);
            for (int p = 0; p < 5; p++) {
                int in = is + dy[w] * p;
                int jn = js + dx[w] * p;
                if (goodIndex(in, jn)) {
                    kol[m_board[get(in, jn)]]++;
                }
            }
            for (int p = 0; p < 5; p++) {
                int in = is + dy[w] * p;
                int jn = js + dx[w] * p;
                if (goodIndex(in, jn))
                    if (kol[3] != 0) break;

                    else if (m_krestik && (m_board[get(in, jn)] == 0))
                        m_score[get(in, jn)] += m_value[kol[1]];
                    else {
                        m_score[get(in, jn)] = 0;
                    }
            }
        }
    }
}

int Board::get(int i, int j)
{
    return i * m_real_n + j;
}

void Board::setCurrentPlayer(bool krestik)
{
    m_krestik = krestik;
}
