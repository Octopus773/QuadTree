//
// Created by cbihan on 20/08/2021.
//

#pragma once

namespace QuadTree
{

	//! @brief Provides an indexed free list with constant-time removals from anywhere
	//! in the list without invalidating indices. T must be trivially constructible
	//! and destructible.
	template<class T>
	class FreeList
	{
	public:
		//! @brief Creates a new free list.
		FreeList();

		//! @brief Inserts an element to the free list and returns an index to it.
		int insert(T element);

		//! @brief Removes the nth element from the free list.
		void remove(int n);

		//! @brief Removes all elements from the free list.
		void clear();

		//! @brief Create a vector with all the values of the list
		[[nodiscard]] std::vector<T> toVector() const;

		//! @brief Returns the range of valid indices.
		[[nodiscard]] int range() const;

		//! @brief Gives the index of an element, if not found returns -1
		[[nodiscard]] int findIndex(T element) const;

		//! @brief Returns the nth element.
		T &operator[](int n);

		//! @brief Returns the nth element.
		const T &operator[](int n) const;

		//! @brief call pred with all the active elements
		//! @note pred arguments 1: element ref, 2: index of the element
		//! @note if the pred return false the forEach stop iterating and returns
		void forEach(std::function<bool (T &, int)> pred);

		//! @brief call pred with all the active elements
		//! @note pred argument element const ref
		//! @note if the pred return false the forEach stop iterating and returns
		void forEach(std::function<bool (T &)> pred);

		//! @brief call pred with all the active elements
		//! @note pred arguments 1: element ref, 2: index of the element
		//! @note if the pred return false the forEach stop iterating and returns
		void forEach(std::function<bool (const T &, int)> pred) const;

		//! @brief call pred with all the active elements
		//! @note pred argument element const ref
		//! @note if the pred return false the forEach stop iterating and returns
		void forEach(std::function<bool (const T &)> pred) const;

	private:
		//! @brief The internal vector
		std::vector <std::pair<T, int>> _data{};
		//! @brief first free index
		int _firstFree;
		int _lastFree;
	};

	template<class T>
	FreeList<T>::FreeList()
		: _firstFree(-1),
		  _lastFree(-1)
	{
	}

	template<class T>
	int FreeList<T>::insert(T element)
	{
		if (this->_firstFree != -1) {
			const int index = this->_firstFree;
			this->_firstFree = this->_data[this->_firstFree].second;
			this->_data[index].first = std::move(element);
			return index;
		} else {
			this->_data.push_back({element, -1});
			return static_cast<int>(this->_data.size() - 1);
		}
	}

	template<class T>
	void FreeList<T>::remove(int n)
	{
		// the this->_firstFree index should be the lowest available and so on to use the lowest possible index at each insertion
		// it helps for the forEach function and if we want to shrink the vector
		if (n < this->_firstFree || this->_firstFree == -1) {
			this->_data[n].second = this->_firstFree;
			this->_firstFree = n;
		} else {
			int freeIndex = this->_firstFree;
			int prevFreeIndex = freeIndex;
			while (freeIndex < n && freeIndex != -1) {
				prevFreeIndex = freeIndex;
				freeIndex = this->_data[freeIndex].second;
			}
			this->_data[n].second = freeIndex;
			this->_data[prevFreeIndex].second = n;
		}
	}

	template<class T>
	void FreeList<T>::clear()
	{
		this->_data.clear();
		this->_firstFree = -1;
	}

	template<class T>
	int FreeList<T>::range() const
	{
		return static_cast<int>(this->_data.size());
	}

	template<class T>
	T &FreeList<T>::operator[](int n)
	{
		return this->_data[n].first;
	}

	template<class T>
	const T &FreeList<T>::operator[](int n) const
	{
		return this->_data[n].first;
	}

	template<class T>
	int FreeList<T>::findIndex(T element) const
	{
		int indexFound = -1;
		this->forEach([&element, &indexFound](const auto &elementList, int elementIndex) {
			if (element == elementList) {
				indexFound = elementIndex;
				return false;
			}
			return true;
		});
		return indexFound;
	}

	template<class T>
	void FreeList<T>::forEach(std::function<bool(T &, int)> pred)
	{
		int next_empty_index = this->_firstFree;

		for (int i = 0; i < this->_data.size(); i++) {
			if (i == next_empty_index) {
				next_empty_index = this->_data[i].second;
				continue;
			}
			if (!pred(this->_data[i].first, i)) {
				break;
			}
		}
	}

	template<class T>
	void FreeList<T>::forEach(std::function<bool(T &)> pred)
	{
		this->forEach([&pred](T &element, int) {
			return pred(element);
		});
	}

	template<class T>
	void FreeList<T>::forEach(std::function<bool(const T &, int)> pred) const
	{
		int next_empty_index = this->_firstFree;

		for (int i = 0; i < static_cast<int>(this->_data.size()); i++) {
			if (i == next_empty_index) {
				next_empty_index = this->_data[i].second;
				continue;
			}
			if (!pred(this->_data[i].first, i)) {
				break;
			}
		}
	}

	template<class T>
	void FreeList<T>::forEach(std::function<bool(const T &)> pred) const
	{
		this->forEach([&pred](T &element, int) {
			return pred(element);
		});
	}

	template<class T>
	std::vector<T> FreeList<T>::toVector() const
	{
		std::vector<T> v;
		this->forEach([&v](const auto &element) {
			v.puish_back(element);
		});
		return v;
	}
}