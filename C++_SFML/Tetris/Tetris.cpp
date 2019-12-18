/*
 * 写在前面：
 * 一个简单的俄罗斯方块游戏，没有制作连消分数奖励，没有制作分数显示，没事判断游戏结束(为什么不做？懒)
 * 最初是考虑使用一个Game(或者Tetris)类，然后在这个类中添加各种方法(例如新建图形，移动图形，消除图像，变形等等)
 * 但是实际上这么做没有任何意义，并不会对这个程序产生什么积极作用，所以就没有使用类编程
 * 有些方法例如新建一个图形，消除图像，绘制图像等等都没有放在一个函数中，因为它们需要相同的参数，
 * 所以必须将参数放在同一个作用域中，但是预期将其分离然后使用参数传递不如直接放在一个作用域中
 * 运行，节省传值占用的额外空间，并不会影响性能(大概)
 */
#include <SFML/Graphics.hpp>

using namespace sf;

const int field_lines = 20; /* 使用宽10格，高20格的矩阵作为游戏场地 */
const int field_rows = 10;

int game_field[field_lines][field_rows] = {{0}}; // 初始化场地

struct point
{
	/* 用于图形变形，理论上使用2*4的矩阵也能支持4*4那样的矩阵变形 */
	int x; /* 这里的x表示横向的第几个位置，从0开始 */
	int y; /* 这里的y表示纵向的第几层位置，从0开始 */
} a[4], b[4];

int figures[7][4] = {
	/* 图形出现的样子，除了I高度占四格以外，其余的图形不论高度占三格还是两格都是从第二层开始绘制，并且以第二行作为旋转中心 */
	{1, 3, 5, 7}, // I
	/* 对于I
	 * x, y		0  1  
	 * 1  0	-->	空 砖 0
	 * 1  1		空 砖 1
	 * 1  2		空 砖 2
	 * 1  3	    空 砖 3
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
		/* 判断图形是否越界 */
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

	auto mid_field_rows = int(field_rows / 2) - 1;	/* 令图形出现在场地上一行的中间，-1是补偿旋转中心的偏差 */
	auto dx = 0; /* 图形左右移动了多少格 */
	auto is_rotate = false; /* 图形是否旋转 */
	float timer = 0; /* 计时器，用来确定是否需要下降 */
	auto delay = 0.3; /* 图形下降的周期 */
	auto score = 0; /* 得分 */

	/* 随机种子 */
	srand(time(nullptr));
	/* 随机构建第一个图形 */
    auto color_num = 1 + rand() % 7;
    /* 方块颜色，从tiles.png中截取，因为第一个和最后一个颜色相同，所以从1开始 */
	auto n = rand() % 7;
	/* 图形类型 */
	for (auto i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2 + mid_field_rows;
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
			delay = 0.05; /* 按下加快下降 */
		}

		/* 让图形左右移动 */
		for (auto i = 0; i < 4; i++)
		{
			b[i] = a[i]; /* 储存当前图形的位置 */
			a[i].x += dx; /* 进行移动 */
		}
		if (!check())
		{
			// 图形越界
			for (auto i = 0; i < 4; i++)
			{
				a[i] = b[i]; /* 回档至储存的状态，此次移动无效 */
			}
		}

		/* 让图形旋转 */
		if (is_rotate)
		{
			auto center = a[1]; /* 以第二行作为旋转中心 */
			for (auto& i : a)
			{
				/* 例如 L 变形	center.x = 1	center.y = 1
				 * 刚出现				转变为(此时变为了y表示横向，x表示纵向)
				 * x, y		0  1		x, y	 0  1	-->  这一行变成了y
				 * 0  1 --> 空 空 0		1  0     空 空 2 (注意以第二行作为旋转中心)
				 * 1  1		砖 砖 1		1  1     砖 砖 1
				 * 1  2		空 砖 2		0  1     空 砖 0
				 * 1  3		空 砖 3		-1 1     空 砖 -1
				 *
				 *	   x:0 x:1
				 *		 _ _
				 *		|_|_| y:0	---> 这一行没有内容，只是说明这上面还有一行
				 *		|_|*| y:1	---> *是旋转中心
				 *		  |_| y:2
				 *		  |_| y:3
				 *
				 *	   x:-1
				 *		 x:0
				 *		   x:1
				 *		     x:2
				 *		   _ _
				 *	   _ _|_|_| y:0	---> *是旋转中心
				 *    |_|_|*|_| y:1
				 *		     ↑这一列没有内容，只是说明这一列在旋转前是名义上的第一行
				 * 
				 */
				auto center_x = i.y - center.y;
				auto center_y = i.x - center.x;
				i.x = center.x - center_x;
				i.y = center.y + center_y;
			}
			if (!check())
			{
				/* 旋转超界 */
				for (auto i = 0; i < 4; i++)
				{
					a[i] = b[i]; /* 旋转取消，回档 */
				}
			}
		}

		/* 图形下降 */
		if (timer > delay)
		{
			for (auto i = 0; i < 4; i++)
			{
				b[i] = a[i]; /* 储存当前位置 */
				a[i].y += 1; /* 图形下降一格 */
			}
			if (!check())
			{
				/* 图形越界(即图形已经下降到不能下降的位置) */
				/* 一个图形结束 */
				for (auto& i : b) 
				/* 将b中备份的坐标对应在场上的位置用对应颜色标记，不然后面无法绘制图形，图形会凭空消失 */
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
					a[i].x = figures[n][i] % 2 + mid_field_rows;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0; /* 重置计时器 */
		}

		/* 检查消除 */
		auto base_line = field_lines - 1; /* 从最底下一行开始往上进行逐行判断 */
		for (auto line = field_lines - 1; line > 0; line--)
		{
			/* 第一行不用判断了，因为如果仅剩一行空间图形出现就应该直接游戏结束 */
			auto is_eliminated = true;
			for (auto row = 0; row < field_rows; row++)
			{
				if (!game_field[line][row])
				{
					/* 一旦有一格为空则无法消除 */
					is_eliminated = false;
				}
				/* 将第line行搬动至baseLine行 */
				game_field[base_line][row] = game_field[line][row];
			}
			if (!is_eliminated)
			{
				/* 未消除则 baseLine不变，判断上一行(这样上面的搬动可以保证总会将图形下降至最底下的不可消除的行上) */
				base_line--;
			}
			else
			{
				score++;
				/* 这个分数暂时没有使用，但是保留了 */
			}
		}

		/* 重置移动，旋转，周期 */
		dx = 0;
		is_rotate = false;
		delay = 0.3;

		window.clear(Color::White);
		window.draw(background);

		/* 绘制场上已经被color_num标记的图形 */
		for (auto line = 0; line < field_lines; line++)
		{
			for (auto row = 0; row < field_rows; row++)
			{
				if (!game_field[line][row])
				{
					/* 如果这个格子为空则不进行绘制 */
					continue;
				}
				tiles.setTextureRect(IntRect(game_field[line][row] * 18, 0, 18, 18));
				tiles.setPosition(float(row * 18), float(line * 18));
				tiles.move(28, 31); // 偏移
				window.draw(tiles);
			}
		}
		/* 绘制当前正在操作的图形 */
		for (auto& i : a)
		{
			tiles.setTextureRect(IntRect(color_num * 18
			                             /* 根据colorNum从tiles.png中截取对应的图形 */,
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
