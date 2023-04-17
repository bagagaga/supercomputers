#pragma once

#include <memory>
#include <vector>
#include "particle.h"

class System
{
public:
	using particle_t = std::vector < Particle >;


public:
	explicit System(std::size_t num_particles, double cube_size, double radius) :
		m_num_particles(num_particles), m_cube_size(cube_size), m_radius(radius)
	{
		initialize();
	}

	void initialize();
	//void structure();
	void structureFCC();

	void update();

	auto find_min_radius(Particle & left, Particle& right);


	auto get_distance(const std::vector < double > & vector);
	auto get_potential(std::vector < double > min_radius, double sigma = 1.0, double epsilon = 1.0, double mass = 1.0);
	auto get_force(const std::vector < double > & min_radius, double sigma = 1.0, double epsilon = 1.0, double mass = 1.0);
	auto accumulate_potential(Particle& p_i, Particle& p_j,
		std::vector < double >& force, double& energy_potential, double sigma = 1.0, double epsilon = 1.0, double mass = 1.0);

	~System() noexcept = default;


public:
	particle_t m_particles;

	std::size_t m_num_particles;

	double m_cube_size;
	double m_radius;
};


//class FCC : public System
//{
//public:
//	void structure();
//};

//class TwoParticles : public System
//{
//public:
//	TwoParticles(std::size_t num_particles, double cube_size, double radius) :
//		System(num_particles, cube_size, radius)
//	{}
//
//	void structure();
//
//	~TwoParticles() noexcept = default;
//
//};