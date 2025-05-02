#include "Car.hpp"
#include <stdexcept>
#include <iostream>

Car::Car(int id, std::string type, double weight, double length) {
    std::cout << "DEBUG: Car构造函数 - 接收到参数：" << std::endl;
    std::cout << "DEBUG: id = " << id << std::endl;
    std::cout << "DEBUG: type = " << type << std::endl;
    std::cout << "DEBUG: weight = " << weight << std::endl;
    std::cout << "DEBUG: length = " << length << std::endl;

    // 先验证参数
    if (id < 0) {
        throw std::invalid_argument("车厢ID不能为负数");
    }
    if (weight <= 0) {
        throw std::invalid_argument("车厢重量必须为正数");
    }
    if (length <= 0) {
        throw std::invalid_argument("车厢长度必须为正数");
    }
    if (type.empty()) {
        throw std::invalid_argument("车厢类型不能为空");
    }

    // 验证通过后再初始化成员变量
    id_ = id;
    type_ = std::move(type);
    weight_ = weight;
    length_ = length;
    timestamp_ = std::time(nullptr);
    prev_ = nullptr;
    next_ = nullptr;
}

// 新增：私有构造函数，用于创建哨兵节点
Car::Car(bool isSentinel) 
    : id_(-1)
    , type_("sentinel")
    , weight_(0)
    , length_(0)
    , timestamp_(std::time(nullptr))
    , prev_(nullptr)
    , next_(nullptr) {
}

// 新增：创建哨兵节点的静态方法
Car* Car::createSentinel() {
    return new Car(true);
}

Car::Car(const Car& other)
    : id_(other.id_)
    , type_(other.type_)
    , weight_(other.weight_)
    , length_(other.length_)
    , timestamp_(other.timestamp_)
    , prev_(nullptr)  // 不复制指针，由Train类管理
    , next_(nullptr) {
}

Car& Car::operator=(const Car& other) {
    if (this != &other) {
        id_ = other.id_;
        type_ = other.type_;
        weight_ = other.weight_;
        length_ = other.length_;
        timestamp_ = other.timestamp_;
        // 不复制prev_和next_指针，由Train类管理
    }
    return *this;
}

nlohmann::json Car::toJson() const {
    nlohmann::json j;
    j["id"] = id_;
    j["type"] = type_;
    j["weight"] = weight_;
    j["length"] = length_;
    j["timestamp"] = timestamp_;
    return j;
}

Car Car::fromJson(const nlohmann::json& j) {
    return Car(
        j["id"].get<int>(),
        j["type"].get<std::string>(),
        j["weight"].get<double>(),
        j["length"].get<double>()
    );
} 
