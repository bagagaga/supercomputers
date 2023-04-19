#include "system.h"
#include "particle.h"
#include "timer.h"

#include <stdio.h>
#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>


namespace Visualization
{

    std::size_t num_particles(2);
    std::size_t num_blocks(7.7);

    System system(Visualization::num_particles, Visualization::num_blocks, Visualization::num_blocks * 10.0f);

}


void dump_step(int i_step)
{
    std::string filename = "C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\steps.txt";

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
        auto coordinates = Visualization::system.m_particles[j].m_position;

        f_out << std::to_string(j) + " " + std::to_string(coordinates[0]) +
            " " + std::to_string(coordinates[1]) +
            " " + std::to_string(coordinates[2]) << std::endl;
    }

    f_out.close();
}


int main(int argc, char** argv)
{
    try
    {
        std::ofstream outfile;
        outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\steps.txt", std::ofstream::out | std::ofstream::trunc);
        outfile.close();

        system("color 0A");

        char a = 177, b = 219;
        auto num_steps = 10000;
        auto dump_step_len = 100;
        auto len = num_steps / dump_step_len;

        printf("\n\n\n\n");
        printf("\n\n\n\n\t\tLoading...\n\n");
        printf("\t\t");

        for (int i = 0; i < len; i++)
            printf("%c", a);

        printf("\r");
        printf("\t\t");

        Timer <> t_v("system");

        for (auto i = 0; i < num_steps; i++)
        {
            Visualization::system.update();

            if (i % dump_step_len == 0) { printf("%c", b); dump_step(i); }
        }

        printf("\n\n\n");

        t_v.stop();

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