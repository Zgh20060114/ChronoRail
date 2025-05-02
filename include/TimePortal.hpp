#ifndef CHRONORAIL_TIMEPORTAL_HPP
#define CHRONORAIL_TIMEPORTAL_HPP

#include "Train.hpp"
#include <string>
#include <vector>
#include "json.hpp"

class TimePortal {
public:
    TimePortal(const std::string& name, const std::string& location);

    // 时空门操作
    std::pair<Train*, Train*> split(Train* train, int splitId);
    Train* merge(Train* train1, Train* train2);
    bool reverse(Train* train, int startId, int endId);

    const std::string& getName() const { return name_; }
    const std::string& getLocation() const { return location_; }
    nlohmann::json toJson() const;
    static TimePortal fromJson(const nlohmann::json& j);

private:
    std::string name_;
    std::string location_;
};

#endif // CHRONORAIL_TIMEPORTAL_HPP 
