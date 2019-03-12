#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <iostream>
#include <fstream>
#include <vector>

#include "../etc/coordinate.hpp"
#include "drawable.hpp"
#include "polygon.hpp"

class Composite : public Drawable {
private:
	std::vector<Polygon*>* polygons;
	char id;

public:
	Composite(std::string filename, color c, char id = 0) {
        std::ifstream f(filename);
		int polygonNum;

		this->polygons = new std::vector<Polygon*>();

		f >> polygonNum;
		for (int i = 0; i < polygonNum; i++) {
			int pointNum;
			int x, y;
			std::vector<Coordinate*>* points = new std::vector<Coordinate*>();

			f >> pointNum;
			for (int j = 0; j < pointNum; j++) {
				f >> x >> y;
				points->push_back(new Coordinate(x, y));
			}
			this->polygons->push_back(new Polygon(points, c, id));
		}
        f.close();
	}

	~Composite() {
		for (int i = 0; i < polygons->size(); i++)
			delete polygons->at(i);
		delete polygons;
	}

	char getId() const {
		return this->id;
	}

	Coordinate* getAnchor() {
		int x = 0, y = 0;
		for (int i= 0; i < this->polygons->size(); i++) {
			Coordinate* temp = this->polygons->at(i)->getAnchor();
			x += temp->getX();
			y += temp->getY();
		}
		x /= this->polygons->size();
		y /= this->polygons->size();
		Coordinate* compAnchor = new Coordinate(x,y);
		return compAnchor;
	}

	void animate() {
		for (Polygon* polygon : *this->polygons)
			polygon->animate();
	}

	void move(int dx, int dy) {
		for (Polygon* polygon : *this->polygons)
			polygon->move(dx, dy);
	}

	void moveWithoutAnchor(int dx, int dy) {
		for (Polygon* polygon : *this->polygons)
			polygon->moveWithoutAnchor(dx, dy);
	}

	void scale(double scaleFactor) {
		for (Polygon* polygon : *this->polygons)
			polygon->scale(scaleFactor);
	}

	void linearScale(double scaleFactor) {
		for (Polygon* polygon : *this->polygons)
			polygon->scale(scaleFactor);
	}

	void rotate(double rotation) {
		for (Polygon* polygon : *this->polygons)
			polygon->rotate(rotation);
	}

	void setAnchor(int x, int y) {
		for (Polygon* polygon : *this->polygons)
			polygon->setAnchor(x, y);
	}

	void draw(IFrameBuffer* framebuffer) {
		for (Polygon* polygon : *this->polygons)
			polygon->draw((FrameBuffer*) framebuffer);
	}
};


#endif