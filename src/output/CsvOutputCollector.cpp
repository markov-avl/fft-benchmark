#include <iostream>
#include <fstream>
#include <filesystem>
#include <utility>
#include "CsvOutputCollector.h"


void truncate_file(const std::string &file_name) {
    if (std::ofstream file(file_name, std::ios::out | std::ios::trunc); !file.is_open()) {
        std::cerr << "Error: Unable to open file " << file_name << " for truncation." << std::endl;
    }
}

CsvOutputCollector::CsvOutputCollector(std::string file_path) : file_path(std::move(file_path)) {
    truncate_file(this->file_path);
}

void CsvOutputCollector::add(const std::string &value) {
    std::ofstream file(file_path, std::ios::out | std::ios::app);
    file << value << ",";
}

void CsvOutputCollector::add(const double value) {
    std::ofstream file(file_path, std::ios::out | std::ios::app);
    file << std::fixed << std::setprecision(10) << value << ",";
}

void CsvOutputCollector::newline() {
    std::ofstream file(file_path, std::ios::out | std::ios::app);
    file << "\n";
}
