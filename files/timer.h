#pragma once

#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>

class TimerException : public std::exception
{
private:
	std::string m_error;
public:
	TimerException(const std::string& error) noexcept : std::exception(error.c_str()), m_error(error)
	{}
	~TimerException() noexcept = default;

	const char* what() const noexcept override
	{
		return m_error.c_str();
	}
};


template <typename Duration = std::chrono::microseconds >
class Timer
{
public:
	using clock_t = std::chrono::steady_clock;
	using time_point_t = clock_t::time_point;
	using duration_t = Duration;

	auto current_time() const noexcept
	{
		return std::chrono::duration_cast <duration_t> (clock_t::now() - m_begin);
	}

	explicit Timer(const std::string& name) : m_begin(clock_t::now()), m_name(name)
	{}

	void pause()
	{
		if (stop_point || paused)
			throw TimerException("Cannot be paused.");
		m_res += current_time();
		paused = true;
	}

	void restart()
	{
		m_begin = clock_t::now();
		m_res = duration_t::zero();
		paused = false;
		stop_point = false;
	}

	void unpause()
	{
		if (stop_point || !paused)
			throw TimerException("Cannot be unpaused.");
		m_begin = clock_t::now();
		paused = false;
	}

	void stop()
	{
		if (stop_point)
			throw TimerException("Cannot be stopped.");
		stop_point = true;
		m_res += current_time();
	}


	~Timer() noexcept
	{
		try
		{
			if (!stop_point)
			{
				m_res += current_time();
			}

			std::cout << std::setw(20) << std::left << m_name << std::setw(20) << m_res.count() << std::endl;
		}
		catch (...)
		{
			std::cerr << "error";
		}

	}

private:
	time_point_t m_begin;
	duration_t m_res = duration_t::zero();
	bool stop_point = false;
	bool paused = false;
	std::string m_name;
};