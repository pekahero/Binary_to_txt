#include "read-write.h";

char* read_from_binary_file(const string input_path, size_t& file_length) {
    ifstream input_file(input_path, ios::binary | ios::in);
    if (!input_file.is_open())
        cerr << "Error opening input file." << endl;

    input_file.seekg(0, input_file.end);
    file_length = input_file.tellg();
    input_file.seekg(0, input_file.beg);

    char* message_binary = new char[file_length];

    input_file.read(message_binary, file_length);

    input_file.close();
    return message_binary;
}

bool convert_and_write_to_file(const string output_path, char* message_binary, size_t& file_length) {
    ofstream output_file(output_path, ios::out);
    if (!output_file.is_open()) {
        cerr << "Error opening output file." << endl;
        return false;
    }
    string command_word[2] = { bin_to_hex(message_binary[0]), bin_to_hex(message_binary[1]) };
    size_t length_counter = file_length;

    string message_txt;
    string tmp;
    string time_1;
    string time_2;
    int size;
    for (size_t i = 2; i < file_length; i += 2) { 
        string word_1 = bin_to_hex(message_binary[i]);
        string word_2 = bin_to_hex(message_binary[i + 1]);
        tmp += "0x" + word_1 + word_2 + "\n";
        if (word_1 == command_word[0]) {
            if (word_2 == command_word[1]) {
                size = size_t(message_binary[i - 2]);

                for (int j = 0; j < 4; j++)
                    time_1 += bin_to_hex(message_binary[i - 2 * size + j]);

                for (int j = 10; j > 2; j--)
                    time_2 += bin_to_hex(message_binary[i - j]);               

                tmp = tmp.substr(14, tmp.length() - 56);

                message_txt += time_1 + time_2 + "\n" + to_string(size - 5) + "\n" + tmp + "\n";

                output_file.write((char*)message_txt.c_str(), message_txt.length());
                cout << message_txt;

                message_txt.clear();
                tmp.clear();
                time_1.clear();
                time_2.clear();
            }
        }
    }
    
    //output_file.write((char*)message_txt.c_str(), message_txt.length());

    output_file.close();
    return true;
}

string bin_to_hex(char bin) {
    static string hex_digits[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
    const unsigned char mask_1 = 0xF0;
    const unsigned char mask_2 = 0x0F;

    return hex_digits[(mask_1 & bin) >> 4] + hex_digits[mask_2 & bin];
}