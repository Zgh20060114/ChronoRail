#include "TimePortal.hpp"
#include <stdexcept>

TimePortal::TimePortal(const std::string& name, const std::string& location)
    : name_(name)
    , location_(location) {
    if (name.empty()) {
        throw std::invalid_argument("时空门名称不能为空");
    }
    if (location.empty()) {
        throw std::invalid_argument("时空门位置不能为空");
    }
}

std::pair<Train*, Train*> TimePortal::split(Train* train, int splitId) {
    if (!train) {
        throw std::invalid_argument("不能分裂空列车");
    }
    
    Train* secondTrain = train->split(splitId);
    return std::make_pair(train, secondTrain);
}

Train* TimePortal::merge(Train* train1, Train* train2) {
    if (!train1 || !train2) {
        throw std::invalid_argument("不能合并空列车");
    }
    
    train1->merge(train2);
    return train1;
}

bool TimePortal::reverse(Train* train, int startId, int endId) {
    if (!train) {
        throw std::invalid_argument("不能反转空列车");
    }
    
    return train->reverse(startId, endId);
}

nlohmann::json TimePortal::toJson() const {
    nlohmann::json j;
    j["name"] = name_;
    j["location"] = location_;
    return j;
}

TimePortal TimePortal::fromJson(const nlohmann::json& j) {
    return TimePortal(
        j["name"].get<std::string>(),
        j["location"].get<std::string>()
    );
} 