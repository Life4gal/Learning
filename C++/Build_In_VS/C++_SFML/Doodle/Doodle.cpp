/*
 * 写在前面：
 * 一个简单的doodle游戏，没有制作显示分数，没有判断游戏结束(为什么不做？懒)
 * 没什么特色，只有左右横跳。。。
 */
#include <SFML/Graphics.hpp>

using namespace sf;

/* 图片像素大小 */
const int background_width = 400;
const int background_height = 533;
const int platform_width = 68;
const int platform_height = 14;
const bool face_right = true;
const bool face_left = false;

struct platform_point
{
	float x;
	float y;
};

struct player_doodle
{
private:
	/*
	 * doodle 的初始水平位置
	 */
	float x_;
	/*
	 * doodle 的初始铅直位置
	 */
	float y_;
	/*
	 * doodle 的水平移动距离
	 */
	float dy_;
	bool is_face_right_;
public:
	player_doodle() = default;
	player_doodle(const float x, const float y, const float dy, const bool is_face_right)
	: x_(x), y_(y), dy_(dy), is_face_right_(is_face_right) {}

	/*
	 * Doodle::x_ += x;
	 * 原有的水平位置加上传入的量
	 */
	void alter_x(const float x)
	{
		player_doodle::x_ += x;
	}
	/*
	 * Doodle::y_ += y;
	 * 原有的铅直位置加上传入的量
	 */
	void alter_y(const float y)
	{
		player_doodle::y_ += y;
	}
	/*
	 * Doodle::dy_ += dy;
	 * 原有的铅直移动距离加上传入的量
	 */
	void alter_dy(const float dy)
	{
		player_doodle::dy_ += dy;
	}
	/*
	 * Doodle::is_face_right_ = is_face_right;
	 * 是否面向右侧
	 */
	void alter_face(const bool is_face_right)
	{
		player_doodle::is_face_right_ = is_face_right;
	}

	float get_x() const
	{
		return x_;
	}
	float get_y() const
	{
		return y_;
	}
	float get_dy() const
	{
		return dy_;
	}
	bool is_face_right() const
	{
		return is_face_right_;
	}
};

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	RenderWindow window(VideoMode(background_width, background_height), "Doodle Game");
	window.setFramerateLimit(60);

	Texture texture_background, texture_platform, texture_doodle_left, texture_doodle_right;
	texture_background.loadFromFile("images_doodle/background.png");
	texture_platform.loadFromFile("images_doodle/platform.png");
	texture_doodle_left.loadFromFile("images_doodle/doodle_left.png");
	texture_doodle_right.loadFromFile("images_doodle/doodle_right.png");

	Sprite s_background(texture_background), s_platforms(texture_platform),
	s_doodle_left(texture_doodle_left), s_doodle_right(texture_doodle_right);

	/* 每次十个台阶 */
	platform_point platforms[10];
	/* 随机放置台阶 */
	for (auto& platform : platforms)
	{
		platform.x = float(rand() % background_width);
		platform.y = float(rand() % background_height);

		/* 超界补偿 */
		if (platform.x < 5)	platform.x = 5;
		else if (platform.x > background_width - platform_width)	platform.x = background_width - platform_width;
	}

	player_doodle doodle(float(background_width)/2, 100, 0, true);
	/*
	 * 当doodle达到这个高度(即doodle的铅直位置小于这个值)刷出新的台阶
	 * 这个数值越大doodle需要跳跃的高度越低,不要小于200像素
	 */
	auto new_platform_height = 250;

	while (window.isOpen())
	{
		Event e{};
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}

		/* 每次按左右移动3像素 */
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			doodle.alter_x(3);
			doodle.alter_face(face_right);
		} 
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			doodle.alter_x(-3);
			doodle.alter_face(face_left);
		} 

		doodle.alter_dy(float(0.2));	/* 铅直方向每帧(大概)向上速度减小0.2像素，如果大于0则变为下降 */
		doodle.alter_y(doodle.get_dy());

		/* 这里是没踩到台阶直接掉到界面最下方，游戏结束，但是现在只设置为和踩到台阶一样 */
		if (doodle.get_y() > background_height - 50)
		{
			doodle.alter_dy(-doodle.get_dy() - 10); /* 铅直方向向上移动速度设置为每帧(大概)10像素 */
		} 

		if (doodle.get_y() < new_platform_height)	/* doodle高度已经到达刷新高度 */
		{
			doodle.alter_y(-doodle.get_y() + new_platform_height);
			for (auto& platform : platforms)
			{
				platform.y -= doodle.get_dy();
				if (platform.y > background_height) /* 该台阶已经到达(越过)最底下界面 */
				{
					platform.y = 0;	/* 移动到最上方 */
					platform.x = float(rand() % background_width); /* 重新分配水平位置 */

					/* 超界补偿 */
					if (platform.x < 5)	platform.x = 5;
					else if (platform.x > background_width - platform_width)	platform.x = background_width - platform_width;
				}
			}
			
		}

		/* doodle与台阶的碰撞检测 */
		for (auto& platform : platforms)
		{
			/*
			 * doodle的大小为80X80像素
			 * 经过测试，doodle面朝右边的水平位置像素要比面朝左边大约大10像素(去掉了按键调转方向导致的偏移)
			 * 即要注意面朝方向，不同方向像素计算开始位置不同
			 */
			if (/* doodle最左边向右偏移50像素所在位置大于台阶的最左侧 */
				(double((doodle.is_face_right() ? 50 : 40)) + doodle.get_x() > platform.x) &&
				/* doodle最左边向右偏移20像素所在位置小于台阶最右侧 */
				(double((doodle.is_face_right() ? 20 : 10)) + doodle.get_x() < platform.x + double(platform_width)) &&
				/* doodle最下边向下偏移70像素所在位置大于台阶最低处 */
				(doodle.get_y() + 70 > platform.y) &&
				/* doodle最下边向下偏移70像素所在位置小于台阶最高处 */
				(doodle.get_y() + 70 < platform.y + platform_height) &&
				/* doodle在铅直方向有速度 */
				(doodle.get_dy() > 0)
				)
			{
				doodle.alter_dy(-doodle.get_dy() - 10); /* 铅直方向向上移动速度设置为每帧(大概)10像素 */
			}
		}

		window.draw(s_background);

		/* 根据朝向绘制对应的doodle */
		if(doodle.is_face_right())
		{
			s_doodle_right.setPosition(doodle.get_x(), doodle.get_y());
			window.draw(s_doodle_right);
		}
		else
		{
			s_doodle_left.setPosition(doodle.get_x(), doodle.get_y());
			window.draw(s_doodle_left);
		}

		/* 绘制台阶 */
		for (auto& platform : platforms)
		{
			s_platforms.setPosition(platform.x, platform.y);
			window.draw(s_platforms);
		}

		window.display();
	}

	return 0;
}

