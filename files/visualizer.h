#include <glut.h>

#include "particle.h"
#include "system.h"


namespace Visualization
{
    double angle = 0.0;

    std::size_t num_particles(3 * 3 * 3);
    std::size_t num_blocks(20);

    System system(Visualization::num_particles, Visualization::num_blocks, Visualization::num_blocks * 10.0f);
}

void init()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);    

    glLoadIdentity();                 
    glTranslatef(0.0f, 0.0f, -7.0f);  
    glRotatef(Visualization::angle, 0.0, 1.0, 0.0);

    glColor3f(1.0f, 0.0f, 1.0f);

    for (std::size_t i = 0; i < Visualization::num_particles; i++)
    {
        auto coordinates = Visualization::system.get_position(i);

        glTranslatef(coordinates.m_x * 0.1f, coordinates.m_y * 0.1f, coordinates.m_z * 0.1f);
        glutWireSphere(0.1, 10, 10);
        glTranslatef(coordinates.m_x * (-0.1f), coordinates.m_y * (-0.1f), coordinates.m_z * (-0.1f));
    }

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{

    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


void timer(int)
{
    display();

    Visualization::system.update();

    glutTimerFunc(10, timer, 0);

    if (std::abs(Visualization::angle) > 360.0) Visualization::angle = 0.0;

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a': case 'A':
    {

        Visualization::angle -= 0.5;

        break;
    }
    case 'd': case 'D':
    {

        Visualization::angle += 0.5;

        break;  
    }
    case 's': case 'S':
    {

        system("pause");

        break;
    }
    }
}