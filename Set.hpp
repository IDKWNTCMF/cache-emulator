#pragma once

#include <vector>
#include <tuple>
#include "Cache_Line.hpp"

struct Set {
    explicit Set(size_t sz = 0) : max_number_of_cache_lines(sz) {}

    std::tuple<bool, std::vector<double> > find(size_t tag) {
        for (int i = 0; i < cur_number_of_cache_lines; i++) {
            if (cache_lines[i].tag == tag) {
                cache_lines[i].update(cur_time++);
                return {true, cache_lines[i].data};
            }
        }
        return {false, {}};
    }

    std::tuple<bool, Cache_Line> add(size_t tag, size_t address, const std::vector<double> &new_data) {
        for (size_t i = 0; i < cur_number_of_cache_lines; i++) {
            if (cache_lines[i].tag == tag) {
                cache_lines[i].data = new_data;
                cache_lines[i].update(cur_time++);
                return {false, Cache_Line()};
            }
        }
        if (cur_number_of_cache_lines != max_number_of_cache_lines) {
            cache_lines.emplace_back(tag, address, cur_time++, new_data);
            cur_number_of_cache_lines++;
            return {false, Cache_Line()};
        }
        size_t removing = 0;
        for (size_t i = 0; i < cur_number_of_cache_lines; i++) {
            if (cache_lines[i].last_query_time < cache_lines[removing].last_query_time) {
                removing = i;
            }
        }
        Cache_Line removed_cache_line = cache_lines[removing];
        cache_lines[removing] = Cache_Line(tag, address, cur_time++, new_data);
        return {true, removed_cache_line};
    }

    void update(size_t tag, const std::vector<double> &new_data, unsigned int query_time) {
        for (size_t i = 0; i < cur_number_of_cache_lines; i++) {
            if (cache_lines[i].tag == tag) {
                cache_lines[i].data = new_data;
                cache_lines[i].last_query_time = query_time;
                return;
            }
        }
    }

    void remove(size_t tag) {
        for (size_t i = 0; i < cur_number_of_cache_lines; i++) {
            if (cache_lines[i].tag == tag) {
                cache_lines.erase(cache_lines.begin() + i);
                cur_number_of_cache_lines--;
                return;
            }
        }
    }

private:
    size_t cur_number_of_cache_lines = 0, max_number_of_cache_lines;
    std::vector<Cache_Line> cache_lines;
};