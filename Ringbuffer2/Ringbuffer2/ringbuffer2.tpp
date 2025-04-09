#pragma once

#include "ringbuffer.h"

template <typename T>
void RingBuffer<T>::push(const T& item) 
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_not_full.wait(lock, [this]() { return !full(); });

	m_buffer[head_] = item;
	head_ = (head_ + 1) % m_capacity;
	++size_;

	m_not_empty.notify_one();
}

template <typename T>
T RingBuffer<T>::pop() 
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_not_empty.wait(lock, [this]() { return !empty(); });

	T item = m_buffer[tail_];
	tail_ = (tail_ + 1) % m_capacity;
	--size_;
	return item;

	m_not_full.notify_one();
}

template <typename T>
size_t RingBuffer<T>::size() const 
{
	return m_size;
}

template <typename T>
bool RingBuffer<T>::full() const 
{
	return m_size == m_capacity;
}

template <typename T>
bool RingBuffer<T>::empty() const 
{
	return m_size == 0;
}

template <typename T>
size_t RingBuffer<T>::capacity() const noexcept 
{
	return m_capacity;
}
