#include "IntelCPUPowerMeter.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


static std::vector<std::string> get_energy_paths() {
    std::vector<std::string> paths;
    const std::string base = "/sys/class/powercap";
    for (int i = 0; ; ++i) {
        std::string path = base + "/intel-rapl:" + std::to_string(i) + "/energy_uj";
        if (std::ifstream f(path); !f.good()) {
            break;
        }
        paths.push_back(path);
    }
    if (paths.empty()) {
        throw std::runtime_error("CPUPowerMeter: no RAPL energy_uj files found. Maybe you should run as root?");
    }
    return paths;
}

static double read_energy(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("CPUPowerMeter: failed to open " + path + ". Maybe you should run as root?");
    }
    double val;
    file >> val;
    return val;
}

IntelCPUPowerMeter::IntelCPUPowerMeter(): energy_paths(get_energy_paths()) {
}

void IntelCPUPowerMeter::start() {
    start_energies.clear();
    stop_energies.clear();
    for (const auto &p: energy_paths) {
        start_energies.push_back(read_energy(p));
    }
}

void IntelCPUPowerMeter::stop() {
    for (const auto &p: energy_paths) {
        stop_energies.push_back(read_energy(p));
    }
}

double IntelCPUPowerMeter::result() {
    if (start_energies.empty() || start_energies.size() != stop_energies.size()) {
        throw std::runtime_error("CPUPowerMeter: start/stop not called properly");
    }

    double total = 0.0;
    for (size_t i = 0; i < start_energies.size(); ++i) {
        double diff = stop_energies[i] - start_energies[i];
        if (diff < 0) {
            diff += 1e9;
        }
        total += diff;
    }

    return total;
}
