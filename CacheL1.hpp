#pragma once

#include "CacheL2.hpp"

class CacheL1 {
public:
    CacheL1(size_t size_in_kb,
            size_t s_a) : size_in_bytes(size_in_kb << 10), set_associativity(s_a) {
        number_of_sets = size_in_bytes / (size_of_one_cache_line * set_associativity);
        sets.resize(number_of_sets, Set(s_a));
        while ((1 << offset) != doubles_in_one_cache_line) {
            offset++;
        }
        while ((1 << index) != number_of_sets) {
            index++;
        }
    }

    double read(size_t address) {
        size_t cur_double_in_cache_line = address & (doubles_in_one_cache_line - 1);
        size_t set_number = (address >> offset) & (number_of_sets - 1);
        size_t tag = (address >> (offset + index));
        std::tuple<bool, std::vector<double> > result = sets[set_number].find(tag);
        if (std::get<0>(result)) {
            /// Cache hit
            cache_hits++;
            std::vector<double> data = std::get<1>(result);
            return data[cur_double_in_cache_line];
        }
        /// Cache miss
        cache_misses++;
        std::vector<double> new_data = L2.read(address);
        std::tuple<bool, Cache_Line> add_result = sets[set_number].add(tag, address, new_data);
        if (std::get<0>(add_result)) {
            Cache_Line removed_cache_line = std::get<1>(add_result);
            L2.write(removed_cache_line.address, removed_cache_line.data);
        }
        return new_data[cur_double_in_cache_line];
    }

    void write(size_t address, double new_data) {
        size_t cur_double_in_cache_line = address & (doubles_in_one_cache_line - 1);
        size_t set_number = (address >> offset) & (number_of_sets - 1);
        size_t tag = (address >> (offset + index));
        std::tuple<bool, std::vector<double> > result = sets[set_number].find(tag);
        if (std::get<0>(result)) {
            /// Cache hit
            cache_hits++;
            std::vector<double> cur_data = std::get<1>(result);
            cur_data[cur_double_in_cache_line] = new_data;
            sets[set_number].update(tag, cur_data, cur_time++);
            return;
        }
        /// Cache miss
        cache_misses++;
        std::vector<double> cur_data = L2.read(address);
        cur_data[cur_double_in_cache_line] = new_data;
        std::tuple<bool, Cache_Line> add_result = sets[set_number].add(tag, address, cur_data);
        if (std::get<0>(add_result)) {
            Cache_Line removed_cache_line = std::get<1>(add_result);
            L2.write(removed_cache_line.address, removed_cache_line.data);
        }
    }

    void out() const {
        std::cout << "L1: " << 100. * double(cache_hits) / double(cache_hits + cache_misses) << "%\n";
    }
private:
    size_t size_in_bytes, set_associativity, number_of_sets, index = 0, offset = 0;
    unsigned int cache_hits = 0, cache_misses = 0;
    std::vector<Set> sets;
};

const size_t L1_size_in_kb = 32, L1_set_associativity = 4;
CacheL1 L1 = CacheL1(L1_size_in_kb, L1_set_associativity);