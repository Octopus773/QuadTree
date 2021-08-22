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
		//! @brief Creates a new free list.
		FreeList();

		//! @brief Inserts an element to the free list and returns an index to it.
		int insert(T element);

		//! @brief Removes the nth element from the free list.
		void erase(int n);

		//! @brief Removes all elements from the free list.
		void clear();

		//! @brief Returns the range of valid indices.
		[[nodiscard]] int range() const;

		//! @brief Gives the index of an element, if not found returns -1
		[[nodiscard]] int findIndex(T element) const;

		//! @brief Returns the nth element.
		T &operator[](int n);

		//! @brief Returns the nth element.
		const T &operator[](int n) const;

		//! @brief call pred with all the active elements
		void for_each(std::function<void (T &)> pred);

	private:
		/*
		union FreeElement
		{
			T element;
			int next;
		}; */
		std::vector <std::pair<T, int>> _data{};
		//! @brief first free index
		int _first_free;
	};

	template<class T>
	FreeList<T>::FreeList()
		: _first_free(-1)
	{
	}

	template<class T>
	int FreeList<T>::insert(T element)
	{
		if (this->_first_free != -1) {
			const int index = this->_first_free;
			this->_first_free = this->_data[this->_first_free].second;
			this->_data[index].first = std::move(element);
			return index;
		} else {
			this->_data.push_back({element, -1});
			return static_cast<int>(this->_data.size() - 1);
		}
	}

	template<class T>
	void FreeList<T>::erase(int n)
	{
		if (this->_first_free != -1 && this->_first_free < n) {
			this->_data[n].second = this->_data[this->_first_free].second;
			this->_data[this->_first_free].second = n;
		} else {
			this->_data[n].second = this->_first_free;
			this->_first_free = n;
		}
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
		auto it = std::find_if(this->_data.begin(), this->_data.end(), [&element](const auto &elementList) {
			return element == elementList.first;
		});
		return it == this->_data.end() ? -1 : it - this->_data.begin();
	}

	template<class T>
	void FreeList<T>::for_each(std::function<void(T &)> pred)
	{
		int next_empty_index = this->_first_free;

		for (int i = 0; i < this->_data.size(); i++) {
			if (i == next_empty_index) {
				next_empty_index = this->_data[i].second;
				continue;
			}
			pred(this->_data[i].first);
		}
	}
}