#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

char* read_from_binary_file(const string input_path, size_t& file_length);
bool convert_and_write_to_file(const string output_path, char* message_binary, size_t& file_length);
string bin_to_hex(char bin);
