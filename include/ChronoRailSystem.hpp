#ifndef CHRONORAIL_SYSTEM_HPP
#define CHRONORAIL_SYSTEM_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <memory>
#include "Train.hpp"
#include "TimePortal.hpp"

class ChronoRailSystem {
public:
    ChronoRailSystem();
    
    void run() {
        std::string line;
        while (std::cout << "> " && std::getline(std::cin, line)) {
            processCommand(line);
        }
    }

private:
    std::map<int, std::unique_ptr<Train>> trains_;
    std::map<std::string, std::unique_ptr<TimePortal>> portals_;
    int nextTrainId_ = 1;

    void processCommand(const std::string& line);
    void handleAddCommand(std::istringstream& iss, bool front);
    void handleInsertCommand(std::istringstream& iss);
    void handleRemoveCommand(std::istringstream& iss);
    void handleTraverseCommand(std::istringstream& iss);
    void handleSplitCommand(std::istringstream& iss);
    void handleMergeCommand(std::istringstream& iss);
    void handleReverseCommand(std::istringstream& iss);
    void handleSortCommand(std::istringstream& iss);
    void handleLoopCommand(std::istringstream& iss, bool makeLoop);
    void handleSaveCommand(std::istringstream& iss);
    void handleLoadCommand(std::istringstream& iss);
};

#endif // CHRONORAIL_SYSTEM_HPP 
