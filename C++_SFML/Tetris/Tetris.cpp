/*
 * д��ǰ�棺
 * һ���򵥵Ķ���˹������Ϸ��û��������������������û������������ʾ��û���ж���Ϸ����(Ϊʲô��������)
 * ����ǿ���ʹ��һ����Game(����Tetris)��Ȼ����������Ӹ��ַ���(�����½�ͼ�Σ��ƶ�ͼ�Σ�����ͼ�Σ����εȵ�)
 * ����ʵ������ô��û���κ����壬�����������������ʲô�������ã����Ծ�û��ʹ������б��
 * ��Щ���������½�һ��ͼ�Σ�����ͼ�񣬻���ͼ��ȵȶ�û�з���һ�������У���Ϊ������Ҫʹ�õ���ͬ�Ĳ�����
 * ���Ա��뽫��������ͬһ��������(main)�У��������佫�����Ȼ��Ȼ��ʹ�ò������ݲ���ֱ����һ����������
 * ���У���ʡ��ֵռ�õĶ���Ŀռ䣬������Щ����һ����һ������(_delay)��ִֻ��һ�Σ�������Ӱ������(���)
 */
#include <SFML/Graphics.hpp>

using namespace sf;

const int field_lines = 20; /* ʹ�ÿ�Ϊ10, ��Ϊ20�ľ�����Ϊ���� */
const int field_rows = 10;

int game_field[field_lines][field_rows] = {{0}}; // ����

struct point
{
	/* ����ͼ�α��Σ�������ʹ��2*4�ľ���Ҳ��֧����4*4������������ */
	int x; /* �����x��ʾ����ĵڼ���λ��,��0��ʼ */
	int y; /* �����y��ʾ����ĵڼ���λ�ã���0��ʼ */
} a[4], b[4];

int figures[7][4] = {
	/* ͼ�γ��ֵ�����,����Iռ�ĸ��⣬����ռ������������Ķ��ӵڶ��п�ʼ�������Եڶ�����Ϊ��ת���� */
	{1, 3, 5, 7}, // I
	/* ����I
	 * x, y		0  1  
	 * 1  0	-->	�� ש 0
	 * 1  1		�� ש 1
	 * 1  2		�� ש 2
	 * 1  3		�� ש 2
	 */
	{2, 4, 5, 7}, // S
	/* ����S
	 * x, y		0  1
	 * 0  1 --> �� �� 0
	 * 0  2		ש �� 1
	 * 1  2		ש ש 2
	 * 1  3		�� ש 3
	 */
	{3, 5, 4, 6}, // Z
	/* ����Z
	 * x, y		0  1
	 * 1  1 --> �� �� 0
	 * 1  2		�� ש 1
	 * 0  2		ש ש 2
	 * 0  3		ש �� 3
	 */
	{3, 5, 4, 7}, // T
	/* ����T
	 * x, y		0  1
	 * 1  1 --> �� �� 0
	 * 1  2		�� ש 1
	 * 0  2		ש ש 2
	 * 1  3		�� ש 3
	 */
	{2, 3, 5, 7}, // L
	/* ����L
	 * x, y		0  1
	 * 0  1 --> �� �� 0
	 * 1  1		ש ש 1
	 * 1  2		�� ש 2
	 * 1  3		�� ש 3
	 */
	{3, 5, 7, 6}, // J
	/* ����J
	 * x, y		0  1
	 * 1  1 --> �� �� 0
	 * 1  2		�� ש 1
	 * 1  3		�� ש 2
	 * 0  3		ש ש 3
	 */
	{2, 3, 4, 5}, // O
	/* ����O
	 * x, y		0  1
	 * 0  1 --> �� �� 0
	 * 1  1		ש ש 1
	 * 0  2		ש ש 2
	 * 1  2		�� �� 3
	 */
};

bool check()
{
	for (auto& i : a)
	{
		/* ���ͼ�α��β��ᳬ�� */
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

	auto mid_field_lines = int(10 / 2) - 1;	/* ͼ�γ��ֵ�λ��,�����ڳ�������,-1����Ϊѡȡ�����ĵ������ */
	auto dx = 0; /* ͼ�������ƶ��˶��ٸ� */
	auto is_rotate = false; /* �Ƿ���ת */
	auto color_num = 1 + rand() % 7;
	/* ������ɫ,��tiles.pngͼƬ�н�ȡ������ɫ����Ϊ��һ�������һ����ɫ��ͬ�����Դ�1��ʼ */
	float timer = 0; /* ��ʱ��,����ȷ���Ƿ���Ҫ�ƶ�(�½�) */
	auto delay = 0.3; /* ͼ���ƶ���ʱ */
	auto score = 0; /* �÷� */

	/* �漴������һ��ͼ�� */
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
					is_rotate = true; /* ������ת */
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
			delay = 0.05; /* ���¼ӿ��½��ٶ� */
		}

		/* �ƶ� */
		for (auto i = 0; i < 4; i++)
		{
			b[i] = a[i]; /* ����ͼ��ԭ����λ�� */
			a[i].x += dx; /* ͼ�θ��ݰ��µļ����������ƶ� */
		}
		if (!check())
		{
			// ��������(���λᳬ��)
			for (auto i = 0; i < 4; i++)
			{
				a[i] = b[i]; /* ʹ��ԭ�������ͼ��(���˴��ƶ���Ч) */
			}
		}

		/* ��ת */
		if (is_rotate)
		{
			auto center = a[1]; /* ȡ�ڶ���Ϊ��ת���� */
			for (auto& i : a)
			{
				/* ���� L ����	center.x = 1	center.y = 1
				 * �ճ���					ת��Ϊ(��ʱ��Ϊ��y��ʾ����,x��ʾ����)
				 * x, y		0  1		x, y	 0  1	--> ��һ�б����y	
				 * 0  1 --> �� �� 0		1  0 --> �� �� 2(ע���Ե�һ��Ϊ��ת����)
				 * 1  1		ש ש 1		1  1	 ש ש 1
				 * 1  2		�� ש 2		0  1	 �� ש 0
				 * 1  3		�� ש 3		-1 1	 �� ש -1
				 *
				 *	   x:0 x:1
				 *		 _ _
				 *		|_|_| y:0	---> ���һ��û�����ݣ�ֻ��˵�������滹��һ��
				 *		|*|_| y:1	---> *��ʾ��ת����
				 *		  |_| y:2
				 *		  |_| y:3
				 *
				 *	 x:-2
				 *	   x:-1
				 *		 x:0
				 *		   x:1 
				 *		 _ _
				 *	 _ _|*|_| y:0	---> *��ʾ��ת����
				 *  |_|_|_|_| y:1
				 *		   ����һ��û������,ֻ��˵����һ������תǰ�������ϵĵ�һ��	
				 * 
				 */
				auto center_x = i.y - center.y;
				auto center_y = i.x - center.x;
				i.x = center.x - center_x;
				i.y = center.y + center_y;
			}
			if (!check())
			{
				/* ��������(���λᳬ��) */
				for (auto i = 0; i < 4; i++)
				{
					a[i] = b[i]; /* ʹ��ԭ�������ͼ��(���˴���ת��Ч) */
				}
			}
		}

		/* ͼ���½� */
		if (timer > delay)
		{
			for (auto i = 0; i < 4; i++)
			{
				b[i] = a[i]; /* �������Լ���ƶ��Ϸ� */
				a[i].y += 1; /* �½�һ�� */
			}
			if (!check())
			{
				/* ��������(��ͼ���Ѿ��½��������½���λ��) */
				/* һ��ͼ�ν���, ��ʼ��һ��ͼ�� */
				for (auto& i : b) 
				/* ��b�м�¼�������Ӧ�ڳ��ϵ�λ������Ϊ��Ӧ����ɫ,Ҫ�ǲ�����ͼ�β��ᱣ���ڶ�Ӧ���� */
				{
					game_field[i.y][i.x] = color_num;
				}
				
				/* �³���һ��ͼ�� */
				/* ������ɫ */
				color_num = 1 + rand() % 7;	
				/* ������״ */
				n = rand() % 7;
				for (auto i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2 + mid_field_lines;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0; /* ��ʱ����ʼ�� */
		}

		/* ����Ƿ������� */
		auto base_line = field_lines - 1; /* ������µ�һ�п�ʼ */
		for (auto line = field_lines - 1; line > 0; line--)
		{
			/* ����һ���������ж������ж��Ƿ��������, ���� i ���ܵ��� 0 */
			/* �������һ�п�ʼһ��һ�еؼ�� */
			auto is_eliminated = true;
			for (auto row = 0; row < field_rows; row++)
			{
				if (!game_field[line][row])
				{
					/* һ����һ��Ϊ�������� */
					is_eliminated = false;
				}
				/* ���� i �е����ݰᵽ�� baseLine �� */
				game_field[base_line][row] = game_field[line][row];
			}
			if (!is_eliminated)
			{
				/* δ������ baseLine ����,�ж���һ��(��������İᶯ���Ա�֤�ܻὫ�����ϵ�ͼ���½�������µĲ�������������) */
				base_line--;
			}
			else
			{
				score++;
				/* ��Ҳ��֪���������Ҫ�˸�����Ű� */
			}
		}

		/* ͼ�θ��� */
		dx = 0;
		is_rotate = false;
		delay = 0.3;

		window.clear(Color::White);
		window.draw(background);

		/* ���Ƴ����ϵ�ͼ�� */
		for (auto line = 0; line < field_lines; line++)
		{
			for (auto row = 0; row < field_rows; row++)
			{
				if (!game_field[line][row])
				{
					/* ����������Ϊ���������ж� */
					continue;
				}
				tiles.setTextureRect(IntRect(game_field[line][row] * 18, 0, 18, 18));
				tiles.setPosition(float(row * 18), float(line * 18));
				tiles.move(28, 31); // offset
				window.draw(tiles);
			}
		}
		/* ���Ƶ�ǰ������ͼ�� */
		for (auto& i : a)
		{
			tiles.setTextureRect(IntRect(color_num * 18
			                             /* ����colorNum�ı��ѡ��tilesͼƬ�еĶ�Ӧλ����ɫ */,
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
