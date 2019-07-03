#include <stdio.h>
#include <easyx.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#define wide 20
#define height 20
#define rom 0.1

int mine[wide][height], mark[wide][height];
int list[2][1000];
int lens,pos;

int main()
{
	list[0][0] = 9999;
	int calc(int px, int py);
	void zero_expand(int px, int py);
	void draw(int px, int py);
	void draw_cursor(int px, int py);

	srand(time(NULL));
	int num=rom*height*wide,left,px,py,mu;
	left=num;px=wide/2;py=height/2;
	for(int i=0;i<wide;i++)
	{
		for(int j=0;j<height;j++)
		{
			mine[i][j] = 0; mark[i][j] = 0;
		}
	}
	while(num!=0)
	{
		int x=rand()%wide,y=rand()%height;
		if(mine[x][y]==0)
		{
			mine[x][y]=-1;
			num--;
		}
	}
	initgraph(wide*20,height*20,SHOWCONSOLE);
	setbkcolor(WHITE);
	setlinecolor(WHITE);
	setfillcolor(BLACK);
	setfillstyle(BS_SOLID);
	for(int m=0;m<wide;m++)
	{
		for(int n=0;n<height;n++)
		{
			draw(m, n);
		}
	}
	draw_cursor(px, py);
	num = left; mu = 0;
	while(left>0)
	{
		if (_kbhit() != 0)
		{
			draw(px, py);
			char c = _getch();
			if (c == 's'&&py<height-1) py++;
			else if (c == 'w' && py > 0) py--;
			else if (c == 'a'&&px>0) px--;
			else if (c == 'd'&&px<wide-1) px++;
			printf("input %c,px %d,py %d,mark %d,mine %d\n", c, px, py, mark[px][py], mine[px][py]);
			if (c == 'm')
			{
				if (mu < num && mark[px][py] == 0)						//标记数目小于雷的数量，且当前块未被标记
				{
					mu++; mark[px][py] = 1;							//标记数目加一，当前块设为已标记
					left -= mine[px][py] == -1 ? 1 : 0;				//若当前方块有雷，剩余雷数减一，否则不变
				}
				else if (mu < num && mark[px][py] == 1)				//标记数目小于雷的数量，且当前块已被标记
				{
					mu--; mark[px][py] = 0;							//标记数目减一，当前块设为已标记
					left += mine[px][py] == -1 ? 1 : 0;				//若当前方块有雷，剩余雷数加一，否则不变
				}
				printf("mark[%d][%d]=%d\n", px, py, mark[px][py]);
			}
			if (c == 'h' && mine[px][py] != -1)
			{
				mark[px][py] = -1 * calc(px, py)-1;					//-1用来将已被点开但数为0 的与未点开的区分开
				if (mark[px][py] == -1)
					zero_expand(px, py);
			}
			else if (c == 'h' && mine[px][py] == -1)
			{
				return -1;
			}
			draw(px, py);
			draw_cursor(px, py);
		}
	}
	return 0;
}

void list_append(int px,int py)
{
	/*
	int flag = 0;
	for (int i = 0; i <= lens; i++)
	{
		if (list[0][i] == px && list[1][i] == py)
		{
			flag = 1;
			break;
		}
	}
	*/
	list[0][++lens] = px; list[1][lens] = py;
	
}

void zero_expand(int px, int py)
{
	int calc(int px, int py);
	void draw(int px, int py);
	int x, y;
	lens = -1; pos = 0;
	list_append(px, py);
	while (pos <= lens)
	{
		x = list[0][pos]; y = list[1][pos];
		int pis = (x == 0 ? 0 : -1),pie=(x==wide-1?0:1),pjs=(y==0?0:-1),pje=(y==height-1?0:1);
		for (int i = pis; i <= pie; i++)
		{
			for (int j =pjs; j <=pje; j++)
			{
				//printf("px=%d, py=%d, i=%d, j=%d, lens=%d, pos=%d\n", x, y, i, j, lens, pos);
				int p = mark[x + i][y + j];
				mark[x + i][y + j] = -1 * calc(x + i, y + j) - 1;
				draw(x + i, y + j);
				if (mark[x+i][y+j] == -1&&p==0)
				{
					list[0][++lens] = x + i; list[1][lens] = y + j;
				}
			}
		}
		pos++;
	}
}

void draw_cursor(int px, int py)
{
	setlinecolor(mark[px][py]>=0?WHITE:BLACK);
	line(20 * px + 2, 20 * py + 2, 20 * px + 2, 20 * py + 18);
	line(20 * px + 18, 20 * py + 2, 20 * px + 18, 20 * py + 18);
	line(20 * px + 2, 20 * py + 2, 20 * px + 18, 20 * py + 2);
	line(20 * px + 2, 20 * py + 18, 20 * px + 18, 20 * py + 18);
	setlinecolor(WHITE);
}

void draw(int px,int py)
{
	switch (mark[px][py])
		{
			case 0:
				fillrectangle(20 * px, 20 * py, 20 + 20 * px, 20 + 20 * py); 
				break;
			case 1:
				printf("mark=1\t,rectangle(%d,%d,%d,%d)\n",20*px,20*py,20*px+20,20*py+20);
				setfillcolor(RED);
				fillrectangle(20 * px, 20 * py, 20 + 20 * px, 20 + 20 * py); 
				setfillcolor(BLACK); 
				break;
			case -1:
				setfillcolor(WHITE);
				fillrectangle(20 * px, 20 * py, 20 + 20 * px, 20 + 20 * py);
				setfillcolor(BLACK);
				break;
			default:
				RECT r = { 20 * px,20 * py,20 + 20 * px,20 + 20 * py };
				setfillcolor(WHITE); settextcolor(BLACK);
				fillrectangle(20 * px, 20 * py, 20 + 20 * px, 20 + 20*py);
				char str[2] = { 0,'\0' }; str[0] = -1 * mark[px][py]+47;
				printf("char:%s\n", str);
				drawtext(_T(str), &r,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				setfillcolor(BLACK);
				break;
		}
	//Sleep(500);
}



int calc(int px, int py)
{
	int sum = 0;
	if (px == 0)
		sum = (mine[px][py - 1] <= -1) + (mine[px][py + 1] <= -1) + (mine[px + 1][py - 1] <= -1) + (mine[px + 1][py] <= -1) + (mine[px + 1][py + 1] <= -1);
	else if (px == wide-1)
		sum = (mine[px - 1][py - 1] <= -1) + (mine[px - 1][py] <= -1) + (mine[px - 1][py + 1] <= -1) + (mine[px][py - 1] <= -1) + (mine[px][py + 1] <= -1);
	else if (py == 0)
		sum = (mine[px - 1][py] <= -1) + (mine[px - 1][py + 1] <= -1) + (mine[px][py + 1] <= -1) + (mine[px + 1][py] <= -1) + (mine[px + 1][py + 1] <= -1);
	else if (py == height-1)
		sum = (mine[px - 1][py - 1] <= -1) + (mine[px - 1][py] <= -1) + (mine[px][py - 1] <= -1) + (mine[px + 1][py - 1] <= -1) + (mine[px + 1][py] <= -1);
	else
		sum =(mine[px - 1][py - 1] <= -1) + (mine[px - 1][py] <= -1) + (mine[px - 1][py + 1] <= -1) + (mine[px][py - 1] <= -1) + (mine[px][py + 1] <= -1) + (mine[px + 1][py - 1] <= -1) + (mine[px + 1][py] <= -1) + (mine[px + 1][py + 1] <= -1);
	return sum;
}