#pragma once
#include <vector>
#include <iterator>
#include <mutex>

template<typename T>
class thread_vector
{
private:
	std::vector <T> m_vector; //for data
	mutable std::mutex m_mut; //without mutable we can't use mutex inside const method as operator[] const 

public:
	explicit thread_vector(int n_size = 0)
	{
		if (n_size > 0)
		{
			m_vector.resize(n_size);
		}
	};

	thread_vector(const thread_vector& other)
	{
		std::lock_guard<std::mutex> lg(m_mut);
		m_vector = other.m_vector;
	};

	thread_vector(thread_vector&& other_vector)
	{
		std::lock_guard<std::mutex> lg(m_mut);
		m_vector = std::move(other_vector.m_vector);
	};

	~thread_vector()
	{
		std::lock_guard<std::mutex> lg(m_mut);
		m_vector.clear();
	};

	thread_vector& operator=(thread_vector const& other_vector)
	{
		std::lock_guard<std::mutex> lg(m_mut);
		m_vector = other_vector;
		return *this;
	};

	thread_vector& operator=(thread_vector &&other_vector)
	{
		if(&other_vector!=this)
		{
			std::lock_guard<std::mutex> lg(m_mut);
			m_vector = std::move(other_vector.m_vector);

		}
	};

	T& operator[](int index) 
	{
		std::lock_guard<std::mutex> lg(m_mut);
		if(index < 0 || index>=static_cast<int>(m_vector.size()))
		{
			throw std::out_of_range("Index is out of range");
		}
		return m_vector[index];
	};

	T& begin() const
	{
		std::lock_guard<std::mutex> lg(m_mut);
		if(m_vector.size()==0)
		{
			throw std::out_of_range("Vector is empty");
		}
		return m_vector.begin();
	};

	auto end() 
	{
		std::lock_guard<std::mutex> lg(m_mut);
		if (m_vector.size() == 0)
		{
			throw std::out_of_range("Vector is empty");
		}
		return m_vector.end();
	};

	void clear()
	{
		std::lock_guard<std::mutex> lg(m_mut);
		m_vector.clear();
	};

	void erase(const int index)
	{
		std::lock_guard<std::mutex> lg(m_mut);
		if (index < 0 || index >= m_vector.size())
		{
			throw std::out_of_range("Index is out of range");
		}
		m_vector.erase(m_vector.begin() + index);
		std::vector<T>(m_vector).swap(m_vector);
	};

	void push_back(T const &new_value)
	{
		std::lock_guard<std::mutex> lg(m_mut);
		m_vector.push_back(new_value);
	};

	void swap(thread_vector& other)
	{
		std::lock_guard<std::mutex> lg(m_mut);
		std::swap(m_vector, other.m_vector);
	};

	size_t size()
	{
		std::lock_guard<std::mutex> lg(m_mut);
		return m_vector.size();
	};

};
