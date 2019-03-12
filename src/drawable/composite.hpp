#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <iostream>
#include <utility>

#include "polygon.hpp"
#include "animated.hpp"
#include "coordinate.hpp"

class Composite : public Drawable
{
private:
	Animated** polygonList;
	int polygonNum;
	int id;

public:
	Composite() : Composite(10, 0) {}

	Composite(int maxAnimatedNum, int id){
		this->polygonNum = 0;
		this->id = id;
		this->polygonList = new Animated*[maxAnimatedNum];
	}

	~Composite() {
		delete[] polygonList;
	}

	char getId() const {
		return this->id;
	}

	void addAnimated(Animated* pol) {
		if (this->polygonNum == 0) {
			this->polygonList[0] =  pol;
		} else {
			int i;
			for (i = this->polygonNum; i > 0; i--) {
				if (this->polygonList[i-1]->getZAxis() >= pol->getZAxis()) {
					this->polygonList[i] = pol;
					break;
				} else {
					this->polygonList[i] = this->polygonList[i-1];
				}
			}
			if (this->polygonList[0]->getZAxis() < pol->getZAxis()){
				this->polygonList[0] = pol;
			}
		}
		this->polygonNum++;
	}

	int getAnimatedNum() {
		return this->polygonNum;
	}

	Animated** getAnimatedList() {
		return this->polygonList;
	}

	void hide() {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->hide();
		}
	}

	Coordinate* getAnchor() {
		int x = 0, y = 0;
		for (int i= 0; i < this->polygonNum; i++) {
			Coordinate* temp = this->polygonList[i]->getAnchor();
			x += temp->getX();
			y += temp->getY();
		}
		x /= this->polygonNum;
		y /= this->polygonNum;
		Coordinate* compAnchor = new Coordinate(x,y);
		return compAnchor;
	}

	void animate() {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->animate();
		}
	}

	void move(int dx, int dy) {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->move(dx, dy);
		}
	}

	void moveWithoutAnchor(int dx, int dy) {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->moveWithoutAnchor(dx, dy);
		}
	}

	void scale(double scaleFactor) {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->scale(scaleFactor);
		}
	}

	void linearScale(double scaleFactor) {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->scale(scaleFactor);
		}
	}

	void rotate(double rotation) {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->rotate(rotation);
		}
	}

	void setAnchor(int x, int y) {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->setAnchor(x, y);
		}
	}

	void draw(IFrameBuffer* fb) {
		FrameBuffer* temp = (FrameBuffer*) fb;
		for (int i= 0; i < this->polygonNum; i++) {
			if (this->polygonList[i]->isOverlapping(new std::pair<Coordinate *, Coordinate *>(new Coordinate(0, 0), new Coordinate(temp->getXRes(), temp->getYRes()))))
				
				this->polygonList[i]->draw(fb);
		}
	}
};


#endif