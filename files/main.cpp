#include <windows.h>  
#include <glut.h> 
#include <iostream>
#include <fstream>
#include <string>

#include "visualizer.h"


//int main(int argc, char** argv)
//{
//    try
//    {
//        glutInit(&argc, argv);            // Initialize GLUT
//        glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
//        glutInitWindowSize(640, 480);   // Set the window's initial width & height
//        glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
//        glutCreateWindow("PARTICLES");          // Create window with the given title
//        glutDisplayFunc(display);       // Register callback handler for window re-paint event
//        glutReshapeFunc(reshape);       // Register callback handler for window re-size event
//        glutKeyboardFunc(keyboard);
//        init();       
//        glutTimerFunc(10, timer, 0); // Our own OpenGL initialization
//        glutMainLoop();
//    }
//
//        catch (const std::exception& exception)
//    {
//        std::cerr << exception.what() << '\n';
//
//        system("pause");
//
//        return EXIT_FAILURE;
//    }
//
//	system("pause");
//    return EXIT_SUCCESS;
//}

void dump_step(int i_step)
{
    std::string filename = "C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\steps.txt";

    std::ofstream f_out;
    f_out.open(filename, std::ios_base::app);
    f_out << "ITEM: TIMESTEP" << std::endl;
    f_out << i_step << std::endl;

    f_out << "ITEM: NUMBER OF ATOMS" << std::endl;
    f_out << Visualization::num_particles << std::endl;

    f_out << "ITEM: BOX BOUNDS pp pp pp" << std::endl;
    f_out << 0 << " " << Visualization::num_blocks << std::endl;
    f_out << 0 << " " << Visualization::num_blocks << std::endl;
    f_out << 0 << " " << Visualization::num_blocks << std::endl;

    f_out << "ITEM: ATOMS id x y z" << std::endl;

    for (auto j = 0; j < Visualization::num_particles; j++)
    {
        auto coordinates = Visualization::system.get_position(j);

        f_out << std::to_string(j)   +  " " + std::to_string(coordinates.m_x) +  
                                        " " + std::to_string(coordinates.m_y) +
                                        " " + std::to_string(coordinates.m_z) << std::endl;
    }

    f_out.close();
}


int main(int argc, char** argv)
{
    try
    {
        std::ofstream outfile;
        outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\steps.txt", std::ofstream::out | std::ofstream::trunc);
        outfile.close();

        for (auto i = 0; i < 100; i++)
        {
            Visualization::system.update();
            dump_step(i);

            //if (i % 10 == 0) dump_step(i);
        }
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