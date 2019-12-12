/*
 * 写在前面：
 * 一个简单的俄罗斯方块游戏，没有制作连消分数奖励，没有制作分数显示，没有判断游戏结束(为什么不做？懒)
 * 最初是考虑使用一个类Game(或者Tetris)，然后在类中添加各种方法(例如新建图形，移动图形，消除图形，变形等等)
 * 但是实际上这么做没有任何意义，并不会对这个程序产生什么积极作用，所以就没有使用类进行编程
 * 有些方法例如新建一个图形，消除图像，绘制图像等等都没有放在一个函数中，因为他们需要使用的相同的参数，
 * 所以必须将参数放在同一个作用域(main)中，但是与其将其分离然后然后使用参数传递不如直接在一个作用域内
 * 运行，节省传值占用的额外的空间，而且这些函数一般在一个周期(_delay)中只执行一次，并不会影响性能(大概)
 */
#include <SFML/Graphics.hpp>

using namespace sf;

const int field_lines = 20; /* 使用宽为10, 高为20的矩形作为场地 */
const int field_rows = 10;

int game_field[field_lines][field_rows] = {{0}}; // 场地

struct point
{
	/* 用以图形变形，理论上使用2*4的矩阵也能支持像4*4矩阵那样变形 */
	int x; /* 这里的x表示横向的第几个位置,从0开始 */
	int y; /* 这里的y表示纵向的第几层位置，从0开始 */
} a[4], b[4];

int figures[7][4] = {
	/* 图形出现的样子,除了I占四格外，其余占两个或者三格的都从第二行开始，并且以第二行作为旋转中心 */
	{1, 3, 5, 7}, // I
	/* 对于I
	 * x, y		0  1  
	 * 1  0	-->	空 砖 0
	 * 1  1		空 砖 1
	 * 1  2		空 砖 2
	 * 1  3		空 砖 2
	 */
	{2, 4, 5, 7}, // S
	/* 对于S
	 * x, y		0  1
	 * 0  1 --> 空 空 0
	 * 0  2		砖 空 1
	 * 1  2		砖 砖 2
	 * 1  3		空 砖 3
	 */
	{3, 5, 4, 6}, // Z
	/* 对于Z
	 * x, y		0  1
	 * 1  1 --> 空 空 0
	 * 1  2		空 砖 1
	 * 0  2		砖 砖 2
	 * 0  3		砖 空 3
	 */
	{3, 5, 4, 7}, // T
	/* 对于T
	 * x, y		0  1
	 * 1  1 --> 空 空 0
	 * 1  2		空 砖 1
	 * 0  2		砖 砖 2
	 * 1  3		空 砖 3
	 */
	{2, 3, 5, 7}, // L
	/* 对于L
	 * x, y		0  1
	 * 0  1 --> 空 空 0
	 * 1  1		砖 砖 1
	 * 1  2		空 砖 2
	 * 1  3		空 砖 3
	 */
	{3, 5, 7, 6}, // J
	/* 对于J
	 * x, y		0  1
	 * 1  1 --> 空 空 0
	 * 1  2		空 砖 1
	 * 1  3		空 砖 2
	 * 0  3		砖 砖 3
	 */
	{2, 3, 4, 5}, // O
	/* 对于O
	 * x, y		0  1
	 * 0  1 --> 空 空 0
	 * 1  1		砖 砖 1
	 * 0  2		砖 砖 2
	 * 1  2		空 空 3
	 */
};

bool check()
{
	for (auto& i : a)
	{
		/* 检测图形变形不会超界 */
		if (i.x < 0 || i.x > field_rows - 1 || i.y >= field_lines || game_field[i.y][i.x])
		{
			return false;
		}
	}
	return true;
}

int main()
{
	RenderWindow window(VideoMode(320, 480), "Tetris Game");

	Texture texture_tiles, texture_background, texture_frame;

	texture_tiles.loadFromFile("images_tetris/tiles.png");
	texture_background.loadFromFile("images_tetris/background.png");
	texture_frame.loadFromFile("images_tetris/frame.png");

	Sprite tiles(texture_tiles), background(texture_background), frame(texture_frame);

	auto mid_field_lines = int(10 / 2) - 1;	/* 图形出现的位置,出现在场地中央,-1是因为选取的中心点在左边 */
	auto dx = 0; /* 图形左右移动了多少格 */
	auto is_rotate = false; /* 是否旋转 */
	auto color_num = 1 + rand() % 7;
	/* 方块颜色,从tiles.png图片中截取方块颜色，因为第一个和最后一个颜色相同，所以从1开始 */
	float timer = 0; /* 计时器,用来确定是否需要移动(下降) */
	auto delay = 0.3; /* 图形移动延时 */
	auto score = 0; /* 得分 */

	/* 随即构建第一个图形 */
	auto n = 1;
	for (auto i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2 + mid_field_lines;
		a[i].y = figures[n][i] / 2;
	}

	Clock clock;

	while (window.isOpen())
	{
		auto elapsed_time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += elapsed_time;

		Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Up)
				{
					is_rotate = true; /* 按上旋转 */
				}
				else if (event.key.code == Keyboard::Left)
				{
					dx = -1;
				}
				else if (event.key.code == Keyboard::Right)
				{
					dx = 1;
				}
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			delay = 0.05; /* 按下加快下降速度 */
		}

		/* 移动 */
		for (auto i = 0; i < 4; i++)
		{
			b[i] = a[i]; /* 储存图形原来的位置 */
			a[i].x += dx; /* 图形根据按下的键进行左右移动 */
		}
		if (!check())
		{
			// 不符规则(变形会超界)
			for (auto i = 0; i < 4; i++)
			{
				a[i] = b[i]; /* 使用原来储存的图形(即此次移动无效) */
			}
		}

		/* 旋转 */
		if (is_rotate)
		{
			auto center = a[1]; /* 取第二行为旋转中心 */
			for (auto& i : a)
			{
				/* 例如 L 变形	center.x = 1	center.y = 1
				 * 刚出现					转变为(此时变为了y表示横向,x表示纵向)
				 * x, y		0  1		x, y	 0  1	--> 这一行变成了y	
				 * 0  1 --> 空 空 0		1  0 --> 空 空 2(注意以第一行为旋转中心)
				 * 1  1		砖 砖 1		1  1	 砖 砖 1
				 * 1  2		空 砖 2		0  1	 空 砖 0
				 * 1  3		空 砖 3		-1 1	 空 砖 -1
				 *
				 *	   x:0 x:1
				 *		 _ _
				 *		|_|_| y:0	---> 这第一行没有内容，只是说明这上面还有一行
				 *		|*|_| y:1	---> *表示旋转中心
				 *		  |_| y:2
				 *		  |_| y:3
				 *
				 *	 x:-2
				 *	   x:-1
				 *		 x:0
				 *		   x:1 
				 *		 _ _
				 *	 _ _|*|_| y:0	---> *表示旋转中心
				 *  |_|_|_|_| y:1
				 *		   ↑这一列没有内容,只是说明这一列在旋转前是名义上的第一行	
				 * 
				 */
				auto center_x = i.y - center.y;
				auto center_y = i.x - center.x;
				i.x = center.x - center_x;
				i.y = center.y + center_y;
			}
			if (!check())
			{
				/* 不符规则(变形会超界) */
				for (auto i = 0; i < 4; i++)
				{
					a[i] = b[i]; /* 使用原来储存的图形(即此次旋转无效) */
				}
			}
		}

		/* 图形下降 */
		if (timer > delay)
		{
			for (auto i = 0; i < 4; i++)
			{
				b[i] = a[i]; /* 储存用以检测移动合法 */
				a[i].y += 1; /* 下降一格 */
			}
			if (!check())
			{
				/* 不符规则(即图形已经下降到不能下降的位置) */
				/* 一个图形结束, 开始下一个图象 */
				for (auto& i : b) 
				/* 将b中记录的坐标对应在场上的位置设置为对应的颜色,要是不设置图形不会保留在对应场地 */
				{
					game_field[i.y][i.x] = color_num;
				}
				
				/* 新出现一个图形 */
				/* 重设颜色 */
				color_num = 1 + rand() % 7;	
				/* 重设形状 */
				n = rand() % 7;
				for (auto i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2 + mid_field_lines;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0; /* 计时器初始化 */
		}

		/* 检查是否能消除 */
		auto base_line = field_lines - 1; /* 从最底下的一行开始 */
		for (auto line = field_lines - 1; line > 0; line--)
		{
			/* 除第一行外所有行都可以判断是否可以消除, 所以 i 不能等于 0 */
			/* 从最底下一行开始一行一行地检查 */
			auto is_eliminated = true;
			for (auto row = 0; row < field_rows; row++)
			{
				if (!game_field[line][row])
				{
					/* 一旦有一格为空则不消除 */
					is_eliminated = false;
				}
				/* 将第 i 行的内容搬到第 baseLine 行 */
				game_field[base_line][row] = game_field[line][row];
			}
			if (!is_eliminated)
			{
				/* 未消除则 baseLine 不变,判断上一行(这样上面的搬动可以保证总会将场地上的图形下降至最底下的不可消除的行上) */
				base_line--;
			}
			else
			{
				score++;
				/* 我也不知道这个分数要了干嘛，留着吧 */
			}
		}

		/* 图形更新 */
		dx = 0;
		is_rotate = false;
		delay = 0.3;

		window.clear(Color::White);
		window.draw(background);

		/* 绘制场地上的图象 */
		for (auto line = 0; line < field_lines; line++)
		{
			for (auto row = 0; row < field_rows; row++)
			{
				if (!game_field[line][row])
				{
					/* 如果这个格子为空则跳过判断 */
					continue;
				}
				tiles.setTextureRect(IntRect(game_field[line][row] * 18, 0, 18, 18));
				tiles.setPosition(float(row * 18), float(line * 18));
				tiles.move(28, 31); // offset
				window.draw(tiles);
			}
		}
		/* 绘制当前操作的图象 */
		for (auto& i : a)
		{
			tiles.setTextureRect(IntRect(color_num * 18
			                             /* 根据colorNum的编号选择tiles图片中的对应位置颜色 */,
			                             0, 18, 18));
			tiles.setPosition(float(i.x) * 18, float(i.y) * 18);
			tiles.move(28, 31);
			window.draw(tiles);
		}

		window.draw(frame);
		window.display();
	}

	return 0;
}
