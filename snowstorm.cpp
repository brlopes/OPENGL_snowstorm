//=========================================================//
//=========================================================//
//
// Supporting library
// http://www.wikihow.com/Install-Mesa-%28OpenGL%29-on-Linux-Mint
//
//=========================================================//
//=========================================================//
// WINDOW ONLY
//#include <windows.h>					// included in all Windows apps
//#include <winuser.h>          // Windows constants
// for windows GL is in small letters
//#include <gl/gl.h>						// OpenGL include
//#include <gl/glu.h>
//#include <GL/openglut.h>
// for linux GL is in capital letters
//=========================================================//
//=========================================================//

// LINUX or UNIX ONLY
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
//=========================================================//
//=========================================================//
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//=========================================================//
//=========================================================//
// person position in the environment
void move_camera(int specialKey,char normalKey);
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 0.0;
GLfloat   change_collor = 1.0;
float rad =0;
const float DEFAULT_SPEED   = 0.4f;
//=========================================================//
//=========================================================//
GLvoid  DrawGround();

GLvoid  DrawNormalObjects(GLfloat rotation);
GLvoid  DrawWireframeObjects(GLfloat rotation);
GLvoid  DrawFlatshadedObjects(GLfloat rotation);
//=========================================================//
//=========================================================//
// Collision detection
GLfloat p1_radius = 0.3f;
GLfloat p2_radius = 0.3f;
GLfloat p1_x = -2.0f;
GLfloat p2_x = 2.0f;
GLfloat p1_y = 2.0f;
GLfloat p2_y = 2.0f;
GLfloat p1_z = 5.0f;
GLfloat p2_z = 5.0f;
GLfloat change_direction = 1.0;
//=========================================================//
//=========================================================//
// quadric objects
void init_dados(void);
void setup_sceneEffects(void);
GLUquadricObj *g_normalObject     = NULL;
GLUquadricObj *g_wireframeObject  = NULL;
GLUquadricObj *g_flatshadedObject = NULL;
void cleanUP_data(void);

// windwheel
float rotateBase_degrees=0;
float wheelRotateFactor=1.0;
float rotatePointer_degrees=0;


const int   WORLD_SIZE = 250;
//=========================================================//
//=========================================================//
static void text_onScreen(int row, int col, const char *fmt, ...);
//=========================================================//
//=========================================================//
typedef struct
{
    const char * const name;
    void (*solid) (void);
    void (*wire)  (void);
} entry;
//=========================================================//
//=========================================================//
static void text_onScreen (int row, int col, const char *fmt, ...)
{
    static char buf[256];
    int viewport[4];
    //void *font = GLUT_BITMAP_9_BY_15;
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);

    glGetIntegerv(GL_VIEWPORT,viewport);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0,viewport[2],0,viewport[3],-1,1);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
//=========================================================//
//=========================================================//
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( ar > .5 )
        glFrustum( -ar, ar, -1.0, 1.0, 2.0, 100.0 );
    else
        glFrustum( -1.0, 1.0, -1/ar, 1/ar, 2.0, 100.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
//=========================================================//
//=========================================================//
GLvoid DrawGround()
{
	//draw white snow ground
	glColor3f(1.0f, 1.01f, 1.0f);
	glNormal3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	    glVertex3f(-1500.0f, -8.0f, -1500.0f);
	    glVertex3f(-1500.0f, -8.0f,  1500.0f);
	    glVertex3f( 1500.0f, -8.0f,  1500.0f);
	    glVertex3f( 1500.0f, -8.0f, -1500.0f);
	glEnd();

	//draw lines for depth
//	glColor3f(0, 102.7f, 204.0f);
//	glBegin(GL_LINES);
//		for (int x = -WORLD_SIZE; x < WORLD_SIZE; x += 6)
//		{
//		  glVertex3i(x, 0, -WORLD_SIZE);
//		  glVertex3i(x, 0, WORLD_SIZE);
//		}
//		for (int z = -WORLD_SIZE; z < WORLD_SIZE; z += 6)
//		{
//		  glVertex3i(-WORLD_SIZE, 0, z);
//		  glVertex3i(WORLD_SIZE, 0, z);
//		}
//	glEnd();

	glDisable(GL_BLEND);
}

GLvoid drawCollision()
{   GLfloat alphaTransparency = 0.5;
	GLfloat distance;

	// move forward and backward
	p1_x+=0.05f*change_direction;
	p2_x-=0.05f*change_direction;

	if(p1_x > 2){
		change_direction=1.0f-2.0f;
	}
	if(p1_x < -2){
		change_direction=1.0f;
	}
	// check-collision
	distance = sqrt(( (p1_x - p2_x) * (p1_x - p2_x) )
			         + ((p1_y - p2_y) * (p1_y - p2_y))
			         + ((p1_z - p2_z) * (p1_z - p2_z)));
	if (distance <= p2_radius + p1_radius){
	  // Red :: collision
	  change_collor = 0.0;
	}else{
	  // Yellow :: no collision
	  change_collor = 1.0;
	}
    // enable blending for transparent sphere
    glEnable(GL_BLEND);     // Turn Blending On
    glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  // first sphere collides against the other
		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(1.0f,change_collor,0.0f,alphaTransparency);
			glTranslatef(p1_x, p1_y, p1_z);
			gluSphere(g_normalObject, p1_radius, 16, 10);
		  glPopMatrix();
		  // second sphere collides against the first
		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor4f(1.0f,change_collor,0.0f,alphaTransparency);
			glTranslatef(p2_x, p2_y, p2_z);
			gluSphere(g_normalObject, p2_radius, 16, 10);
		  glPopMatrix();

	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}
//=========================================================//
//=========================================================//
GLvoid drawCone(void)
{
	GLfloat alphaTransparency = 0.5;
	// a transparency example: green cylinder
	// enable blending for transparent cylinder
	glEnable(GL_BLEND);     // Turn Blending On
	glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		  glPushMatrix();
			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
			glColor3f(1,0.6,0);
			glTranslatef(0.0, 0.4, 0);
			glRotatef(-90, 1.0, 0.0, 0.0);
			gluCylinder(g_normalObject, 0.15, 0.0, 0.4, 32, 8);
		  glPopMatrix();


//		  glPushMatrix();
//			glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
//			glColor3f(0,0,0);
//			glTranslatef(0.0, 0.4, 2);
//			glRotatef(-90, 1.0, 0.0, 0.0);
//			gluCylinder(g_normalObject, 0.15, 0.15, 0.5, 32, 8);
//		  glPopMatrix();





	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}

// func for generating random float for snow pos
float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
	//return 1.0;
}

GLvoid drawSnow(GLfloat snowloc)
{
	GLfloat alphaTransparency = 0.6;
	int max_snow = 10;
	// enable blending for transparent cylinder
	glEnable(GL_BLEND);     // Turn Blending On
	glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	  //for (float x = 0.0; x > max_snow; x += 1)
	  //{
		  glPushMatrix();
		  	  glColor4f(1.0f,1.0f,1.0f,alphaTransparency);
		  	  //glColor3f(1.0f, 1.0f, 1.0f);
			  glTranslatef(RandomFloat(-20.0, 20.0), snowloc + rand() % 70, RandomFloat(-20.0, 20.0));
			  glutSolidSphere(0.03, 6, 6);
		  glPopMatrix();
	  //}

		  //glPushMatrix();
			//glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
		//	glColor4f(0.0f,1.0f,0.0f,alphaTransparency);
			//glTranslatef(-5.0, 0.0, -5.0);
			//glRotatef(-90, 1.0, 0.0, 0.0);
			//gluCylinder(g_normalObject, 1.0, 0.0, 3.0, 32, 4);
		  //glPopMatrix();

	glDisable(GL_BLEND);        // Turn Blending Off
	glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
}
//=========================================================//
void drawColorFan(GLfloat radius, int sections)
{
	//DRAWS A WHEEL WITH RAINBOW COLORS FOR SECTIONS

	//define light so rotation doesn't change the matrix
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    glBegin(GL_TRIANGLE_FAN);
        int i;
        glVertex2f(0.0, 0.0);
        for(i = 0; i <= sections;i++) {

        	//circle-like triangle fan
            glVertex2f(radius*cos(i*2.0 * 3.14159 / sections),
                       radius*sin(i*2.0 * 3.14159 / sections));

            //set color sections
            switch (i%6)
            {
            case 0:
                glColor3f(1.0f, 0.0f, 0.0f);
                break;
            case 1:
                glColor3f(1.0f, 0.5f, 0.0f);
                break;
            case 2:
                glColor3f(1.0f, 1.0f, 0.0f);
                break;
            case 3:
                glColor3f(0.0f, 1.0f, 0.0f);
                break;
            case 4:
                glColor3f(0.0f, 0.0f, 1.0f);
                break;
            case 5:
                glColor3f(1.0f, 0.0f, 1.0f);
                break;
            }
        }

    glEnd();
}

//=========================================================//
GLvoid DrawNormalObjects(GLfloat rotation, GLfloat snowloc, GLfloat snowloc2)
{


  // save the existing color properties
  glPushAttrib(GL_CURRENT_BIT);

  //drawCollision();
  //drawCone();
  //drawTransparentCylinder();

  // start snow
  // use snowloc for changing direction
  // 200 * 2 snowflakes
  int max_snow = 1000;

  for (int n=max_snow; n>0; n--) {
    drawSnow(snowloc);
  }

  for (int n=max_snow; n>0; n--) {
	  drawSnow(snowloc2);
  }
  // make sure the random color values we get are the same every time
  srand(200);


  // tree #1
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3ub(102,0,0);
    glTranslatef(2.0, -2.0, -14.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.5, 0.5, 4.0, 32, 4);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 4.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);
  glPopMatrix();

  // tree #2
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3ub(0,51,0);
    glTranslatef(-5.0, -2.0, -10.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.5, 0.5, 4.0, 32, 4);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 4.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);
  glPopMatrix();

  // tree #3
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3ub(102,51,0);
    glTranslatef(-10.0, -2.0, -8.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.5, 0.5, 4.0, 32, 4);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 4.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);
  glPopMatrix();

  // tree #4
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3ub(102,51,0);
    glTranslatef(6.0, -2.0, -11.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.5, 0.5, 4.0, 32, 4);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 4.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);

    glColor3ub(0,128,0);
    glTranslatef(0.0, 0.0, 1.0);
    glRotatef(0, 0.0, 1.0, 0.0);
    gluCylinder(g_normalObject, 2.0, 0.0, 6.0, 32, 2);
  glPopMatrix();

  glPushMatrix();
    //STEM cylinder
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0,1,1);
    glTranslatef(20, -5, -18);
    glRotatef(-90, 1.0, 0.0, 0);
    glRotatef(rotateBase_degrees, 0.0, 0.0, 1.0);
    gluCylinder(g_flatshadedObject, 0.5, 0.5, 10.0, 32, 4);

    //pipe on disks
    glTranslatef(0, 0, 9);
    glRotatef(-270, 1, 0, 0);
    gluCylinder(g_flatshadedObject, 0.4, 0.4, 4.0, 32, 4);

    //large windwheel
    glColor3f(1,0,0);
    glTranslatef(0,0,1);
    glRotatef(rotation * 3.0f * wheelRotateFactor, 0.0, 0.0, 1.0);
    drawColorFan(4, 36); // radius is 5, 36 is num triangles

    //medium wheel
    glColor3f(0,1,0);
    glTranslatef(0,0,1);
    glRotatef(rotation * -3.50f * wheelRotateFactor, 0.0, 0.0, 1.0);
    drawColorFan(3, 36);

    //small wheel
    glTranslatef(0,0,1);
    glRotatef(rotation * 0.25f * wheelRotateFactor, 0.0, 0.0, 1.0);
    drawColorFan(2, 36);
  glPopMatrix();


  // snowman
  glPushMatrix();

  	//belly
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(1,0.3,1);
    glTranslatef(0.0f, -1.0f, -6.0f);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    gluSphere(g_flatshadedObject, 1.0f, 16, 16);

    //torso
    glColor3f(1,0.4,1);
    glTranslatef(0, 0, 1);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    gluSphere(g_flatshadedObject, 0.75f, 16, 16);

    //head
    glColor3f(1,0.5,1);
    glTranslatef(0, 1, 0);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    gluSphere(g_flatshadedObject, 0.5f, 16, 16);

    // nose
    drawCone();

  //body details
  glPushMatrix();
    //eyes front:
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
	glColor3f(0,0,0);
	glTranslatef(-0.2, 0.45, -0.2);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluDisk(g_normalObject, 0.01, 0.1, 32, 4); // iris1
	//back:
	glTranslatef(0, 0, -0.1);
    glRotatef(0, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.1, 0.1, 0.1, 32, 4);

    //front:
	glColor4f(0,0,0,0.5);
	glTranslatef(0.4, 0, 0.1);
	gluDisk(g_normalObject, 0.01, 0.1, 32, 4); // iris2
	//back:
	glTranslatef(0, 0, -0.1);
    glRotatef(0, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.1, 0.1, 0.1, 32, 4);
  glPopMatrix();


  glPushMatrix();
    //buttons
  	//front:
	glColor4f(1,0,0,0.5);
	glTranslatef(0, 0.8, 1);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluDisk(g_normalObject, 0.01, 0.1, 32, 4); // iris2
	//back:
	glTranslatef(0, 0, -0.1);
    glRotatef(0, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.1, 0.1, 0.1, 32, 4);

	glColor4f(1,0,0,0.5);
	glTranslatef(0, 0.4, 0);
	gluDisk(g_normalObject, 0.01, 0.1, 32, 4); // iris2
	//back:
	glTranslatef(0, 0, -0.1);
    glRotatef(0, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.1, 0.1, 0.1, 32, 4);
  glPopMatrix();
glPopMatrix();
//end of snowman


  //hat disk:
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0,0,1);
    glTranslatef(0.0, 1.4, -6.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluDisk(g_normalObject, 0.1, 0.5, 32, 4); // note inner
  glPopMatrix();

  //hat top:
  glPushMatrix();
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0,0,1);
    glTranslatef(0.0, 1.4, -6.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(g_normalObject, 0.3, 0.3, 0.4, 32, 4);
  glPopMatrix();



  glPopAttrib();
}


//=========================================================//

static void display(void)
{
    update_camera();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);

    // position the light
    GLfloat pos[4] = { 5.0, 5.0, 5.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    // rotation is used for animation
    static GLfloat rotation = 0.0;
    static GLfloat snowloc = 5.0;
    static GLfloat snowloc2 = snowloc + 50.0;
    // it's increased by one every frame
    rotation += 1.0;
    snowloc -= 0.1;
    snowloc2 -= 0.1;
    // and ranges between 0 and 360
    if (rotation > 360.0)
    rotation = 0.0;

    if (snowloc < -50.0)
    snowloc = 60.0;

    if (snowloc2 < -80.0)
    snowloc2 = snowloc + 50.0;
    // draw all of our objects in their normal position
    DrawNormalObjects(rotation, snowloc, snowloc2);


    //glDisable(GL_LIGHTING);
    glColor3d(1,1,1);

    DrawGround();

    glutSwapBuffers();
}
//=========================================================//
//=========================================================//
static void keyboard(unsigned char key, int x, int y)
{ int number=-1;

    move_camera(number,key);

    switch (key)
       {
          case 'l':
              rotateBase_degrees--;
              break;
          case 'L':
              rotateBase_degrees--;
              break;
          case 'r':
              rotateBase_degrees++;
              break;
          case 'R':
              rotateBase_degrees++;
              break;
          case '1':
              wheelRotateFactor=1.0;
              break;
          case '2':
              wheelRotateFactor=2.0;
              break;
          case '3':
              wheelRotateFactor=3.0;
              break;
          default:
              break;
       }


    glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void move_camera(int specialKEY,char normalKEY)
{
    // keyboard :: normal keys
    switch(normalKEY)
    {   // looking up
        case 'A':
        case 'a':    g_elevationAngle += 2.0; break;
       // looking down
        case 'Z':
        case 'z':g_elevationAngle -= 2.0;  break;

        default:
        {    break;
        }
    }

    // special :: special keys
    switch(specialKEY)
    {   // camera setup
        // check if it is moving the view to look left
        case GLUT_KEY_LEFT:
        {
            g_viewAngle -= 2.0;
            // calculate camera rotation angle radians
            rad =  float(3.14159 * g_viewAngle / 180.0f);
            break;
        }
        // check if it is moving the view to look right
        case GLUT_KEY_RIGHT:
        {
             g_viewAngle += 2.0;
            // calculate camera rotation angle radians
            rad =  float(3.14159 * g_viewAngle / 180.0f);
            break;
        }
        // pressing keys Up/Down, update coordinates "x" and "z"
        // based on speed and angle of view.
        case GLUT_KEY_UP:
        {
            g_playerPos[2] += sin(rad) * DEFAULT_SPEED;
            g_playerPos[0] += cos(rad) * DEFAULT_SPEED;
            break;
        }
        case GLUT_KEY_DOWN:
        {
            g_playerPos[2] -= sin(rad) * DEFAULT_SPEED;
            g_playerPos[0] -= cos(rad) * DEFAULT_SPEED;
            break;
        }
        default:
        {   break;
        }
    }
}
//=========================================================//
//=========================================================//
static void special(int key, int x, int y)
{   char letter=' ';

    move_camera(key,letter);

    glutPostRedisplay();
}

//=========================================================//
//=========================================================//
static void idle(void)
{
    glutPostRedisplay();
}
//=========================================================//
//=========================================================//
void update_camera()
{
// don't allow the player to wander past the "edge of the world"
    if (g_playerPos[0] < -(WORLD_SIZE-50))
    g_playerPos[0] = -(WORLD_SIZE-50);
    if (g_playerPos[0] > (WORLD_SIZE-50))
    g_playerPos[0] = (WORLD_SIZE-50);
    if (g_playerPos[2] < -(WORLD_SIZE-50))
    g_playerPos[2] = -(WORLD_SIZE-50);
    if (g_playerPos[2] > (WORLD_SIZE-50))
    g_playerPos[2] = (WORLD_SIZE-50);

  // calculate the player's angle of rotation in radians
    float rad =  float(3.14159 * g_viewAngle / 180.0f);
    // use the players view angle to correctly set up the view matrix
    g_lookAt[0] = float(g_playerPos[0] + 100*cos(rad));
    g_lookAt[2] = float(g_playerPos[2] + 100*sin(rad));

    rad = float (3.13149 * g_elevationAngle / 180.0f);

    g_lookAt[1] = float (g_playerPos[1] + 100 * sin(rad));

    // clear the modelview matrix
    glLoadIdentity();

    // setup the view matrix
    gluLookAt(g_playerPos[0], g_playerPos[1], g_playerPos[2],
              g_lookAt[0],    g_lookAt[1],    g_lookAt[2],
              0.0,            1.0,            0.0);
}
//=========================================================//
//=========================================================//
void setup_sceneEffects(void)
{
  // enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // enable using glColor to change material properties
  // we'll use the default glColorMaterial setting (ambient and diffuse)
  glEnable(GL_COLOR_MATERIAL);

  // set the default blending function
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // set up the fog parameters for reflections
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_START, -100.0);
  glFogf(GL_FOG_END, 100.0);

  // enable line anti-aliasing and make the lines slightly bigger than default
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(1.5f);
}
//=========================================================//
//=========================================================//
void init_dados(void)
{
  setup_sceneEffects();
  // create a normal quadric (uses default settings)
  g_normalObject = gluNewQuadric();

  g_wireframeObject = gluNewQuadric();
  gluQuadricDrawStyle(g_wireframeObject, GLU_LINE);

  // create an object that uses flat shading
  g_flatshadedObject = gluNewQuadric();
  gluQuadricNormals(g_flatshadedObject, GLU_FLAT);

}
//=========================================================//
//=========================================================//
void cleanUP_data(void)
{  // delete all quadratic objects
  if (g_normalObject)
    gluDeleteQuadric(g_normalObject);
  if (g_wireframeObject)
    gluDeleteQuadric(g_wireframeObject);
  if (g_flatshadedObject)
    gluDeleteQuadric(g_flatshadedObject);
}
//=========================================================//
//=========================================================//
int main(int argc, char *argv[])
{
    glutInitWindowSize(640,480);
    glutInitWindowPosition(40,40);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    init_dados();

    glutCreateWindow("Snow storm");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutIdleFunc(idle);

    // environment background color
    //glClearColor(1,1,1,0);//(1,1,1,1);
    // deepth efect to objects
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // light and material in the environment
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();
    cleanUP_data();

    return EXIT_SUCCESS;
}
//=========================================================//
//=========================================================//
