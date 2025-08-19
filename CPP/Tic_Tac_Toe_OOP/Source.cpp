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
    cout << "������, ������������!\n��� ��������-������ v." << ver << ".\n";
}

playfield::~playfield()
{
    cout << "������� �� ����!\n";
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

bool turns::pickside() // ����� � ��� �
{
    if (side == 1 || side == 2 || side == 0)
    {
        switch (side)
        {
        case 1:
            cout << "�� ������� �.\n";
            break;
        case 2:
            cout << "�� ������� �.\n";
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
        if (cells[i][0] == cells[i][1] && cells[i][1] == cells[i][2] && cells[i][0] != ' ') // �������� ������ �� ������������
        {
            cout << cells[i][0] << " - ��������!\n";
            return true;
        }
        else if (cells[0][i] == cells[1][i] && cells[1][i] == cells[2][i] && cells[0][i] != ' ') // �������� ������ �� ���������
        {
            cout << cells[0][i] << " - ��������!\n";
            return true;
        }
    };
    if (cells[0][0] == cells[1][1] && cells[1][1] == cells[2][2] && cells[0][0] != ' ')
    {
        cout << cells[0][0] << " - ��������!\n";
        return true;
    }
    if (cells[0][2] == cells[1][1] && cells[1][1] == cells[2][0] && cells[0][2] != ' ')
    {
        cout << cells[0][0] << " - ��������!\n";
        return true;
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (cells[i][j] == ' ') return false; // continue game condition
        };
    };
    cout << "�����!\n";
    return true;
}
bool turns::player()
{
    int p_line, p_row;

    cout << "�������� ����� ������.\n";
    cin >> p_line;
    p_line--;
    if (p_line < 0 || p_line > 2) //try-catch ����� ��������
    {
        cout << "�������� ����������, ��������� ��� ���!\n";
        return false;
    }
    else
    {
        cout << "�������� ����� �������.\n";
        cin >> p_row; //try-catch ����� ��������
        p_row--;
        if (p_row < 0 || p_row > 2)
        {
            cout << "�������� ����������, ��������� ��� ���!\n";
            return false;
        }
        else
        {
            if (playfield::cells[p_line][p_row] != ' ')
            {
                cout << "������ ������, ������� ���������� ��� ���!\n";
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
    c_line = rand() % 3; // �������� �� 0 �� 2 
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
        cout << "�������� �������. ��� � - ������� 1, ��� � - ������� 2, 0 - ��� �� �����.\n";
        cin >> turn.side; //try-catch ����� ��������
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

