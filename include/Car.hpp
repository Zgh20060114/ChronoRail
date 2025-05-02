#ifndef CHRONORAIL_CAR_HPP
#define CHRONORAIL_CAR_HPP

#include <string>
#include <ctime>
#include "json.hpp"

class Car {
public:
    Car(int id, std::string type, double weight, double length);
    Car(const Car& other);
    Car& operator=(const Car& other); 
    ~Car() = default;
    static Car* createSentinel(); 

    int getId() const { return id_; }
    const std::string& getType() const { return type_; }
    double getWeight() const { return weight_; }
    double getLength() const { return length_; }
    time_t getTimestamp() const { return timestamp_; }

    nlohmann::json toJson() const;
    static Car fromJson(const nlohmann::json& j);

private:
    Car(bool isSentinel);
    
    int id_;
    std::string type_;
    double weight_;
    double length_;
    time_t timestamp_;
    //双向链表指针
    Car* prev_;
    Car* next_;

    friend class Train;  // Train 类可以访问私有成员
};

#endif // CHRONORAIL_CAR_HPP 
