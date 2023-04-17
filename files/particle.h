#pragma once

#include <vector>
#include <random>


class Particle
{
public:
	using coordinate_t = std::vector < double >;
public:
	explicit Particle(const std::vector < double > & position, const std::vector < double >& velocity, 
		const std::vector < double >& acceleration, const std::vector < double >& new_acceleration) :
		m_position(position), m_velocity(velocity), m_acceleration(acceleration), m_new_acceleration(new_acceleration)
		{}

	explicit Particle()
	{
		static std::default_random_engine generator;
		static std::gamma_distribution<double> distribution(1.5, 0.01);

		std::vector < double > position{ 0.0, 0.0, 0.0 };
		m_position = position;
		auto v_x = distribution(generator);
		auto v_y = distribution(generator);
		auto v_z = distribution(generator);
		std::vector < double > velocity{ v_x, v_y, v_z };
		m_velocity = velocity;
		std::vector < double > acceleration{ 0.0, 0.0, 0.0 };
		m_acceleration = acceleration;
		std::vector < double > new_acceleration{ 0.0, 0.0, 0.0 };
		m_new_acceleration = new_acceleration;
	}


	~Particle() noexcept  = default;

public:
	coordinate_t m_position;
	coordinate_t m_velocity;
	coordinate_t m_acceleration;
	coordinate_t m_new_acceleration;

};
