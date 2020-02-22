#include<graphics.h>
#include<iostream>
#include<windows.h>
#include<ctime>
#include<string>
#include<cstdlib>
#include<cmath>
#include<fstream>
using namespace std;
int win=0;//ʤ���¼�����
TCHAR s[10] = { '0','1','2','3','4','5','6','7','8','9' };//��Ӧͼ�ν����ʽ�е�����
int direction[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
//�˸����򣬱�����������ʹ��ѭ��
//���δ������ϡ������¡��ϡ��¡����ϡ����º��ҡ�
struct mm//�������ṹ�壬���ڡ�<�������������
{
	int xx;
	int yy;
}mmm;
struct nn//�������������ṹ�壬���ڡ�<�������������
{
	int x1, x2, y1, y2;
}nnn;
bool operator <(mm m0, nn n0)//�����������<������ʾĳһ���Ƿ���һ�����ڣ��Ƿ���1���񷵻�0
{
	if ((m0.xx > n0.x1 && m0.xx < n0.x2)
		&& (m0.yy > n0.y1 && m0.yy < n0.y2))
		return 1;
	else return 0;
}
class counttime//ʱ���¼�봦����
{
public:
	void startcounttime()//��ʼ���ʱ��ʱ
	{
		starttime = clock();
	}
	int showtime()//���شӿ�ʼ��ʱ����ǰʱ�������
	{
		return (clock() - starttime) / 1000;
	}
private:
	int starttime;
}tt;
class file//�ļ����������
{
public:
	file(int line0,int t)
	{
		line = line0;
		ttt = t;
	}
	int check()//��¼�Ƿ���Ƽ�麯��
	{
		ifstream infile("��¼.txt", ios::in);
		if (!infile)//�쳣����
		{
			cerr << "�ļ���ȡ�쳣";
			exit(2);//һ������ļ���ȡ����ֱ�ӷ���2
		}
		for (int i = 1; i <= line; i++)infile >> tttt;
		infile.close();
		if (ttt < tttt)return 1; else return 0;
	}
	void change()//��¼���ĺ���
	{
		fstream filename("��¼.txt", ios::in | ios::out);
		filename.seekp((line-1) * 8);
		filename << ttt;
		if (ttt < 10)for (int i = 1; i <= 5; i++)filename << " ";
		else if (ttt < 100)for (int i = 1; i <= 4; i++)filename << " ";
		else if (ttt < 1000)for (int i = 1; i <= 3; i++)filename << " ";
		filename << endl;
	}
private:
	int line;//�����������������ȷ�ϼ�¼�ڵڼ���
	int ttt,tttt;
};
class click//������¼���
{
public:
	click();
	int checkclick(int,int);//���������Ŀǰ���״̬
	void countclick(int,int);//�������������
private:
	int saveclick[32][32];//�������¼�����Ϣ
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
	//��ν��������������һ��Ĵ������㣬�������Ϊδ֪����
}
class minesweeper//��������Ҫ���࣬�ؼ����߼��ж϶�������
{
public:
	minesweeper()//+1���أ��±��Ѷȣ���ͼ9*9��������Ŀ10
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
	minesweeper(int m1)//+2���أ���ͨ�Ѷȣ���ͼ16*16��������Ŀ40
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
	minesweeper(int m1, int n1)//+3���أ��ϱ��Ѷȣ���ͼ30*16��������Ŀ99
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
	void drawplayspace();//�������ר�Ż���Ϸ��ͼ
	void setmine();//������һ�ε��������ɵ��׺���ʾ�Ĺ���
	void getmap();//����setmine()�������ɵĽ�����õ���������ͼ
	void bfs(int x1,int y1);//�����������
	void play();//�ռ������Ϣ�����������ʾ
private:
	int maxx, maxy, mine;
	int safespace, maxsafespace;
	int width, hight;//���������������ʾͼƬ��С�ģ��Ͼ���ͼ��һ����
	int minespace[32][32], clearspace[32][32], map[32][32];
};
void minesweeper::drawplayspace()//����ͼ
{
	initgraph(maxx * 20, maxy * 20 + 80);
	IMAGE img1;
	loadimage(&img1, _T("δ֪����.jpg"));
	for (int i = 0; i <= maxx - 1; i++)
		for (int j = 0; j <= maxy - 1; j++)
			putimage(i * 20, j * 20 + 40, &img1);
	IMAGE img2;
	loadimage(&img2, _T("���.jpg"),width,hight);
	putimage(0, 0, &img2);
	loadimage(&img2, _T("��.jpg"), width, hight);
	putimage(0, maxy*20+40, &img2);
	loadimage(&img1, _T("��ȫ����.jpg"));
	putimage(0, 0, &img1),putimage(20, 0, &img1), putimage(40,0, &img1);
	putimage(maxx * 20 - 20, 0, &img1), putimage(maxx * 20 - 40, 0, &img1);
	loadimage(&img1, _T("�������.jpg"));
	putimage(maxx * 20 - 60, 0, &img1);
	setcolor(RGB(38, 188, 213));
	setbkmode(TRANSPARENT);
	outtextxy(4, 2, s[0]), outtextxy(24, 2, s[0]), outtextxy(44, 2, s[0]);
	outtextxy(maxx * 20 - 36, 2, s[mine / 10]);
	outtextxy(maxx * 20 - 16, 2, s[mine % 10]);
	outtextxy(maxx*20-6*16,maxy*20+60,L"�˳���Ϸ");
	setmine();
}
void minesweeper::setmine()//������ҵ�һ�ε����λ��������ɵ���
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
					loadimage(&img5, _T("��Ϸʤ��.jpg"), width, maxy * 20 / 3);
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
void minesweeper::getmap()//����setmine()���ɵĵ��׷ֲ����������ֲ�����ͼ
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
void minesweeper::bfs(int m, int n)//������������㷨��������ȱ�����������ʹ�ö���
{
	struct queue { int xx; int yy; };//������У����������ԣ��ֱ��¼��������
	struct queue* q = (struct queue *)malloc(2 * sizeof(int) * maxx*maxy);
	//��̬�����ڴ棬��֤���й�����ͬʱ��ռ�ù���ռ䡣
	int front = 0; int rear = 1;
	q[rear].xx = m; q[rear].yy = n;
	map[m][n] = -1;
	while (front<rear)
	{
		front++;//����ָ�����һλ����һԪ�س���
		for (int i = 0; i <= 7; i++)
		{
			int x1 = q[front].xx + direction[i][0];
			int y1 = q[front].yy + direction[i][1];
			if ((map[x1][y1] == 0) && (x1 >= 0) && (x1 <= maxx) && (y1 >= 0) && (y1 <= maxy))
			{
				rear++;//��βָ�����һλ��׼�����
				q[rear].xx = x1; q[rear].yy = y1;
				map[x1][y1] = -1;//�Ѿ������������򣬱��Ϊ��ȫ�������ٴ����
			}
		}
	}
	for (int i = 0; i <= maxx-1; i++)
		for (int j = 0; j <= maxy-1; j++)
		{
			if (map[i][j] == -1 && clearspace[i][j] == 0)
			{
				IMAGE img6;
				loadimage(&img6, _T("��ȫ����.jpg"));
				putimage(i * 20, j * 20 + 40, &img6);
				safespace++;
				clearspace[i][j] = 1;//clearspace��ȷ�ϸ������Ƿ��Ѿ�������
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
					loadimage(&img6, _T("��ȫ����.jpg"));
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
void minesweeper::play()//�ռ��������Ϣ�����������ʾ
{
	MOUSEMSG m;
	while (true)
	{
		m = GetMouseMsg();
		IMAGE img1;
		int t = tt.showtime();
		if (t <= 999)
		{
			loadimage(&img1, _T("��ȫ����.jpg"));
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
				//����㵽��һ��û�б���ǵ�������
			{
				if (clearspace[x][y] == 0)//ȷ��û�б�������
				{
					if (map[x][y] == 0)//����Ǿ��԰�ȫ����
					{
						bfs(x, y);
						if (safespace == maxsafespace)
						{
							win = 1;
							Sleep(100);
							IMAGE img5;
							loadimage(&img5, _T("��Ϸʤ��.jpg"), width, maxy * 20 / 3);
							putimage(0, (maxy * 20 + 80) / 3, &img5);
							Sleep(1000);
							break;
						}
					}
					if (map[x][y] > 0 && minespace[x][y] == 0)
						//�������ȫ������Χ����
					{
						IMAGE img3;
						loadimage(&img3, _T("��ȫ����.jpg"));
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
							loadimage(&img5, _T("��Ϸʤ��.jpg"), width, maxy * 20 / 3);
							putimage(0, (maxy * 20 + 80) / 3, &img5);
							Sleep(1000);
							break;
						}
					}
					if (minespace[x][y] == 1)//������Ҵ���
					{
						IMAGE img3;
						loadimage(&img3, _T("����.jpg"));
						for(int i=0;i<=maxx-1;i++)
							for (int j = 0; j <= maxy - 1; j++)
								if(minespace[i][j]==1) putimage(i * 20, j * 20 + 40, &img3);
						Sleep(1500);
						IMAGE img4;
						loadimage(&img4, _T("��Ϸʧ��.jpg"),width,maxy*20/3);
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
		if (m.uMsg == WM_RBUTTONDOWN)//�һ��¼�����
		{
			int x, y;
			x = m.x / 20; y = (m.y - 40) / 20;
			if (clearspace[x][y] == 0 && m.y>40 && m.y<maxy*20+40)
			{
				if (c.checkclick(x, y) == 0)//���û���һ��������֮
				{
					IMAGE img7;
					loadimage(&img7, _T("�������.jpg"));
					putimage(x * 20, y * 20 + 40, &img7);
					c.countclick(x, y);
					mine--;
					IMAGE img1;
					loadimage(&img1, _T("��ȫ����.jpg"));
					putimage(maxx * 20 - 20, 0, &img1), putimage(maxx * 20 - 40, 0, &img1);
					setcolor(RGB(38, 188, 213));
					setbkmode(TRANSPARENT);
					outtextxy(maxx * 20 - 36, 2, s[mine / 10]);
					outtextxy(maxx * 20 - 16, 2, s[mine % 10]);
				}
				else if (c.checkclick(x, y) == 1)//����Ѿ��һ���һ�Σ���Ϊ��������
				{
					IMAGE img7;
					loadimage(&img7, _T("��������.jpg"));
					putimage(x * 20, y * 20 + 40, &img7);
					c.countclick(x, y);
				}
				else if (c.checkclick(x, y) == 2)//����Ѿ��һ������ˣ�ȡ�����
				{
					IMAGE img7;
					loadimage(&img7, _T("δ֪����.jpg"));
					putimage(x * 20, y * 20 + 40, &img7);
					c.countclick(x, y);
					mine++;
					IMAGE img1;
					loadimage(&img1, _T("��ȫ����.jpg"));
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
void drawgraph()//����ʼ����ĺ���
{
	initgraph(400, 440);
	IMAGE img0;
	loadimage(&img0, _T("��ʼ����.jpg"));
	putimage(0, 0, &img0);
	int x0 = 160, y0 = 220;
	setcolor(RGB(38,188,213));
	setbkmode(TRANSPARENT);
	outtextxy(x0, y0, L"1.�±�");
	line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
	y0 = y0 + 18;
	outtextxy(x0, y0, L"2.��ͨ");
	line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
	y0 = y0 + 18;
	outtextxy(x0, y0, L"3.�ϱ�");
	line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
	y0 = y0 + 54;
	outtextxy(x0, y0, L"�鿴��¼");
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
			if (mmm < nnn)//��������1.�±���
			{
				minesweeper mineplay;
				mineplay.drawplayspace();
				if (win == 1)//�����ʤ
				{
					int wintime = tt.showtime() - 1;
					file f(1,wintime);
					if (f.check())//���ȷ�������˼�¼���޸Ĳ���ʾ
					{
						f.change();
						IMAGE img8;
						int mx = 9, my = 9;
						loadimage(&img8, _T("�Ƽ�¼.jpg"), mx * 20, my * 20 / 3);
						putimage(0, (my * 20 + 80) / 3, &img8);
						IMAGE img9;
						loadimage(&img9, _T("����.jpg"), mx * 20, 40);
						putimage(0, (2*my * 20 + 80) / 3, &img9);
						setcolor(RGB(38, 188, 213));
						outtextxy(0, (2*my * 20 + 80) / 3, L"������ʱΪ��");
						outtextxy(6*16, (2*my * 20 + 80) / 3, s[wintime/100]);
						outtextxy(6 * 16+8, (2*my * 20 + 80) / 3, s[wintime / 10%10]);
						outtextxy(6 * 16+16, (2*my * 20 + 80) / 3, s[wintime%10]);
						outtextxy(0, (2*my * 20 + 80) / 3+18, L"����λ�����������Ϸ");
						while (true)//�������λ���˳�
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
			if (mmm < nnn)//��������2.��ͨ��
			{
				minesweeper mineplay(16);
				mineplay.drawplayspace();
				if (win == 1)//ȷ�ϻ�ʤ
				{
					int wintime = tt.showtime() - 1;
					file f(2, wintime);
					if (f.check())//���ȷ�������˼�¼���޸Ĳ���ʾ
					{
						f.change();
						IMAGE img8;
						int mx = 16, my = 16;
						loadimage(&img8, _T("�Ƽ�¼.jpg"), mx * 20, my * 20 / 3);
						putimage(0, (my * 20 + 80) / 3, &img8);
						IMAGE img9;
						loadimage(&img9, _T("����.jpg"), mx * 20, 40);
						putimage(0, (2 * my * 20 + 80) / 3, &img9);
						setcolor(RGB(38, 188, 213));
						outtextxy(0, (2 * my * 20 + 80) / 3, L"������ʱΪ��");
						outtextxy(6 * 16, (2 * my * 20 + 80) / 3, s[wintime / 100]);
						outtextxy(6 * 16 + 8, (2 * my * 20 + 80) / 3, s[wintime / 10 % 10]);
						outtextxy(6 * 16 + 16, (2 * my * 20 + 80) / 3, s[wintime % 10]);
						outtextxy(0, (2 * my * 20 + 80) / 3 + 18, L"����λ�����������Ϸ");
						while (true)//�������λ���˳�
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
			if (mmm < nnn)//��������3.�ϱ���
			{
				minesweeper mineplay(30,16);
				mineplay.drawplayspace();
				if (win == 1)//ȷ�ϻ�ʤ
				{
					int wintime = tt.showtime() - 1;
					file f(3, wintime);
					if (f.check())//���ȷ�������˼�¼���޸Ĳ���ʾ
					{
						f.change();
						IMAGE img8;
						int mx = 30, my = 16;
						loadimage(&img8, _T("�Ƽ�¼.jpg"), mx * 20, my * 20 / 3);
						putimage(0, (my * 20 + 80) / 3, &img8);
						IMAGE img9;
						loadimage(&img9, _T("����.jpg"), mx * 20, 40);
						putimage(0, (2 * my * 20 + 80) / 3, &img9);
						setcolor(RGB(38, 188, 213));
						outtextxy(0, (2 * my * 20 + 80) / 3, L"������ʱΪ��");
						outtextxy(6 * 16, (2 * my * 20 + 80) / 3, s[wintime / 100]);
						outtextxy(6 * 16 + 8, (2 * my * 20 + 80) / 3, s[wintime / 10 % 10]);
						outtextxy(6 * 16 + 16, (2 * my * 20 + 80) / 3, s[wintime % 10]);
						outtextxy(0, (2 * my * 20 + 80) / 3 + 18, L"����λ�����������Ϸ");
						while (true)//�������λ���˳�
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
			if (mmm < nnn)//���������鿴��¼��
			{
				int t;
				ifstream infile("��¼.txt", ios::in);
				if (!infile)
				{
					cerr << "��ȡ�ļ�ʱ����!1����Զ��ر�";
					Sleep(1000);
					exit(2);
				}
				setcolor(RGB(38, 188, 213));
				outtextxy(160, 328, L"�±���");
				outtextxy(160, 346, L"��ͨ��");
				outtextxy(160, 364, L"�ϱ���");
				for (int i = 1; i <= 3; i++)//����Ӧλ����ʾ
				{
					infile >> t;
					int aa = t / 100, bb = t / 10 % 10, cc = t % 10;
					outtextxy(208, 310 + i * 18, s[aa]), outtextxy(208 + 8, 310 + i * 18, s[bb]), outtextxy(208 + 16, 310 + i * 18, s[cc]);
					outtextxy(208 + 24, 310 + i * 18, L'��');
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
			if (mmm < nnn)//�������ƶ�����1.�±�����
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(244, 208, 0));
				outtextxy(x0, y0, L"1.�±�");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"1.�±�");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220 + 18, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//�������ƶ�����2.��ͨ����
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(244, 208, 0));
				outtextxy(x0, y0, L"2.��ͨ");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"2.��ͨ");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			nnn.x1 = 160, nnn.x2 = 160 + 3 * 36, nnn.y1 = 220 + 2 * 18, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//�������ƶ�����3.�ϱ�����
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(244, 208, 0));
				outtextxy(x0, y0, L"3.�ϱ�");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"3.�ϱ�");
				line(x0, y0 + 16, x0 + 3 * 16, y0 + 16);
			}
			nnn.x1 = 160, nnn.x2 = 160 + 4 * 36, nnn.y1 = 220 + 90, nnn.y2 = nnn.y1 + 16;
			if (mmm < nnn)//�������ƶ������鿴��¼����
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(213, 26, 33));
				outtextxy(x0, y0, L"�鿴��¼");
				line(x0, y0 + 16, x0 + 4 * 16, y0 + 16);
			}
			else
			{
				x0 = nnn.x1; y0 = nnn.y1;
				setcolor(RGB(38, 188, 213));
				outtextxy(x0, y0, L"�鿴��¼");
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