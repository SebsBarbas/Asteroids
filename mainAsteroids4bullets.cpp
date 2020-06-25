
#include "commonstuff.h"

#define SYSTEM WINDOWS
#if SYSTEM==WINDOWS
#include"glut.h"
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

using namespace std;

// Prototipos de funciones 
void myLogic();


//Callback para teclado y raton
void OnKeyboardDown(unsigned char key, int x, int y);
void OnMouseBtn(int button, int state,int x, int y);
void OnMouseMoveBtn(int x, int y);
void OnSpecKeyboardDown(int key, int x, int y);

//Callbacks de dibujo
void OnDibuja(void);	



// posición y step de la camara
float cam_pos[6]={0, 0, 27};
float camvel =0.25;

int MODE=NONE;

ObjectsList worldobjects;
Ship *theShip=NULL;
Bullet *theBullet[4];
int shotTime=0;
int nShips=3;
int score=0;
int numbal = 0;
// Lo añado para ver si puedo hacer los 4 bullets
int shottime[4] = { 0,0,0,0 };
int cont;

int main(int argc,char* argv[])
{

  //list<Asteroid> LA;
  // Inicializaciones

  //Creacion y definicion de la ventana
  glutInit(&argc, argv);
  glutInitWindowSize(WINX,WINY);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Asteroids GL");

  //Habilitar las luces, la renderizacion y el color de los materiales
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
	
  //definir la proyeccion
  glMatrixMode(GL_PROJECTION);
  gluPerspective( 40.0, WINX/WINY, 0.1, 50);

  // define call backs te GLUT
  glutDisplayFunc(OnDibuja);
  glutKeyboardFunc(OnKeyboardDown);
  glutSpecialFunc(OnSpecKeyboardDown);
  glutMotionFunc(OnMouseMoveBtn);
  glutMouseFunc(OnMouseBtn);
	
  glutIdleFunc(myLogic);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);    
	
  //Para definir el punto de vista
  glMatrixMode(GL_MODELVIEW);	
  
 
  // posicciona el punto de vista 
  gluLookAt(cam_pos[0],cam_pos[1],cam_pos[2],  // posicion del  ojo  
	    0.0, 0.0, 0.0,		        // hacia que punto mira  
	    0.0, 1.0, 0.0);         // vector "UP"  (vertical positivo)


  theShip = worldobjects.getShip();


  // bucle del programa
  glutMainLoop();
  
  return 0;   

  /**************************************************************/}
 
void printdata()
{
  int i,l;
  char buffer[50];
  
  glPushMatrix();

  sprintf(buffer,"Ships: %d       Score: %d",nShips,score);
  l=strlen(buffer); 

  glDisable(GL_LIGHTING);
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos3f(-9, 9,-0.1);
  for( i=0; i < l; i++) 
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]); // Print a character on the screen
    }
  glEnable(GL_LIGHTING);

  glPopMatrix();

}


void myLogic()
{
  int res;
  int cont;
	  worldobjects.getBullets(theBullet);
	  for (cont = 0; cont < 4; cont++) {
		  if (shottime[cont]++ > MAXSHOTTIME){
			  worldobjects.remove((theBullet[cont]));
			  theBullet[cont] = NULL;
			  shottime[cont] = 0;
		  }
	  }
  worldobjects.move();    
  res = worldobjects.collisions(*theBullet, theShip);  
  
  if(res==1)    
    {
      nShips--;
      printf("BOOM!!!...\n");
      
      if(nShips==0)
	{
	  printf("GAME OVER ! Your score: %d\n",score);
	  exit(1);
	}

      Sleep(3);
      theShip->resetpos();
      //worldobjects.reposition(theShip);
    }

  if(res>=2)    
    {
	  worldobjects.getBullets(theBullet);
 	  for (cont = 0; cont < 4; cont++) {
 		  if (theBullet[cont] == NULL) {
			  shottime[cont] = 0;
		  }
	  }
      score += 100*(res-1);
    }
}
 
  /**************************************************************/  /**************************************************************/
  /**************************************************************/  /**************************************************************/
void OnDibuja(void)
{ 
  //Borrado de la pantalla	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  worldobjects.draw();    


  printdata();


  //Al final, cambiar el buffer
  glutSwapBuffers();
  glutPostRedisplay();//se le indica que redibuje la pantalla
  /**************************************************************/}



   


void OnKeyboardDown(unsigned char key, int x, int y)
{
	int cont;
  int mod;

  mod=glutGetModifiers();

  switch(key)
    {
    case 'q':
    case ESC:
      exit(1);
    case ' ':
      //if (mod==GLUT_ACTIVE_ALT) Le he quitado if(!theBullet)
		for (cont = 0; cont < 4; cont++) {
			if (theBullet[cont]==NULL) {
				theBullet[cont] = theShip->fire();
				worldobjects.add(theBullet[cont]);
				break;     
			}
		}
	     break;
    case '-': theShip->thrust(SHIPSPEED); break;
    case ',': theShip->hyperjump(); break;
    case 'w': cam_pos[0]+=camvel; break;
    case 'z': cam_pos[0]-=camvel; break;
    case 'a': cam_pos[1]+=camvel; break;
    case 's': cam_pos[1]-=camvel; break;
    case '>': cam_pos[2]+=camvel; break;
    case '<': cam_pos[2]-=camvel; break;


    }		

}


void OnSpecKeyboardDown(int key, int x, int y)
{ 
  switch(key)
    {

    case GLUT_KEY_DOWN:
      break;
    case GLUT_KEY_UP:
      break;
    case GLUT_KEY_LEFT: theShip->rotate(0,-5,0);
      break;
    case GLUT_KEY_RIGHT: theShip->rotate(0,5,0);
      break;
    case GLUT_KEY_PAGE_DOWN:
      break;
    case GLUT_KEY_PAGE_UP:
      break;
    }		

}


void OnMouseBtn(int button, int state,int x, int y)
{
  if(state==GLUT_DOWN)
    if(button==GLUT_LEFT_BUTTON)
      MODE=TRANS;
    else
      if(button==GLUT_RIGHT_BUTTON)
        MODE=ROT;
      else
        MODE=NONE;
 	
}

 

void  OnMouseMoveBtn  (int x, int y)
{

  if(MODE==TRANS)
    {
      /*
      if(x-px>EPS)
	pos[0] -= vel[0];
      else
	if(px-x>EPS)
	  pos[0] += vel[0];
  
      if(y-py>EPS)
	pos[1] -= vel[1];
      else
	if(py-y>EPS)
	  pos[1] += vel[1];
      */
    }
  else
    {
      /*
      if(x-px>EPS)
	pos[3] -= vel[3];
      else
	if(px-x>EPS)
	  pos[3] += vel[3];
  
      if(y-py>EPS)
	pos[4] -= vel[4];
      else
	if(py-y>EPS)
	  pos[4] += vel[4];
      */
    }

 
 
}
