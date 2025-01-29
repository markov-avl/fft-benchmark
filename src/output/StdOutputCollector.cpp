#include <iomanip>
#include <iostream>
#include "StdOutputCollector.h"


StdOutputCollector::StdOutputCollector(const std::size_t num_columns, const int column_width, const bool header)
    : num_columns(num_columns), column_width(column_width), header(header) {
}

void StdOutputCollector::add(const std::string &value) {
    std::cout << std::left << std::setw(column_width) << value << " | " << std::flush;
}

void StdOutputCollector::add(const double value) {
    std::cout << std::left << std::setw(column_width) << std::fixed << std::setprecision(10) << value << " | " <<
            std::flush;
}

void StdOutputCollector::newline() {
    std::cout << std::endl;

    if (header) {
        header = false;
        for (std::size_t i = 0; i < num_columns; ++i) {
            std::cout << std::string(column_width, '-') << "-+-";
        }
        std::cout << std::endl;
    }
}
