#include <random>
#include <iostream>
#include <fstream>
#include <chrono>
#include <numeric>
#include <functional>
#include <algorithm>
#include <vector>

#include "system.h"

void System::initialize()
{

	face_centered_cubic();

	for (auto i = 0; i < m_num_particles; ++i)
	{
		find_neighbors(i);
	}


	std::ofstream outfile;
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_k.txt", std::ofstream::out | std::ofstream::trunc);
	outfile.close();
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_p.txt", std::ofstream::out | std::ofstream::trunc);
	outfile.close();
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_t.txt", std::ofstream::out | std::ofstream::trunc);
	outfile.close();
}

void System::random()
{
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	// Boltzmann factor times temperature
	const double k_T = 0.1;

	static std::default_random_engine dre;

	static std::uniform_real_distribution < float > uid_x(0.0, m_cube_size);
	static std::uniform_real_distribution < float > uid_y(0.0, m_cube_size);
	static std::uniform_real_distribution < float > uid_z(0.0, m_cube_size);

	static std::uniform_real_distribution < float > uid_direction(0.0, 2.0f);
	static std::gamma_distribution < float > maxwell(3.0f / 2.0f, k_T);

	for (auto i = 0; i < m_num_particles; ++i)
	{
		auto position =		Coordinates(uid_x(dre),
										uid_y(dre),
										uid_z(dre));

		auto velocity = Coordinates(maxwell(generator) * (uid_direction(dre) - 1.0f),
						maxwell(generator) * (uid_direction(dre) - 1.0f),
						maxwell(generator) * (uid_direction(dre) - 1.0f));
		//auto velocity =		Coordinates(0.0f, 0.0f, 0.0f);
		auto acceleration = Coordinates(0.0f, 0.0f, 0.0f);
		m_particles.push_back(Particle(position, velocity, acceleration));
	}
}

void System::two_particles()
{
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	// Boltzmann factor times temperature
	const double k_T = 0.1;

	static std::default_random_engine dre;

	static std::uniform_real_distribution < float > uid_x(0.0, m_cube_size);
	static std::uniform_real_distribution < float > uid_y(0.0, m_cube_size);
	static std::uniform_real_distribution < float > uid_z(0.0, m_cube_size);

	static std::uniform_real_distribution < float > uid_direction(0.0, 2.0f);
	static std::gamma_distribution < float > maxwell(3.0f / 2.0f, k_T);

	for (auto x = 5.0f; x < 5.0 + 1.75; x += 1.12)
	{
		auto y = m_cube_size / 2;
		auto z = m_cube_size / 2;

		auto position = Coordinates(x, y, z);
		//auto velocity = Coordinates(maxwell(generator) * (uid_direction(dre) - 1.0f),
		//			maxwell(generator) * (uid_direction(dre) - 1.0f),
		//			maxwell(generator) * (uid_direction(dre) - 1.0f));

		auto velocity = Coordinates(0.0f, 0.0f, 0.0f);
		auto acceleration = Coordinates(0.0f, 0.0f, 0.0f);

		m_particles.push_back(Particle(position, velocity, acceleration));

	}
}

void System::face_centered_cubic()
{
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	// Boltzmann factor times temperature
	const double k_T = 2.5;

	static std::default_random_engine dre;

	static std::uniform_real_distribution < float > uid_x(0.0, m_cube_size);
	static std::uniform_real_distribution < float > uid_y(0.0, m_cube_size);
	static std::uniform_real_distribution < float > uid_z(0.0, m_cube_size);

	static std::uniform_real_distribution < float > uid_direction(0.0, 2.0f);
	static std::gamma_distribution < float > maxwell(3.0f / 2.0f, k_T);

	const auto a = 7.0f;
	const auto layers = 5;
	const auto columns = 5;
	const auto rows = 5;
	const auto add = 1.0f;


	double x0 = -a, y0 = -a, z0 = -a, x1, y1, z1 = -a / 2;

	for (int i = 0; i < layers; i++) {
		z0 += a;
		z1 += a;

		x0 = -a;

		for (int j = 0; j < columns; j++) {
			x0 += a;
			x1 = x0 + a / 2;

			y0 = -a;

			for (int k = 0; k < rows; k++) {
				y0 += a;
				y1 = y0 + a / 2;

				auto velocity = Coordinates(maxwell(generator) * (uid_direction(dre) - 1.0f),
											maxwell(generator) * (uid_direction(dre) - 1.0f),
											maxwell(generator) * (uid_direction(dre) - 1.0f));
				//auto velocity = Coordinates(0.0f, 0.0f, 0.0f);
				auto acceleration = Coordinates(0.0f, 0.0f, 0.0f);


				m_particles.push_back(Particle(Coordinates(x0 + add, y0 + add, z0+ add), velocity, acceleration));
				m_particles.push_back(Particle(Coordinates(x1 + add, y1 + add, z0 + add), velocity, acceleration));
				m_particles.push_back(Particle(Coordinates(x0 + add, y1 + add, z1 + add), velocity, acceleration));
				m_particles.push_back(Particle(Coordinates(x0 + add, y0 + add, z1 + add), velocity, acceleration));
			}
		}
	}
}


float System::get_distance(Particle particle_i, Particle particle_j) const
{
	auto position_i = particle_i.get_position();
	auto position_j = particle_j.get_position();

	auto distance = (position_i - position_j).get_length();

	return distance;
}


float System::get_outer_distance(Particle particle_i, Particle particle_j) const
{
	auto position_i = particle_i.get_position();
	auto position_j = particle_j.get_position();

	std::vector < Coordinates > intersections;
	intersections.push_back(get_intersection(position_i, position_i - position_j, m_num_blocks, position_i.m_x, (position_i - position_j).m_x));
	intersections.push_back(get_intersection(position_i, position_i - position_j, (-1.0f) * m_num_blocks, position_i.m_x, (position_i - position_j).m_x));
	intersections.push_back(get_intersection(position_i, position_i - position_j, m_num_blocks, position_i.m_y, (position_i - position_j).m_y));
	intersections.push_back(get_intersection(position_i, position_i - position_j, (-1.0f) * m_num_blocks, position_i.m_y, (position_i - position_j).m_y));
	intersections.push_back(get_intersection(position_i, position_i - position_j, m_num_blocks, position_i.m_z, (position_i - position_j).m_z));
	intersections.push_back(get_intersection(position_i, position_i - position_j, (-1.0f) * m_num_blocks, position_i.m_z, (position_i - position_j).m_z));

	std::vector < Coordinates > cube_inersections;

	for (auto i = 0; i < std::size(intersections); ++i)
	{
		if (((intersections[i].m_x <= m_num_blocks) && (intersections[i].m_x >= (-1.0f) * m_num_blocks)) &&
			((intersections[i].m_y <= m_num_blocks) && (intersections[i].m_y >= (-1.0f) * m_num_blocks)) &&
			((intersections[i].m_z <= m_num_blocks) && (intersections[i].m_z >= (-1.0f) * m_num_blocks)))
		{

			if (std::find(cube_inersections.begin(), cube_inersections.end(), intersections[i]) != cube_inersections.end()) {
				continue;
			}
			else {
				cube_inersections.push_back(intersections[i]);

			}
		}
	}

	auto outer_distance = 0.0f;

	if (std::size(cube_inersections) >= 2)
	{
		outer_distance = (cube_inersections[0] - cube_inersections[1]).get_length() - get_distance(particle_i, particle_j);
	}

	return outer_distance;
}

Coordinates System::get_intersection(const Coordinates point, const Coordinates direction, float a, float x_0, float x_1) const
{
	if (x_1 == 0.0) { return Coordinates(m_num_blocks * 2, m_num_blocks * 2, m_num_blocks * 2); }

	auto t = (a - x_0) / x_1;
	
	return direction * t + point;
}


void System::find_neighbors(std::size_t i)
{
	m_neighbors[i].clear();

	for (auto j = 0; j < m_num_particles; ++j)
	{

		std::vector < Particle > images;

		for (auto x = -1.0f; x <= 1.0f; x += 1.0f)
		{
			for (auto y = -1.0f; y <= 1.0f; y += 1.0f)
			{
				for (auto z = -1.0f; z <= 1.0f; z += 1.0f)
				{
					images.push_back(Particle(		Coordinates(m_particles[j].get_position().m_x + x * m_cube_size,
																m_particles[j].get_position().m_y + y * m_cube_size,
																m_particles[j].get_position().m_z + z * m_cube_size),

													m_particles[j].get_velocity(),
													m_particles[j].get_acceleration()));
				}
			}
		}

		auto min_distance = get_distance(m_particles[i], m_particles[j]);
		auto closest_particle = m_particles[j];

		for (auto k = 0; k < std::size(images); k++)
		{
			auto distance = get_distance(m_particles[i], images[k]);
			if (distance < min_distance)
			{
				min_distance = distance;
				closest_particle = images[k];
			}
		}


		if ((min_distance <= m_radius) && (i != j))
		{
			m_neighbors[i].push_back(closest_particle);
		}
	}
}

auto System::get_force(Particle particle_i, Particle particle_j, float sigma, float epsilon, float mass) const
{
	Coordinates result;
	auto position_i = particle_i.get_position();
	auto position_j = particle_j.get_position();
	auto distance = get_distance(particle_i, particle_j);

	return  24.0f * epsilon * (2.0f * std::pow(sigma, 12) / std::pow(distance, 14) -
		std::pow(sigma, 6) / std::pow(distance, 8)) * (position_j - position_i);;
}

auto System::get_potential(Particle particle_i, Particle particle_j, float sigma, float epsilon, float mass) const
{

	auto position_i = particle_i.get_position();
	auto position_j = particle_j.get_position();
	auto distance = get_distance(particle_i, particle_j);

	return 4.0f * mass * epsilon * float(std::pow(sigma / distance, 12) - std::pow(sigma / distance, 6));
}	

void System::update()
{
	auto delta_t = 0.01f;
	auto energy_total = 0.0f;
	auto energy_potential = 0.0f;
	auto energy_kinetic = 0.0f;

	for (auto i = 0; i < m_num_particles; ++i)
	{
		Coordinates force(0.0, 0.0, 0.0);

		for (auto j = 0; j < std::size(m_neighbors[i]); ++j)
		{
			force += get_force(m_particles[i], m_neighbors[i][j]);
			energy_potential -= get_potential(m_particles[i], m_neighbors[i][j]);
		}
		
		m_particles[i].set_new_acceleration(force);
	}


	for (auto i = 0; i < m_num_particles; ++i)
	{
		auto new_position = m_particles[i].get_position() + m_particles[i].get_velocity() * delta_t + 0.5f * m_particles[i].get_acceleration() * std::pow(delta_t, 2);
		auto new_velocity = m_particles[i].get_velocity() + 0.5f * (m_particles[i].get_acceleration() + m_particles[i].get_new_acceleration()) * delta_t;
		auto new_acceleration = m_particles[i].get_new_acceleration();


		if (new_position.m_x >= m_cube_size) {
			new_position.m_x = new_position.m_x - m_cube_size;
		}
		if (new_position.m_x < 0.0f) {
			new_position.m_x = m_cube_size - new_position.m_x;
		}
		if (new_position.m_y >= m_cube_size) {
			new_position.m_y = new_position.m_y - m_cube_size;
		}
		if (new_position.m_y < 0.0f) {
			new_position.m_y = m_cube_size - new_position.m_y;
		}
		if (new_position.m_z >= m_cube_size) {
			new_position.m_z = new_position.m_z - m_cube_size;
		}
		if (new_position.m_z < 0.0f) {
			new_position.m_z = m_cube_size - new_position.m_z;
		}

		m_particles[i].set_position(new_position);
		m_particles[i].set_velocity(new_velocity);
		m_particles[i].set_acceleration(new_acceleration);

		energy_kinetic += float(std::pow(m_particles[i].get_velocity().get_length(), 2)) / 2;

	}

	energy_total = energy_kinetic + energy_potential / 2;

	std::ofstream outfile;
	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_k.txt", std::ios_base::app);
	outfile << energy_kinetic << std::endl;
	outfile.close();

	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_p.txt", std::ios_base::app);
	outfile << energy_potential << std::endl;
	outfile.close();

	outfile.open("C:\\Users\\User\\mipt\\supercomputers\\Project1\\data\\energy_t.txt", std::ios_base::app);
	outfile << energy_total << std::endl;
	outfile.close();


	
	std::cout << energy_total << std::endl;

	for (auto i = 0; i < m_num_particles; ++i)
	{
		find_neighbors(i);
	}

}