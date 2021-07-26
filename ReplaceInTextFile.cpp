#include <fstream>
#include <iostream>

void ReplaceInPlace(std::string& buffer,
	const char placeholder[], const char replacement[])
{
	const size_t placeholder_size = strlen(placeholder);
	size_t i = buffer.find(placeholder);
	while (i != std::string::npos) {
		buffer.replace(i, placeholder_size, replacement);
		i = buffer.find(placeholder);
	}
}

// return 0 - success
// return 1 - failure
int Process(const char template_filename[], const char output_filename[],
	const char placeholder[], const char replacement[])
{
	// Open
	std::ifstream input(template_filename);
	if (!input.is_open()) {
		std::cerr << "Couldn't open input template '"
			<< template_filename << "'." << std::endl;
		// FAILURE
		return 1;
	}
	std::ofstream output(output_filename);
	if (!output.is_open()) {
		std::cerr << "Couldn't open output '"
			<< output_filename << "'." << std::endl;
		// FAILURE
		return 1;
	}
	// Get input size
	input.seekg(0, std::ios::end);
	size_t size = input.tellg();
	input.seekg(0);
	// Allocate
	std::string buffer(size, ' ');
	// Read
	input.read(&buffer[0], size);
	// Print
	//std::cout << buffer << std::endl;
	// Replace
	ReplaceInPlace(buffer, placeholder, replacement);
	// Print
	//std::cout << buffer << std::endl;
	// Write
	output << buffer;
	// Close
	input.close();
	output.close();
	// SUCCESS
	return 0;
}

int main(int argc, char** argv)
{
	constexpr char placeholder[] = "$PLACEHOLDER$";
	constexpr char replacement[] = "data";
	constexpr char template_filename[] = "template.txt";
	constexpr char output_filename[] = "output.txt";
	int res = Process(template_filename, output_filename, placeholder, replacement);
	return res;
}
