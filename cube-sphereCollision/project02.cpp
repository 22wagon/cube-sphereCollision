//Brandon Becerra, 300481351, CSCI 173 Project 2
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <ctime>

#include <math.h>
/*
INSTURCTION ON HOW TO USE THIS PROGRAM:
Left key to rotate left
Right key to rotate right
Up key to zoom in
Down key to zoom out
Space bar to reset the camera
*/
using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

float angle = 0.0; //This is to be used and rotate the cube
float zoomY = 0.0; //Zooming to the middle of the cube
float zoomZ = 0.0; //Zooming to the middle of the cube
bool spaceBar = false; //This is to set the spacebar false init.
#define fourSpheres 4 //The four spheres being define
#define sphereSize 0.6f //This is the sphere size
#define sizeOfCube 3.0f //The cube initial size
/* GLUT callback Handlers */
class vectors //This class is going to be used as the vectors for the spheres
{
    public:
        float x, y, z; //Initialize x, y, and z
        vectors(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z){} //This is for use to call the vectors also the constructor definition of it.
        vectors operator+(const vectors& anoVec) const //Takes in another vectors object and returns a new vectors object that represents the sum of the two vectors.
        {
            return vectors(x + anoVec.x, y + anoVec.y, z + anoVec.z); //While not really modifying the original vectors object.
        }
        vectors operator-(const vectors& anoVec) const //This takes in another vectors object and returns a new vectors object that represents the difference between the two vecctors.
        {
            return vectors(x - anoVec.x, y - anoVec.y, z - anoVec.z); //The x,y, and z parts are passed as vectors object being subtracted from the corresponding part of the current object.
        }
        vectors operator*(float scalar) //Now this part takes in the scalar value and returns a new vec object with each part of the current vec object being multiplied by the scalar value.
        {
            return vectors(x * scalar, y * scalar, z * scalar);
        }
};
class Sphere //This is where the spheres are created with collisions and speed.
{
    public:
        vectors position; //Position of the spheres.
        vectors velocity; //As in the name the velocity for the spheres.
        float radius; //Size of the spheres.
        float color[3]; //Init Color for four spheres.
        Sphere(const vectors& position, float radius) : position(position), radius(radius) //Thiis is to initializes the member variables position and radius of the current object using the values being passed as arguments.
        {
            // Generate a random velocity
            velocity = vectors((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX) * 0.0035f; //The number 0.0035f is where you could change to either make it fast or slower.
            color[0] = (float)rand() / RAND_MAX; // Generate a random color as it initial.
            color[1] = (float)rand() / RAND_MAX; // Generate a random color as it initial.
            color[2] = (float)rand() / RAND_MAX; // Generate a random color as it initial.
        }
        void drawSpheres() //This is to draw the spheres
        {
            glColor3fv(color); //Existing arrays of color values to be used
            glPushMatrix();
            glTranslatef(position.x, position.y, position.z); //The movement and position of the spheres.
            glutSolidSphere(radius, 20, 20); //Size of the sphere
            glPopMatrix();
        }
        void move(); //To be call later down the code for the movement of the spheres.

};
Sphere spheres[fourSpheres] = //The four spheres and the positions of it
{
    Sphere(vectors(-1.5f, -1.5f, -1.5f), sphereSize),
    Sphere(vectors(1.5f, -1.5f, -1.5f), sphereSize),
    Sphere(vectors(-1.5f, 1.5f, -1.5f), sphereSize),
    Sphere(vectors(1.5f, 1.5f, -1.5f), sphereSize)
};
void Sphere::move()
{
    position = position + velocity; //Updating the spheres positions.
    // The code being line 100 to 129 will check for collision with the walls of the cube.
    if (position.x - radius < -sizeOfCube) //Check if it collided with the LEFT of the cube and if so will change velocity to go back inside the cube.
    {
        position.x = -sizeOfCube + radius;
        velocity.x = -velocity.x;
    }
    else if (position.x + radius > sizeOfCube) //Check if it collided with the RIGHT of the cube and if so will change velocity to go back inside the cube.
    {
        position.x = sizeOfCube - radius;
        velocity.x = -velocity.x;
    }
    if (position.y - radius < -sizeOfCube) //Check if it collided with the BOTTOM of the cube and if so will change velocity to go back inside the cube.
    {
        position.y = -sizeOfCube + radius;
        velocity.y = -velocity.y;
    }
    else if (position.y + radius > sizeOfCube) //Check if it collided with the TOP of the cube and if so will change velocity to go back inside the cube.
    {
        position.y = sizeOfCube - radius;
        velocity.y = -velocity.y;
    }
    if (position.z - radius < -sizeOfCube) //Check if it collided with the BACK of the cube and if so will change velocity to go back inside the cube.
    {
        position.z = -sizeOfCube + radius;
        velocity.z = -velocity.z;
    }
    else if (position.z + radius > sizeOfCube) //Check if it collided with the FRONT of the cube and if so will change velocity to go back inside the cube.
    {
        position.z = sizeOfCube - radius;
        velocity.z = -velocity.z;
    }
    for (int i = 0; i < fourSpheres; i++) //This is to go through the 4 spheres where the color change and maintain speed will occur.
    {
        if (this != &spheres[i]) //Checks if the current sphere is not the same as the sphere[i].
        {
            //Too long of a code in single line so change it in this form for easier read.
            //Use the Distance formula with x,y, and z.
            float distForm = (position.x - spheres[i].position.x)*(position.x-spheres[i].position.x)
                             +(position.y-spheres[i].position.y)*(position.y-spheres[i].position.y)
                             +(position.z-spheres[i].position.z)*(position.z-spheres[i].position.z);

            if (distForm <= (radius + spheres[i].radius)*(radius + spheres[i].radius)) //Checking of the collision has occur between the spheres.
            {
                color[0] = (float)rand() / RAND_MAX; // Generate new random colors for both spheres
                color[1] = (float)rand() / RAND_MAX; // Generate new random colors for both spheres
                color[2] = (float)rand() / RAND_MAX; // Generate new random colors for both spheres
                spheres[i].color[0] = (float)rand() / RAND_MAX; //Each sphere getting the rand color.
                spheres[i].color[1] = (float)rand() / RAND_MAX; //Each sphere getting the rand color.
                spheres[i].color[2] = (float)rand() / RAND_MAX; //Each sphere getting the rand color.
                //Reason for some code below of having momentum and energy laws to determine speed and impulse so that if we want the spheres to speed up after every collision or slow down after every collsision then minor
                //changes in the below code could do that however just for now we have it like this so the speed doesn't change at all.
                vectors norm = position - spheres[i].position; // Calculate the normal vector of the collision plane.
                norm = norm * (1.0f / sqrt(distForm));
                vectors relVel = velocity - spheres[i].velocity; // Calculate the relative velocity vector of the two spheres.
                float dotProduct = relVel.x * norm.x + relVel.y * norm.y + relVel.z * norm.z; // Calculating the impulse vector also know as momentum which by the way won't change the speed of the spheres I ensured that.
                float noSp = dotProduct * (1.0f + 1.0f) / (1.0f / sphereSize + 1.0f / sphereSize); //This is the calculation of momentum and energy laws to determine the speed.
                vectors impulse = norm * (noSp); //This is to ensured that speed of the spheres won't change after collision.
                velocity = velocity - impulse * (1.0f / sphereSize); // Update the velocities of the two spheres
                spheres[i].velocity = spheres[i].velocity + impulse * (1.0f / sphereSize);
            }
        }
    }
}
static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME
   // your code here
    glTranslatef(0.0, zoomY, zoomZ); //This is the movement of zooming in and out.
    glRotatef(angle, 0.0f, 1.0f, 0.0f); //This is for rotating around the cube.
    if(spaceBar == true) //This function will reset the pov of the camera if spacebar was pressed.
    {
        zoomY = 0.0;
        zoomZ = 0.0;
        angle = 0.0;
        spaceBar = false;
    }
    glColor3f(0.0f, 1.0f, 1.0f); //color of the cube
    glutWireCube(sizeOfCube * 2.0f); // Draw the wireframe cube with double of the initial size.
    for(int i = 0; i < fourSpheres; i++) //The four movement created spheres.
    {
        spheres[i].move(); //calling it in the movement for the spheres.
        spheres[i].drawSpheres(); //Drawing it out.
    }
    // Check for collisions between all pairs of spheres
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 32 : //Pressed spacebar
            spaceBar = true;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_LEFT:
        angle += 5.0; //move left by 5 degrees
        break;
    case GLUT_KEY_RIGHT:
        angle -= 5.0; //move right by 5 degrees
        break;
    case GLUT_KEY_UP:
        zoomY += 0.2; //moving it slight to go in the middle of the cube of zooming in.
        zoomZ += 0.5; //moving it by increment of 5 to the middle of the cube.
        break;
    case GLUT_KEY_DOWN:
        zoomY -= 0.2; //moving it slight to go away from the the cube.
        zoomZ -= 0.5; //moving it by 0.5 decrement to zoom out of the cube.
        break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 2");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
