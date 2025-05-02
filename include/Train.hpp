#ifndef CHRONORAIL_TRAIN_HPP
#define CHRONORAIL_TRAIN_HPP

#include "Car.hpp"
#include <string>
#include <memory>
#include "json.hpp"

class Train {
public:
    enum class SortType {
        WEIGHT,
        LENGTH
    };

    enum class SortOrder {
        ASC,
        DESC
    };

    Train();
    ~Train();
    Train(const Train& other);  // 拷贝构造函数
    Train& operator=(const Train& other);  // 拷贝赋值运算符
    Train(Train&& other) noexcept;  // 移动构造函数
    Train& operator=(Train&& other) noexcept;  // 移动赋值运算符

    // 基本编组操作
    void addFront(Car* car);
    void addBack(Car* car);
    bool insertAfter(int id, Car* car);
    bool remove(int id);
    void traverseForward() const;
    void traverseBackward() const;

    // 高级列车操作
    Train* split(int startId);
    void merge(Train* other);
    bool reverse(int startId, int endId);
    void sort(SortType type, SortOrder order);

    // 循环操作
    void makeLoop();
    void breakLoop();

    // 序列化支持
    nlohmann::json toJson() const;
    static Train fromJson(const nlohmann::json& j);

    // 获取列车信息
    bool isEmpty() const;
    bool isLoop() const;
    size_t size() const;

private:
    Car* head_;  // 哨兵头节点
    Car* tail_;  // 尾节点指针（非哨兵）
    bool isLoop_;
    size_t size_;

    // 辅助函数
    Car* findById(int id) const;
    void clearAll();
    void copyFrom(const Train& other);
    void init();
};

#endif // CHRONORAIL_TRAIN_HPP 
