#include <iostream>
#include <fstream>
#include "Car.hpp"
#include "Train.hpp"
#include "TimePortal.hpp"
#include "ChronoRailSystem.hpp"

ChronoRailSystem::ChronoRailSystem() : nextTrainId_(1) {
    // 初始化为1，但会在添加列车时动态调整
}

void ChronoRailSystem::processCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    try {
        if (command == "ADD_FRONT" || command == "ADD_BACK") {
            handleAddCommand(iss, command == "ADD_FRONT");
        }
        else if (command == "INSERT_AFTER") {
            handleInsertCommand(iss);
        }
        else if (command == "REMOVE") {
            handleRemoveCommand(iss);
        }
        else if (command == "TRAVERSE") {
            handleTraverseCommand(iss);
        }
        else if (command == "SPLIT") {
            handleSplitCommand(iss);
        }
        else if (command == "MERGE") {
            handleMergeCommand(iss);
        }
        else if (command == "REVERSE") {
            handleReverseCommand(iss);
        }
        else if (command == "SORT") {
            handleSortCommand(iss);
        }
        else if (command == "MAKE_LOOP" || command == "BREAK_LOOP") {
            handleLoopCommand(iss, command == "MAKE_LOOP");
        }
        else if (command == "SAVE") {
            handleSaveCommand(iss);
        }
        else if (command == "LOAD") {
            handleLoadCommand(iss);
        }
        else {
            std::cout << "未知命令: " << command << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "错误: " << e.what() << std::endl;
    }
}

void ChronoRailSystem::handleAddCommand(std::istringstream& iss, bool front) {
    int trainId;
    int carId;
    std::string type;
    double weight, length;
    
    // 在读取之前清除任何错误标志
    iss.clear();
    
    // 逐个读取并验证每个参数
    if (!(iss >> trainId)) {
        throw std::invalid_argument("无法读取列车ID");
    }
    std::cout << "DEBUG: 读取到trainId = " << trainId << std::endl;
    
    if (!(iss >> carId)) {
        throw std::invalid_argument("无法读取车厢ID");
    }
    std::cout << "DEBUG: 读取到carId = " << carId << std::endl;
    
    if (!(iss >> type)) {
        throw std::invalid_argument("无法读取车厢类型");
    }
    std::cout << "DEBUG: 读取到type = " << type << std::endl;
    
    if (!(iss >> weight)) {
        throw std::invalid_argument("无法读取车厢重量");
    }
    std::cout << "DEBUG: 读取到weight = " << weight << std::endl;
    
    if (!(iss >> length)) {
        throw std::invalid_argument("无法读取车厢长度");
    }
    std::cout << "DEBUG: 读取到length = " << length << std::endl;

    // 验证trainId和carId
    if (trainId < 1) {
        throw std::invalid_argument("列车ID必须为正数");
    }
    if (carId < 1) {
        throw std::invalid_argument("车厢ID必须为正数");
    }
    
    // 验证weight和length
    if (weight <= 0) {
        throw std::invalid_argument("车厢重量必须为正数");
    }
    if (length <= 0) {
        throw std::invalid_argument("车厢长度必须为正数");
    }
    
    // 验证type
    if (type.empty()) {
        throw std::invalid_argument("车厢类型不能为空");
    }

    // 更新nextTrainId_
    // 遍历所有现有列车ID，找到最大值
    int maxTrainId = 0;
    for (const auto& [id, _] : trains_) {
        maxTrainId = std::max(maxTrainId, id);
    }
    // 确保nextTrainId_大于当前最大ID和新添加的trainId
    nextTrainId_ = std::max(std::max(maxTrainId, trainId) + 1, nextTrainId_);
    
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        trains_[trainId] = std::make_unique<Train>();
        std::cout << "创建新列车，ID: " << trainId << std::endl;
    }
    
    try {
        Car* car = new Car(carId, type, weight, length);
        if (front) {
            trains_[trainId]->addFront(car);
        } else {
            trains_[trainId]->addBack(car);
        }
        
        std::cout << "成功添加车厢 " << carId << " 到列车 " << trainId << std::endl;
    } catch (const std::exception& e) {
        throw; // 重新抛出异常
    }
}

void ChronoRailSystem::handleInsertCommand(std::istringstream& iss) {
    int trainId, afterId, carId;
    std::string type;
    double weight, length;
    
    iss >> trainId >> afterId >> carId >> type >> weight >> length;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    Car* car = new Car(carId, type, weight, length);
    if (!it->second->insertAfter(afterId, car)) {
        delete car;
        throw std::runtime_error("插入失败：找不到指定ID的车厢");
    }
    
    std::cout << "成功在车厢 " << afterId << " 后插入车厢 " << carId << std::endl;
}

void ChronoRailSystem::handleRemoveCommand(std::istringstream& iss) {
    int trainId, carId;
    iss >> trainId >> carId;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    if (!it->second->remove(carId)) {
        throw std::runtime_error("删除失败：找不到指定ID的车厢");
    }
    
    std::cout << "成功从列车 " << trainId << " 移除车厢 " << carId << std::endl;
}

void ChronoRailSystem::handleTraverseCommand(std::istringstream& iss) {
    int trainId;
    std::string direction;
    iss >> trainId >> direction;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    std::cout << "列车 " << trainId << " 的车厢：" << std::endl;
    if (direction == "FORWARD") {
        it->second->traverseForward();
    } else if (direction == "BACKWARD") {
        it->second->traverseBackward();
    } else {
        throw std::invalid_argument("无效的遍历方向");
    }
}

void ChronoRailSystem::handleSplitCommand(std::istringstream& iss) {
    int trainId, splitId;
    iss >> trainId >> splitId;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    // 检查原列车是否存在
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    // 寻找一个未使用的列车ID
    while (trains_.find(nextTrainId_) != trains_.end()) {
        nextTrainId_++;
    }
    
    int newTrainId = nextTrainId_;
    std::cout << "开始分裂列车，原列车ID: " << trainId << ", 新列车ID: " << newTrainId << std::endl;
    
    // 执行分裂操作
    Train* newTrain = it->second->split(splitId);
    if (!newTrain) {
        throw std::runtime_error("分裂操作失败");
    }
    
    // 将新列车添加到管理器中
    trains_[newTrainId] = std::unique_ptr<Train>(newTrain);
    
    std::cout << "成功将列车 " << trainId << " 分裂为两列车：" << std::endl;
    std::cout << "- 原列车保持ID: " << trainId << std::endl;
    std::cout << "- 新列车分配ID: " << newTrainId << std::endl;
    
    // 更新nextTrainId_
    nextTrainId_++;
}

void ChronoRailSystem::handleMergeCommand(std::istringstream& iss) {
    int trainId1, trainId2;
    iss >> trainId1 >> trainId2;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    auto it1 = trains_.find(trainId1);
    auto it2 = trains_.find(trainId2);
    if (it1 == trains_.end() || it2 == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    TimePortal portal("merge_portal", "temporary");
    Train* mergedTrain = portal.merge(it1->second.get(), it2->second.release());
    trains_.erase(trainId2);
    
    std::cout << "成功将列车 " << trainId2 << " 合并到列车 " << trainId1 << std::endl;
}

void ChronoRailSystem::handleReverseCommand(std::istringstream& iss) {
    int trainId, startId, endId;
    iss >> trainId >> startId >> endId;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    TimePortal portal("reverse_portal", "temporary");
    if (!portal.reverse(it->second.get(), startId, endId)) {
        throw std::runtime_error("反转失败：找不到指定的车厢区间");
    }
    
    std::cout << "成功反转列车 " << trainId << " 中从车厢 " << startId 
              << " 到车厢 " << endId << " 的区间" << std::endl;
}

void ChronoRailSystem::handleSortCommand(std::istringstream& iss) {
    int trainId;
    std::string typeStr, orderStr;
    iss >> trainId >> typeStr >> orderStr;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    Train::SortType type;
    if (typeStr == "WEIGHT") {
        type = Train::SortType::WEIGHT;
    } else if (typeStr == "LENGTH") {
        type = Train::SortType::LENGTH;
    } else {
        throw std::invalid_argument("无效的排序类型");
    }
    
    Train::SortOrder order;
    if (orderStr == "ASC") {
        order = Train::SortOrder::ASC;
    } else if (orderStr == "DESC") {
        order = Train::SortOrder::DESC;
    } else {
        throw std::invalid_argument("无效的排序顺序");
    }
    
    it->second->sort(type, order);
    std::cout << "成功对列车 " << trainId << " 按" 
              << (typeStr == "WEIGHT" ? "重量" : "长度")
              << (orderStr == "ASC" ? "升序" : "降序")
              << "排序" << std::endl;
}

void ChronoRailSystem::handleLoopCommand(std::istringstream& iss, bool makeLoop) {
    int trainId;
    iss >> trainId;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    auto it = trains_.find(trainId);
    if (it == trains_.end()) {
        throw std::invalid_argument("列车不存在");
    }
    
    if (makeLoop) {
        it->second->makeLoop();
        std::cout << "成功将列车 " << trainId << " 设置为循环列车" << std::endl;
    } else {
        it->second->breakLoop();
        std::cout << "成功解除列车 " << trainId << " 的循环状态" << std::endl;
    }
}

void ChronoRailSystem::handleSaveCommand(std::istringstream& iss) {
    std::string filename;
    iss >> filename;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    nlohmann::json j;
    j["nextTrainId"] = nextTrainId_;
    
    nlohmann::json trainsJson;
    for (const auto& [id, train] : trains_) {
        trainsJson[std::to_string(id)] = train->toJson();
    }
    j["trains"] = trainsJson;
    
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("无法打开文件进行写入");
    }
    
    file << j.dump(4);
    std::cout << "成功保存列车配置到文件 " << filename << std::endl;
}

void ChronoRailSystem::handleLoadCommand(std::istringstream& iss) {
    std::string filename;
    iss >> filename;
    
    if (iss.fail()) {
        throw std::invalid_argument("参数格式错误");
    }
    
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("无法打开文件进行读取");
    }
    
    nlohmann::json j;
    file >> j;
    
    // 清除当前所有列车
    trains_.clear();
    
    nextTrainId_ = j["nextTrainId"].get<int>();
    
    for (const auto& [idStr, trainJson] : j["trains"].items()) {
        int id = std::stoi(idStr);
        trains_[id] = std::make_unique<Train>(Train::fromJson(trainJson));
    }
    
    std::cout << "成功从文件 " << filename << " 加载列车配置" << std::endl;
} 
