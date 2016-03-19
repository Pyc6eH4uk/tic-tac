#include <iostream>
#include <cstdlib>
#include "Board.h"

using namespace std;

int main()
{
    int n, m, a, b;
    char c;
    char f;
    cout << "Укажите размер доски!" << endl;
    cin >> n >> m;
    Board board(n, m);
    cout << "Чем будете ходить? : ";
    cin >> c; f = c;
    board.setCurrentPlayer(c == 'x');

    do {
        SetCursorPos(0, m);
        cout << "Xod " << c << endl;
        if (c == f) {
            do{
            SetCursorPos(0, m+1);
            cout << "Куда поставить желаемый элемент? " << endl;
            cin >> a >> b;
            }
            while(!board.rightTurn(a, b));
            if (board.place(a, b, c == 'x')) {
                SetCursorPos(0, m + 2);
                cout << "Текущий ход: строка = " << a << ", столбец = " << b;
                board.showx();
                board.showy();
                SetCursorPos(0, m + 3);
                cout << "DA TI BLIAD VIGRALLL!111)0" << endl;
                break;
            }
        } else {
            SetCursorPos(0, m+1);
            cout << "MACHINA DUMAEt, sasi pusy" << endl;
            int turn = board.turnOfAI();
            if (board.place(turn, c == 'x')) {
                SetCursorPos(0, m + 2);
                cout << "Текущий ход: строка = " << a << ", столбец = " << b;
                board.showx();
                board.showy();
                SetCursorPos(0, m + 3);
                cout << "DIE HUMAN! (net, ti prosto dolbaeb)" << endl;
                break;
            }
        }
        SetCursorPos(0, m + 2);
        cout << "Текущий ход: строка = " << a << ", столбец = " << b;
        board.showx();
        board.showy();
        c = (c == 'x' ? 'o' : 'x');
        board.setCurrentPlayer(c == 'x');
        SetCursorPos(0, m + 3);
    } while (true);
    getchar();
    return 0;
}
