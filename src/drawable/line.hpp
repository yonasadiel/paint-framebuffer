#ifndef LINE
#define LINE

#include "../etc/color.hpp"
#include "../etc/coordinate.hpp"
#include "../framebuffer/framebuffer.hpp"
#include "drawable.hpp"

class Line : public Drawable
{
  public:
	Coordinate *from;
	Coordinate *to;
	color from_color;
	color to_color;

	Line()
	{
		this->from = new Coordinate(0, 0);
		this->to = new Coordinate(0, 0);
		this->from_color = CWHITE;
		this->to_color = CWHITE;
	}

	Line(int from_x, int from_y, int to_x, int to_y) : Line()
	{
		this->setFrom(from_x, from_y);
		this->setTo(to_x, to_y);
	}

	Line(int from_x, int from_y, int to_x, int to_y, color from_color, color to_color) : Line(from_x, from_y, to_x, to_y)
	{
		this->setFrom(from_x, from_y);
		this->setTo(to_x, to_y);
		this->from_color = from_color;
		this->to_color = to_color;
	}

	~Line()
	{
		delete this->from;
		delete this->to;
	}

	void setFrom(int x, int y)
	{
		this->from->setX(x);
		this->from->setY(y);
	}

	void setFrom(int x, int y, color from_color)
	{
		this->from->setX(x);
		this->from->setY(y);
		this->from_color = from_color;
	}

	void setTo(int x, int y)
	{
		this->to->setX(x);
		this->to->setY(y);
	}

	void setTo(int x, int y, color to_color)
	{
		this->to->setX(x);
		this->to->setY(y);
		this->to_color = to_color;
	}

	void moveLine(int dx, int dy)
	{
		this->from->move(dx, dy);
		this->to->move(dx, dy);
	}

	void draw(IFrameBuffer *framebuffer)
	{
		int dx = this->to->getX() - this->from->getX();
		int dy = this->to->getY() - this->from->getY();
		if (abs(dx) <= 0 && abs(dy) <= 0)
		{
			return;
		}

		int shortStart, shortFinish, longStart, longFinish, longSigned, shortSigned, D;
		if (abs(dx) < abs(dy))
		{
			shortStart = this->from->getX();
			shortFinish = this->to->getX();
			shortSigned = (dx == 0 ? 0 : dx / abs(dx));
			longStart = this->from->getY();
			longFinish = this->to->getY();
			longSigned = dy / abs(dy);
			D = 2 * abs(dx) - abs(dy);
		}
		else
		{
			shortStart = this->from->getY();
			shortFinish = this->to->getY();
			shortSigned = (dy == 0 ? 0 : dy / abs(dy));
			longStart = this->from->getX();
			longFinish = this->to->getX();
			longSigned = dx / abs(dx);
			D = 2 * abs(dy) - abs(dx);
		}

		int shortIt = shortStart;

		for (int longIt = longStart; longIt != longFinish; longIt += longSigned)
		{
			Coordinate *coordinate;
			coordinate = (abs(dx) < abs(dy) ? new Coordinate(shortIt, longIt) : new Coordinate(longIt, shortIt));
			color c = colorMid(this->from_color, this->to_color, 1.0 * abs(longIt - longStart) / abs(longFinish - longStart));
			framebuffer->lazyDraw(coordinate, c);
			delete coordinate;
			if (D > 0)
			{
				shortIt += shortSigned;
				D -= 2 * (abs(dx) < abs(dy) ? abs(dy) : abs(dx));
			}
			D += 2 * (abs(dx) < abs(dy) ? abs(dx) : abs(dy));
		}
	}
};

#endif