#include<iostream>
#include<easyx.h>
#include<vector>
#include<ctime>

int ifSurvie;//蛇是否存活

class Sprite
{
public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x,int y):m_x(x),m_y(y),m_color(RED){};

	//绘制精灵
	virtual void draw()
	{
		setfillcolor(m_color);//填充颜色
		setlinecolor(WHITE);//边框颜色
		fillrectangle(m_x, m_y, m_x+10, m_y+10);//绘制矩形
	}

	//移动
	void move(int deltaX, int deltaY)
	{
		m_x += deltaX;
		m_y += deltaY;
	}

	//碰撞检测
	bool collision(const Sprite& other)
	{
		return m_x == other.m_x && m_y == other.m_y;
	}

	//检测是否越界
	bool ifOutBoundary()
	{
		if (m_y <0 || m_y > 480 || m_x <0 || m_x >640)
			return TRUE;
		else return FALSE;
	}

protected:
	int m_x;
	int m_y;
	COLORREF m_color;//颜色
};

class Snake: public Sprite
{
public:
	Snake():Snake(0,0){}
	Snake(int x,int y):Sprite(x,y),dir(VK_RIGHT)
	{
		//初始化三节蛇
		nodes.push_back(Sprite(20, 0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}

	void draw() override
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		}
	}

	void MoveBody()//蛇身体的移动
	{
		for (size_t i = nodes.size()-1; i >0; i--)
		{
			nodes[i] = nodes[i - 1];

		}
		switch (dir)
		{
		case VK_UP:
			nodes[0].move(0,-10);
			break;
		case VK_DOWN:
			nodes[0].move(0,10);
			break;
		case VK_RIGHT:
			nodes[0].move(10, 0);
			break;
		case VK_LEFT:
			nodes[0].move(-10, 0);
			break;
		}
	}

	//检测蛇头碰撞
	bool collision(const Sprite& other)
	{
		return nodes[0].collision(other);
	}

	//边界碰撞
	bool ifOut()
	{
		return nodes[0].ifOutBoundary();
	}

	//判断自我碰撞
	bool SelfCollision()
	{
		for (int i = 1; i < nodes.size(); i++)
		{
			if (nodes[0].collision(nodes[i]))
				return TRUE;
		}
		return FALSE;
	}

	//蛇增加一节
	void increment()
	{
		nodes.push_back(Sprite());
	}

	int getLength()
	{
		return nodes.size();
	}

	int dir;//蛇的方向

private:
	std::vector<Sprite> nodes;//存放蛇的所有节点
};

class Food :public Sprite
{
public:
	Food() :Sprite(0, 0) 
	{
		changeLoc();
	}

	void draw()override
	{
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}

	//改变食物坐标
	void changeLoc()
	{
		m_x = rand() % 64 * 10;//保证食物坐标是10的整数
		m_y = rand() % 48 * 10;
	}
};

class GameScene
{
public:
	GameScene(){};
	void run()
	{
		BeginBatchDraw();//双缓冲绘图以防止频闪 begin

		cleardevice();//清屏

		ifSnakeEateFood();//判断是否吃到食物

		EndBatchDraw();//双缓冲绘图以防止频闪 end

		snake.MoveBody();//改变蛇的移动方向

		//获取消息
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EX_KEY))
		{
			onMsg(msg);
		}
	}

	//响应消息，包括鼠标和键盘
	void onMsg(const ExMessage& msg)
	{
		//若有键盘消息
		if (msg.message == WM_KEYDOWN)
		{
			//判断具体是哪个键 virtual key code
			switch (msg.vkcode)
			{
			case VK_UP:
				if (snake.dir != VK_DOWN)
					snake.dir = msg.vkcode;
				break;
			case VK_DOWN:
				if (snake.dir != VK_UP)
					snake.dir = msg.vkcode;
				break;
			case VK_RIGHT:
				if (snake.dir != VK_LEFT)
					snake.dir = msg.vkcode;
				break;
			case VK_LEFT:
				if (snake.dir != VK_RIGHT)
					snake.dir = msg.vkcode;
				break;
			}
		}
	}

	//判断蛇能否吃到食物
	void ifSnakeEateFood()
	{
		if (snake.collision(food))//如果蛇和食物产生碰撞
		{
			snake.increment();
			food.changeLoc();
		}
	}

private:
	Snake snake;
	Food food;
};

int main()
{
	ifSurvie = 1;

	initgraph(640, 480,EX_SHOWCONSOLE);//画板

	//设计随机数种子
	srand(time(nullptr));

	GameScene scene;
	while (true) 
	{
		if (ifSurvie == 0)
			break;
		system("cls");
		scene.run();
		Sleep(100);
	}

	getchar();
	return 0;
}