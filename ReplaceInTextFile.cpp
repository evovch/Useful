#include <fstream>
#include <iostream>

void TrimTrailingSpaces(std::string& buffer)
{
	for (std::string::const_reverse_iterator it = buffer.crbegin(); it != buffer.crend(); it++) {
		if (*it == ' ') {
			continue;
		} else {
			//const size_t newsize = it - buffer.crend();
			const size_t newsize = std::distance(it, buffer.crend());
			buffer.resize(newsize);
			break;
		}
	}
}

void ReplaceInPlace(std::string& buffer,
	const char placeholder[], const std::string& replacement)
{
	const size_t placeholder_size = strlen(placeholder);
	size_t i = buffer.find(placeholder);
	while (i != std::string::npos) {
		buffer.replace(i, placeholder_size, replacement);
		i = buffer.find(placeholder);
	}
}

// The replacement string is passed as an argument directly
// return 0 - success
// return 1 - failure
int Process(const char template_filename[], const char output_filename[],
	const std::string& replacement, const char placeholder[])
{
	// Open
	// input template
	std::ifstream input_file(template_filename);
	if (!input_file.is_open()) {
		std::cerr << "Couldn't open input template '"
			<< template_filename << "'." << std::endl;
		// FAILURE
		return 1;
	}
	// output
	std::ofstream output_file(output_filename);
	if (!output_file.is_open()) {
		std::cerr << "Couldn't open output '"
			<< output_filename << "'." << std::endl;
		// FAILURE
		return 1;
	}

	// Get the input template size
	input_file.seekg(0, std::ios::end);
	const size_t tsize = input_file.tellg();
	input_file.seekg(0);
	// Allocate space for the template
	std::string tbuffer(tsize, ' ');
	// Read the template
	input_file.read(&tbuffer[0], tsize);
	// Trim trailing spaces
	TrimTrailingSpaces(tbuffer);

	// Print
	//std::cout << tbuffer << std::endl;
	// Replace
	ReplaceInPlace(tbuffer, placeholder, replacement);
	// Print
	//std::cout << tbuffer << std::endl;
	// Write
	output_file << tbuffer;
	// Close
	input_file.close();
	output_file.close();
	// SUCCESS
	return 0;
}

// The replacement string is contained in the replacement_filename file
// return 0 - success
// return 1 - failure
int Process2(const char template_filename[], const char output_filename[],
	const char replacement_filename[], const char placeholder[])
{
	// Open
	// replacement
	std::ifstream replacement_file(replacement_filename);
	if (!replacement_file.is_open()) {
		std::cerr << "Couldn't open replacement '"
			<< replacement_filename << "'." << std::endl;
		// FAILURE
		return 1;
	}

	// Get the replacement size
	replacement_file.seekg(0, std::ios::end);
	const size_t rsize = replacement_file.tellg();
	replacement_file.seekg(0);
	// Allocate space for the replacement
	std::string rbuffer(rsize, ' ');
	// Read the replacement
	replacement_file.read(&rbuffer[0], rsize);
	// Trim trailing spaces
	TrimTrailingSpaces(rbuffer);

	// Close
	replacement_file.close();
	// Replace
	int res = Process(template_filename, output_filename, rbuffer, placeholder);
	return res;
}

int main(int argc, char** argv)
{
	constexpr char template_filename[] = "template.txt";
	constexpr char output1_filename[] = "tmp.txt";
	constexpr char output2_filename[] = "output.txt";
	constexpr char replacement1_filename[] = "replacement1.txt";
	constexpr char replacement2[] = "replacement2";
	constexpr char placeholder1[] = "$PLACEHOLDER1$";
	constexpr char placeholder2[] = "$PLACEHOLDER2$";
	int res1 = Process2(template_filename, output1_filename, replacement1_filename, placeholder1);
	if (res1 != 0) return res1;
	int res2 = Process (output1_filename,  output2_filename, replacement2,          placeholder2);
	if (res2 != 0) return res2;
	return 0;
}
