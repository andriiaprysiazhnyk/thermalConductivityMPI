//
// Created by andriiprysiazhnyk on 9/18/19.
//

#include "config_reader.h"
#include <fstream>
#include <algorithm>
#include <iostream>


configuration_t read_config(std::istream &config_file) {
    std::string temp;
    configuration_t res = {};
    res.alpha = 1;

    while (getline(config_file, temp)) {
        temp.erase(std::remove_if(temp.begin(), temp.end(), isspace), temp.end());
        if (temp[0] == '#' || temp.empty())
            continue;
        auto delimiterPos = temp.find('=');
        std::string name = temp.substr(0, delimiterPos);
        std::string value = temp.substr(delimiterPos + 1);
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        value.erase(std::remove(value.begin(), value.end(), '"'), value.end());

        if (name == "dt") {
            res.dt = std::stod(value);
        } else if (name == "dx") {
            res.dx = std::stod(value);
        } else if (name == "dy") {
            res.dy = std::stod(value);
        } else if (name == "density") {
            res.alpha /= std::stod(value);
        } else if (name == "c") {
            res.alpha /= std::stod(value);
        } else if (name == "k") {
            res.alpha *= std::stod(value);
        } else if (name == "m") {
            res.m = std::stoi(value);
        } else if (name == "n") {
            res.n = std::stoi(value);
        } else if (name == "path_to_matrix") {
            res.path_to_matrix = value;
        }
    }

    return res;
}

void read_matrix(std::ifstream &matrix_file, Matrix &matrix, unsigned int begin, unsigned int end) {
    std::string number;
    unsigned int total_read = 0;

    while ((matrix_file >> number) && (total_read < (end + 1) * matrix.n)) {
        total_read += 1;
        if (total_read <= begin * matrix.n) continue;
//        std::cout << number << "\n";
        matrix((total_read - 1) / matrix.n - begin, (total_read - 1) % matrix.n ) = std::stod(number);
    }
}
