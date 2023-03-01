#pragma once

struct Coordinates
{
	float m_x;
	float m_y;
	float m_z;

	Coordinates(float x = 0.0f, float y = 0.0f, float z = 0.0f) :
		m_x(x), m_y(y), m_z(z)
	{}

	void set_position(float x, float y, float z) noexcept
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}


	Coordinates operator*= (float value)
	{
		this->m_x *= value;
		this->m_y *= value;
		this->m_z *= value;
		return *this;
	}

	friend Coordinates operator* (Coordinates first, float value)
	{
		first *= value;

		return first;
	}

	friend Coordinates operator* (float value, Coordinates first)
	{
		first *= value;
		return first;
	}

	Coordinates operator+= (const Coordinates& object)
	{
		this->m_x += object.m_x;
		this->m_y += object.m_y;
		this->m_z += object.m_z;

		return *this;
	}

	friend Coordinates operator+ (Coordinates first, const Coordinates& object)
	{
		first += object;
		return first;
	}

	friend Coordinates operator- (Coordinates first, const Coordinates& object)
	{
		first += (-1.0f) * object;
		return first;
	}

	friend bool operator== (const Coordinates & first, const Coordinates& object)
	{
		if (first.m_x == object.m_x && first.m_y == object.m_y && first.m_z == object.m_z) { return true; }

		return false;
	}

	friend bool operator!= (const Coordinates& first, const Coordinates& object)
	{
		return !(first == object);
	}


	void limit(float value)
	{
		if (this->m_x > value) { this->m_x = (-1.0f) * value + 1.0f; }
		if (this->m_x < (-1.0f) * value) { this->m_x = (value - 1.0f); }
		if (this->m_y > value) { this->m_y = (-1.0f) * value + 1.0f; }
		if (this->m_y < (-1.0f) * value) { this->m_y = value - 1.0f; }
		if (this->m_z > value) { this->m_z = (-1.0f) * value + 1.0f; }
		if (this->m_z < (-1.0f) * value) { this->m_z = value - 1.0f; }
	}

	void translate(float value)
	{
		this->m_x = (-1.0f) * value + 1.0f;
		if (this->m_x < (-1.0f) * value) { this->m_x = (value - 1.0f); }
		if (this->m_y > value) { this->m_y = (-1.0f) * value + 1.0f; }
		if (this->m_y < (-1.0f) * value) { this->m_y = value - 1.0f; }
		if (this->m_z > value) { this->m_z = (-1.0f) * value + 1.0f; }
		if (this->m_z < (-1.0f) * value) { this->m_z = value - 1.0f; }
	}

	float get_length() const
	{
		return sqrt(std::pow(m_x, 2) + std::pow(m_y, 2) + std::pow(m_z, 2));
	}

	~Coordinates() noexcept = default;
};