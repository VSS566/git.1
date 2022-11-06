#include <iostream>
#include <random>
#include <conio.h>
#include <cstdlib>
#include <iomanip>
using namespace std;

enum Direction : int
{
    //W:    119
    //S:    115
    //A:    97
    //D:    100
    LEFT = 97, RIGHT = 100, UP = 119, DOWN = 115
};
struct Point
{
    int x;
    int y;
};
int random(int x, int y)
{
    random_device rng;
    uniform_int_distribution<int> distribution(x, y);
    return distribution(rng);
}

void qwerty(int x, int y)
{
    int** arr = new int* [x];
    int temp = 1;
    for (int i = 0; i < x; i++)
    {
        arr[i] = new int[y];
        for (int j = 0; j < y; j++)
        {
            if (temp == x * y) break;
            arr[i][j] = temp;
            cout << arr[i][j] << ' ';
            temp++;
        }
    }
}

int** normal_field(int x, int y)
{
    int** arr = new int* [x];
    int temp = 1;
    for (int i = 0; i < x; i++)
    {
        arr[i] = new int[y];
        for (int j = 0; j < y; j++)
        {
            if (temp == x * y) break;
            arr[i][j] = temp;
            temp++;
        }
    }
    return arr;
}


int** create_field(Point &coord)
{
    int n = coord.x * coord.y;
    int** arr = new int* [coord.x];
    int* arr_temp = new int[n];
    for (int i = 0; i < n; i++)
    {
        arr_temp[i] = i;
    }
    int num;
    for (int i = 0; i < random(5, n*2); i++) {
        int index1 = random(0, n-1);
        int index2 = random(0, n-1);
        int temp = arr_temp[index1];
        arr_temp[index1] = arr_temp[index2];
        arr_temp[index2] = temp;
    }
    for (int i = 0; i < coord.x; i++)
    {
        arr[i] = new int[coord.y];
        for (int j = 0; j < coord.y; j++)
        {
            arr[i][j] = arr_temp[coord.y*i+j];
        }
    }

    return arr;
}

void print_field(int** arr, int x, int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            cout << arr[i][j] << ' ';
        }
        cout << endl;
    }
}

int** MoveUp(Point coord, int** arr)
{
    if (coord.x > 0)
    {
        arr[coord.x][coord.y] = arr[coord.x - 1][coord.y];
        coord.x--;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return 0;
};
int** MoveDown(Point coord, int** arr, int x)
{
    if (coord.x < x)
    {
        arr[coord.x][coord.y] = arr[coord.x + 1][coord.y];
        coord.x++;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return 0;
};
int** MoveLeft(Point coord, int** arr)
{
    if (coord.y > 0)
    {
        arr[coord.x][coord.y] = arr[coord.x][coord.y - 1];
        coord.y++;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return 0;
};
int** MoveRight(Point coord, int** arr, int y)
{
    if (coord.y < y)
    {
        arr[coord.x][coord.y] = arr[coord.x][coord.y + 1];
        coord.y++;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return 0;
};


int** PlayerMove(Point coord, int** arr, int x, int y, Direction s)
{
    switch (s) {
    case LEFT:
        return MoveLeft(coord, arr);
    case RIGHT:
        return MoveRight(coord, arr, y);
    case UP:
        return MoveUp(coord, arr);
    case DOWN:
        return MoveDown(coord, arr, x);
    default:
        return 0;
    }

}

bool checking_fields(int** norm, int** unnorm)
{
    if (unnorm == norm)
        return true;
    return false;
}

int main()
{
    Point size;
    Point player;
    setlocale(LC_ALL, "rus");
    cout << "Èãðà ÏßÒÍÀØÊÈ\n"; cout << "ENTER FIELD SIZE";
    cin >> size.x >> size.y;
    int** field_normal = normal_field(size.x, size.y);
    int** field = create_field(size);
    while (!checking_fields(field_normal, field))
    {
        print_field(field, size.x, size.y);
        Direction step = static_cast<Direction>(_getch());
        field = PlayerMove(size, field, size.x, size.y, step);
        system("cls");
    }
}