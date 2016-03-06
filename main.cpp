#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>

using namespace std;

    int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int n, m, l;
    int stone[1000];//поле
    int pot[1000];//potencial
    int est[6] = {0, 1, 10, 100, 1000, 10000};//ocenka
    int ocenka[5][5] = {{0, 1, 2, 3, 4}, {0, 10, 11, 21, 31}, {}, {}, {}};
    int counts[4];//peterka
    int last_i, last_j;
    const string red("\033[31m");
    const string green("\033[32m");
    const string blue("\033[33m");
    const string reset("\033[0m");

#define getIndex(i, j) ((i-1) * n + j)

void SetCursorPos(int XPos, int YPos)
{
    printf("\033[%d;%dH", YPos+1, XPos+1);
}

void mind(int z)
{
    int z1, z2, del, h, kol;
    h = 1;//Временно
    z1 = z;
    for(int i = 0; i < 4; i++){
            counts[i] = 0;
            z2 = z1;
            for(int j = 0; j < 4; j++){
                counts[stone[z2]]++;
                z2 += h;
            }
            kol = counts[1] + 1;
            del = est[kol];
            if(kol > 1){
                del -= est[kol - 1];
            }
            z2 = z1;
            for(int j = 0; j < 4; j++){
                pot[z2] += del;
                z2 += h;
            }
            z1 -= h;
    }
}

/*void outpot(int z, int k)//Выводить потенциал 1 2 3 4 X 4 3 2 1
{
    cout<<endl;
    //SetCursorPos(15, 15);
    for(int i = 1; i <= z; i++){
        if(stone[i] == 1){
            est[1] = 1;
            cout<<"Для строки "<<k<<" Оценка = ";

            cout<<endl;
            cout<<reset;
        }
    }
}*/
/*void outpot(int k)//Выводить потенциал 1 2 3 4 Х 4 3 2 1
{
    int c = -1,  sum = 0;
    for (int i = 0; i < n && c == -1; i++){
        for(int j = 0; j < m; j++){
            if(stone[i * n + j] == 1){
                c = i;
                //cout<<k;
                //break;
            }
        }
    }
    for(int i = 0; i < 5; i++){
        counts[i] = stone[c * n + i + 1];
       // cout<<counts[i]<<' ';
    }
    for(int i = 0; i < 5; i++){
        sum += counts[i];
    }
    //cout<<endl;
    cout<<"Для строки "<<k<<" Оценка = ";
   // if(sum == 0){
     //   cout<<"0";
    //}
    if(sum == 1){
        for(int j = 0; j < 4; j++){
            cout<<est[1]<<' ';
            l = est[1]++;
        }
        for(int k = 4; k > 0; k--){
            est[1]--;
            cout<<est[1]<<' ';
        }
    }
    if(sum == 2){
        for(int j = 0; j < 4; j++){
            cout<<est[2]<<' ';
            est[2]++;
        }
        for(int k = 4; k > 0; k--){
            est[2]--;
            cout<<est[2]<<' ';
        }
    }
    cout<<endl;
    if(sum == 3){
        for(int j = 0; j < 4; j++){
            cout<<est[3]<<' ';
            est[3]++;
        }
        for(int k = 4; k > 0; k--){
            est[3]--;
            cout<<est[3]<<' ';
        }
    }
    if(sum == 4){
        for(int j = 0; j < 4; j++){
            cout<<est[4]<<' ';
            est[4]++;
        }
        for(int k = 4; k > 0; k--){
            est[4]--;
            cout<<est[4]<<' ';
        }
    }
    if(sum == 5){
        for(int j = 0; j < 4; j++){
            cout<<est[5]<<' ';
            est[5]++;
        }
        for(int k = 4; k > 0; k--){
            est[5]--;
            cout<<est[5]<<' ';
        }
    }

}
*/
void screen()//Главная хрень
{
  cout<<"Введите размерность(n * m)"<<endl;
    cin>>n>>m;
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            cout<<stone[(i-1)*n+j]<<' ';
        }
        cout<<endl;
    }
}

void place(int i, int j, bool x)
{
    auto goodIndex = [&] (int i, int j) {
        bool ok = i >= 1 && i <= n && j >= 1 && j <= m;
        if (!ok) return false;
        // if (stone[getIndex(i, j)] == GRANICA) return false;
        return true;
    };
    for (int w = 0; w < 4; w++) {
        for (int k = 0; k < 5; k++) {
            int kol_x = 0, kol_o = 0;
            int is = i - dy[w] * (4-k);
            int js = j - dx[w] * (4-k);
            for (int p = 0; p < 5; p++) {
                int in = is + dy[w] * p;
                int jn = js + dx[w] * p;
                if (goodIndex(in, jn))
                    if (stone[getIndex(in, jn)] == 1)
                        kol_x++;
                    if (stone[getIndex(in, jn)] == 2)
                        kol_o++;
            }
            //cout << kol_x << " " << kol_o << endl;
            for (int p = 0; p < 5; p++) {
                int in = is + dy[w] * p;
                int jn = js + dx[w] * p;
                //cout << "in " << in << " " << jn << endl;
                if (goodIndex(in, jn))
                    if (x && (stone[getIndex(in, jn)] == 0))
                        pot[getIndex(in, jn)] += est[kol_x];
                    else {
                        //pot[getIndex(in, jn)] += est[kol_o];
                        pot[getIndex(in, jn)] = 0;
                    }
            }
        }

    }
}

void showoutpot()//Оценка в виде матрицы
{
    //int c;
    SetCursorPos(40, 0);
    /*
    for(int i = 1; i <= n; i++){
        SetCursorPos(40, i - 1);
        for(int j = 1; j <= m; j++){
            if(stone[(i - 1) * n + j] == 1){
                pot[(i - 1) * n + j] = 4;
            }
            cout<<pot[(i - 1) * n + j]<<' ';
        }
        cout<<endl;
    }
    */

    // 4 3 11 11 3 4
    // 31 21 21 31

    for (int i = 1; i <= n; i++) {
        SetCursorPos(40, i - 1);
        for (int j = 1; j <= m; j++) {
            if (i == last_i && j == last_j)
                cout << blue;
            if (stone[(i - 1) * n + j] == 1)
                    cout << red << setw(6) << 'X' << " ";
            else if (stone[(i - 1) * n + j] == 2)
                    cout << green << setw(6) << 'O' << " ";
            else {
                cout << setw(6) << pot[getIndex(i, j)] << " ";
            }
            cout << reset;
        }
        cout << endl;
    }
}

void board()//Заполнение доски
{
    int k, b, z;
    char c;
    cout<<"Введите крестик или нолик"<<endl;
    cin >> c;
    /*if(c != 'x' || c != 'o'){
        cout<<"Плохо\n Введите повторно! ";
    }*/
    bool krestik = c == 'x';

    cout<<"Укажите координаты поля"<<endl;
    while(cin>>k>>b){
    last_i = k;
    last_j = b;

    /*switch (c){
            case 'x':
                stone[z] = 1;
                break;
            case 'o':
                stone[z] = 2;
                break;
            default:
                cout<<"Введён неверный символ"<<endl;
                board();
                break;
        }*/
    cout<<"Строка = "<<k<<" Столбец = "<<b<<endl;
    //cout<<red;
    system("clear");
    SetCursorPos(0, 0);
    z = (k - 1) * n + b;
        if(krestik){
            stone[z] = 1;
        }
        else if(!krestik){
            stone[z] = 2;
        }
        place(k, b, krestik);
        SetCursorPos(0, 0);
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= m; j++){
                if (stone[(i - 1) * n + j] == 1)
                    cout << red;
                else if (stone[(i - 1) * n + j] == 2)
                    cout << green;
                cout<<stone[(i - 1) * n + j]<<' ';
                cout << reset;
            }
            cout<<endl;
        }
        showoutpot();
        //outpot(k);
        cout<<"Текущий ход: Строка = "<<k<<" Столбец = "<<b<<endl;

        krestik = !krestik;
        cout<<"Укажите координаты поля"<<endl;
    }

}
int main()
{
    screen();
    board();
    return 0;
}
