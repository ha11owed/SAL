// SAL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "algo/heap.h"

using namespace cppex::algo;

void stackTests() {
	TStack<int> stackTest;
	int v;

	ASSERT(stackTest.size() == 0);
	stackTest.push(12);
	ASSERT(stackTest.size() == 1);
	v = stackTest.peak();
	ASSERT(v == 12 && stackTest.size() == 1);
	v = stackTest.pop();
	ASSERT(v == 12 && stackTest.size() == 0);
}

template<typename T>
void testSpeed(int n, const T& insertedValue) {
	DWORD time1, time2;
	T v = insertedValue;

	TStack<T> stackTest;
	std::stack<T> stdStack;
	std::vector<T> stdVector;
	std::list<T> stdList;

	// My stack
	time1 = GetTickCount();
	for(int i=0; i<n; i++) {
		for(int j = 0; j<i*2; j++) {
			stackTest.push(v);
		}
		for(int j = 0; j<i; j++) {
			stackTest.pop();
		}
	}
	time2 = GetTickCount();
	std::cout << "Diff: " << time2 - time1 << std::endl;

	// STD stack
	time1 = GetTickCount();
	for(int i=0; i<n; i++) {
		for(int j = 0; j<i*2; j++) {
			stdStack.push(v);
		}
		for(int j = 0; j<i; j++) {
			stdStack.pop();
		}
	}
	time2 = GetTickCount();
	std::cout << "Diff: " << time2 - time1 << std::endl;

	// STD vector
	time1 = GetTickCount();
	for(int i=0; i<n; i++) {
		for(int j = 0; j<i*2; j++) {
			stdVector.push_back(v);
		}
		for(int j = 0; j<i; j++) {
			stdVector.pop_back();
		}
	}
	time2 = GetTickCount();
	std::cout << "Diff: " << time2 - time1 << std::endl;

	// STD list
	time1 = GetTickCount();
	for(int i=0; i<n; i++) {
		for(int j = 0; j<i*2; j++) {
			stdList.push_back(v);
		}
		for(int j = 0; j<i; j++) {
			stdList.pop_back();
		}
	}
	time2 = GetTickCount();
	std::cout << "Diff: " << time2 - time1 << std::endl;
}


template<typename T>
void testHeapCorrectness(T* values, size_t n) {
	DWORD time1, time2;

	T v;

	THeap<T> myHeap(n);
	std::priority_queue<T> stdHeap;

	for(size_t i=0; i<n; i++) {
		for(size_t j = 0; j<i; j++) {
			myHeap.push(values[j]);
			stdHeap.push(values[j]);
		}
		std::cout << std::endl;
		for(size_t j = 1; j<i; j++) {
			v = myHeap.pop();
			if(stdHeap.top() != v) {
				myHeap.debug();
				std::cout << "WTF\n";
			}
			v = stdHeap.top();
			stdHeap.pop();
		}
		std::cout << std::endl;
	}
	for(size_t i=0; i<n - 1; i++) {
		v = myHeap.pop();
		if(stdHeap.top() != v) {
			myHeap.debug();
			std::cout << "WTF\n";
		}
		v = stdHeap.top();
		stdHeap.pop();
	}
}

template<typename T>
void testHeapSpeed(size_t n) {
	DWORD time1, time2;

	T v;

	THeap<T> myHeap(n*n);
	std::priority_queue<T> stdHeap;

	time1 = GetTickCount();
	for(size_t i=0; i<n; i++) {
		for(size_t j = 0; j<i; j++) {
			myHeap.push(j * i % 5000);
		}
		for(size_t j = 1; j<i; j++) {
			myHeap.pop();
		}
	}
	for(size_t i=0; i<n - 1; i++) {
		myHeap.pop();
	}
	time2 = GetTickCount();
	std::cout << "Diff: " << time2 - time1 << std::endl;


	time1 = GetTickCount();
	for(size_t i=0; i<n; i++) {
		for(size_t j = 0; j<i; j++) {
			stdHeap.push(j * i % 5000);
		}
		for(size_t j = 1; j<i; j++) {
			stdHeap.pop();
		}
	}
	for(size_t i=0; i<n - 1; i++) {
		stdHeap.pop();
	}
	time2 = GetTickCount();
	std::cout << "Diff: " << time2 - time1 << std::endl;


}

int _tmain(int argc, TCHAR* argv[])
{
	DWORD time1, time2;
	
	int values[] = { 12, 234, 2, 123, 83, 9, 26, 51, 93, 10, 78, 25, 3, 7, 173 };
	testHeapSpeed<int>(10000);



	std::cout << std::endl;
	char waitC;
	std::cin >> waitC;
	return 0;
}

