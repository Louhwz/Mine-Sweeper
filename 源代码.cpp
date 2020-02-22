#include<graphics.h>
#include<iostream>
#include<windows.h>
#include<ctime>
#include<string>
#include<cstdlib>
#include<cmath>
#include<fstream>
using namespace std;
int win=0;//胜负事件变量
TCHAR s[10] = { '0','1','2','3','4','5','6','7','8','9' };//对应图形界面格式中的数字
int direction[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
//八个方向，便于在搜索中使用循环
//依次代表左上、左、左下、上、下、右上、右下和右。
struct mm//鼠标坐标结构体，用于“<”的运算符重载
{
	int xx;
	int yy;
}mmm;
struct nn//按键区域的坐标结构体，用于“<”的运算符重载
{
	int x1, x2, y1, y2;
}nnn;
bool operator <(mm m0, nn n0)//重载运算符“<”，表示某一点是否在一区域内，是返回1，否返回0
{
	if ((m0.xx > n0.x1 && m0.xx < n0.x2)
		&& (m0.yy > n0.y1 && m0.yy < n0.y2))
		return 1;
	else return 0;
}
class counttime//时间记录与处理类
{
public:
	void startcounttime()//开始点击时计时
	{
		starttime = clock();
	}
	int showtime()//返回从开始计时到当前时间的秒数
	{
		return (clock() - starttime) / 1000;
	}
private:
	int starttime;
}tt;
class file//文件处理操作类
{
public:
	file(int line0,int t)
	{
		line = line0;
		ttt = t;
	}
	int check()//纪录是否打破检查函数
	{
		ifstream infile("纪录.txt", ios::in);
		if (!infile)//异常处理
		{
			cerr << "文件读取异常";
			exit(2);//一般而言文件读取错误直接返回2
		}
		for (int i = 1; i <= line; i++)infile >> tttt;
		infile.close();
		if (ttt < tttt)return 1; else return 0;
	}
	void change()//纪录更改函数
	{
		fstream filename("纪录.txt", ios::in | ios::out);
		filename.seekp((line-1) * 8);
		filename << ttt;
		if (ttt < 10)for (int i = 1; i <= 5; i++)filename << " ";
		else if (ttt < 100)for (int i = 1; i <= 4; i++)filename << " ";
		else if (ttt < 1000)for (int i = 1; i <= 3; i++)filename << " ";
		filename << endl;
	}
private:
	int line;//这个变量的意义在于确认纪录在第几行
	int ttt,tttt;
};
class click//鼠标点击事件类
{
public:
	click();
	int checkclick(int,int);//检查鼠标截至目前点击状态
	void countclick(int,int);//计数鼠标点击次数
private:
	int saveclick[32][32];//储存点击事件的信息
}c;
click::click()
{
	for(int i=0;i<=32;i++)
		for (int j = 0; j <= 32; j++)
		{
			saveclick[i][j] = 0;
		}
}
int click::checkclick(int x,int y)
{
	return(saveclick[x][y]);
}
void click::countclick(int x,int y)
{
	saveclick[x][y]++;
	if (saveclick[x][y] == 3) saveclick[x][y] = 0;
	//所谓“清除”，即将右击的次数清零，这样标记为未知区域
}
class minesweeper//这是最重要的类，关键的逻辑判断都在其中
{
public:
	minesweeper()//+1重载，新兵难度，地图9*9，地雷数目10
	{
		maxx = 9; maxy = 9; mine = 10;
		safespace = 0;
		maxsafespace = maxx*maxy - mine;
		width = 20 * maxx, hight = 40;
		for (int i = 0; i <= maxx - 1; i++)
			for (int j = 0; j <= maxy - 1; j++)
			{
				minespace[i][j] = 0;
				map[i][j] = 0;
				clearspace[i][j] = 0;
			}
	}
	minesweeper(int m1)//+2重载，普通难度，地图16*16，地雷数目40
	{
		maxx = 16; maxy = 16; mine = 40;
		safespace = 0;
		width = 20 * maxx, hight = 40;
		maxsafespace = maxx*maxy - mine;
		for (int i = 0; i <= maxx - 1; i++)
			for (int j = 0; j <= maxy - 1; j++)
			{
				minespace[i][j] = 0;
				map[i][j] = 0;
				clearspace[i][j] = 0;
			}
	}
	minesweeper(int m1, int n1)//+3重载，老兵难度，地图30*16，地雷数目99
	{
		maxx = 30; maxy = 16; mine = 99;
		safespace = 0;
		width = 20 * maxx, hight = 40;
		maxsafespace = maxx*maxy - mine;
		for (int i = 0; i <= maxx - 1; i++)
			for (int j = 0; j <= maxy - 1; j++)
			{
				minespace[i][j] = 0;
				map[i][j] = 0;
				clearspace[i][j] = 0;
			}
	}
	void drawplayspace();//这个函数专门画游戏地图
	void setmine();//包含第一次点击随机生成地雷和显示的功能
	void getmap();//根据setmine()函数生成的结果来得到雷区计数图
	void bfs(int x1,int y1);//广度优先搜索
	void play();//收集鼠标信息，给予相关显示
private:
	int maxx, maxy, mine;
	int safespace, maxsafespace;
	int width, hight;//这个是用来调整显示图片大小的，毕竟地图不一样大
	int minespace[32][32], clearspace[32][32], map[32][32];
};
void minesweeper::drawplayspace()//画地图
{
	initgraph(maxx * 20, maxy * 20 + 80);
	IMAGE img1;
	loadimage(&img1, _T("未知区域.jpg"));
	for (int i = 0; i <= maxx - 1; i++)
		for (int j = 0; j <= maxy - 1; j++)
			putimage(i * 20, j * 20 + 40, &img1);
	IMAGE img2;
	loadimage(&img2, _T("天空.jpg"),width,hight);
	putimage(0, 0, &img2);
	loadimage(&img2, _T("大海.jpg"), width, hight);
	putimage(0, maxy*20+40, &img2);
	loadimage(&img1, _T("安全区域.jpg"));
	putimage(0, 0, &img1),putimage(20, 0, &img1), putimage(40,0, &img1);
	putimage(maxx * 20 - 20, 0, &img1), putimage(maxx * 20 - 40, 0, &img1);
	loadimage(&img1, _T("标记区域.jpg"));
	putimage(maxx * 20 - 60, 0, &img1);
	setcolor(RGB(38, 188, 213));
	setbkmode(TRANSPARENT);
	outtextxy(4, 2, s[0]), outtextxy(24, 2, s[0]), outtextxy(44, 2, s[0]);
	outtextxy(maxx * 20 - 36, 2, s[mine / 10]);
	outtextxy(maxx * 20 - 16, 2, s[mine % 10]);
	outtextxy(maxx*20-6*16,maxy*20+60,L"退出游戏");
	setmine();
}
void minesweeper::setmine()//根据玩家第一次点击的位置随机生成地雷
{
	MOUSEMSG m;
	int safex, safey,tot=0;
	while (true)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if(m.y>40 && m.y<40+maxy*20)
			{ 
			    safex = m.x / 20; safey = (m.y - 40) / 20;
			    srand((unsigned)time(NULL));
				
				while (tot < mine)
				{
					int p = rand() % (maxx*maxy);
					int xx = p%maxx;
					int yy = p/maxx;
					if ((abs(xx - safex) > 1 || abs(yy - safey) > 1) && minespace[xx][yy] == 0)
					{
						minespace[xx][yy] = 1;
						tot++;
					}
				}
				getmap();
				bfs(safex, safey);
				if (safespace == maxsafespace)
				{
					win = 1;
					Sleep(100);
					IMAGE img5;
					loadimage(&img5, _T("游戏胜利.jpg"), width, maxy * 20 / 3);
					putimage(0, (maxy * 20 + 80) / 3, &img5);
					Sleep(1000);
					break;
				}
				tt.startcounttime();
				play();
				break;
			}
			mmm.xx = m.x, mmm.yy = m.y;
			nnn.x1 = maxx * 20 - 6 * 16, nnn.y2 = maxx * 20 - 2 * 16;
			nnn.y1 = 60 + maxy * 20; nnn.y2 = 76 + maxy * 20;
			if (mmm<nnn)
			{
				closegraph();
				break;
			}
		}
	}
}
void minesweeper::getmap()//根据setmine()生成的地雷分布计算雷区分布计数图
{
	for(int i=0;i<=maxx-1;i++)
		for (int j = 0; j <= maxy - 1; j++)
		{
			for (int k = 0; k <= 7; k++)
			{
				int xxx = i + direction[k][0];
				int yyy = j + direction[k][1];
				if (xxx >= 0 && xxx <= maxx - 1 && yyy >= 0 && yyy <= maxy - 1)
				{
					map[i][j] = map[i][j] + minespace[xxx][yyy];
				}
			}
		}
}
void minesweeper::bfs(int m, int n)//广度优先搜索算法，宽度优先遍历搜索树，使用队列
{
	struct queue { int xx; int yy; };//定义队列，有两个属性，分别记录横纵坐标
	struct queue* q = (struct queue *)malloc(2 * sizeof(int) * maxx*maxy);
	//动态分配内存，保证队列够长的同时不占用过多空间。
	int front = 0; int rear = 1;
	q[rear].xx = m; q[rear].yy = n;
	map[m][n] = -1;
	while (front<rear)
	{
		front++;//队首指针后移一位，上一元素出队
		for (int i = 0; i <= 7; i++)
		{
			int x1 = q[front].xx + direction[i][0];
			int y1 = q[front].yy + direction[i][1];
			if ((map[x1][y1] == 0) && (x1 >= 0) && (x1 <= maxx) && (y1 >= 0) && (y1 <= maxy))
			{
				rear++;//队尾指针后移一位，准备入队
				q[rear].xx = x1; q[rear].yy = y1;
				map[x1][y1] = -1;//已经搜索到的区域，标记为安全，避免再次入队
			}
		}
	}
	for (int i = 0; i <= maxx-1; i++)
		for (int j = 0; j <= maxy-1; j++)
		{
			if (map[i][j] == -1 && clearspace[i][j] == 0)
			{
				IMAGE img6;
				loadimage(&img6, _T("安全区域.jpg"));
				putimage(i * 20, j * 20 + 40, &img6);
				safespace++;
				clearspace[i][j] = 1;//clearspace是确认该区域是否已经被翻开
			}
			else
			{
				int flag = 0;
				for (int k = 0; k <= 7; k++)
					if ((i + direction[k][0] >= 0) && (i + direction[k][0] <= maxx) &&
						(j + direction[k][1] >= 0) && (j + direction[k][1] <= maxy) &&
						(map[i + direction[k][0]][j + direction[k][1]] == -1))
					{
						flag = 1; break;
					};
				if (flag && clearspace[i][j] == 0)
				{
					IMAGE img6;
					loadimage(&img6, _T("安全区域.jpg"));
					putimage(i * 20, j * 20 + 40, &img6);
					clearspace[i][j] = 1;
					setbkmode(TRANSPARENT);
					settextcolor(BLUE);
					outtextxy(i * 20 + 4, j * 20 + 42, s[map[i][j]]);
					safespace++;
				}
			}
		}
}
void minesweeper::play()//收集鼠标点击信息并给予相关显示
{
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		IMAGE img1;
		int t = tt.showtime();
		if (t <= 999)
		{
			loadimage(&img1, _T("安全区域.jpg"));
			putimage(0, 0, &img1), putimage(20, 0, &img1), putimage(40, 0, &img1);
			setcolor(RGB(38, 188, 213));
			setbkmode(TRANSPARENT);
			outtextxy(4, 2, s[t / 100]);
			outtextxy(24, 2, s[t % 100 / 10]);
			outtextxy(44, 2, s[t % 10]);
		}
		else
		{
			closegraph();
			break;
		}
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			int x, y;
			x = m.x / 20; y = (m.y - 40) / 20;
			if (m.y > 40 && m.y < maxy * 20 + 40 && c.checkclick(x,y)==0)
				//如果点到了一个没有被标记的区域上
			{
				if (clearspace[x][y] == 0)//确认没有被翻开过
				{
					if (map[x][y] == 0)//如果是绝对安全区域
					{
						bfs(x, y);
						if (safespace == maxsafespace)
						{
							win = 1;
							Sleep(100);
							IMAGE img5;
							loadimage(&img5, _T("游戏胜利.jpg"), width, maxy * 20 / 3);
							putimage(0, (maxy * 20 + 80) / 3, &img5);
							Sleep(1000);
							break;
						}
					}
					if (map[x][y] > 0 && minespace[x][y] == 0)
						//如果本身安全但是周围有雷
					{
						IMAGE img3;
						loadimage(&img3, _T("安全区域.jpg"));
						safespace++;
						clearspace[x][y] = 1;
						putimage(x * 20, y * 20 + 40, &img3);
						setbkmode(TRANSPARENT);
						settextcolor(BLUE);
						outtextxy(x * 20 + 4, y * 20 + 42, s[map[x][y]]);
						if (safespace == maxsafespace)
						{
							win = 1;
							Sleep(100);
							IMAGE img5;
							loadimage(&img5, _T("游戏胜利.jpg"), width, maxy * 20 / 3);
							putimage(0, (maxy * 20 + 80) / 3, &img5);
							Sleep(1000);
							break;
						}
					}
					if (minespace[x][y] == 1)//如果不幸触雷
					{
						IMAGE img3;
						loadimage(&img3, _T("雷区.jpg"));
						for(int i=0;i<=maxx-1;i++)
							for (int j = 0; j <= maxy - 1; j++)
								if(minespace[i][j]==1) putimage(i * 20, j * 20 + 40, &img3);
						Sleep(1500);
						IMAGE img4;
						loadimage(&img4, _T("游戏失败.jpg"),width,maxy*20/3);
						win = -1;
						putimage(0, (maxy * 20 + 80) / 3, &img4);
						Sleep(1000);
						closegraph();
						break;
					}
				}
			}
			mmm.xx = m.x, mmm.yy = m.y;
			nnn.x1 = maxx * 20 - 6 * 16, nnn.y2 = maxx * 20 - 2 * 16;
			nnn.y1 = 60 + maxy * 20; nnn.y2 = 76 + maxy * 20;
			if (mmm<nnn)
			{
				closegraph();
				break;
			}
		}
		if (m.uMsg == WM_RBUTTONDOWN)//右击事件处理
		{
			int x, y;
			x = m.x / 20; y = (m.y - 40) / 20;
			if (clearspace[x][y] == 0 && m.y>40 && m.y<maxy*20+40)
			{
				if (c.checkclick(x, y) == 0)//如果没有右击过，标记之
				{
					IMAGE img7;
					loadimage(&img7, _T("标记区域.jpg"));
					putimage(x * 20, y * 20 + 40, &img7);
					c.countclick(x, y);
					mine--;
					IMAGE img1;
					loadimage(&img1, _T("安全区域.jpg"));
					putimage(maxx * 20 - 20, 0, &img1), putimage(maxx * 20 - 40, 0, &img1);
					setcolor(RGB(38, 188, 213));
					setbkmode(TRANSPARENT);
					outtextxy(maxx * 20 - 36, 2, s[mine / 10]);
					outtextxy(maxx * 20 - 16, 2, s[mine % 10]);
				}
				else if (c.checkclick(x, y) == 1)//如果已经右击过一次，改为疑问区域
				{
					IMAGE img7;
					loadimage(&img7, _T("疑问区域.jpg"));
					putimage(x * 20, y * 20 + 40, &img7);
					c.countclick(x, y);
				}
				else if (c.checkclick(x, y) == 2)//如果已经右击两次了，取消标记
				{
					IMAGE img7;
					loadimage(&img7, _T("未知区域.jpg"));
					putimage(x * 20, y * 20 + 40, &img7);
					c.countclick(x, y);
					mine++;
					IMAGE img1;
					loadimage(&img1, _T("安全区域.jpg"));
					putimage(maxx * 20 - 20, 0, &img1), putimage(maxx * 20 - 40, 0, &img1);
					setcolor(RGB(38, 188, 213));
					setbkmode(TRANSPARENT);
					outtextxy(maxx * 20 - 36, 2, s[mine / 10]);
					outtextxy(maxx * 20 - 16, 2, s[mine % 10]);
				}
			}
		}
	}
}
void drawgraph()//画开始界面的函数
{
	initgraph(400, 440);
	IMAGE img0;
	loadimage(&img0, _T("开始界面.jpg"));
	putimage(0, 0, &img0);
	int x0 = 160, y0 = 220;
	setcolor(RGB(38,188,213));
	setbkmode(TRANSPARENT);
	outtextxy(x0, y0, L"1.新兵");
	line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
	y0 = y0 + 18;
	outtextxy(x0, y0, L"2.普通");
	line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
	y0 = y0 + 18;
	outtextxy(x0, y0, L"3.老兵");
	line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
	y0 = y0 + 54;
	outtextxy(x0, y0, L"查看记录");
	line(x0, y0 + 16, x0 + 4 * 16, y0 + 16);
}
void getinfo()
{
	MOUSEMSG m;
	int x0, y0;
	while (true)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			mmm.xx = m.x;
			mmm.yy = m.y;
			setbkmode(TRANSPARENT);
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果左击“1.新兵”
			{
				minesweeper mineplay;
				mineplay.drawplayspace();
				if (win == 1)//如果获胜
				{
					int wintime = tt.showtime() - 1;
					file f(1,wintime);
					if (f.check())//如果确认是破了纪录，修改并显示
					{
						f.change();
						IMAGE img8;
						int mx = 9, my = 9;
						loadimage(&img8, _T("破纪录.jpg"), mx * 20, my * 20 / 3);
						putimage(0, (my * 20 + 80) / 3, &img8);
						IMAGE img9;
						loadimage(&img9, _T("背景.jpg"), mx * 20, 40);
						putimage(0, (2*my * 20 + 80) / 3, &img9);
						setcolor(RGB(38, 188, 213));
						outtextxy(0, (2*my * 20 + 80) / 3, L"您的用时为：");
						outtextxy(6*16, (2*my * 20 + 80) / 3, s[wintime/100]);
						outtextxy(6 * 16+8, (2*my * 20 + 80) / 3, s[wintime / 10%10]);
						outtextxy(6 * 16+16, (2*my * 20 + 80) / 3, s[wintime%10]);
						outtextxy(0, (2*my * 20 + 80) / 3+18, L"任意位置左击结束游戏");
						while (true)//左击任意位置退出
						{
							m= GetMouseMsg();
							if (m.uMsg == WM_LBUTTONDOWN)
							{
								break;
							}
						}
					}
					closegraph();
				}
				break;
			}
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220 + 18, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果左击“2.普通”
			{
				minesweeper mineplay(16);
				mineplay.drawplayspace();
				if (win == 1)//确认获胜
				{
					int wintime = tt.showtime() - 1;
					file f(2, wintime);
					if (f.check())//如果确认是破了纪录，修改并显示
					{
						f.change();
						IMAGE img8;
						int mx = 16, my = 16;
						loadimage(&img8, _T("破纪录.jpg"), mx * 20, my * 20 / 3);
						putimage(0, (my * 20 + 80) / 3, &img8);
						IMAGE img9;
						loadimage(&img9, _T("背景.jpg"), mx * 20, 40);
						putimage(0, (2 * my * 20 + 80) / 3, &img9);
						setcolor(RGB(38, 188, 213));
						outtextxy(0, (2 * my * 20 + 80) / 3, L"您的用时为：");
						outtextxy(6 * 16, (2 * my * 20 + 80) / 3, s[wintime / 100]);
						outtextxy(6 * 16 + 8, (2 * my * 20 + 80) / 3, s[wintime / 10 % 10]);
						outtextxy(6 * 16 + 16, (2 * my * 20 + 80) / 3, s[wintime % 10]);
						outtextxy(0, (2 * my * 20 + 80) / 3 + 18, L"任意位置左击结束游戏");
						while (true)//左击任意位置退出
						{
							m = GetMouseMsg();
							if (m.uMsg == WM_LBUTTONDOWN)
							{
								break;
							}
						}
					}
					closegraph();
				}
				break;
			}
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220 + 2 * 18, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果左击“3.老兵”
			{
				minesweeper mineplay(30,16);
				mineplay.drawplayspace();
				if (win == 1)//确认获胜
				{
					int wintime = tt.showtime() - 1;
					file f(3, wintime);
					if (f.check())//如果确认是破了纪录，修改并显示
					{
						f.change();
						IMAGE img8;
						int mx = 30, my = 16;
						loadimage(&img8, _T("破纪录.jpg"), mx * 20, my * 20 / 3);
						putimage(0, (my * 20 + 80) / 3, &img8);
						IMAGE img9;
						loadimage(&img9, _T("背景.jpg"), mx * 20, 40);
						putimage(0, (2 * my * 20 + 80) / 3, &img9);
						setcolor(RGB(38, 188, 213));
						outtextxy(0, (2 * my * 20 + 80) / 3, L"您的用时为：");
						outtextxy(6 * 16, (2 * my * 20 + 80) / 3, s[wintime / 100]);
						outtextxy(6 * 16 + 8, (2 * my * 20 + 80) / 3, s[wintime / 10 % 10]);
						outtextxy(6 * 16 + 16, (2 * my * 20 + 80) / 3, s[wintime % 10]);
						outtextxy(0, (2 * my * 20 + 80) / 3 + 18, L"任意位置左击结束游戏");
						while (true)//左击任意位置退出
						{
							m = GetMouseMsg();
							if (m.uMsg == WM_LBUTTONDOWN)
							{
								break;
							}
						}
					}
					closegraph();
				}
				break;
			}
			nnn.x1 = 160, nnn.x2 = 160 + 4 * 36, nnn.y1 = 220 + 90, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果左击“查看记录”
			{
				int t;
				ifstream infile("纪录.txt", ios::in);
				if (!infile)
				{
					cerr << "读取文件时错误!1秒后自动关闭";
					Sleep(1000);
					exit(2);
				}
				setcolor(RGB(38, 188, 213));
				outtextxy(160, 328, L"新兵：");
				outtextxy(160, 346, L"普通：");
				outtextxy(160, 364, L"老兵：");
				for (int i = 1; i <= 3; i++)//在相应位置显示
				{
					infile >> t;
					int aa = t / 100, bb = t / 10 % 10, cc = t % 10;
					outtextxy(208, 310 + i * 18, s[aa]), outtextxy(208 + 8, 310 + i * 18, s[bb]), outtextxy(208 + 16, 310 + i * 18, s[cc]);
					outtextxy(208 + 24, 310 + i * 18, L'秒');
				}
				infile.close();
			}
		}
		else
		{
			mmm.xx = m.x;
			mmm.yy = m.y;
			setbkmode(TRANSPARENT);
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果鼠标移动到“1.新兵”上
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(244, 208, 0));
				outtextxy(x0, y0, L"1.新兵");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"1.新兵");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220 + 18, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果鼠标移动到“2.普通”上
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(244, 208, 0));
				outtextxy(x0, y0, L"2.普通");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"2.普通");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220 + 2 * 18, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果鼠标移动到“3.老兵”上
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(244, 208, 0));
				outtextxy(x0, y0, L"3.老兵");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"3.老兵");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			nnn.x1 = 160, nnn.x2 = 160 + 4 * 36, nnn.y1 = 220 + 90, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//如果鼠标移动到“查看记录”上
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(213, 26, 33));
				outtextxy(x0, y0, L"查看记录");
				line(x0, y0 + 16, x0 + 4 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"查看记录");
				line(x0, y0 + 16, x0 + 4 * 16, y0 + 16);
			}
		}
	}
	closegraph();
}
int main()
{
	drawgraph();
	getinfo();
	return 0;
}