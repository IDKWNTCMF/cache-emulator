#pragma once

#include <vector>

int cur_time = 0;

const unsigned int size = 1024 * 1024;
double x[size];
double y[size];
double z[size];
double xx[size];
double yy[size];
double zz[size];

const size_t size_of_one_cache_line = 64;
const size_t doubles_in_one_cache_line = size_of_one_cache_line / sizeof(double);

class Memory {
public:
    Memory() {
        for (int i = 0; i < size; i++) {
            data[i] = x[i];
        }
        for (int i = 0; i < size; i++) {
            data[size + i] = y[i];
        }
        for (int i = 0; i < size; i++) {
            data[2 * size + i] = z[i];
        }
        for (int i = 0; i < size; i++) {
            data[3 * size + i] = xx[i];
        }
        for (int i = 0; i < size; i++) {
            data[4 * size + i] = yy[i];
        }
        for (int i = 0; i < size; i++) {
            data[5 * size + i] = zz[i];
        }
    }

    static size_t get_line_address(size_t address) {
        return address - (address & (doubles_in_one_cache_line - 1));
    }

    std::vector<double> read(size_t address) {
        size_t line_address = get_line_address(address);
        std::vector<double> line(doubles_in_one_cache_line);
        for (int i = 0; i < doubles_in_one_cache_line; i++) {
            line[i] = data[line_address + i];
        }
        return line;
    }

    void write(size_t address, const std::vector<double> &new_data) {
        size_t line_address = get_line_address(address);
        for (int i = 0; i < doubles_in_one_cache_line; i++) {
            data[line_address + i] = new_data[i];
        }
    }
private:
    double data[6 * size]{};
};

Memory RAM;