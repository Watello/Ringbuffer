#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "ringbuffer.h"

void reader_thread(std::stop_token stop_token, RingBuffer<char>& buffer, int delay)
{
	while (!stop_token.stop_requested())
	{
		std::cout << buffer.pop();
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
}

void keyboard_thread(std::stop_token stop_token, RingBuffer<char>& buffer, int delay)
{
	std::string input;
	while (!stop_token.stop_requested())
	{
		std::getline(std::cin, input);
		if (input == "exit")
		{
			return;
		}

		for (char c : input)
		{
			buffer.push(c);
			std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
	}
}

void writer_thread(std::stop_token stop_token, RingBuffer<char>& buffer, const std::string& data, int delay)
{
	for (char c : data)
	{
		if (stop_token.stop_requested())
		{
			return;
		}
		buffer.push(c);
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	}
}

int main()
{
	RingBuffer<char> buffer(10);

	std::jthread reader(reader_thread, std::ref(buffer), 100);
	std::jthread writer(writer_thread, std::ref(buffer), "0123456789", 100);
	std::jthread keyboard(keyboard_thread, std::ref(buffer), 100);

	keyboard.join();

	writer.request_stop();
	write.join();

	reader.request_stop();
	reader.join();

	return 0;
}
