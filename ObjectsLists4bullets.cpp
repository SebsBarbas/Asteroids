#include "commonstuff.h"

#define SYSTEM WINDOWS
#if SYSTEM==WINDOWS
#include "glut.h"
#else
#include "GLUT.h"
#endif
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Shape.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Ship.h"
#include "ObjectsList4bullets.h"
struct objeto {
	Shape *info;
	struct objeto *next;
};
ObjectsList::ObjectsList() {
	Ship *theShip = new Ship;
	Asteroid *MiAsteroide;
	int i;
	list_head = NULL;
	add(theShip);
	for (i = 0; i <  NUMASTEROIDS; i++) {
		MiAsteroide = new Asteroid(0);
		add(MiAsteroide);
	}
	for (i = 0; i < 4; i++) {
		bullets[i] = NULL;
	}
}

ObjectsList::~ObjectsList() {
	struct objeto *l, *j;
	l = list_head;
	while (l->next) {
		j = l;
		l = l->next;
		delete j->info;
		j = l;
	}
	delete l->info;
	l=NULL;
}

void ObjectsList::move() {
	struct objeto *l;
	l = list_head;
	do {
		(l->info)->move();
		l = l->next;
	}
		while (l);
	}

void ObjectsList::draw() {
	struct objeto *l;
	Asteroid *EsAsteroid;
	Bullet *EsBullet;
	Ship *EsShip;
	l = list_head;
	do {
		EsAsteroid = dynamic_cast<Asteroid *>(l->info);
		if (EsAsteroid != NULL)EsAsteroid->draw();
		else {
			EsBullet = dynamic_cast<Bullet *>(l->info);
			if (EsBullet != NULL) EsBullet->draw();
			else {
				EsShip = dynamic_cast<Ship *>(l->info);
				if (EsShip) EsShip->draw();
			}
		}
		l = l->next;
	} while (l!= NULL);
	
	}

void ObjectsList::add(Shape *s) {
	struct objeto *l, *tmp;
	Ship *Esnave;
	Bullet *Esbala;
	int i, aux=0;
	Esbala = dynamic_cast<Bullet *>(s);
	l = list_head;
	tmp = (struct objeto *)malloc(sizeof(struct objeto *));
	tmp->info = s;
	tmp->next = NULL;
	if (list_head == NULL) {
		list_head = tmp;
	}
	else {
		if (Esnave = dynamic_cast<Ship *>(s)) {
			tmp->next = list_head;
			list_head = tmp;
		}
		else {
			while (l->next)
				l = l->next;
			l->next = tmp;
			tmp = NULL;
		}
	}
	}


void ObjectsList::remove(Shape *s) {
	struct objeto *l, *j;
	int i;
	Ship *Esnave;
	Bullet *Esbala;
	int t = 0;
	l = list_head;
	j = l;
	do {
		if (l->info == s) {
				for (i = 0; i < t-1; i++) {
					j = j->next;
				}
				j->next = l->next;
				delete l->info;
				l = j;
				return;
			}
			l = l->next;
			t++;
	} while (l);
	}
Ship *ObjectsList::getShip(){ 
	Ship *nav;/*El primer elemento de nuestra lista es SIEMPRE la nave, hay que hacer un cast*/
	struct objeto *l;
	l = list_head;
	nav=dynamic_cast<Ship *>(l->info);
	return nav;
}

void ObjectsList::reposition(Ship *ship) {
	float UnaDistanciaConsiderable;
	Asteroid *EsAsteroid;
	struct objeto *l;
	float a[3], c[3] = { 0,0,0 };
	l = list_head;
	l = l->next;
	do {
		EsAsteroid = dynamic_cast<Asteroid *>(l->info);
		if (EsAsteroid) {
			(l->info)->getPos(a); //a guarda la posición del asteroide
			UnaDistanciaConsiderable = ((ship->getSize() + EsAsteroid->getSize())/2);
			if (mydistance(a[0], a[1], c[0], c[1]) <= UnaDistanciaConsiderable) {
				do {
					EsAsteroid->reposition();
					EsAsteroid->getPos(a);
				} while (mydistance(a[0], a[1], c[0], c[1]) <= UnaDistanciaConsiderable);
			}
		}
		l = l->next;
	} while (l);
}

int ObjectsList::collisions(Bullet *theBullet, Ship *ship) {
	int i, t = 0, h,z;
	float distancia, tship, tbullet, ta;
	struct objeto *l, *j;
	Asteroid *EsAsteroid;
	Bullet *Esbala;
	float p[3], a[3], b[3]; /*Para la posicion del ship usamos el vector p, para el asteroide el a y para la bala el b*/
	l = list_head;
	l = l->next;
	j = l;
	ship->getPos(p);
	tship = ship->getSize();
	do {
		if (j) {
			t++;
			j = j->next;
		}
	} while (j);
	j = list_head->next;
	for (i = 0; i < t; i++) {
		if (EsAsteroid = dynamic_cast<Asteroid *>(l->info)) {
			(l->info)->getPos(a);
			distancia = mydistance(p[0], p[1], a[0], a[1]); /*Comparamos la distancia entre ship y asteroide*/
			ta = (l->info)->getSize();
			if (distancia < ((tship + ta) / 2)) {
				ship->resetpos();
				reposition(ship);
				return 1;
			}
		}
		l = l->next;
	}
	j = list_head->next;
	l=j;
	//Ahora vamos a ver el choque de las balas con los asteroides
	for (i = 0; i < t; i++) {
		Esbala = dynamic_cast<Bullet *>(l->info);
		if (Esbala)   {
			tbullet = (l->info)->getSize();
			(l->info)->getPos(b);
			for (z = 0; z < t; z++) {
				EsAsteroid = dynamic_cast<Asteroid *>(j->info);
				if (EsAsteroid) { 
					(j->info)->getPos(a);
					distancia = mydistance(b[0], b[1], a[0], a[1]); /*Comparamos la distancia entre ship y asteroide*/
					ta = (j->info)->getSize();
					if (distancia < (tbullet + ta)) {
						remove(l->info);
    					if (ta > 0.89) h = 3;
						if (ta >=0.59 &&  ta <= 0.61) h = 2;
   						if (ta <= 0.32) h = 1;
 						switch (h) {
						case 3: add((EsAsteroid)->split()); return 3;
			 			case 2: add((EsAsteroid)->split());return 2;
						case 1: remove(EsAsteroid); return 4;
						}

					}
				}
				j = j->next;

			}
			j = list_head->next;
		}
		l = l->next;
	}
	return 0;
}
void ObjectsList::getBullets(Bullet *(*tb)) {
	tb = bullets;
	
}

