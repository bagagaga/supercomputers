#include <random>
#include <iostream>
#include <chrono>
#include <numeric>
#include <functional>

#include "system.h"

void System::initialize()
{
	static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	// Boltzmann factor times temperature
	const double k_T = 0.1;

	static std::default_random_engine dre;

	static std::uniform_real_distribution <> uid_x(0.0, 2.0f * (m_num_blocks - 1.0f));
	static std::uniform_real_distribution <> uid_y(0.0, 2.0f * (m_num_blocks - 1.0f));
	static std::uniform_real_distribution <> uid_z(0.0, 2.0f * (m_num_blocks - 1.0f));

	static std::uniform_real_distribution <> uid_direction(0.0, 2.0f);
	static std::gamma_distribution < float > maxwell(3.0f / 2.0f, k_T);

	//for (auto i = 0; i < m_num_particles; ++i)
	//{
	//	auto position =		Coordinates(uid_x(dre) - float(m_num_blocks - 1.0f),
	//									uid_y(dre) - float(m_num_blocks - 1.0f),
	//									uid_z(dre) - float(m_num_blocks - 1.0f));
	//	auto velocity = Coordinates(maxwell(generator) * (uid_direction(dre) - 1.0f),
	//								maxwell(generator) * (uid_direction(dre) - 1.0f),
	//								maxwell(generator) * (uid_direction(dre) - 1.0f));
	//	//auto velocity =		Coordinates(0.0f, 0.0f, 0.0f);
	//	auto acceleration = Coordinates(0.0f, 0.0f, 0.0f);
	//	
	//	m_particles.push_back(std::make_shared<Particle>(position, velocity, acceleration));
	//}

	for (auto x = 2 * m_num_blocks / (cbrt(m_num_particles) + 1); x < 2 * m_num_blocks; x += 2 * m_num_blocks / (cbrt(m_num_particles) + 1))
	{
		for (auto y = 2 * m_num_blocks / (cbrt(m_num_particles) + 1); y < 2 * m_num_blocks; y += 2 * m_num_blocks / (cbrt(m_num_particles) + 1))
		{
			for (auto z = 2 * m_num_blocks / (cbrt(m_num_particles) + 1); z < 2 * m_num_blocks; z += 2 * m_num_blocks / (cbrt(m_num_particles) + 1))
			{
				auto position = Coordinates(x - m_num_blocks, y - m_num_blocks, z - m_num_blocks);
				auto velocity = Coordinates(0.0f, 0.0f, 0.0f);
				auto acceleration = Coordinates(0.0f, 0.0f, 0.0f);

				m_particles.push_back(Particle(position, velocity, acceleration));
			}
		}
	}

	for (auto i = 0; i < m_num_particles; ++i)
	{
		find_neighbors(i);
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
		auto distance = get_distance(m_particles[i], m_particles[j]);
		auto outer_distance = get_outer_distance(m_particles[i], m_particles[j]);


		if ((distance <= m_radius) && (i != j))
		{
			m_neighbors[i].push_back(m_particles[j]);
		}

		//if ((outer_distance <= m_radius) && (i != j))
		//{
		//	auto image_particle = Particle(	m_particles[j].get_position(), 
		//									m_particles[j].get_velocity(),
		//									m_particles[j].get_acceleration()) ;
		//	image_particle.set_position((m_particles[i].get_position() - m_particles[j].get_position()) * std::pow(distance, -1) * (distance + outer_distance) + m_particles[j].get_position());
		//	m_neighbors[i].push_back(image_particle);
		//}
	}
}

auto System::get_force(Particle particle_i, Particle particle_j, float sigma, float epsilon, float mass) const
{
	Coordinates result;
	auto position_i = particle_i.get_position();
	auto position_j = particle_j.get_position();
	auto distance = get_distance(particle_i, particle_j);

	return  24.0f * mass * epsilon * (2.0f * std::pow(sigma, 12) / std::pow(distance, 14) -
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
	auto delta_t = 10.0f;
	auto energy = 0.0f;

	for (auto i = 0; i < m_num_particles; ++i)
	{
		Coordinates force(0.0, 0.0, 0.0);

		for (auto j = 0; j < std::size(m_neighbors[i]); ++j)
		{
			force += get_force(m_particles[i], m_neighbors[i][j]);
			energy += get_potential(m_particles[i], m_neighbors[i][j]);
		}

		auto new_position = m_particles[i].get_position() + m_particles[i].get_velocity() * delta_t + 0.5f * force * std::pow(delta_t, 2);
		auto new_velocity = m_particles[i].get_velocity() + 0.5f * (m_particles[i].get_acceleration() + force) * delta_t;
		auto new_acceleration = force;

		if (new_position.m_x > m_num_blocks)  { 
			new_position.m_x = (-1.0f) * m_num_blocks + 1.0f; 
		}
		if (new_position.m_x < (-1.0f) * m_num_blocks) { 
			new_position.m_x = (m_num_blocks - 1.0f); 
		}
		if (new_position.m_y > m_num_blocks) {
			new_position.m_y = (-1.0f) * m_num_blocks + 1.0f;
		}
		if (new_position.m_y < (-1.0f) * m_num_blocks) {
			new_position.m_y = (m_num_blocks - 1.0f);
		}
		if (new_position.m_z > m_num_blocks) {
			new_position.m_z = (-1.0f) * m_num_blocks + 1.0f;
		}
		if (new_position.m_z < (-1.0f) * m_num_blocks) {
			new_position.m_z = (m_num_blocks - 1.0f);
		}

		m_particles[i].set_position(new_position);
		m_particles[i].set_velocity(new_velocity);
		m_particles[i].set_acceleration(new_acceleration);

		energy += float(std::pow(m_particles[i].get_velocity().get_length(), 2));
	}

	std::cout << energy << std::endl;

	for (auto i = 0; i < m_num_particles; ++i)
	{
		find_neighbors(i);
	}

}