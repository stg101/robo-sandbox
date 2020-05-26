#ifndef BODY_H
#define BODY_H

#include "box2d/box2d.h"

class Body
{
public:
	Body(){};
	virtual ~Body(){};

	b2Body *m_body;

	virtual void createBody(b2World *world){};
	virtual void setState(int controlState, float speedRatio){};

protected:
};

#endif
