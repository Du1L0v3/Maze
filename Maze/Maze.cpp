#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

/*地图总长度L*/
#define L 40

/*在用深度优先和随机数法产生地图时设置的五个常量*/
#define Wall 0  //墙的值（也是标识）
#define Road 1 //路的值（也是标识）
#define Start 2  //入口的值（也是标识）
#define End 3  //出口的值（也是标识）
#define Ans 4  //已经走过的路的值（也是标识）

/*定义结构体来储存当前所在的的坐标*/
struct Position
{
	int x;
	int y;
}Player;

/*用一个二维数组来储存四个方向*/
int Dst[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };

/*在用递归分割产生地图时设置的五个常量*/
static int WALL = 1;
static int ROUTE = 0;
static int START = 2;
static int END = 3;
static int ANS = 4;

/*控制由深度优先创造的地图的复杂度，数值越大复杂度越低，最小值为 0 */
int Rank = 0;

/*定义全局二维数组来储存在调用函数中用动态内存分配临时产生的地图*/
int Dot[L + 5][L + 5];

int S;

int T;

int u = 0, v = 0;

void HideCursor(int i);  //隐藏与恢复光标

void turnto(int x, int y);  //光标移动

void move_LR(int x, int y);  //字符向左向右移动

void move_UD(int x, int y);  //字符向上向下移动

void Color(const unsigned short color);  //颜色控制

void Create_main_DF();  //创造地图——主干——深度优先

void Create_Maze_DF(int** maze, int x, int y);  //创造地图——生成迷宫——深度优先

void Create_main_RS();  //创造地图——主干——递归分割

void Create_Maze_RS(int** maze, int x1, int y1, int x2, int y2);  //创造地图——生成迷宫——递归分割

void Create_main_Random();  //创造地图——主干——随机创造

void Create_Maze_Random(int** maze);  //创造地图——生成迷宫——随机创造

void Create_Paint(int** maze);  //创造地图——画地图

void Game_1_main();  //人工摸索探路

void Game_1_Control_S0(int a, int b);  //人工操作_1

void Game_1_Control_S1(int a, int b);  //人工操作_2

void Game_2_main();  //缺德地图导航

void DFS_main();  //深度优先搜索——主体

void DFS_Make(int x, int y);  //深度优先搜索——实现

void DFS_main_Instance();  //深度优先搜索——主体——特判递归分割

void DFS_Make_Instance(int x, int y);  //深度优先搜索——实现——特判递归分割

void Music_win(int n);  //胜利音效

int main(void)  //主函数
{
Rp:
	int e = 0;  //e 是用来判断是否输入错误并对界面进行调整的变量
Re:
	/*这是后面输入错误的情况*/
	if (e == 1)
	{
		Sleep(700);
		e = 2;
	}

	system("cls");
	Color(9);
	printf("\n\n\n\n\t\t\t\t\tWelcome to duidui's Game --- Maze !\n\n");

	/*这个是刚进来时的操作*/
	if (e != 2)
		Sleep(500);  //延时0.5秒
	else
		e = 0;

	Color(7);
	printf("\t\t\t\t\t   请选择你想要进行的游戏模式:\n\n");
	printf("\t\t\t\t\t         1.人工摸索探路\n\n");
	printf("\t\t\t\t\t         2.缺德地图导航\n\n");
	HideCursor(0);  //隐藏光标

	char f = _getch();  //f 用来做输入的那个字符，判断进行哪种操作
	system("cls");
	if (f == '1')  //人工
	{
		printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t    请输入一个数值，值越小难度越大\n\n\n\t\t\t\t\t\t\t   ");
		HideCursor(1);
		scanf_s("%d", &Rank);
		HideCursor(0);
		Game_1_main();
	}
	else if (f == '2')  //智能
	{
		printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t    请输入一个数值，值越小难度越大\n\n\n\t\t\t\t\t\t\t   ");
		HideCursor(1);
		scanf_s("%d", &Rank);
		HideCursor(0);
		Game_2_main();
	}
	else  //如果输入的字符与操作字符无关，则重新输入
	{
		Color(4);
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t  输错了嗷，请重新输入！\n");
		e = 1;
		goto Re;
	}

	if (T == 1)
		goto Rp;
	/*等待退出*/
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t   按任意键退出！\n\n\n\n\n\n\n\n");
	int z = _getch();
	return 0;
}

void HideCursor(int i)  //隐藏与恢复光标
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, i };  //i 为 0 的时候隐藏光标，i 为 1 的时候恢复
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void turnto(int x, int y)  //光标移动
{
	COORD loc;
	loc.X = x;
	loc.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
}

void move_LR(int x, int y)  //字符向左向右移动
{
	printf("人");
	turnto(x, y);
	printf("  ");
}

void move_UD(int x, int y)  //字符向上向下移动
{
	printf("人");
	turnto(x, y);
	printf("  ");
}

void Color(const unsigned short color)  //颜色控制
{   /*仅限改变0-15的颜色;如果在0-15那么实现他的颜色   因为如果超过15后面的改变的是文本背景色。*/
	if (color >= 0 && color <= 15)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
/*  颜色对应值：
　　0=黑色                8=灰色　　
	1=蓝色                9=淡蓝色                　　
　　2=绿色                10=淡绿色     　　
　　3=湖蓝色              11=淡浅绿色
　　4=红色                12=淡红色 　
　　5=紫色                13=淡紫色    　　
　　6=黄色                14=淡黄色
　　7=白色                15=亮白色
*/

void Create_main_DF()  //创造地图——主干——深度优先
{
	srand((unsigned)time(NULL));  //随机数种子

	/*给二维数组分配动态内存*/
	int** Maze = (int**)malloc(L * sizeof(int));
	for (int i = 0; i < L; i++)
		Maze[i] = (int*)calloc(L, sizeof(int));

	/*最外围层设为路径的原因，为了防止挖路时挖出边界，同时为了保护迷宫主体外的一圈墙体被挖穿*/
	for (int i = 0; i < L; i++)
	{
		Maze[i][0] = Road;
		Maze[0][i] = Road;
		Maze[i][L - 1] = Road;
		Maze[L - 1][i] = Road;
	}

	/*以（2，2）为起点创造迷宫*/
	Create_Maze_DF(Maze, 2, 2);

	/*迷宫入口及出口*/
	Maze[2][1] = Road;

	/*由于算法随机性，出口有一定概率不在（L-3,L-2）处，此时需要寻找出口*/
	for (int i = L - 3; i >= 0; i--)
		if (Maze[i][L - 3] == Road)
		{
			Maze[i][L - 2] = Road;
			break;
		}

	Create_Paint(Maze);  //画出地图

	/*释放内存*/
	for (int i = 0; i < L; i++)
		free(Maze[i]);
	free(Maze);
}

void Create_Maze_DF(int** maze, int x, int y)  //创造地图——生成迷宫——深度优先
{
	maze[x][y] = Road;

	/*确保四个方向随机，即打乱方向*/
	int direction[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
	for (int i = 0; i < 4; i++)
	{
		int r = rand() % 4;
		int temp = direction[0][0];
		direction[0][0] = direction[r][0];
		direction[r][0] = temp;

		temp = direction[0][1];
		direction[0][1] = direction[r][1];
		direction[r][1] = temp;
	}

	/*朝四个方向开挖*/
	for (int i = 0; i < 4; i++)
	{
		int dx = x;
		int dy = y;

		/*控制挖的距离，由Rank来调整*/
		int range = (Rank == 0 ? 0 : rand() % Rank) + 1;

		while (range > 0)
		{
			dx += direction[i][0];
			dy += direction[i][1];

			if (maze[dx][dy] == Road)  //排除回头路
				break;

			/*判断是否挖穿路径*/
			int count = 0;
			for (int j = dx - 1; j < dx + 2; j++)
				for (int k = dy - 1; k < dy + 2; k++)
					/* abs(j - dx) + abs(k - dy) == 1 确保只判断九宫格内四个特定位置 即上下左右*/
					if (abs(j - dx) + abs(k - dy) == 1 && maze[j][k] == Road)
						count++;

			if (count > 1)
				break;

			/*确保不会挖穿，前进*/
			range--;
			maze[dx][dy] = Road;
		}

		/*没有挖穿危险，以此节点递归*/
		if (range <= 0)
			Create_Maze_DF(maze, dx, dy);
	}
}
void Create_main_RS()  //创造地图——主干——递归分割
{
	srand((unsigned)time(NULL));  //随机数种子

	/*给二维数组分配动态内存*/
	int** Maze = (int**)malloc(L * sizeof(int*));
	for (int i = 0; i < L; i++)
		Maze[i] = (int*)calloc(L, sizeof(int));

	/*最外侧是墙*/
	for (int i = 0; i < L; i++)
	{
		Maze[0][i] = WALL;
		Maze[i][0] = WALL;
		Maze[L - 1][i] = WALL;
		Maze[i][L - 1] = WALL;
	}

	/*生成迷宫*/
	Create_Maze_RS(Maze, 1, 1, L - 2, L - 2);

	/*起点与终点*/
	Maze[2][0] = START;
	Maze[L - 2][L - 1] = END;

	/*画迷宫*/
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
			if (Maze[i][j] == WALL)
				printf("口");
			else
				printf("  ");
		printf("\n");
	}
	turnto(0, 2);
	printf("入");
	turnto(2 * (L - 1), L - 2);
	printf("出");

	/*把临时储存的地图转入全局数组中*/
	for (int i = 0; i < L; i++)
		for (int j = 0; j < L; j++)
			Dot[i][j] = Maze[i][j];

	/*释放内存*/
	for (int i = 0; i < L; i++)
		free(Maze[i]);
	free(Maze);
}

void Create_Maze_RS(int** maze, int x1, int y1, int x2, int y2)  //创造地图——生成迷宫——递归分割
{
	/*判断能否继续分割*/
	if (x2 - x1 < 2 || y2 - y1 < 2)
		return;

	/*随机取点*/
	int x = rand() % (x2 - x1 - 1) + x1 + 1;
	int y = rand() % (y2 - y1 - 1) + y1 + 1;

	/*划墙*/
	for (int i = x1; i <= x2; i++)
		maze[i][y] = WALL;
	for (int i = y1; i <= y2; i++)
		maze[x][i] = WALL;

	/*递归分割，继续划分区域*/
	Create_Maze_RS(maze, x1, y1, x - 1, y - 1);
	Create_Maze_RS(maze, x + 1, y + 1, x2, y2);
	Create_Maze_RS(maze, x + 1, y1, x2, y - 1);
	Create_Maze_RS(maze, x1, y + 1, x - 1, y2);

	/*随机取其中三面墙*/
	int r[4] = { 0 };
	r[rand() % 4] = 1;

	/*在墙上随机取点开孔*/
	for (int i = 0; i < 4; i++)
		if (r[i] == 0)
		{
			int rx = x;
			int ry = y;

			switch (i)
			{
			case 0:
				/*判断该位置是否能确保打通相邻两块区域，判断依据，上下左右位置最多只有两面墙，下面一样*/
				do
				{
					rx = rand() % (x - x1) + x1;
				} while (maze[rx - 1][ry] + maze[rx + 1][ry] + maze[rx][ry - 1] + maze[rx][ry + 1] > 2 * WALL);
				break;
			case 1:
				do
				{
					ry = rand() % (y2 - y) + y + 1;
				} while (maze[rx - 1][ry] + maze[rx + 1][ry] + maze[rx][ry - 1] + maze[rx][ry + 1] > 2 * WALL);
				break;
			case 2:
				do
				{
					rx = rand() % (x2 - x) + x + 1;
				} while (maze[rx - 1][ry] + maze[rx + 1][ry] + maze[rx][ry - 1] + maze[rx][ry + 1] > 2 * WALL);
				break;
			case 3:
				do
				{
					ry = rand() % (y - y1) + y1;
				} while (maze[rx - 1][ry] + maze[rx + 1][ry] + maze[rx][ry - 1] + maze[rx][ry + 1] > 2 * WALL);
				break;
			default:
				break;
			}
			maze[rx][ry] = ROUTE;
		}
}

void Create_main_Random()  //创造地图——主干——随机创造
{
	/*给二维数组分配动态内存*/
	int** Maze = (int**)malloc(L * sizeof(int*));
	for (int i = 0; i < L; i++)
		Maze[i] = (int*)calloc(L, sizeof(int));

	/*生成迷宫*/
	Create_Maze_Random(Maze);

	/*画迷宫*/
	Create_Paint(Maze);

	/*释放内存*/
	for (int i = 0; i < L; i++)
		free(Maze[i]);
	free(Maze);
}

void Create_Maze_Random(int** maze)  //创造地图——生成迷宫——随机创造
{
	srand((unsigned)time(NULL));

	/*使用随机数来生成迷宫中的墙*/
	for (int i = 0; i < L; i++)
		for (int j = 0; j < L; j++)
			if (i == L - 3 && j == L - 2)
				maze[i][j] = Road;
			else if (i == 1 || i == L - 2 || j == 1 || j == L - 2)
				maze[i][j] = Wall;
			else if (i == 0 || i == L - 1 || j == 0 || j == L - 1)
				maze[i][j] = Road;
			else if (((i == 0 || i == 1) && j == 1) || ((i == L - 1 || i == L - 2) && j == L - 2))
				maze[i][j] = Road;
			else
			{
				maze[i][j] = rand() % 2;
				if (maze[i][j] == 0)
				{
					int kaf = rand() % 100 + 1;
					if (kaf <= 25)
						maze[i][j] = Road;
				}
			}
}

void Create_Paint(int** maze)  //创造地图——画地图
{
	/*将地图画出*/
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < L; j++)
			if (maze[i][j] == Wall)
				printf("口");
			else if (i == 2 && j == 0)
				printf("入");
			else if (i == L - 3 && j == L - 1)
				printf("出");
			else
				printf("  ");
		printf("\n");
	}

	/*把临时储存的地图转入全局数组中*/
	for (int i = 0; i < L; i++)
		for (int j = 0; j < L; j++)
			Dot[i][j] = maze[i][j];

	Dot[2][0] = Start;
	Dot[L - 3][L - 1] = End;
}

void Game_1_main()  //人工摸索探路
{
T:
	srand((unsigned)time(NULL));  //随机种子

	system("cls");
	Color(10);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t   欢迎来到——Maze！\n\n\n\n\n\n\n\n");
	Color(7);

	Sleep(600);
	system("cls");

	/*利用随机数来产生概率*/
	int random = rand() % 100 + 1;
	if (random <= 5)
	{
		S = 0;
		Create_main_Random();
	}
	else if (random >= 80)
	{
		S = 1;
		Create_main_RS();
	}
	else
	{
		S = 0;
		Create_main_DF();
	}

	int a = 2;
	int b = 2;

	turnto(a, b);
	printf("人");  //角色

	/*因为利用递归分割得到的地图墙和路的值是不一样的，所以这里使用 S 来判断是用哪种方法产生的地图*/
	if (S == 0)
		Game_1_Control_S0(a, b);
	else
		Game_1_Control_S1(a, b);
K:
	Sleep(1000);
	system("cls");
	Color(2);
	printf("\n\n\n\n\n\n\n\n\n\t\t\t\t\t  emmm既然通关了，要不要再来一次？");
	Color(3);
	printf("\n\n\t\t\t\t\t\t\t1.来！");
	Color(4);
	printf("\n\n\t\t\t\t\t\t\t2.溜！");
	Color(5);
	printf("\n\n\t\t\t\t\t\t    3.返回主菜单!");
	Color(7);

	T = 0;
	/*判断用户输入*/
	fflush(stdin);
	char t = _getch();
	if (t == '1')
		goto T;
	else if (t == '2')
	{
		printf("\n\n\t\t\t\t\t\t    谢谢您的游玩啦");
		Sleep(800);
		return;
	}
	else if (t == '3')
	{
		T = 1;
		return;
	}
	else
	{
		printf("\n\n\t\t\t\t\t\t 输错咯，再输一次嗷");
		Sleep(500);
		goto K;
	}
}

void Game_1_Control_S0(int a, int b)  //人工操作_1
{
	char c;

	Player.x = b;  //这个是纵坐标
	Player.y = a - 1;  //这个才是横坐标

	while ((c = _getch()) != 'p')  //字符的移动
	{
		if (c == (int)'w')
		{
			if (Dot[Player.x - 1][Player.y] == Wall)
			{
				turnto(a, b - 1);
				Color(4);
				printf("口");
				turnto(a, b - 1);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x - 1][Player.y] == Start)
			{
				turnto(a, b - 1);
				Color(3);
				printf("入");
				turnto(a, b - 1);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x - 1][Player.y] == End)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a, b - 1);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a, b - 1);
				move_UD(a, b);
				turnto(a, b - 1);
				b = b - 1;
				Player.x = Player.x - 1;
			}
		}
		if (c == (int)'a')
		{
			if (Dot[Player.x][Player.y - 1] == Wall)
			{
				turnto(a - 2, b);
				Color(4);
				printf("口");
				turnto(a - 2, b);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y - 1] == Start)
			{
				turnto(a - 2, b);
				Color(3);
				printf("入");
				turnto(a - 2, b);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y - 1] == End)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a - 2, b);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a - 2, b);
				move_LR(a, b);
				turnto(a - 2, b);
				a = a - 2;
				Player.y = Player.y - 1;
			}
		}
		if (c == (int)'s')
		{
			if (Dot[Player.x + 1][Player.y] == Wall)
			{
				turnto(a, b + 1);
				Color(4);
				printf("口");
				turnto(a, b + 1);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x + 1][Player.y] == Start)
			{
				turnto(a, b + 1);
				Color(3);
				printf("入");
				turnto(a, b + 1);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x + 1][Player.y] == End)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a, b + 1);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a, b + 1);
				move_UD(a, b);
				turnto(a, b + 1);
				b = b + 1;
				Player.x = Player.x + 1;
			}
		}
		if (c == (int)'d')
		{
			if (Dot[Player.x][Player.y + 1] == Wall)
			{
				turnto(a + 2, b);
				Color(4);
				printf("口");
				turnto(a + 2, b);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y + 1] == Start)
			{
				turnto(a + 2, b);
				Color(3);
				printf("入");
				turnto(a + 2, b);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y + 1] == End)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a + 2, b);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a + 2, b);
				move_LR(a, b);
				turnto(a + 2, b);
				a = a + 2;
				Player.y = Player.y + 1;
			}
		}
	}
}

void Game_1_Control_S1(int a, int b)  //人工操作_2
{
	char c;

	Player.x = b;  //这个是纵坐标
	Player.y = a - 1;  //这个才是横坐标

	while ((c = _getch()) != 'p')  //字符的移动
	{
		if (c == (int)'w')
		{
			if (Dot[Player.x - 1][Player.y] == WALL)
			{
				turnto(a, b - 1);
				Color(4);
				printf("口");
				turnto(a, b - 1);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x - 1][Player.y] == START)
			{
				turnto(a, b - 1);
				Color(3);
				printf("入");
				turnto(a, b - 1);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x - 1][Player.y] == END)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a, b - 1);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a, b - 1);
				move_UD(a, b);
				turnto(a, b - 1);
				b = b - 1;
				Player.x = Player.x - 1;
			}
		}
		if (c == (int)'a')
		{
			if (Dot[Player.x][Player.y - 1] == WALL)
			{
				turnto(a - 2, b);
				Color(4);
				printf("口");
				turnto(a - 2, b);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y - 1] == START)
			{
				turnto(a - 2, b);
				Color(3);
				printf("入");
				turnto(a - 2, b);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y - 1] == END)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a - 2, b);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a - 2, b);
				move_LR(a, b);
				turnto(a - 2, b);
				a = a - 2;
				Player.y = Player.y - 1;
			}
		}
		if (c == (int)'s')
		{
			if (Dot[Player.x + 1][Player.y] == WALL)
			{
				turnto(a, b + 1);
				Color(4);
				printf("口");
				turnto(a, b + 1);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x + 1][Player.y] == START)
			{
				turnto(a, b + 1);
				Color(3);
				printf("入");
				turnto(a, b + 1);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x + 1][Player.y] == END)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a, b + 1);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a, b + 1);
				move_UD(a, b);
				turnto(a, b + 1);
				b = b + 1;
				Player.x = Player.x + 1;
			}
		}
		if (c == (int)'d')
		{
			if (Dot[Player.x][Player.y + 1] == WALL)
			{
				turnto(a + 2, b);
				Color(4);
				printf("口");
				turnto(a + 2, b);
				Sleep(300);
				Color(7);
				printf("口");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y + 1] == START)
			{
				turnto(a + 2, b);
				Color(3);
				printf("入");
				turnto(a + 2, b);
				Sleep(400);
				Color(7);
				printf("入");
				turnto(a, b);
				printf("\a");
			}
			else if (Dot[Player.x][Player.y + 1] == END)
			{
				for (int i = 0; i <= 15; i++)
				{
					turnto(a + 2, b);
					Color(i);
					printf("出");
					Sleep(100);
					Music_win(i);
				}
				system("cls");
				printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t哈哈哈哈恭喜你通关！\n\n\n\n\n\n\n\n");
				break;
			}
			else
			{
				turnto(a + 2, b);
				move_LR(a, b);
				turnto(a + 2, b);
				a = a + 2;
				Player.y = Player.y + 1;
			}
		}
	}
}

void Game_2_main()  //缺德地图导航
{
	srand((unsigned)time(NULL));

	system("cls");
	Color(10);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t欢迎使用缺德地图导航！\n\n\n\n\n\n\n\n");
	Color(7);

	Sleep(600);
	system("cls");
	int random = rand() % 99 + 1;
	if (random <= 5)
	{
		S = 0;
		Create_main_Random();
	}
	else if (random >= 80)
	{
		S = 1;
		Create_main_RS();
	}
	else
	{
		S = 0;
		Create_main_DF();
	}

	Sleep(1000);
	if (S == 0)
		DFS_main();
	else
		DFS_main_Instance();

	int t = _getch();
}

void DFS_main()  //深度优先搜索——主体
{
	int x = 2;
	int y = 1;
	DFS_Make(x, y);
	if (u == 0)
	{

		Color(4);
		printf("\a\n\n\n\n\t\t\t\t\t\t 抱歉哇，没得出口嗷！\n\n\n\n\n\n\n\n");
		Color(7);
		Sleep(300);
	}
}

void DFS_Make(int x, int y)  //深度优先搜索——实现
{
	if (v == 1)
		return;

	if (Dot[x][y] == End)
	{
		system("cls");
		for (int i = 0; i < L; i++)
		{
			for (int j = 0; j < L; j++)
				if (Dot[i][j] == Wall)
					printf("口");
				else if (Dot[i][j] == Ans)
				{
					Color(4);
					printf("国");
					Color(7);
				}
				else if (i == 2 && j == 0)
				{
					Color(3);
					printf("入");
					Color(7);
				}
				else if (i == L - 3 && j == L - 1)
				{
					Color(2);
					printf("出");
					Color(7);
				}
				else
					printf("  ");
			printf("\n");
		}
		u = 1;
		v = 1;
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		int next_x = x + Dst[i][0];
		int next_y = y + Dst[i][1];

		if (next_x >= 0 && next_x <= L - 1 && next_y >= 0 && next_y <= L - 1 && (Dot[next_x][next_y] != Wall) && (Dot[next_x][next_y] != Ans) && (Dot[next_x][next_y] != Start))
		{
			Dot[x][y] = Ans;
			DFS_Make(next_x, next_y);
			Dot[x][y] = Road;
		}
	}

	return;
}

void DFS_main_Instance()  //深度优先搜索——主体——特判递归分割
{
	int x = 2;
	int y = 1;
	DFS_Make_Instance(x, y);
}

void DFS_Make_Instance(int x, int y)  //深度优先搜索——实现——特判递归分割
{
	if (v == 1)
		return;

	if (Dot[x][y] == END)
	{
		system("cls");
		for (int i = 0; i < L; i++)
		{
			for (int j = 0; j < L; j++)
				if (Dot[i][j] == WALL)
					printf("口");
				else if (Dot[i][j] == ANS)
				{
					Color(4);
					printf("国");
					Color(7);
				}
				else if (i == 2 && j == 0)
				{
					Color(3);
					printf("入");
					Color(7);
				}
				else if (Dot[i][j] == END)
				{
					Color(2);
					printf("出");
					Color(7);
				}
				else
					printf("  ");
			printf("\n");
		}
		u = 1;
		v = 1;
		return;
	}

	for (int i = 0; i < 4; i++)
	{
		int next_x = x + Dst[i][0];
		int next_y = y + Dst[i][1];

		if (next_x >= 0 && next_x <= L - 1 && next_y >= 0 && next_y <= L - 1 && (Dot[next_x][next_y] != WALL) && (Dot[next_x][next_y] != ANS) && (Dot[next_x][next_y] != START))
		{
			Dot[x][y] = ANS;
			DFS_Make_Instance(next_x, next_y);
			Dot[x][y] = ROUTE;
		}
	}

	return;
}

void Music_win(int n)  //胜利音效
{
	switch (n)
	{
	case 0:
		Beep(494, 100);
		break;
	case 1:
		Beep(523, 100);
		break;
	case 2:
		Beep(578, 100);
		break;
	case 3:
		Beep(659, 100);
		break;
	case 4:
		Beep(698, 100);
		break;
	case 5:
		Beep(784, 100);
		break;
	case 6:
		Beep(880, 100);
		break;
	case 7:
		Beep(932, 100);
		break;
	case 8:
		Beep(1046, 100);
		break;
	case 9:
		Beep(1175, 100);
		break;
	case 10:
		Beep(1381, 100);
		break;
	case 11:
		Beep(1480, 100);
		break;
	case 12:
		Beep(1661, 100);
		break;
	default:
		break;
	}
}