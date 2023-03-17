#include <iostream>
#include <random>
#include <conio.h>
#include <cstdlib>
#include <iomanip>

using namespace std;

enum Direction : int
{
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
    arr[x - 1][y - 1] = 0;
    return arr;
}


std::pair<int**, Point> create_field(Point &coord)
{
    Point player_pos;
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
            if (arr[i][j] == 0)
                player_pos = { i, j };
        }
    }

    delete[] arr_temp;

    return { arr, player_pos };
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


int** MoveUp(Point &coord, int** arr)
{
    if (coord.x > 0)
    {
        arr[coord.x][coord.y] = arr[coord.x - 1][coord.y];
        coord.x--;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return nullptr;
};


int** MoveDown(Point &coord, int** arr, int x)
{
    if (coord.x < x-1)
    {
        arr[coord.x][coord.y] = arr[coord.x + 1][coord.y];
        coord.x++;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return nullptr;
};


int** MoveLeft(Point &coord, int** arr)
{
    if (coord.y > 0)
    {
        arr[coord.x][coord.y] = arr[coord.x][coord.y - 1];
        coord.y--;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return nullptr;
};


int** MoveRight(Point &coord, int** arr, int y)
{
    if (coord.y < y-1)
    {
        arr[coord.x][coord.y] = arr[coord.x][coord.y + 1];
        coord.y++;
        arr[coord.x][coord.y] = 0;
        return arr;
    }
    return nullptr;
};


int** PlayerMove(Point &player_coord, int** arr, Point field_size, Direction s)
{
    switch (s) {
    case LEFT:
        return MoveLeft(player_coord, arr);
    case RIGHT:
        return MoveRight(player_coord, arr, field_size.y);
    case UP:
        return MoveUp(player_coord, arr);
    case DOWN:
        return MoveDown(player_coord, arr, field_size.x);
    default:
        return nullptr;
    }
}


std::pair<int**, Point> create_correct_field(const Point& coord)
{
    Direction arr[4] = { LEFT, RIGHT, UP, DOWN };
    int** field = new int* [coord.x];
    Point player_pos = { coord.x - 1, coord.y - 1 };
    for (int i = 0; i < coord.x; i++)
    {
        field[i] = new int[coord.y];
        for (int j = 0; j < coord.y; j++)
        {
            field[i][j] = coord.y * i + j + 1;
        }
    }
    field[player_pos.x][player_pos.y] = 0;

    for (size_t count = 0; count < random(50, 100); count++)
    {
        PlayerMove(player_pos, field, coord, arr[random(0, 4)]);
    }

    return { field, player_pos };
}


bool checking_fields(int** norm, int** unnorm, Point coords)
{
    int size = coords.x * coords.y, count = 0;
    for (int i = 0; i < coords.x; i++)
        for (int j = 0; j < coords.y; j++)
            if (unnorm[i][j] == norm[i][j])
                count++;
    return count == size ? true : false;
}


int** generate_test_field() {
    int* a = new int[9]{
        1, 2, 3,
        4, 5, 6,
        7, 0, 8
    };
    return new int* [3]{ a, a + 3, a + 6 };
}


int main()
{
    Point size;
    
    setlocale(LC_ALL, "rus");
    cout << "Èãðà ÏßÒÍÀØÊÈ\n"; cout << "ENTER FIELD SIZE\n";
    cin >> size.x >> size.y;
    int** field_normal = normal_field(size.x, size.y);
    // Get the field and player position pack
    auto pack = create_correct_field(size);
    int** field = pack.first;
    Point player = pack.second;
    bool check = false;
    int step_count = 0;
    while (!check)
    {
        cout << "STEP:\t" << step_count << endl;
        print_field(field, size.x, size.y);
        Direction step = static_cast<Direction>(_getch());
        if (PlayerMove(player, field, size, step)) 
        {
            step_count++;
            check = checking_fields(field_normal, field, size);
        }
        system("cls");
    }
    cout << "STEP:\t" << step_count << endl;
    print_field(field, size.x, size.y);
    cout << "You won!";
}