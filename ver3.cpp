#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
const int width = 15;
const int high = 25;
const int enemyNum = 5;
int position_x, position_y;
int enemy_x[enemyNum], enemy_y[enemyNum];
int canvas[high][width];
int score;
bool dead = false;
int enemyMoveSpeed;

void gotoxy(int x, int y)  //光标移动到(x,y)位置
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void welcom()
{
	printf("打飞机小游戏");
	printf("w，a，s，d控制方向\n空格发射子弹\n按任意键继续");
	getch();
}

void startup()
{
	position_x = 22;
	position_y = 7;
	canvas[position_x][position_y] = 1;
	int k;
	for (k = 0; k < enemyNum; k++)
	{
		enemy_x[k] = rand() % 2;           // 产生新的飞机
		enemy_y[k] = rand() % width;
		canvas[enemy_x[k]][enemy_y[k]] = 2;
	}
	enemyMoveSpeed = 20;
	score = 0;
}

void show()
{
	HideCursor();
	gotoxy(0, 0);
	for (int i = 0; i < high; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (canvas[i][j] == 0)printf(" ");
			else if (canvas[i][j] == 1)printf("*");
			else if (canvas[i][j] == 2)printf("@");
			else if (canvas[i][j] == 3)printf("|");
		}
		printf("\n");
	}
	printf("score:%d", score);
}

void updateWithoutInput()
{
	int i, j, k;
	for (i = 0; i < high; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (canvas[i][j] == 3)
			{
				for (k = 0; k < enemyNum; k++)
				{
					if ((i == enemy_x[k]) && (j == enemy_y[k]))  // 子弹击中敌机
					{
						score++;                // 分数加1
						if (score % 5 == 0 && enemyMoveSpeed > 3)   // 达到一定积分后，敌机变快
							enemyMoveSpeed--;
						canvas[enemy_x[k]][enemy_y[k]] = 0;
						enemy_x[k] = rand() % 2;           // 产生新的飞机
						enemy_y[k] = rand() % width;
						canvas[enemy_x[k]][enemy_y[k]] = 2;
						canvas[i][j] = 0;      // 子弹消失
					}
				}
				// 子弹向上移动
				canvas[i][j] = 0;
				if (i > 0)
					canvas[i - 1][j] = 3;
			}
		}
	}
	
	static int speed = 0;
	if (speed < enemyMoveSpeed)
		speed++;

	for (k = 0; k < enemyNum; k++)
	{
		if ((position_x == enemy_x[k] && position_y == enemy_y[k]) || score < 0)
		{
			dead = true;
		}
		if (enemy_x[k] >= high)
		{
			canvas[enemy_x[k]][enemy_y[k]] = 0;
			enemy_x[k] = rand() % 2;           // 产生新的飞机
			enemy_y[k] = rand() % width;
			canvas[enemy_x[k]][enemy_y[k]] = 2;
			score--;
		}
		if (speed == enemyMoveSpeed)
		{
			// 敌机下落
			for (k = 0; k<enemyNum; k++)
			{
				canvas[enemy_x[k]][enemy_y[k]] = 0;
				enemy_x[k]++;
				speed = 0;
				canvas[enemy_x[k]][enemy_y[k]] = 2;
			}
		}
	}
}
void UpdateWithInput()
{
	char input;
	if (kbhit())
	{
		input = getch();
		if (input == 'a')
		{
			canvas[position_x][position_y] = 0;
			if (position_y - 1 <= 0)position_y = 0;
			else position_y--;
			canvas[position_x][position_y] = 1;
		}
		if (input == 'd')
		{
			canvas[position_x][position_y] = 0;
			if (position_y + 1 >= width)position_y = width - 1;
			else position_y++;
			canvas[position_x][position_y] = 1;
		}
		if (input == 'w')
		{
			canvas[position_x][position_y] = 0;
			if (position_x - 1 <= 0)position_x = 0;
			else position_x--;
			canvas[position_x][position_y] = 1;
		}
		if (input == 's')
		{
			canvas[position_x][position_y] = 0;
			if (position_x + 1 >= high)position_x = high - 1;
			else position_x++;
			canvas[position_x][position_y] = 1;
		}
		if (input == ' ')
		{
			
			canvas[position_x - 1][position_y] = 3;
		}
	}
}

int main()
{
	welcom();
	system("cls");
	startup();
	while (!dead)
	{
		show();
		updateWithoutInput();
		UpdateWithInput();
	}
	printf("you are dead!");
}