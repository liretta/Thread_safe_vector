#include "Threads_vector.hpp"
#include <thread>
#include <iostream>

const int OPERATION_QUANTITY = 40;
const int MAX_THREAD_QUANTITY = 10;

template<typename T>
void rem(thread_vector<T>& vc)
{
	for(auto i = 0; i<OPERATION_QUANTITY; ++i)
	{
		if(vc.size()<0)
		{
			vc.erase(vc.size() - 1);
		}
	}
};

template<typename T>
void add(thread_vector<T> &vc)
{
	for(auto i= 0; i< OPERATION_QUANTITY; ++i)
	{
		vc.push_back(i * 2); //random idea, just for checking
	}
};

void test_func();
int main()
{
	test_func();
	system("pause");
	return 0;
}

void test_func()
{
	std::vector <std::thread> for_add, for_remove;
	thread_vector<int> vc(3);
	for(int i = 0; i < vc.size(); ++i)
	{
		vc[i] = 0;
	}

	for(auto i = 0; i<MAX_THREAD_QUANTITY; ++i)
	{
		for_add.push_back(std::move(std::thread(add<int>, std::ref(vc))));
		for_remove.push_back(std::move(std::thread(rem<int>, std::ref(vc))));
	}

	for (auto &it: for_add)
	{
		if (it.joinable())
		{
			it.join();
		}
	}

	for(auto &it:for_remove)
	{
		if(it.joinable())
		{
			it.join();
		}
	}

	std::cout << "Result vector:\n";
	for(size_t i = 0; i< vc.size(); ++i)
	{
		std::cout << i << ". " << vc[i] << std::endl;
	}
};