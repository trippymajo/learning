// Tic-Tac-Toe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib> // ��� rand() � RAND_MAX
using namespace std;

char cells[3][3];
void make_cells(void);
void playerturn(void);
void compturn(void);
void show_cells(void);
int check_win(void);

int main()
{
    setlocale(LC_ALL, "Russian"); //�� ���� � ���������
    cout << "������, ������������!\n��� ��������-������ v.1.1, �� ������� �� ��������.\n";
    make_cells();
    show_cells();
    do
    {
        playerturn();
        check_win();
        if (check_win() == 1) break;
        compturn();
        check_win();
        if (check_win() == 1) break;
        show_cells();
    }
        while (check_win() == 0);
    show_cells();
    cout << "������� �� ����!\n";
    return(0);
}
void make_cells()
{
    int i, j;
    for (i = 0; i<3; i++)
    {
        for (j = 0; j < 3; j++) cells[i][j] = ' ';
    };
}
void playerturn() //��� ������������
{
    int p_line, p_row;

    cout << "�������� ����� ������.\n";
    cin >> p_line;
    p_line--;
    if (p_line < 0 || p_line > 2)
    {
        cout << "�������� ����������, ��������� ��� ���!\n";
        playerturn();
    }
    else
    {
        cout << "�������� ����� �������.\n";
        cin >> p_row;
        p_row--;
        if (p_line < 0 || p_line > 2)
        {
            cout << "�������� ����������, ��������� ��� ���!\n";
            playerturn();
        }
        else
        {
            if (cells[p_line][p_row] != ' ')
            {
                cout << "������ ������, ������� ���������� ��� ���!\n";
                playerturn();
            }
            else cells[p_line][p_row] = 'X';
        }
    }
    /*do
    {
        cout << "�������� ����� ������.\n";
        cin >> p_stroka;
        p_stroka--;
    }
    while (p_stroka > 0 && p_stroka < 2);
    do
    {
        cout << "�������� ����� �������.\n";
        cin >> p_stolbez;
        p_stolbez--;
    }
    while (p_stolbez > 0 && p_stolbez < 2); 
    if (yacheiki[p_stroka][p_stolbez] != ' ')
    {
        cout << "������ ������, ������� ���������� ��� ���!\n";
        playerturn();
    }
    else yacheiki[p_stroka][p_stolbez] = 'X';*/
}
void compturn() // ��� ����������
{
    int c_line, c_row;
    c_line = rand() % 3; // �������� �� 0 �� 2 
    c_row = rand() % 3;
    if (cells[c_line][c_row] != ' ') compturn();
    else cells[c_line][c_row] = 'O';
}
void show_cells()
{
    int x;
    cout << "- - -\n";
    for (x = 0; x < 3; x++)
    {
        cout << cells[x][0] << "|" << cells[x][1] << "|" << cells[x][2] << "\n";
    };
    cout << "- - -\n";
}
int check_win() //��� ����� � ��� ����������� ����.
{
    int i, j;
    for (i = 0; i < 3; i++) 
    {
        if (cells[i][0] == cells[i][1] && cells[i][1] == cells[i][2] && cells[i][0] != ' ') // �������� ������ �� ������������
        {
            cout << cells[i][0] << " - ��������!\n";
            return(1);
        }
        else if (cells[0][i] == cells[1][i] && cells[1][i] == cells[2][i] && cells[0][i] != ' ') // �������� ������ �� ���������
        {
            cout << cells[0][i] << " - ��������!\n";
            return(1);
        }
    };
    if (cells[0][0] == cells[1][1] && cells[1][1] == cells[2][2] && cells[0][0] != ' ')
    {
        cout << cells[0][0] << " - ��������!\n";
        return(1);
    }
    if (cells[0][2] == cells[1][1] && cells[1][1] == cells[2][0] && cells [0][2] != ' ')
    {
        cout << cells[0][0] << " - ��������!\n";
        return(1);
    }
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (cells[i][j] == ' ') return(0); // ������� ����������� ����
        };
    };
    cout << "�����!\n";
    return(1);
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
