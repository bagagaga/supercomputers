#include <windows.h>  
#include <glut.h> 
#include <iostream>

#include "visualizer.h"


int main(int argc, char** argv)
{
    try
    {
        glutInit(&argc, argv);            // Initialize GLUT
        glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
        glutInitWindowSize(640, 480);   // Set the window's initial width & height
        glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
        glutCreateWindow("PARTICLES");          // Create window with the given title
        glutDisplayFunc(display);       // Register callback handler for window re-paint event
        glutReshapeFunc(reshape);       // Register callback handler for window re-size event
        glutKeyboardFunc(keyboard);
        init();       
        glutTimerFunc(10, timer, 0); // Our own OpenGL initialization
        glutMainLoop();
    }

        catch (const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';

        system("pause");

        return EXIT_FAILURE;
    }

	system("pause");
    return EXIT_SUCCESS;
}