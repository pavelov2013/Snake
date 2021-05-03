#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include <conio.h>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

int height = 20;
int width = 20;
char ln = '#';
char icon = '+';
int speed = 5;
char food = '@';

int xf, yf;//food

char act = 'r';
vector<char> movies;
int counter_blocks = 0;
vector<char> actions;

void Load()
{
	string str;

	ifstream in("settings");
	if (in.is_open())
	{
		int i = 0;
		while (getline(in,str))
		{
			switch (i)
			{
			case 0:
			{
				if (str.find("height:") != string::npos)
				{
					int pos = str.find("height:");
					string str2;
					for (int z = pos + 7; z < str.length(); z++)
					{
						str2.push_back(str[z]);
					}
					height = stoi(str2);
				}
			}
			case 1:
			{
				if (str.find("width:") != string::npos)
				{
					int pos = str.find("width:");
					string str2;
					for (int z = pos + 6; z < str.length(); z++)
					{
						str2.push_back(str[z]);
					}
					width = stoi(str2);
				}
			}
			case 2:
			{
				if (str.find("ln:") != string::npos)
				{
					int pos = str.find("ln:");
					string str2;
					for (int z = pos + 3; z < str.length(); z++)
					{
						str2.push_back(str[z]);
					}
					ln = str2[0];
				}
			}
			case 3:
			{
				if (str.find("icon:") != string::npos)
				{

					int pos = str.find("icon:");
					string str2;
					for (int z = pos + 5; z < str.length(); z++)
					{
						str2.push_back(str[z]);
					}
					icon = str2[0];
				}
			}
			case 4:
			{
				if (str.find("speed:") != string::npos)
				{
					int pos = str.find("speed:");
					string str2;
					for (int z = pos + 6; z < str.length(); z++)
					{
						str2.push_back(str[z]);
					}
					speed = stoi(str2);
				}
			}
			case 5:
			{

				if (str.find("food:") != string::npos)
				{
					int pos = str.find("food:");
					string str2;
					for (int z = pos + 5; z < str.length(); z++)
					{
						str2.push_back(str[z]);
					}
					food = str2[0];
				}
			}
			}
		}
	}
	
	in.close();
}
void RepairSettings()
{
	if (height > 40) height = 40;
	if (width > 60) width = 60;
	if(speed > 30) speed = 30;
}
 void Color(int cl)
{
	 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)cl);
}

void SetCursor(const int& x, const int& y)
{
	COORD crd;
	crd.X = x;
	crd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), crd);
}

class Player
{
public:
	int x;
	int y;
	int dx;
	int dy;
	int id;
	Player(const int x,const int y,const int dx,const int dy)
	{
		counter_blocks++;
		this->x = x;
		this->y = y;
		this->dx = dx;
		this->dy = dy;
		this->id = counter_blocks - 1;
	}
	void Move()
	{
		int a = movies.size() - 1 - id;
		if (a <= 0) a = 0;

		Do(movies[a]);
		InArea();
		SetCursor(this->x, this->y);
		cout << " ";
		this->x += 2 * this->dx;
		this->y += this->dy;
		Draw();

	}
	void Draw()
	{
		SetCursor(this->x,this->y);
	//	if (this->id == 0) 		Color(2);
		//else Color(1);
		cout << icon;
	
		//Color(0);
	}
	bool InArea()
	{
		if (this->x >= 2*width-3 && this->dx == 1)
		{
			SetCursor(this->x, this->y);
			cout << " ";
			this->x = 1;
			return false;
		}
		if (this->x + 2 * dx <= 0 && this->dx == -1)
		{
			SetCursor(this->x, this->y);
			cout << " ";
			this->x = 2 * width - 3;
			return false;
		}
		if (this->y+dy <= 0 && this->dy == -1)
		{
			SetCursor(this->x, this->y);
			cout << " ";
			this->y = height+1;
			return false;
		}
		if (this->y+dy >= height+1 && this->dy == 1)
		{
			SetCursor(this->x, this->y);
			cout << " ";
			this->y = 0;
			return false;
		}
		return true;
	}
	void Do(const char& action)
	{
		switch (action)
		{
			case 'u': {this->dx = 0; this->dy = -1; break; }
			case 'd': {this->dx = 0; this->dy = 1; break; }
			case 'l': {this->dx = -1; this->dy = 0; break; }
			case 'r': {this->dx = 1; this->dy = 0; break; }
		}
		if (this->id == 0) act = action;
	}

};

void DrawArea()
{
	for (int i = 0; i < width; i++) cout << ln << " ";
	cout << endl;
	for (int i = 0; i < height; i++)
	{
		cout << ln;
		for (int z = 0; z < 2*width-3; z++) cout << " ";
		cout << ln << endl;
	}
	for (int i = 0; i < width; i++) cout << ln << " ";
}

void Delay(int ms)
{
	this_thread::sleep_for(chrono::milliseconds(ms));
}

void Over(thread& th)
{
	th.detach();
	SetCursor(0,height+2);
	cout << "Game Over" << endl;
	Delay(1000);
	cout << "3 ";
	Delay(1000);
	cout << "2 ";
	Delay(1000);
	cout << "1 " << endl;;
	Delay(1000);
	cout << "...";
	system("pause");
}
void Key()
{
	int button = 0;
	while (true)
	{
		if (_kbhit())
		{
			button = _getch();
			switch (button)
			{
			case 72: 
			{
				if (movies.size() > 1)
				{
					if (movies[movies.size() - 1] != 'd')
					{
						actions.push_back('u');
					}
				}else actions.push_back('u');
				break;
			} //вверх
			case 80:
			{
				if (movies.size() > 1)
				{
					if (movies[movies.size() - 1] != 'u')
					{
						actions.push_back('d');
					}
				}
				else actions.push_back('d');
				break;
			}//вниз 80
			case 75: 
			{
				if (movies.size() > 1)
				{
					if (movies[movies.size() - 1] != 'r')
					{
						actions.push_back('l');
					}
				}
				else actions.push_back('l');
				break;
			}
			//влево 75
			case 77:
			{
				if (movies.size() > 1)
				{
					if (movies[movies.size() - 1] != 'l')
					{
						actions.push_back('r');
					}
				}
				else actions.push_back('r');
				break;
			} //вправо 77
			}
		}

		Delay(250/speed);
	}
}

void Win(const int& size,thread& th)
{
	th.detach();
	Delay(2000);
	SetCursor(0,height+1);
	cout << "You win" << endl << "Your length is " << size << endl;
	Delay(1000);
	cout << "3 ";
	Delay(1000);
	cout << "2 ";
	Delay(1000);
	cout << "1 " << endl;;
	Delay(1000);
	cout << "...";
	system("pause");
}
void NewPlayer(vector<Player>& blocks)
{
#define last blocks[blocks.size() - 1]
	Player next(last.x - 2 * last.dx, last.y - last.dy, last.dx, last.dy);
	blocks.push_back(next);
}
void SpawnFood(vector<Player>& blocks)
{
	xf = rand() % (2 * width-2) + 1;
	while (xf%2 == 0)
	{
		xf = rand() % (2 * width-2)+1;
	}
	yf = rand() % (height) + 1;
	SetCursor(xf,yf);
	cout << food;
	for(int i=0;i<blocks.size()-1;i++)
	{
		if (blocks[i].x == xf && blocks[i].y == yf)
		{
			SpawnFood(blocks);
			NewPlayer(blocks);
		}
	}

}
int main()
{

	Load();
	RepairSettings();
	srand(time(NULL));
	setlocale(LC_ALL,"ru");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	DrawArea();
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);

	int w = width;
	if (width % 2 == 0) w++;
	Player head(w, height / 2, 1, 0);
	head.Draw();

	bool game_over = false;
	thread th(Key);

	vector<Player> blocks;
	blocks.push_back(head);

	
	SpawnFood(blocks);

	while (!game_over)
	{

		if (actions.size() > 0)
		{
			blocks[0].Do(actions[0]);
			actions.erase(actions.begin());
		}
		movies.push_back(act);
		for (int i = 0; i < (int)blocks.size(); i++)
		{
			blocks[i].Move();
		}
		for (int i = 1; i < blocks.size() - 1; i++)
		{
			if (blocks[0].x == blocks[i].x && blocks[0].y == blocks[i].y)
			{
				game_over = true;
				break;
			}
		}
		if (game_over)
		{
			Over(th);
			return 0;
		}
		if((int)movies.size()-(int)blocks.size()> 0)
		{
			for (int i = 0; i < (int)movies.size() - (int)blocks.size(); i++)
			{
				movies.erase(movies.begin());
			}
		}
		if (blocks[0].x == xf && blocks[0].y == yf) 
		{
			SpawnFood(blocks);
			NewPlayer(blocks);
		}
		if (blocks.size()==width*height)
		{
			Win(blocks.size(),th);
			return 0;
		}
		Delay(1000/speed);
	}

	Over(th);
	return 0;
}
