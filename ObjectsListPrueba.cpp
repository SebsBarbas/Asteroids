#include "commonstuff.h"


#include "glut.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "Shape.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Ship.h"
#include "ObjectsList.h"
struct objeto {
	Shape *info;
	struct objeto *next;
};
ObjectsList::ObjectsList() {
	Ship theShip;
	Asteroid Asteroide1, Asteroide2, Asteroide3, Asteroide4, Asteroide5;
	int i;
	struct objeto *tmp, *l;
	tmp = (struct objeto *)malloc(sizeof(struct objeto));
	tmp->info = &theShip;
	tmp->next = NULL;
	l = tmp;
	list_head =tmp;
	tmp = NULL;
		tmp= (struct objeto *)malloc(sizeof(struct objeto));
		*(tmp->info) = Asteroide1;
		tmp->next = NULL;
		l->next = tmp;
		l = tmp;
		tmp = NULL;
		tmp = (struct objeto *)malloc(sizeof(struct objeto));
		*(tmp->info) = Asteroide2;
		tmp->next = NULL;
		l->next = tmp;
		l = tmp;
		tmp = NULL;
		tmp = (struct objeto *)malloc(sizeof(struct objeto));
		*(tmp->info) = Asteroide3;
		tmp->next = NULL;
		l->next = tmp;
		l = tmp;
		tmp = NULL;
		tmp = (struct objeto *)malloc(sizeof(struct objeto));
		*(tmp->info) = Asteroide4;
		tmp->next = NULL;
		l->next = tmp;
		l = tmp;
		tmp = NULL;
		tmp = (struct objeto *)malloc(sizeof(struct objeto));
		*(tmp->info) = Asteroide5;
		tmp->next = NULL;
		l->next = tmp;
		l = tmp;
		tmp = NULL;
	}

}

void ObjectsList::move() {
	struct objeto *l;
	l = list_head;
	while (l->next != NULL) {
		*(l->info).move();
		l = l->next;
	}
	}

void ObjectsList::draw() {
	struct objeto *l;
	l = list_head;
	while (l->next != NULL) {
		*(l->info).draw();
		l = l->next;
	}
	
	}

void ObjectsList::add(Shape *s) {
	struct objeto *l, *tmp;
	l = list_head;
	tmp = (struct objeto *)malloc(sizeof(struct objeto *));
	tmp->info = *s;
	tmp->next = NULL;
	while(l->next)
		l = l->next;
	l->next = tmp;
}

Ship *ObjectsList::getShip(){ 
	Ship *nav; /*El primer elemento de nuestra lista es SIEMPRE la nave*/
	nav=((list_head->info));
	return nav; /*Podríamos mirar si nos podemos ahorrar el puntero l*/
}

void ObjectsList::reposition(Ship *ship) {
	int i;
	int UnaDistanciaConsiderable;
	struct objeto *l, *j;
	float p[3], a[3], b[3], c[3] = { 0,0,0 }; /*Para la posicion del ship usamos el vector p, para el asteroide el a, para la bala el b y para el centro el vector c*/
	n = 0;

	l = list_head;
	l = l->next;
	j = l;
	while (j->next != NULL) {
		n++;
		j = j->next;
	}

	for (i = 0; i < n; i++) { //recorre todos los asteroides
		*(l->info).getPos(a); //a guarda la posicion del asteroide
		UnaDistanciaConsiderable = (((*(list_head->info).getSize()) / 0.3) / 2) + ((l->info.getSize / 0.3) / 2);
		if (mydistance(a[0], a[1], c[0], c[1]) <= UnaDistanciaConsiderable)
			*(l->info).reposition();

	}
}


int ObjectsList::collisions(Bullet *bullet, Ship *ship) {
	int i, n = 0, ta;
	float distancia, tship, tbullet;
	struct objeto *l, *j;
	float p[3], a[3], b[3]; /*Para la posicion del ship usamos el vector p, para el asteroide el a y para la bala el b*/
	l = list_head;
	l = l->next;
	j = l;
	ship->getPos(p);
	tship = ship->getSize();
	while (j->next != NULL) {
		n++;
		j = j->next;
	}
	if (bullet) {
		bullet->getPos(b);
		tbullet = bullet->getSize();
		for (i = 0; i < n; i++) {
			*(l->info).getPos(a);
			distancia = mydistance(p[0], p[1], a[0], a[1]); /*Comparamos la distancia entre ship y asteroide*/
			ta = *(l->info).getSize();
			if (distancia < tship + ta) {
				remove(ship);
				return 1;
			}
			distancia = mydistance(b[0], b[1], a[0], a[1]);
			if (distancia < tbullet + ta) {
				remove(bullet);
				ta = ta / 0.3;
				switch (ta) {
				case 3: *(l->info).split(); return 3; break;
				case 2: *(l->info).split(); return 2; break;
				case 1: remove((l->info)); return 4; break;
				};

			}
		}

	}
	else {
		for (i = 0; i <= n; i++) { /*Antes era SOLO <*/
			*(l->info).getPos(a);
			distancia = mydistance(p[1], p[2], a[1], a[2]); /*Comparamos la distancia entre ship y asteroide*/
			ta = *(l->info).getSize();
			if (distancia < tship + ta) {
				remove(ship);
				return 1;
			}

		}

	}
}