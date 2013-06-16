#ifndef AlgheLib_cppex_algo_stack_h__
#define AlgheLib_cppex_algo_stack_h__

#include "../base.h"

namespace cppex {

	namespace algo {

		//
		// A generic
		//
		template<typename T>
		class TStack {
		public:
			explicit TStack(size_t initialCapacity);
			explicit TStack();
			virtual ~TStack();

			virtual void push(const T& newElement);
			virtual const T& peak() const;
			virtual T pop();

			size_t size() const;
			size_t capacity() const;
			
			void clear();

		protected:
			void increaseSizeIfNeeded();

			size_t  _capacity;
			size_t  _n;
			T*      _ptr;

		private:
			// Hide copy-constructor and assignment operator
			TStack (const TStack &);
			TStack & operator= (const TStack &);
		};

		//
		// Implementation for the template class methods
		//
		template<typename T>
		inline void TStack<T>::push(const T& newElement) {
			increaseSizeIfNeeded();
			_ptr[_n++] = newElement;
		}

		template<typename T>
		inline const T& TStack<T>::peak() const {
			ASSERT(NULL != _ptr && _n > 0);
			return _ptr[_n - 1];
		}

		template<typename T>
		inline T TStack<T>::pop() {
			ASSERT(NULL != _ptr && _n > 0);
			return _ptr[-- _n];
		}

		template<typename T>
		size_t TStack<T>::size() const {
			return _n;
		}
		
		template<typename T>
		size_t TStack<T>::capacity() const {
			return _capacity;
		}

		template<typename T>
		inline void TStack<T>::increaseSizeIfNeeded() {
			if(_n < _capacity) {
				return;
			}
			else {
				// increase the size.
				size_t newSize = 64 + _capacity;
				if(_capacity < 16 * 1024) { newSize += _capacity; }
				else { newSize += 16 * 1024; }

				// resize the array and preserve the content
				T* ptr = new T[newSize];
				if(NULL != _ptr) {
					for(size_t i=0; i<_n; i++) {
						ptr[i] = _ptr[i];
					}
					delete [] _ptr;
				}
				_ptr  = ptr;
				_capacity = newSize;
			}
		}

		template<typename T>
		inline void TStack<T>::clear() {
			if(NULL != _ptr) {
				delete [] _ptr;
				_ptr = NULL;
				_capacity = 0;
				_n = 0;
			}
		}

		template<typename T>
		TStack<T>::TStack(size_t initialCapacity) 
			: _ptr(new T[initialCapacity]), _capacity(initialCapacity), _n(0) {
		}
		
		template<typename T>
		TStack<T>::TStack() 
			: _ptr(NULL), _capacity(0), _n(0) {
		}
		
		template<typename T>
		TStack<T>::~TStack() {
			clear();
		}
	}

}

#endif
