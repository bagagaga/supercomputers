#pragma once

#include <memory>
#include <vector>
#include <cmath>

#include "particle.h"
#include "position.h"


class System
{
public:
	explicit System(std::size_t num_particles, std::size_t num_blocks, float radius) :
		m_num_particles(num_particles), m_num_blocks(num_blocks), m_radius(radius), 
		m_neighbors(std::vector < std::vector < Particle > > (num_particles, std::vector < Particle > (0)))
	{
		initialize();
	}

	void initialize();
	void find_neighbors(std::size_t i);
	void update();

	auto get_potential(Particle particle_i, Particle particle_j, float sigma = 1.0f, float epsilon = 1.0f, float mass = 1.0f) const;
	auto get_force(Particle particle_i, Particle particle_j, float sigma = 1.0f, float epsilon = 1.0f, float mass = 1.0f) const;
	auto get_position(std::size_t i) const { return m_particles.at(i).get_position();}
	float get_distance(Particle particle_i, Particle particle_j) const;
	float get_outer_distance(Particle particle_i, Particle particle_j) const;
	Coordinates get_intersection(const Coordinates point, const Coordinates direction, float a, float x_0, float x_1) const;

	~System() noexcept = default;

private:
	std::vector < Particle > m_particles;
	std::vector < std::vector < Particle > > m_neighbors;

	std::size_t m_num_particles;
	std::size_t m_num_blocks;
	
	float m_radius;
};