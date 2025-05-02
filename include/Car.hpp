#ifndef CHRONORAIL_CAR_HPP
#define CHRONORAIL_CAR_HPP

#include <string>
#include <ctime>
#include "json.hpp"

class Car {
public:
    Car(int id, std::string type, double weight, double length);
    Car(const Car& other);  // 拷贝构造函数
    Car& operator=(const Car& other);  // 拷贝赋值运算符
    ~Car() = default;
    static Car* createSentinel();  // 新增：创建哨兵节点的静态方法

    // Getters
    int getId() const { return id_; }
    const std::string& getType() const { return type_; }
    double getWeight() const { return weight_; }
    double getLength() const { return length_; }
    time_t getTimestamp() const { return timestamp_; }

    // 序列化支持
    nlohmann::json toJson() const;
    static Car fromJson(const nlohmann::json& j);

private:
    Car(bool isSentinel);  // 新增：私有构造函数，用于创建哨兵节点
    
    int id_;
    std::string type_;
    double weight_;
    double length_;
    time_t timestamp_;
    
    // 双向链表指针
    Car* prev_;
    Car* next_;

    friend class Train;  // 允许 Train 类访问私有成员
};

#endif // CHRONORAIL_CAR_HPP 
