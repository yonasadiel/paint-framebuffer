#ifndef LINE
#define LINE

#include "../etc/color.hpp"
#include "../etc/coordinate.hpp"
#include "../framebuffer/framebuffer.hpp"
#include "../etc/utils.hpp"
#include "drawable.hpp"

class Line : public Drawable
{
  public:
	Coordinate *from;
	Coordinate *to;
	color c;

	Line()
	{
		this->from = new Coordinate(0, 0);
		this->to = new Coordinate(0, 0);
		this->c = CWHITE;
	}

	Line(int from_x, int from_y, int to_x, int to_y) : Line()
	{
		this->from = new Coordinate(from_x, from_y);
		this->to = new Coordinate(to_x, to_y);
	}

	Line(int from_x, int from_y, int to_x, int to_y, color c) : Line(from_x, from_y, to_x, to_y)
	{
		this->c = c;
	}

	~Line()
	{
		delete this->from;
		delete this->to;
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

		int yStart, yEnd, xStart, xEnd, xSigned, D, yD, xD;
		if (this->from->getY() < this->to->getY()) {
			xStart = this->from->getX();
			yStart = this->from->getY();
			xEnd = this->to->getX();
			yEnd = this->to->getY();
		} else {
			xStart = this->to->getX();
			yStart = this->to->getY();
			xEnd = this->from->getX();
			yEnd = this->from->getY();
		}
		yD = abs(dy);
		xD = abs(dx);
		xSigned = xD == 0 ? 0 : (xStart < xEnd ? 1 : -1);
		D = -xD;

		int xIt = xStart;

		for (int yIt = yStart; ; yIt++) {
			do
			{
				Coordinate *coordinate;
				coordinate = new Coordinate(xIt, yIt);
				framebuffer->lazyDraw(coordinate, this->c);
				delete coordinate;
				if (D <= 0) {
					D += 2 * yD;
					xIt += xSigned;
				}
			} while (D <= 0 && xIt != xEnd);
			D -= 2 * xD;
			if (yIt == yEnd) break;
		}
	}
};

#endif