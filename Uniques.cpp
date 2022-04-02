#include <chrono>
#include <iostream>

#include <vector>
#include <algorithm>

//#define PRINT_DEBUG_OUTPUT

void PrintContainer(const std::vector<int>& container)
{
	for (size_t j = 0; j < container.size(); j++) {
		std::cout << container[j] << " ";
	}
	std::cout << std::endl;
}

void PrintContainers(const int& k, const std::vector<int>* const containers)
{
	std::cout << "i\t[n]:" << std::endl;
	for (size_t i = 0; i < k; i++) {
		std::cout << i << "\t" << "[" << containers[i].size() << "]" << ":" << "\t";
		PrintContainer(containers[i]);
	}
}

int main(int argc, char** argv)
{
	constexpr size_t k = 100;
	constexpr size_t n_min = 100;
	constexpr size_t n_max = 200;
	//constexpr size_t k_huge = 5;     //TODO implement those rare objects with
	//constexpr size_t n_huge = 10000; //TODO a nuge number of entries
	constexpr int x_min = 0;
	constexpr int x_max = 20;

	// Timestamping
	const auto timestamp0 = std::chrono::steady_clock::now();

	// Input generation - no sorting on generation
	std::vector<int> inputs[k];
	for (size_t i = 0; i < k; i++) {
		const size_t n = rand() % (n_max - n_min) + n_min;
		inputs[i].resize(n);
		for (size_t j = 0; j < n; j++) {
			inputs[i][j] = rand() % (x_max - x_min) + x_min;
		}
	}

	// Timestamping
	const auto timestamp1 = std::chrono::steady_clock::now();
	const std::chrono::duration<double> diff_time_01 = timestamp1 - timestamp0;
	const double dt01 = 1000.0 * diff_time_01.count();
	std::cout << "Input generation: dt01=" << dt01 << std::endl;

	// Debug output
#ifdef PRINT_DEBUG_OUTPUT
	PrintContainers(k, inputs);
#endif

	const auto timestamp2 = std::chrono::steady_clock::now();

	// Copy, sort, remove _consecutive_ uniques, merge all containers into one
	std::vector<int> sum;// TODO preallocate some size
	for (size_t i = 0; i < k; i++) {
		// Timestamping
		//const auto timestamp_i_start = std::chrono::steady_clock::now();
		// Copy
		std::vector<int> tmp_i = inputs[i]; // i-th temporary container
		// Sort
		std::sort(tmp_i.begin(), tmp_i.end());
		// Remove _consecutive_ uniques
		const auto new_end = std::unique(tmp_i.begin(), tmp_i.end());
		tmp_i.erase(new_end, tmp_i.end()); // std::unique leaves a tail which needs to be trimmed
		// Merge all containers into one
		sum.insert(sum.end(), tmp_i.cbegin(), tmp_i.cend());
		// Timestamping
		//const auto timestamp_i_end = std::chrono::steady_clock::now();
		//const std::chrono::duration<double> diff_time_i = timestamp_i_end - timestamp_i_start;
		//const double dti = 1000.0 * diff_time_i.count();
		//std::cout << i << ": dti=" << dti << std::endl;
		// Debug output
#ifdef PRINT_DEBUG_OUTPUT
		std::cout << i << "\t" << "[" << tmp_i.size() << "]" << ":" << "\t";
		PrintContainer(tmp_i);
#endif
	}

#ifdef PRINT_DEBUG_OUTPUT
	// Debug output
	PrintContainer(sum);
#endif

	// Sort, remove _consecutive_ uniques
	{
		// Timestamping
		//const auto timestamp_i_start = std::chrono::steady_clock::now();
		// Sort
		std::sort(sum.begin(), sum.end());
		// Remove _consecutive_ uniques
		const auto new_end = std::unique(sum.begin(), sum.end());
		sum.erase(new_end, sum.end()); // std::unique leaves a tail which needs to be trimmed
		// Timestamping
		//sconst auto timestamp_i_end = std::chrono::steady_clock::now();
		//sconst std::chrono::duration<double> diff_time_i = timestamp_i_end - timestamp_i_start;
		//sconst double dti = 1000.0 * diff_time_i.count();
		//sstd::cout << "sum sorting: dti=" << dti << std::endl;
	}

	// Timestamping
	const auto timestamp3 = std::chrono::steady_clock::now();
	const std::chrono::duration<double> diff_time_23 = timestamp3 - timestamp2;
	const double dt23 = 1000.0 * diff_time_23.count();
	std::cout << "Finding uniques over all containers: dt23=" << dt23 << std::endl;

	// Debug output
	//PrintContainer(sum);

	return 0;
}
