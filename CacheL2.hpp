#pragma once

#include <vector>
#include <tuple>
#include <iostream>
#include "Memory.hpp"
#include "Cache_Line.hpp"
#include "Set.hpp"

class CacheL2 {
public:
    CacheL2(size_t size_in_kb,
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

    std::vector<double> read(size_t address) {
        size_t set_number = (address >> offset) & (number_of_sets - 1);
        size_t tag = (address >> (offset + index));
        std::tuple<bool, std::vector<double> > result = sets[set_number].find(tag);
        if (std::get<0>(result)) {
            /// Cache hit
            cache_hits++;
            std::vector<double> data = std::get<1>(result);
            sets[set_number].remove(tag);
            // sets[set_number].update(tag, data, 0);
            return data;
        }
        /// Cache miss
        cache_misses++;
        std::vector<double> data = RAM.read(address);
        /*std::tuple<bool, Cache_Line> add_result = sets[set_number].add(tag, address, data);
        if (std::get<0>(add_result)) {
            Cache_Line removed_cache_line = std::get<1>(add_result);
            RAM.write(removed_cache_line.address, removed_cache_line.data);
        }*/
        return data;
    }

    void write(size_t address, const std::vector<double> &new_data) {
        size_t set_number = (address >> offset) & (number_of_sets - 1);
        size_t tag = (address >> (offset + index));
        std::tuple<bool, std::vector<double> > result = sets[set_number].find(tag);
        if (std::get<0>(result)) {
            /// Cache hit
            cache_hits++;
            sets[set_number].update(tag, new_data, cur_time++);
            return;
        }
        /// Cache miss
        cache_misses++;
        std::tuple<bool, Cache_Line> add_result = sets[set_number].add(tag, address, new_data);
        if (std::get<0>(add_result)) {
            Cache_Line removed_cache_line = std::get<1>(add_result);
            RAM.write(removed_cache_line.address, removed_cache_line.data);
        }
    }

    void out() const {
        std::cout << "L2: " << 100. * double(cache_hits) / double(cache_hits + cache_misses) << "%\n";
    }
private:
    size_t size_in_bytes, set_associativity, number_of_sets, index = 0, offset = 0;
    unsigned int cache_hits = 0, cache_misses = 0;
    std::vector<Set> sets;
};

const size_t L2_size_in_kb = 1024, L2_set_associativity = 8;
CacheL2 L2(L2_size_in_kb, L2_set_associativity);
