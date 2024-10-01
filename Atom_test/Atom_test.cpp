
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>
#include <mutex>

std::mutex mutex;

int main()
{
	setlocale(LC_CTYPE, "Polish");

	std::atomic<int> a{ 0 };
	int b{ 0 };
	int c{ 0 };

	std::vector<std::thread> workers(std::thread::hardware_concurrency());
	std::for_each(begin(workers), end(workers), [&](std::thread& worker)
		{
			worker = std::thread([&]()
				{
					for (int i{ 0 }; i < 1'000'000; ++i)
					{
						++a;
						++b;
						{
							std::unique_lock<std::mutex> lock(mutex);
							++c;
						}
						std::this_thread::sleep_for(std::chrono::nanoseconds(1));
					}
				});
		});

	std::for_each(begin(workers), end(workers), [&](std::thread& worker)
		{
			if (worker.joinable())
			{
				worker.join();
			}
		});

	std::cout << "Wartość zmiennej       mutex: " << c << std::endl;
	std::cout << "Wartość zmiennej    atomowej: " << a << std::endl;
	std::cout << "Wartość zmiennej nieatomowej: " << b << std::endl;
	std::cout << "--------------------------------------" << std::endl;
}

