#include "read-write.h";

int main(int argc, char* argv[]) {
	string input_path = argv[2];
	string output_path = argv[4];
	size_t file_length;

	char* message_binary = read_from_binary_file(input_path, file_length);
	if ( message_binary[0] == NULL )
		return 1;

	if (!convert_and_write_to_file(output_path, message_binary, file_length))
		return 2;

	delete[] message_binary;
	return 0;
}