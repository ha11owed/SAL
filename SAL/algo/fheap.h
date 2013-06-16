#ifndef AlgheLib_cppex_algo_fheap_h__
#define AlgheLib_cppex_algo_fheap_h__

#include "../base.h"
#include "stack.h"
#include "../util/list.h"

namespace cppex {

	namespace algo {

		namespace detail {

			template<typename T>
			class TFibonacciHeapListElem {
			public:
				LIST_LINK(TFibonacciHeapListElem) m_link;

				T data;
			};

		}

		// Generic binary heap.
		//
		// T is the type of the element hold by the heap.
		// P is a functuator used to compare two T elements.
		template<typename T, typename P = std::less<T>>
		class TFibonacciHeap {
		public:
			explicit TFibonacciHeap(size_t initialCapacity);
			explicit TFibonacciHeap();

			void heapify(T* data, size_t n);

			virtual void push(const T& newElement);
			virtual const T& peak() const;
			virtual T pop();
			
		protected:
			void downHeap(int pos);
			void upHeap(int pos);
		};

		//
		// Implementation for the template class methods
		//
		template<typename T, typename P>
		inline void TFibonacciHeap<T, P>::push(const T& newElement) {
			TStack<T>::increaseSizeIfNeeded();
			_ptr[_n] = newElement;
			// Consolidate Heap
			upHeap(_n++);
		}

		template<typename T, typename P>
		inline const T& TFibonacciHeap<T, P>::peak() const {
			ASSERT(NULL != _ptr && _n > 0);
			return _ptr[0];
		}

		template<typename T, typename P>
		inline T TFibonacciHeap<T, P>::pop() {
			ASSERT(NULL != _ptr && _n > 0);
			// Get the max/min and save the last element as root
			T max = _ptr[0];
			_ptr[0] = _ptr[--_n];
			// Consolidate Heap
			downHeap(0);
			return max;
		}

		template<typename T, typename P>
		inline void TFibonacciHeap<T, P>::downHeap(int pos) {
			size_t son = 2 * pos + 1; // child

			while(son < _n) {
				size_t min = pos;
				if(P()(_ptr[min], _ptr[son])) {
					min = son;
				}
				if(son + 1 < _n && P()(_ptr[min], _ptr[son + 1])) {
					min = son + 1; // other child
				}
				if(min != pos) {
					swap(_ptr[min], _ptr[pos]);
					pos = min;
					son = 2 * pos + 1; // child
				}
				else {
					break;
				}
			}
		}

		template<typename T, typename P>
		inline void TFibonacciHeap<T, P>::upHeap(int pos) {
			T temp  = _ptr[pos];
			int p = (pos - 1) / 2; // parent
			while(pos > 0 && P()(_ptr[p], temp)) {
				_ptr[pos] = _ptr[p];
				pos = p;
				p = (pos - 1) / 2; // parent
			}
			_ptr[pos] = temp;
		}

		template<typename T, typename P>
		TFibonacciHeap<T, P>::TFibonacciHeap(size_t initialCapacity) 
			: TStack<T>() {
				_ptr = new T[initialCapacity];
				_capacity = initialCapacity;
		}
		
		template<typename T, typename P>
		TFibonacciHeap<T, P>::TFibonacciHeap() 
			: TStack<T>() {
		}
	}

}

#endif
