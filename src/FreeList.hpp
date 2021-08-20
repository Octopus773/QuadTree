//
// Created by cbihan on 20/08/2021.
//

#pragma once

namespace QuadTree
{

	/// Provides an indexed free list with constant-time removals from anywhere
	/// in the list without invalidating indices. T must be trivially constructible
	/// and destructible.
	template<class T>
	class FreeList
	{
	public:
		/// Creates a new free list.
		FreeList();

		/// Inserts an element to the free list and returns an index to it.
		int insert(const T &element);

		// Removes the nth element from the free list.
		void erase(int n);

		// Removes all elements from the free list.
		void clear();

		// Returns the range of valid indices.
		[[nodiscard]] int range() const;

		// Returns the nth element.
		T &operator[](int n);

		// Returns the nth element.
		const T &operator[](int n) const;

	private:
		union FreeElement
		{
			T element;
			int next;
		};
		std::vector <FreeElement> _data{};
		int _first_free;
	};

	template<class T>
	FreeList<T>::FreeList()
		: _first_free(-1)
	{
	}

	template<class T>
	int FreeList<T>::insert(const T &element)
	{
		if (this->_first_free != -1) {
			const int index = this->_first_free;
			this->_first_free = this->_data[this->_first_free].next;
			this->_data[index].element = element;
			return index;
		} else {
			FreeElement fe;
			fe.element = element;
			this->_data.push_back(fe);
			return static_cast<int>(this->_data.size() - 1);
		}
	}

	template<class T>
	void FreeList<T>::erase(int n)
	{
		this->_data[n].next = this->_first_free;
		this->_first_free = n;
	}

	template<class T>
	void FreeList<T>::clear()
	{
		this->_data.clear();
		this->_first_free = -1;
	}

	template<class T>
	int FreeList<T>::range() const
	{
		return static_cast<int>(this->_data.size());
	}

	template<class T>
	T &FreeList<T>::operator[](int n)
	{
		return this->_data[n].element;
	}

	template<class T>
	const T &FreeList<T>::operator[](int n) const
	{
		return this->_data[n].element;
	}
}