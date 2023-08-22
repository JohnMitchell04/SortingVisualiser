#include "Sorter.h"

namespace Sorting {
	std::vector<int> Sorter::GetData() const {
		std::vector<int> output = m_sortedList;
		std::copy(m_unsortedList.begin(), m_unsortedList.end(), std::back_inserter(output));
		return output;
	}

	void Sorter::RandomiseData() {
		// Clear previously sorted data and reset counters
		m_sortedList.clear();
		m_comparisonCount = 0;
		m_arrayAccesses = 0;

		// Create a list with integers 1-100
		for (int i = 1; i < 301; i++) {
			m_unsortedList.push_back(i);
		}

		// Shuffle list
		std::shuffle(m_unsortedList.begin(), m_unsortedList.end(), m_randomGenerator);

		// Class is now sorting
		m_isSorting = true;
	}

	void Sorter::NextStep() {
		switch (m_currentAlgorithm) {
		case Selection:
			SelectionStep();
			break;
		}
	}

	void Sorter::SelectionStep() {
		// Assume first element of unsorted data is smallest
		int smallest = m_unsortedList[0];
		m_arrayAccesses++;
		size_t smallestPos = 0;

		// Loop through unsorted data looking for smaller value
		for (size_t i = 1; i < m_unsortedList.size(); i++) {
			if (m_unsortedList[i] < smallest) {
				smallest = m_unsortedList[i];
				m_arrayAccesses++;
				smallestPos = i;
			}

			m_arrayAccesses++;
			m_comparisonCount++;
		}

		// Add smallest value to end of sorted list
		m_sortedList.push_back(smallest);
		m_arrayAccesses++;

		// Delete the newly sorted value form unsorted list
		m_unsortedList.erase(m_unsortedList.begin() + smallestPos);

		// Data has been fully sorted
		if (m_unsortedList.size() == 0) { m_isSorting = false; }
	}
}