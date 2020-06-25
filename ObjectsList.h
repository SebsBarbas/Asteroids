#include "Shape.h"
#include "Bullet.h"
class ObjectsList {
private:
	int n; 
	int numbal;
	struct objeto *list_head;
public:
	ObjectsList();
	~ObjectsList();
	void move();
	void draw();
	void add(Shape *s);
	void remove(Shape *s);
	Ship *getShip();
	int collisions(Bullet *bullet, Ship *ship);// A lo mejor no funciona
	void reposition(Ship *ship);
};
