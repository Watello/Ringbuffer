#pragma once

#include <vector>
#include <stdexcept>
#include <utility>

template <typename T>
class RingBuffer {
public:
	explicit Ringbuffer(size_t capacity)
		: m_buffer(capacity),
		m_capacity(capacity),
		head_(0),
		tail_(0),
		size_(0)
	{
	}

	void push(const T& item);
	T pop();

	bool empty() const;
	bool full() const;
	size_t size() const;
	size_t capacity() const noexcept;

private:
	std::vector<T> m_buffer;
	const size_t   m_capacity;
	size_t		   head_;
	size_t		   tail_;
	size_t		   size_;

	mutable std::mutex		 m_mutex;
	std::condition_variable  m_not_empty;
	std::condition_variable  m_not_full;
};

#include "ringbuffer.tpp"	
