#pragma once

#include <memory>

#include "position.h"


class Particle
{
public:

	Particle() noexcept = default;
	explicit Particle(Coordinates position, Coordinates velocity, Coordinates acceleration):
		m_position(position), m_velocity(velocity), m_acceleration(acceleration), m_new_acceleration(Coordinates(0, 0, 0))
	{}

	auto get_position() const noexcept { return m_position; }
	auto get_velocity() const noexcept { return m_velocity; }
	auto get_acceleration() const noexcept { return m_acceleration; }
	auto get_new_acceleration() const noexcept { return m_new_acceleration; }

	void set_position(float x, float y, float z)		{ m_position.set_position(x, y, z); }
	void set_position(Coordinates coordinates) { m_position = coordinates; }

	void set_velocity(float x, float y, float z)		{ m_velocity.set_position(x, y, z); }
	void set_velocity(Coordinates coordinates) { m_velocity = coordinates; }

	void set_acceleration(float x, float y, float z)	{ m_acceleration.set_position(x, y, z); }
	void set_acceleration(Coordinates coordinates) { m_acceleration = coordinates; }

	void set_new_acceleration(float x, float y, float z) { m_new_acceleration.set_position(x, y, z); }
	void set_new_acceleration(Coordinates coordinates) { m_new_acceleration = coordinates; }


	~Particle() noexcept = default;

private:
	Coordinates m_position;
	Coordinates m_velocity;
	Coordinates m_acceleration;
	Coordinates m_new_acceleration;

};
