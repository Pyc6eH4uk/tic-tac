#include <iostream>
#include "Board.h"

using namespace std;

int main()
{
    int n, m, a, b;
    char c;
    cout << "Укажите размер доски!" << endl;
    cin >> n >> m;
    Board board(n, m);
    cout << "Чем будете ходить? : ";
    cin >> c;
    board.setCurrentPlayer(c == 'x');
    SetCursorPos(0, m);
    cout << "Куда поставить желаемый элемент? " << endl;
    while (cin >> a >> b) {
        board.place(a, b);
        SetCursorPos(0, m + 2);
        cout << "Текущий ход: строка = " << a << ", столбец = " << b;
        board.showx();
        board.showy();
        c = (c == 'x' ? 'o' : 'x');
        board.setCurrentPlayer(c == 'x');
        SetCursorPos(0, m + 3);
        cout << "Куда поставить желаемый элемент? " << endl;
    }
    return 0;
}
