//PROCESS
Console.WriteLine("ИГРА ПЯТНАШКИ\n");
Console.WriteLine("ENTER FIELD SIZE\n");
Point size = new(
    Int32.Parse(Console.ReadLine()),
    Int32.Parse(Console.ReadLine())
);

int[,] field_normal = normal_field(size.y, size.x);
Tuple<int[,], Point> pack = create_correct_field(ref size);
int[,] field = pack.Item1;
Point player = pack.Item2;
bool checking = false;
int step_count = 0;
Console.Clear();
while(!checking)
{
    Console.WriteLine($"STEP: {step_count}");
    print_field(field, size.y, size.x);
    Direction? step = Console.ReadKey(true).Key switch
    {
        ConsoleKey.UpArrow => Direction.UP,
        ConsoleKey.DownArrow => Direction.DOWN,
        ConsoleKey.RightArrow => Direction.RIGHT,
        ConsoleKey.LeftArrow => Direction.LEFT,
        _ => null
    };
    Console.Clear();
    if (!step.HasValue)
        continue;
    if (PlayerMove(ref player, field, size, step.Value) == null)
        continue;
    step_count++;
    checking = checking_fields(field_normal, field, size);
}

Console.WriteLine($"STEP: {step_count}");
print_field(field, size.y, size.x);
Console.Write("You won!");


///LOGIC
int random(int begin, int end) => new Random().Next(begin, end);


int[,] normal_field(int y, int x)
{
    int[,] arr = new int[y, x];
    int temp = 1;
    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
        {
            if (temp == x * y) break;
            arr[i, j] = temp;
            temp++;
        }
    }
    arr[y - 1, x - 1] = 0;
    return arr;
}


Tuple<int[,], Point> create_field(in Point coord)
{
    Point player_pos = new Point(0, 0);
    int n = coord.x * coord.y;
    int[,] arr = new int[coord.y, coord.x];
    int[] arr_temp = new int[n];
    for (int i = 0; i < n; i++)
        arr_temp[i] = i;
    int num;
    
    for (int i = 0; i < random(5, n * 2); i++)
    {
        int index1 = random(0, n - 1);
        int index2 = random(0, n - 1);
        int temp = arr_temp[index1];
        arr_temp[index1] = arr_temp[index2];
        arr_temp[index2] = temp;
    }

    for (int i = 0; i < coord.y; i++)
    {
        for (int j = 0; j < coord.x; j++)
        {
            arr[i, j] = arr_temp[coord.x * i + j];
            if (arr[i, j] == 0)
                player_pos = new Point(i, j);
        }
    }

    return Tuple.Create(arr, player_pos);
}


void print_field(int[,] field, int y, int x)
{
    for (int i = 0; i < y; i++)
    {
        for (int j = 0; j < x; j++)
            Console.Write($"{field[i, j]}\t");
        Console.WriteLine();
    }
}


int[,] MoveUp(ref Point coord, int[, ] arr)
{
    if(coord.y > 0)
    {
        arr[coord.y, coord.x] = arr[coord.y - 1, coord.x];
        coord.y--;
        arr[coord.y, coord.x] = 0;
        return arr;
    }
    return null;
}


int[,] MoveDown(ref Point coord, int[,] arr, int y)
{
    if (coord.y < y - 1)
    {
        arr[coord.y, coord.x] = arr[coord.y + 1, coord.x];
        coord.y++;
        arr[coord.y, coord.x] = 0;
        return arr;
    }
    return null;
}


int[,] MoveLeft(ref Point coord, int[,] arr)
{
    if (coord.x > 0)
    {
        arr[coord.y, coord.x] = arr[coord.y, coord.x - 1];
        coord.x--;
        arr[coord.y, coord.x] = 0;
        return arr;
    }
    return null;
}


int[,] MoveRight(ref Point coord, int[,] arr, int x)
{
    if (coord.x < x - 1)
    {
        arr[coord.y, coord.x] = arr[coord.y, coord.x + 1];
        coord.x++;
        arr[coord.y, coord.x] = 0;
        return arr;
    }
    return null;
}


int[,] PlayerMove(ref Point player_coord, int[,] arr, Point field_size, Direction s)
{
    switch (s)
    {
        case Direction.LEFT:
            return MoveLeft(ref player_coord, arr);
        case Direction.RIGHT:
            return MoveRight(ref player_coord, arr, field_size.y);
        case Direction.UP:
            return MoveUp(ref player_coord, arr);
        case Direction.DOWN:
            return MoveDown(ref player_coord, arr, field_size.x);
        default:
            return null;
    }
}


Tuple<int[,], Point>create_correct_field(ref Point coord)
{
    Direction[] arr = { Direction.LEFT, Direction.RIGHT, Direction.UP, Direction.DOWN };
    int[,] field = new int[coord.y, coord.x];
    Point player_pos = new Point(coord.y - 1, coord.x - 1);
    for (int i = 0; i < coord.y; i++)
        for (int j = 0; j < coord.x; j++)
            field[i, j] = coord.y * i + j + 1;
    field[player_pos.y, player_pos.x] = 0;

    for (int count = 0; count < random(50, 100); count++)
        PlayerMove(ref player_pos, field, coord, arr[random(0, 4)]);

    return Tuple.Create(field, player_pos);

}


bool checking_fields(int[,] norm, int[,] unnorm, Point coords)
{
    int size = coords.y * coords.x, count = 0;
    for (int i = 0; i < coords.y; i++)
        for (int j = 0; j < coords.x; j++)
            if (unnorm[i, j] == norm[i, j])
                count++;
    return count == size ? true : false;
}


enum Direction : int
{
    LEFT = 97,
    RIGHT = 100,
    UP = 119,
    DOWN = 115
};


struct Point
{
    public Point(int x, int y)
    {
        this.x = x;
        this.y = y;
    }
    public int y { get; set; }
    public int x { get; set; }
};



