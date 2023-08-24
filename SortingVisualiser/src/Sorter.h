#pragma once

#include <iterator>
#include <vector>
#include <random>
#include <algorithm>

namespace Sorting {
	enum SortingAlgorithms {
		null,
		Selection,
		Insertion
	};

	class Sorter {
	public:
		Sorter() = default;

		void SetAlgorithm(SortingAlgorithms algorithm) { m_currentAlgorithm = algorithm; };

		std::vector<int> GetData() const;
		int GetComparisons() const { return m_comparisonCount; }
		int GetAccesses() const { return m_arrayAccesses; }

		bool IsSorting() const { return m_isSorting; };

		void RandomiseData(unsigned int number);

		void NextStep();

	private:
		void SelectionStep();
		void InsertionStep();

	private:
		SortingAlgorithms m_currentAlgorithm = SortingAlgorithms::null;
		std::mt19937 m_randomGenerator;
		std::vector<int> m_unsortedList;
		std::vector<int> m_sortedList;
		bool m_isSorting = false;
		int m_comparisonCount = 0, m_arrayAccesses = 0;
	};
}