#include<iostream>
#include<easyx.h>
#include<vector>
#include<ctime>

int ifSurvie;//���Ƿ���

class Sprite
{
public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x,int y):m_x(x),m_y(y),m_color(RED){};

	//���ƾ���
	virtual void draw()
	{
		setfillcolor(m_color);//�����ɫ
		setlinecolor(WHITE);//�߿���ɫ
		fillrectangle(m_x, m_y, m_x+10, m_y+10);//���ƾ���
	}

	//�ƶ�
	void move(int deltaX, int deltaY)
	{
		m_x += deltaX;
		m_y += deltaY;
	}

	//��ײ���
	bool collision(const Sprite& other)
	{
		return m_x == other.m_x && m_y == other.m_y;
	}

	//����Ƿ�Խ��
	bool ifOutBoundary()
	{
		if (m_y <0 || m_y > 480 || m_x <0 || m_x >640)
			return TRUE;
		else return FALSE;
	}

protected:
	int m_x;
	int m_y;
	COLORREF m_color;//��ɫ
};

class Snake: public Sprite
{
public:
	Snake():Snake(0,0){}
	Snake(int x,int y):Sprite(x,y),dir(VK_RIGHT)
	{
		//��ʼ��������
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

	void MoveBody()//��������ƶ�
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

	//�����ͷ��ײ
	bool collision(const Sprite& other)
	{
		return nodes[0].collision(other);
	}

	//�߽���ײ
	bool ifOut()
	{
		return nodes[0].ifOutBoundary();
	}

	//�ж�������ײ
	bool SelfCollision()
	{
		for (int i = 1; i < nodes.size(); i++)
		{
			if (nodes[0].collision(nodes[i]))
				return TRUE;
		}
		return FALSE;
	}

	//������һ��
	void increment()
	{
		nodes.push_back(Sprite());
	}

	int getLength()
	{
		return nodes.size();
	}

	int dir;//�ߵķ���

private:
	std::vector<Sprite> nodes;//����ߵ����нڵ�
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

	//�ı�ʳ������
	void changeLoc()
	{
		m_x = rand() % 64 * 10;//��֤ʳ��������10������
		m_y = rand() % 48 * 10;
	}
};

class GameScene
{
public:
	GameScene(){};
	void run()
	{
		BeginBatchDraw();//˫�����ͼ�Է�ֹƵ�� begin

		cleardevice();//����

		ifSnakeEateFood();//�ж��Ƿ�Ե�ʳ��

		EndBatchDraw();//˫�����ͼ�Է�ֹƵ�� end

		snake.MoveBody();//�ı��ߵ��ƶ�����

		//��ȡ��Ϣ
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EX_KEY))
		{
			onMsg(msg);
		}
	}

	//��Ӧ��Ϣ���������ͼ���
	void onMsg(const ExMessage& msg)
	{
		//���м�����Ϣ
		if (msg.message == WM_KEYDOWN)
		{
			//�жϾ������ĸ��� virtual key code
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

	//�ж����ܷ�Ե�ʳ��
	void ifSnakeEateFood()
	{
		if (snake.collision(food))//����ߺ�ʳ�������ײ
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

	initgraph(640, 480,EX_SHOWCONSOLE);//����

	//������������
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