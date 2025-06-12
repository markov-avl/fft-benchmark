#include "StdOutputCollector.h"

#include <iomanip>
#include <iostream>


StdOutputCollector::StdOutputCollector(const int column_width, const bool header)
    : column_width(column_width), header(header) {
}

void StdOutputCollector::add(const std::string &value) {
    print_delimiter();
    apply_format();
    std::cout << value << std::flush;
    ++current_column;
}

void StdOutputCollector::add(const double value, const int precision) {
    print_delimiter();
    apply_format();
    std::cout << std::fixed << std::setprecision(precision) << value << std::flush;
    ++current_column;
}

void StdOutputCollector::newline() {
    std::cout << std::endl;

    if (header) {
        header = false;
        if (current_column > 0) {
            std::cout << std::string(column_width, '-');
        }
        for (std::size_t i = 1; i < current_column; ++i) {
            std::cout << "-+-" << std::string(column_width, '-');
        }
        std::cout << std::endl;
    }

    current_column = 0;
}

void StdOutputCollector::print_delimiter() const {
    if (current_column > 0) {
        std::cout << " | ";
    }
}

void StdOutputCollector::apply_format() const {
    std::cout << std::left << std::setw(column_width);
}
