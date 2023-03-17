

int[,] normal_field(int x, int y)
{
    int[,] arr = new int[y, x];
    int temp = 1;
    for(int i = 0; i < y; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (temp == x * y) break;
            arr[i, j] = temp;
            temp++;
        }
    }
    arr[y - 1, x - 1] = 0;
    return arr;

}


Tuple<int[,], Point> create_field(ref Point coord)
{
    Point player_pos;
    int n = coord.x * coord.y;
    int[,] arr = new int[coord.y, coord.x];
    int[] arr_temp = new int[n];
    for (int i = 0; i < n; i++)
        arr_temp[i] = i;
    int num;
    Random rnd = new Random();
    for(int i = 0; i < rnd.Next(5, n * 2); i++)
    {
        int index1 = rnd.Next(0, n - 1);
        int index2 = rnd.Next(0, n - 1);
        int temp = arr_temp[index1];
        arr_temp[index1] = arr_temp[index2];
        arr_temp[index2] = temp;
    }

    for(int i = 0; i < coord.y; i++)
    {
        arr[i] = new int[coord.y];
        for(int j = 0; j < coord.x; j++)
        {
            arr[i, j] = arr_temp[coord.x * i + j];
            if (arr[i, j] == 0)
                player_pos = (i, j);
        }    
    }


}



enum Direction : int
{
    LEFT = 97, RIGHT = 100, UP = 119, DOWN = 115
};

struct Point
{
    public int y; 
    public int x;
};
