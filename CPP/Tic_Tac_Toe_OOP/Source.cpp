#include <iostream>
#include <cstdlib>
#include <ctime>
#include "header.h"

const float ver = 2.0;
using namespace std;

playfield::playfield()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cells[i][j] = ' ';
        }
    }
    cout << "Привет, пользователь!\nЭто крестики-нолики v." << ver << ".\n";
}

playfield::~playfield()
{
    cout << "Спасибо за игру!\n";
}

void playfield::show_cells()
{
    cout << "- - -\n";
    for (int i = 0; i < 3; i++)
    {
        cout << cells[i][0] << "I" << cells[i][1] << "I" << cells[i][2] << "\n";
    };
    cout << "- - -\n";
}

bool turns::pickside() // выбор Х или О
{
    if (side == 1 || side == 2 || side == 0)
    {
        switch (side)
        {
        case 1:
            cout << "Вы выбрали Х.\n";
            break;
        case 2:
            cout << "Вы выбрали О.\n";
            break;
        case 0:
            exit(0);
        }
        return true;
    }
    else return false;
}

bool playfield::check_win()
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        if (cells[i][0] == cells[i][1] && cells[i][1] == cells[i][2] && cells[i][0] != ' ') // Проверка победы по горизонталям
        {
            cout << cells[i][0] << " - победили!\n";
            return true;
        }
        else if (cells[0][i] == cells[1][i] && cells[1][i] == cells[2][i] && cells[0][i] != ' ') // Проверка победы по вертикали
        {
            cout << cells[0][i] << " - победили!\n";
            return true;
        }
    };
    if (cells[0][0] == cells[1][1] && cells[1][1] == cells[2][2] && cells[0][0] != ' ')
    {
        cout << cells[0][0] << " - победили!\n";
        return true;
    }
    if (cells[0][2] == cells[1][1] && cells[1][1] == cells[2][0] && cells[0][2] != ' ')
    {
        cout << cells[0][0] << " - победили!\n";
        return true;
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (cells[i][j] == ' ') return false; // continue game condition
        };
    };
    cout << "Ничья!\n";
    return true;
}
bool turns::player()
{
    int p_line, p_row;

    cout << "Напишите номер строки.\n";
    cin >> p_line;
    p_line--;
    if (p_line < 0 || p_line > 2) //try-catch нужно вставить
    {
        cout << "Неверные координаты, поробуйте еще раз!\n";
        return false;
    }
    else
    {
        cout << "Напишите номер столбца.\n";
        cin >> p_row; //try-catch нужно вставить
        p_row--;
        if (p_row < 0 || p_row > 2)
        {
            cout << "Неверные координаты, поробуйте еще раз!\n";
            return false;
        }
        else
        {
            if (playfield::cells[p_line][p_row] != ' ')
            {
                cout << "Ячейка занята, введите координаты еще раз!\n";
                return false;
            }
            else
            {
                if (turns::side == 1) playfield::cells[p_line][p_row] = 'X';
                else playfield::cells[p_line][p_row] = 'O';
                return true;
            };
        }
    }
}
bool turns::computer()
{
    int c_line, c_row;
    c_line = rand() % 3; // диапазон от 0 до 2 
    c_row = rand() % 3;
    if (playfield::cells[c_line][c_row] != ' ') return false;
    else {
        if (turns::side == 1) playfield::cells[c_line][c_row] = 'O';
        else playfield::cells[c_line][c_row] = 'X';
        return true;
    };
}

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "Russian"); //RU Lang in console

    playfield field;
    turns turn;

    field.show_cells();
    do
    {
        cout << "Выберите сторону. Для Х - введите 1, Для О - введите 2, 0 - что бы выйти.\n";
        cin >> turn.side; //try-catch нужно вставить
    } while (!turn.pickside());
    if (turn.side == 1) 
    {
        do
        {
            while (!turn.player());
            while (!turn.computer());
            field.show_cells();
        } while (!field.check_win());
    }
    else
    {
        while (!field.check_win())
        {
            while (!turn.computer());
            if (field.check_win()) break;
            field.show_cells();
            while (!turn.player());
        } 
        field.show_cells();
    }
 
    return 0;
}

