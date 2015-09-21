/*
 * CSE 385 - Computer Graphics
 * Wind-wheel assignment
 * Bruno Lopes & Gabriel Esposito
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// position camera in the environment
void update_camera();

GLdouble  g_playerPos[] = { 0.0, 1, 10.0 };//{ 0.0, 0.5, 10.0 };
GLdouble  g_lookAt[] = { 0.0, 0.0, 0.0 };
GLfloat   g_viewAngle = -90.0;
GLfloat   g_elevationAngle = 6.0;
float rad =0;
const float DEFAULT_SPEED   = 0.5f;
//=========================================================//
//=========================================================//

GLvoid  DrawNormalObjects(GLfloat rotation);

//=========================================================//
//=========================================================//
// quadric objects
void init_data(void);
void setup_sceneEffects(void);
GLUquadricObj *g_normalObject     = NULL;
GLUquadricObj *g_wireframeObject  = NULL;
GLUquadricObj *g_flatshadedObject = NULL;
void cleanup_data(void);

const int   WORLD_SIZE = 100;

typedef struct
{
    const char * const name;
    void (*solid) (void);
    void (*wire)  (void);
} entry;


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

//static stuff
float wheelRotateFactor=1.0;
float rotateBase_degrees=0;
float rotatePointer_degrees=0;

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
//=========================================================//
GLvoid DrawNormalObjects(GLfloat rotation)
{
  // save the existing color properties
  glPushAttrib(GL_CURRENT_BIT);

  glPushMatrix();

    //STEM cylinder
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, rand() % 128);
    glColor3f(0,1,1);
    glTranslatef(0, 0, -8);
    glRotatef(-90, 1.0, 0.0, 0);
    glRotatef(rotateBase_degrees, 0.0, 0.0, 1.0);
    gluCylinder(g_flatshadedObject, 0.5, 0.5, 6.0, 32, 4);

    //pipe on disks
    glTranslatef(0, 0, 5);
    glRotatef(-270, 1, 0, 0);
    gluCylinder(g_flatshadedObject, 0.5, 0.5, 4.0, 32, 4);

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

  // restore the previous color values
  glPopAttrib();
}


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
    // it's increased by one every frame
    rotation += 1.0;
    // and ranges between 0 and 360
    if (rotation > 360.0)
    rotation = 0.0;
    // draw all of our objects in their normal position
    DrawNormalObjects(rotation);

    //glDisable(GL_LIGHTING);
    glColor3d(0.1,0.1,0.4);

    glutSwapBuffers();
}
//=========================================================//
//=========================================================//
static void keyboard(unsigned char key, int x, int y)
{
	//map rotation of the wheel and set speeds

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
void init_data(void)
{
  //init lighting
  setup_sceneEffects();

  //create quadrics
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
void cleanup_data(void)
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

    init_data();

    glutCreateWindow("Wind Wheel");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    // environment background color
    glClearColor(0.9,0.9,0.7,1);//(1,1,1,1);
    // depth efect to objects
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // light and material in the environment
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();
    cleanup_data();

    return EXIT_SUCCESS;
}
//=========================================================//
//=========================================================//
