#include <iostream>
#include "Board.h"

using namespace std;

int main()
{
    int n, m, a, b;
    char c;

    cin >> n >> m;

    Board board(n, m);
    cout << "Chose player: ";
    cin >> c;
    board.setCurrentPlayer(c == 'x');

    while (cin >> a >> b) {
        board.place(a, b);
        board.show();

        c = (c == 'x' ? 'o' : 'x');
        board.setCurrentPlayer(c == 'x');
    }
    return 0;
}
