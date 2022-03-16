#pragma once

#include <vector>

struct Cache_Line {
public:
    size_t tag, address;
    unsigned int last_query_time;
    std::vector<double> data;

    Cache_Line() : tag(-1), address(0), last_query_time(0) {}
    Cache_Line(size_t tag_,
               size_t address_,
               unsigned int query_time,
               const std::vector<double> &data_) : tag(tag_), address(address_), last_query_time(query_time) {
        data = data_;
    }

    void update(unsigned int new_last_query_time) {
        last_query_time = new_last_query_time;
    }
};