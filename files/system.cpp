#include "system.h"
#include "particle.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <vector>
#include "timer.h"


void System::initialize()
{
	//Timer <> t_n("create system");
	structureFCC();
	//t_n.stop();

	std::ofstream outfile;
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\energy_k.txt", std::ofstream::out | std::ofstream::trunc);
	outfile.close();
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\energy_p.txt", std::ofstream::out | std::ofstream::trunc);
	outfile.close();
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\energy_t.txt", std::ofstream::out | std::ofstream::trunc);
	outfile.close();
}

//void System::structure()
//{
//	auto half = m_cube_size / 2;
//	auto distance = 1.25;
//
//	for (auto i = 0; i < m_num_particles; i++)
//	{
//		m_particles.push_back(Particle(	std::vector < double > {half + i * distance, half, half}, 
//										std::vector < double >{0.0, 0.0, 0.0},
//										std::vector < double >{0.0, 0.0, 0.0},
//										std::vector < double >{0.0, 0.0, 0.0}));
//	}
//}

void System::structureFCC()
{
	m_num_particles = 500U;

	for (auto m = 0U; m < m_num_particles; m++)
	{
		Particle p;
		m_particles.push_back(p);
	}

	//double start = m_cube_size / 10.0;
	//double mini_cell_size = (m_cube_size - start * 2.0) / 5.0;
	//start += mini_cell_size / 4.0;

	auto i = 0U;
	double start = m_cube_size / 20.0;
	auto mini_cell_size = start * 4.0;
	double trans_len = mini_cell_size / 2.0;
	double x_0, y_0, z_0;

	for (auto j = 0U; j <= 4; j++)
	{
		x_0 = start + j * mini_cell_size;

		for (auto k = 0U; k <= 4; k++)
		{
			y_0 = start + k * mini_cell_size;

			for (auto l = 0U; l <= 4; l++)
			{
				z_0 = start + l * mini_cell_size;

				m_particles[i].m_position[0] = x_0;
				m_particles[i].m_position[1] = y_0;
				m_particles[i].m_position[2] = z_0;

				m_particles[i + m_num_particles / 4].m_position[0] = x_0 + trans_len;
				m_particles[i + m_num_particles / 4].m_position[1] = y_0 + trans_len;
				m_particles[i + m_num_particles / 4].m_position[2] = z_0;

				m_particles[i + 2 * m_num_particles / 4].m_position[0] = x_0 + trans_len;
				m_particles[i + 2 * m_num_particles / 4].m_position[1] = y_0;
				m_particles[i + 2 * m_num_particles / 4].m_position[2] = z_0 + trans_len;

				m_particles[i + 3 * m_num_particles / 4].m_position[0] = x_0;
				m_particles[i + 3 * m_num_particles / 4].m_position[1] = y_0 + trans_len;
				m_particles[i + 3 * m_num_particles / 4].m_position[2] = z_0 + trans_len;

				i++;
			}
		}
	}

}

template < typename T >
struct square
{
	T operator()(const T& Left, const T& Right) const
	{
		return (Left + Right * Right);
	}
};

auto System::get_distance(const std::vector < double > & vector)
{
	return std::sqrt(std::accumulate(vector.begin(), vector.end(), 0.0, square < double >() ));
}

auto System::get_potential(std::vector < double > min_radius, double sigma, double epsilon, double mass) 
{

	auto distance = get_distance(min_radius);

	return 4.0f * mass * epsilon * float(std::pow(sigma / distance, 12) - std::pow(sigma / distance, 6));
}


auto System::get_force(const std::vector < double > & min_radius, double sigma, double epsilon, double mass) 
{

	auto distance = get_distance(min_radius);

	std::vector < double > force{ 0.0, 0.0, 0.0 };

	auto multiply = [sigma, epsilon, mass, distance](auto n) 
	{ n *= 24.0f * epsilon * (2.0f * std::pow(sigma, 12) / std::pow(distance, 14) - std::pow(sigma, 6) / std::pow(distance, 8)); };

	std::for_each(min_radius.begin(), min_radius.end(), multiply);

	std::transform(force.begin(), force.end(), min_radius.begin(), force.begin(), std::plus<double>());

	return force;
}

auto System::find_min_radius(Particle & left, Particle& right)
{
	auto comparator = [](auto& i, auto& j) { return (abs(i) < abs(j)); };
	auto lambda = [&, this](auto& i, auto& j) {return std::min({ i - j - m_cube_size, i - j + m_cube_size, i - j}, comparator); };

	std::vector <double > min_radius{ 0.0, 0.0, 0.0 };

	std::transform(left.m_position.begin(), left.m_position.end(), right.m_position.begin(), min_radius.begin(), lambda);
	
	return min_radius;
}

auto System::accumulate_potential(Particle & p_i, Particle & p_j,
	std::vector < double >& force, double & energy_potential, double sigma, double epsilon, double mass)
{
	auto min_radius = find_min_radius(p_i, p_j);
	auto distance = get_distance(min_radius);
	
	auto r_12 = 4.0 * epsilon * std::pow(sigma / distance, 12) - std::pow(sigma / distance, 6);
	auto r_2 = std::pow(distance, 2);

	energy_potential += r_12;

	auto r_14 = 6.0 * r_12 / r_2;

	auto multiply = [&](auto& n)
	{ n *= r_14; };


	auto itA = force.begin(); auto itB = min_radius.begin(); 
	for (; (itA != force.end()) && (itB != min_radius.end()); (++itA, ++itB))
	{
		*itA = r_14 * *itB;
	}

}

void System::update()
{
	auto delta_t = 0.01;
	auto energy_total = 0.0;
	auto energy_potential = 0.0;
	auto energy_kinetic = 0.0;

	//Timer <> t_v("count forces");

	for (auto i = std::begin(m_particles); i != std::end(m_particles); i++)
	{
		std::vector < double > force{ 0.0, 0.0, 0.0 };

		for (auto j = std::begin(m_particles); j != std::end(m_particles); j++)
		{
			if (i != j)
			{
				accumulate_potential(*i, *j, force, energy_potential);
			}
		}

		i->m_new_acceleration = force;
	}

	//t_v.stop();

	//Timer <> t_p("update forces");

	auto cycle_update = [this, &delta_t, &energy_kinetic](auto & p) {

		auto itA = p.m_position.begin(); auto itB = p.m_velocity.begin(); 
		auto itC = p.m_acceleration.begin(); auto itD = p.m_new_acceleration.begin();
		for (; (itA != p.m_position.end()) && (itB != p.m_velocity.end()) &&
			(itC != p.m_acceleration.end()) && (itD != p.m_new_acceleration.end()); (++itA, ++itB, ++itC, ++itD))
		{
			*itA = *itA + *itB * delta_t + 0.5 * *itC * delta_t * delta_t;

			if (*itA >= m_cube_size) {
				*itA = *itA - m_cube_size;
			}
			if (*itA < 0.0) {
				*itA = m_cube_size - *itA;
			}
			*itB = *itB + 0.5 * (*itC + *itD) * delta_t;

			*itC = *itD;
		}

		energy_kinetic += std::pow(get_distance(p.m_velocity), 2) / 2.0;
	};

	std::for_each(m_particles.begin(), m_particles.end(), cycle_update);

	//t_p.stop();

	energy_total = energy_kinetic + energy_potential / 2.0;

	//Timer <> t_x("write results");

	std::ofstream outfile;
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\energy_k.txt", std::ios_base::app);
	outfile << energy_kinetic << std::endl;
	outfile.close();

	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\energy_p.txt", std::ios_base::app);
	outfile << energy_potential << std::endl;
	outfile.close();

	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project2\\data\\energy_t.txt", std::ios_base::app);
	outfile << energy_total << std::endl;
	outfile.close();

	//t_x.stop();

	//std::cout << energy_total << std::endl;
}