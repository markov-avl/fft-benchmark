#include "CsvOutputCollector.h"

#include <filesystem>
#include <fstream>
#include <iostream>


void truncate_file(const std::string &file_name) {
    if (std::ofstream file(file_name, std::ios::out | std::ios::trunc); !file.is_open()) {
        throw std::runtime_error("CsvOutputCollector: unable to open file `" + file_name + "` for truncation.");
    }
}

CsvOutputCollector::CsvOutputCollector(const std::string &file_path) : file_path(file_path) {
    truncate_file(file_path);
}

void CsvOutputCollector::add(const std::string &value) {
    std::ofstream file(file_path, std::ios::out | std::ios::app);
    append_delimiter(file);
    file << value;
    ++current_column;
}

void CsvOutputCollector::add(const double value, const int precision) {
    std::ofstream file(file_path, std::ios::out | std::ios::app);
    append_delimiter(file);
    file << std::fixed << std::setprecision(precision) << value;
    ++current_column;
}

void CsvOutputCollector::newline() {
    std::ofstream file(file_path, std::ios::out | std::ios::app);
    file << "\n";
    current_column = 0;
}

void CsvOutputCollector::append_delimiter(std::ofstream &file) const {
    if (current_column > 0) {
        file << ",";
    }
}
