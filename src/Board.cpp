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
    m_scorex.assign(m_real_n * m_real_m, 0);
    m_scorey.assign(m_real_n * m_real_m, 0);
    for (int j = 0; j < m_real_m; j++) m_board[get(0, j)] = 3, m_board[get(m_real_n-1, j)] = 3;
    for (int i = 0; i < m_real_n; i++) m_board[get(i, 0)] = 3, m_board[get(i, m_real_m-1)] = 3;
}

Board::~Board()
{
}

void Board::showx()//Показываем оценку для крестиков
{
    system("clear");
    SetCursorPos(0, 0);
    for (int i = 0; i < m_real_n; i++) {
        for (int j = 0; j < m_real_m; j++) {
            cout << m_board[get(i, j)] << " ";
        }
        cout << endl;
    }
    SetCursorPos(41, 1);
    for (int i = -1; i <= m_n; i++) {
        SetCursorPos(41, i + 1);
        for (int j = 1; j <= m_m; j++) {
            if( i == -1) {
                cout << setw(6) << j << " ";
                continue;
            }
            if( i == 0) continue;
            if (i == last_i && j == last_j)
                cout << blue;
            if (m_board[get(i, j)] == 1)
                    cout << red << setw(6) << 'X' << " ";
            else if (m_board[get(i, j)] == 2)
                    cout << green << setw(6) << 'O' << " ";
            else {
                cout << setw(6) << m_scorex[get(i, j)] << " ";
            }
            cout << reset;
        }
        cout << endl;
    }

    for (int i = 1; i <= m_m; i++) {
        SetCursorPos(40, i + 1);
        cout << i << " ";
    }


}

void Board::showy()//Показываем оценку для нулей
{
    SetCursorPos(40, 24);
    for (int i = 1; i <= m_n; i++) {
        cout << setw(6) << i << " ";
    }

    SetCursorPos(40, 25);
    for (int i = 1; i <= m_n; i++) {
        SetCursorPos(40, 25 + i);
        for (int j = 1; j <= m_m; j++) {
            if (i == last_i && j == last_j)
                cout << blue;
            if (m_board[get(i, j)] == 1)
                    cout << red << setw(6) << 'X' << " ";
            else if (m_board[get(i, j)] == 2)
                    cout << green << setw(6) << 'O' << " ";
            else {
                cout << setw(6) << m_scorey[get(i, j)] << " ";
            }
            cout << reset;
        }
        cout << endl;
    }

    for (int i = 1; i <= m_m; i++) {
        SetCursorPos(38, 25 + i);
        cout << i << " ";
    }
}

bool Board::place(int pos, bool who,
    vector<int> &board,
    vector<int> &scorex,
    vector<int> &scorey)//Втавляем элемент и определяем оценку по пятёркам
{
    board[pos] = who ? 1 : 2;
    scorex[pos] = 0;
    scorey[pos] = 0;
    static int dir[4] = {1, 1 - m_real_n, -m_real_n, -1 - m_real_n};

    auto goodIndex = [&] (int p) {
        bool ok = p >= 0 && p < m_real_n * m_real_m;
        return ok;
    };

    for (int w = 0; w < 4; w++) {
        for (int k = 0; k < 5; k++) { // po pyaterkam
            vector<int> kol(4, 0);
            int post = pos - dir[w] * (4-k);
            for (int p = 0; p < 5; p++) {
                int posn = post + dir[w] * p;
                if (goodIndex(posn)) {
                    kol[m_board[posn]]++;
                }
            }

            if (kol[1] == 5) {
                m_pause = true;
                if (who) return true;
                else return false;
            } else if (kol[2] == 5) {
                m_pause = true;
                if (who) return false;
                return true;
            }

            for (int p = 0; p < 5; p++) {
                int posn = post + dir[w] * p;
                if (goodIndex(posn)) {
                    if (kol[3] != 0) break;

                    else if (who && (m_board[posn] == 0)) {
                        scorex[posn] += m_value[kol[1]] - m_value[kol[2]];
                        scorex[posn] = max(scorex[posn], 0);
                        scorey[posn] -= m_value[kol[2]];
                        scorey[posn] = max(scorey[posn], 0);
                    }
                    else if (!who && (m_board[posn] == 0)) {
                        scorey[posn] += m_value[kol[2]] - m_value[kol[1]];
                        scorey[posn] = max(scorey[posn], 0);
                        scorex[posn] -= m_value[kol[1]];
                        scorex[posn] = max(scorex[posn], 0);
                    }
                }
            }
        }
    }

    return false;
}
bool Board::place(int i, bool who)
{
    return place(i, who, m_board, m_scorex, m_scorey);
}

bool Board::place(int i, int j, bool who)
{
    return place(get(i, j), who, m_board, m_scorex, m_scorey);
}


int Board::get(int i, int j)
{
    return i * m_real_n + j;
}

void Board::setCurrentPlayer(bool krestik)
{
    m_krestik = krestik;
}


/*
 * AI logic
 */

int Board::getAILogic(
            vector<int> board,
            vector<int> scorex,
            vector<int> scorey, bool krestik, bool &isWon, int MAX_DEPTH)
{
    if (MAX_DEPTH == 0) {
        isWon = false;
        return get(1, 1);
    }

    vector<int> freePosition;
    int bestTurn;

    static int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    static int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int max_i = 0;
    int max_j = 0;
    int max_val = -1;

    auto goodIndex = [&] (int i, int j) {
        bool ok = i >= 0 && i < m_real_n && j >= 0 && j < m_real_m;
        if (!ok) return false;
        return true;
    };

    auto analisysFive = [&] (int i, int j, int w, int k) {
        freePosition.clear();
        vector<int> kol(4, 0);
        int is = i - dy[w] * (4 - k);
        int js = j - dx[w] * (4 - k);
        for (int p = 0; p < 5; p++) {
            int in = is + dy[w] * p;
            int jn = js + dx[w] * p;
            if (goodIndex(in, jn)) {
                kol[board[get(in, jn)]] ++;
                if (kol[3] != 0) return false;
                if (board[get(in, jn)] == 0)
                    freePosition.push_back(get(in, jn));
            }
        }
        if(!m_krestik && kol[0] == 1 && kol[2] == 4){
            bestTurn = freePosition[0];
            isWon = true;
            return true;
        }
        if(m_krestik && kol[0] == 1 && kol[1] == 4){
            bestTurn = freePosition[0];
            isWon = true;
            return true;
        }
        bool isNeed = freePosition.size() == 2 && (
            kol[2] == 3 || kol[1] == 3
        );
        if (!isNeed) return false;

        vector<int> scorex_t = scorex, scorey_t = scorey, board_t = board;
        place(freePosition[0], true, board_t, scorex_t, scorey_t);
        place(freePosition[1], false, board_t, scorex_t, scorey_t);
        int bestTurn1 = getAILogic(board_t, scorex_t, scorey_t, krestik, isWon, MAX_DEPTH - 1);
        if (isWon) {
            bestTurn = bestTurn1;
            return true;
        }

        scorex_t = scorex, scorey_t = scorey, board_t = board;
        place(freePosition[1], true, board_t, scorex_t, scorey_t);
        place(freePosition[0], false, board_t, scorex_t, scorey_t);
        int bestTurn2 = getAILogic(board_t, scorex_t, scorey_t, krestik, isWon, MAX_DEPTH - 1);
        if (isWon) {
            bestTurn = bestTurn2;
            return true;
        }

        return false;
    };

    for (int i = 1; i <= m_n; i++) {
        for (int j = 1; j <= m_m; j++) {
            /*
            if (krestik && max_val < scorex[get(i, j)]) {
                max_val = scorex[get(i, j)];
                max_i = i;
                max_j = j;
            }

            if (!krestik && max_val < scorey[get(i, j)]) {
                max_val = scorey[get(i, j)];
                max_i = i;
                max_j = j;
            }
            */
            if (max_val < max(scorex[get(i, j)], scorey[get(i, j)])) {
                max_val = max(scorex[get(i, j)], scorey[get(i, j)]);
                max_i = i;
                max_j = j;
            }

            for (int w = 0; w < 4; w++) {
                for (int k = 0; k < 5; k++) { // po pyaterkam
                    if (analisysFive(i, j, w, k)) {
                        return bestTurn;
                    }
                }
            }
        }
    }

    return get(max_i, max_j);
}

bool Board::rightTurn(int i, int j)
{
    if(!(m_board[get(i, j)] == 0)){
        SetCursorPos(0, m_real_m + 5);
        cout << red <<"TI KUDA STAVISH GNIDA!!11 " << endl;
    }
    cout << reset;
    return m_board[get(i, j)] == 0;
}

int Board::turnOfAI()
{
    bool isWon = false;
    return getAILogic(m_board, m_scorex, m_scorey, m_krestik, isWon);
}
