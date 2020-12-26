#define _CRT_SECURE_NO_WARNINGS

#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
//#include <ctime>
//#include <vector>

using namespace std;

struct timeval {
	unsigned int tv_usec;
	unsigned long long tv_sec;
};

int main(int argc, char* argv[]) {
	//names of input and output files
	string input_path = argv[2];
	string output_path = argv[4];

	size_t file_length;
	//open input file
	ifstream input_file(input_path, ios::binary | ios::in);
	if (!input_file.is_open())
		cerr << "Error opening input file." << endl;

	//open output file
	ofstream output_file(output_path, ios::out);
	if (!output_file.is_open())
		cerr << "Error opening output file." << endl;

	//find file lenth
	input_file.seekg(0, input_file.end);
	file_length = input_file.tellg();
	input_file.seekg(0, input_file.beg);

	struct timeval tv;
	char* command_word_binary = new char[2];
	unsigned short command_word_int;
	char* tv_usec_binary = new char[4];
	//char* tv_usec_txt = new char[4];
	char* tv_sec_binary = new char[8];
	char* words_binary;
	unsigned short* words_int;
	char* N_binary = new char[2];
	unsigned short N_int;
	char* buffer = new char[1];
	size_t counter_m = 0;
	size_t counter_s = 0;
	size_t file_pos = 0;
	size_t size = 0;
	size_t buffer_pos = 0;

	while (counter_m != file_length) {
		//read command_word
		input_file.read(command_word_binary, 2);
		memcpy(&command_word_int, &command_word_binary[0], 2);
		size = command_word_int >> 10;
		counter_s += 2;

		//read tv_usec
		input_file.read(tv_usec_binary, 4);
		memcpy(&tv.tv_usec, &tv_usec_binary[0], 4);
		counter_s += 4;

		//read words
		words_binary = new char[2 * size];
		words_int = new unsigned short[size];
		input_file.read(words_binary, 2 * size);
		memcpy(words_int, &words_binary[0], 2 * size);
		counter_s += 2 * size;

		//read tv_sec
		input_file.read(tv_sec_binary, 8);
		memcpy(&tv.tv_sec, &tv_sec_binary[0], 8);
		counter_s += 8;

		//read N
		input_file.read(N_binary, 2);
		memcpy(&N_int, &N_binary[0], 2);
		counter_s += 2;
		if (N_int - 5 != size + 2)
			cerr << "Error in message #" << counter_m << " size!" << endl;
		
		//converting to txt
		buffer_pos = sprintf(buffer, "%lld seconds ", tv.tv_sec);
		buffer_pos += sprintf(buffer + buffer_pos, "%d microseconds\n", tv.tv_usec);
		buffer_pos += sprintf(buffer + buffer_pos, "%d\n", N_int - 5);
		for (size_t i = 0; i < size; i++) {
			buffer_pos += sprintf(buffer + buffer_pos, "0x%04x\n", words_int[i]);
		}

		//writing to file;
		output_file.write(buffer, buffer_pos);

		counter_m++;
	}

	input_file.close();
	output_file.close();
	return 0;
}