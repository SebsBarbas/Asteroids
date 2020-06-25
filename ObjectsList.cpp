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
	Ship *theShip = new Ship;
	Asteroid *MiAsteroide;
	int i;
	list_head = NULL;
	add(theShip);
	for (i = 0; i <  NUMASTEROIDS; i++) {
		MiAsteroide = new Asteroid(0);
		add(MiAsteroide);
	}
	numbal = 0;
}

ObjectsList::~ObjectsList() {
	struct objeto *l, *j;
	l = list_head;
	while (l->next) {
		j = l;
		l = l->next;
		delete j->info;
		free(j);//Por que no es free(j)
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
			if ((Esbala!=NULL && (numbal < 4)) || Esbala==NULL) {
				while (l->next)
					l = l->next;
				if (Esbala)numbal++;
				l->next = tmp;
				tmp = NULL;
			}
			}
		}
	}


void ObjectsList::remove(Shape *s) {
	struct objeto *l, *j;
	Ship *Esnave;
	Bullet *Esbala;
	Esbala = dynamic_cast<Bullet *>(s);
	if(Esbala!=NULL) numbal--;
	int t = 0,i;
	l = list_head;
	j = l;
	do {
		if (l->info == s) {
			if (Esnave = dynamic_cast<Ship *>(l->info)) {
				l = l->next;
				list_head = l;
				delete j->info;
				j=NULL;
				j = l;
				return;
			}
				for (i = 0; i < t-1; i++) {
					j = j->next;
				}
				j->next = l->next;
				delete l->info; //Por que no vale free(l);
				l=NULL;
				l = j;
				return;
			}
			l = l->next;
			t++;
		}
	while (l);
	}
Ship *ObjectsList::getShip(){ 
	Ship *nav;/*El primer elemento de nuestra lista es SIEMPRE la nave, hay que hacer un cast*/
	struct objeto *l;
	l = list_head;//Antes había un cast
	nav=dynamic_cast<Ship *>(l->info);
	return nav; /*Podríamos mirar si nos podemos ahorrar el puntero l*/
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

/*void ObjectsList::reposition(Ship *ship) {
	int i,t;
	int UnaDistanciaConsiderable;
	Asteroid *EsAsteroid;
	struct objeto *l, *j;
	float a[3], c[3] = { 0,0,0 }; /*Para la posicion del ship usamos el vector c , para el asteroide el a, para la bala el b y para el centro el vector c*/
	/*t=0;
	l = list_head;
	l = l->next;
	j = l;
	do {
		j = j->next;
		t++;
	} while (j->next != NULL);
	for (i = 0; i <= t; i++) { //recorre todos los asteroides
		EsAsteroid = dynamic_cast<Asteroid *>(l->info);
		if (EsAsteroid) {
			(l->info)->getPos(a); //a guarda la posición del asteroide
			UnaDistanciaConsiderable = ((ship->getSize() / 0.3) / 2) + ((EsAsteroid->getSize() / 0.3) / 2);
			if (mydistance(a[0], a[1], c[0], c[1]) <= UnaDistanciaConsiderable)
				do {
					EsAsteroid->reposition();
					(l->info)->getPos(a);
				}
					while (mydistance(a[0], a[1], c[0], c[1]) <= UnaDistanciaConsiderable);
		}
	}
}*/

int ObjectsList::collisions(Bullet *theBullet, Ship *ship) {
	int i, t = 0, h;
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
		t++;
		j = j->next;
	} while (j->next != NULL);
	j = list_head->next;
	for (i = 0; i <= t; i++) {
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
	for (i = 0; i <= t; i++) {
		Esbala = dynamic_cast<Bullet *>(l->info);
		if (Esbala)   {
			tbullet = (l->info)->getSize();
			(l->info)->getPos(b);
			for (i = 0; i < t; i++) { // Antes tenia un =
				EsAsteroid = dynamic_cast<Asteroid *>(j->info);
				if (EsAsteroid) { 
					(j->info)->getPos(a);
					distancia = mydistance(b[0], b[1], a[0], a[1]); /*Comparamos la distancia entre ship y asteroide*/
					ta = (j->info)->getSize();
					if (distancia < (tbullet + ta)) {//Le he quitado el /2 a ver si funciona mejor
						remove(l->info);
						if (ta >= 0.9) h = 3;
						if (ta >=0.59 ||   ta <= 0.6) h = 2;
						if (ta <= 0.3) h = 1;
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


			/*distancia = mydistance(b[0], b[1], a[0], a[1]);
			if (distancia < (tbullet + ta)/2) {
				remove(bullet);
				if (ta = 0.9) h = 3;
				if (ta = 0.6) h = 2;
				if (ta = 0.3) h = 1;
				switch (h) {
				case 3: add(EsAsteroid->split()); (l->info) = EsAsteroid->split(); return 3;
				case 2: add(EsAsteroid->split()); (l->info) = EsAsteroid->split(); return 2;
				case 1: remove(l->info); return 4; break;
				};
			}
			}
		}

	}
	else {
		for (i = 0; i < t; i++) { /*Antes era SOLO <*/
			/*EsAsteroid = dynamic_cast<Asteroid *>(l->info);
			if (EsAsteroid) {
				(l->info)->getPos(a);
				distancia = mydistance(p[1], p[2], a[1], a[2]); /*Comparamos la distancia entre ship y asteroide*/
				/*ta = EsAsteroid->getSize();
				if (distancia < (tship + ta)/2) {
					ship->resetpos();
					reposition(ship); //Añadido el 18/05/2017
					return 1;
				}
			}
		}*/
/*	return 0;
	} */
