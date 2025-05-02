#include "Train.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <algorithm>

Train::Train() {
    init();
}

Train::~Train() {
    clearAll();
    delete head_;  //delete哨兵节点
}

void Train::init() {
    head_ = Car::createSentinel();
    head_->next_ = head_;
    head_->prev_ = head_;
    tail_ = head_;
    isLoop_ = false;
    size_ = 0;
}

void Train::clearAll() {
    if (isEmpty()) {
        return;
    }
    
    //先断开循环
    if (isLoop_) {
        breakLoop();
    }
    
    Car* current = head_->next_;
    while (current != head_) {
        Car* next = current->next_;
        delete current;
        current = next;
    }
    head_->next_ = head_;
    head_->prev_ = head_;
    tail_ = head_;
    size_ = 0;
}

void Train::copyFrom(const Train& other) {
    if (other.isEmpty()) {
        return;
    }
    
    Car* otherCurrent = other.head_->next_;
    while (otherCurrent != other.head_) {
        Car* newCar = new Car(*otherCurrent);
        addBack(newCar);
        otherCurrent = otherCurrent->next_;
    }
    
    if (other.isLoop_) {
        makeLoop();
    }
}

Train::Train(const Train& other) {
    init();
    copyFrom(other);
}

Train& Train::operator=(const Train& other) {
    if (this != &other) {
        clearAll();
        copyFrom(other);
    }
    return *this;
}

Train::Train(Train&& other) noexcept
    : head_(other.head_)
    , tail_(other.tail_)
    , isLoop_(other.isLoop_)
    , size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

Train& Train::operator=(Train&& other) noexcept {
    if (this != &other) {
        clearAll();
        delete head_;
        
        head_ = other.head_;
        tail_ = other.tail_;
        isLoop_ = other.isLoop_;
        size_ = other.size_;
        
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

void Train::addFront(Car* car) {
    if (!car) {
        throw std::invalid_argument("不能添加空车厢");
    }
    
    car->next_ = head_->next_;
    car->prev_ = head_;
    head_->next_->prev_ = car;
    head_->next_ = car;
    
    if (size_ == 0) {
        tail_ = car;
    }
    size_++;
}

void Train::addBack(Car* car) {
    if (!car) {
        throw std::invalid_argument("不能添加空车厢");
    }
    
    if (isLoop_) {
        throw std::runtime_error("循环列车不能在尾部添加车厢");
    }
    
    car->next_ = head_;
    car->prev_ = tail_;
    tail_->next_ = car;
    head_->prev_ = car;
    tail_ = car;
    size_++;
}

bool Train::insertAfter(int id, Car* car) {
    if (!car) {
        throw std::invalid_argument("不能插入空车厢");
    }
    
    Car* pos = findById(id);
    if (!pos) {
        return false;
    }
    
    car->next_ = pos->next_;
    car->prev_ = pos;
    pos->next_->prev_ = car;
    pos->next_ = car;
    
    if (pos == tail_) {
        tail_ = car;
    }
    size_++;
    return true;
}

bool Train::remove(int id) {
    Car* car = findById(id);
    if (!car) {
        return false;
    }
    
    car->prev_->next_ = car->next_;
    car->next_->prev_ = car->prev_;
    
    if (car == tail_) {
        tail_ = car->prev_;
    }
    
    delete car;
    size_--;
    return true;
}

Car* Train::findById(int id) const {
    if (isEmpty()) {
        return nullptr;
    }
    
    Car* current = head_->next_;
    do {
        if (current->getId() == id) {
            return current;
        }
        current = current->next_;
    } while (current != head_ && (!isLoop_ || current != head_->next_));
    
    return nullptr;
}

void Train::traverseForward() const {
    if (isEmpty()) {
        std::cout << "列车为空" << std::endl;
        return;
    }
    
    Car* current = head_->next_;
    do {
        std::cout << "车厢ID: " << current->getId()
                  << ", 类型: " << current->getType()
                  << ", 重量: " << current->getWeight()
                  << ", 长度: " << current->getLength()
                  << std::endl;
        current = current->next_;
    } while (current != head_ && (!isLoop_ || current != head_->next_));
}

void Train::traverseBackward() const {
    if (isEmpty()) {
        std::cout << "列车为空" << std::endl;
        return;
    }
    
    Car* current = tail_;
    do {
        std::cout << "车厢ID: " << current->getId()
                  << ", 类型: " << current->getType()
                  << ", 重量: " << current->getWeight()
                  << ", 长度: " << current->getLength()
                  << std::endl;
        current = current->prev_;
    } while (current != head_ && (!isLoop_ || current != tail_));
}

bool Train::isEmpty() const {
    return size_ == 0;
}

bool Train::isLoop() const {
    return isLoop_;
}

size_t Train::size() const {
    return size_;
}

void Train::makeLoop() {
    if (isEmpty()) {
        throw std::runtime_error("空列车不能成环");
    }
    if (!isLoop_) {
        isLoop_ = true;
    }
}

void Train::breakLoop() {
    isLoop_ = false;
}

Train* Train::split(int startId) {
    std::cout << "开始分裂列车，分裂点ID: " << startId << std::endl;
    std::cout << "当前列车状态：" << std::endl;
    std::cout << "大小: " << size_ << ", 是否循环: " << (isLoop_ ? "是" : "否") << std::endl;
    
    Car* splitPoint = findById(startId);
    if (!splitPoint) {
        std::cout << "错误：找不到ID为 " << startId << " 的车厢" << std::endl;
        throw std::invalid_argument("找不到指定ID的车厢");
    }
    std::cout << "找到分裂点车厢：ID = " << splitPoint->getId() << std::endl;
    
    Train* newTrain = new Train();
    std::cout << "创建新列车成功" << std::endl;
    bool wasLoop = isLoop_;
    if (isLoop_) {
        std::cout << "断开循环列车" << std::endl;
        breakLoop();
    }
    
    Car* beforeSplit = splitPoint->prev_;
    Car* afterSplit = splitPoint;
    
    std::cout << "分裂点前车厢ID: " << (beforeSplit == head_ ? "哨兵" : std::to_string(beforeSplit->getId())) << std::endl;
    std::cout << "分裂点后车厢ID: " << afterSplit->getId() << std::endl;
    
    // 找到尾部
    size_t newSize = 0;
    Car* current = afterSplit;
    Car* newTail = nullptr;
    
    std::cout << "开始计算新列车大小和寻找尾部..." << std::endl;
    while (current != head_) {
        std::cout << "当前处理车厢ID: " << current->getId() << std::endl;
        newSize++;
        newTail = current;
        current = current->next_;
    }
    
    std::cout << "新列车大小: " << newSize << std::endl;
    if (newTail) {
        std::cout << "新列车尾部车厢ID: " << newTail->getId() << std::endl;
    }
    
    if (newSize == 0) {
        std::cout << "错误：新列车大小为0" << std::endl;
        delete newTrain;
        throw std::runtime_error("分割点无效");
    }

    std::cout << "更新原列车连接..." << std::endl;
    beforeSplit->next_ = head_;
    head_->prev_ = beforeSplit;
    tail_ = beforeSplit;
    size_ -= newSize;
    std::cout << "原列车新大小: " << size_ << std::endl;

    std::cout << "更新新列车连接..." << std::endl;
    afterSplit->prev_ = newTrain->head_;
    newTrain->head_->next_ = afterSplit;
    newTail->next_ = newTrain->head_;
    newTrain->head_->prev_ = newTail;
    newTrain->tail_ = newTail;
    newTrain->size_ = newSize;
    
    if (wasLoop) {
        std::cout << "重新设置循环状态..." << std::endl;
        makeLoop();
        newTrain->makeLoop();
    }
    std::cout << "分裂完成。原列车大小: " << size_ << ", 新列车大小: " << newTrain->size_ << std::endl;
    return newTrain;
}

void Train::merge(Train* other) {
    if (!other) {
        throw std::invalid_argument("不能合并空列车");
    }
    
    if (other->isEmpty()) {
        delete other;
        return;
    }
    
    bool thisWasLoop = isLoop_;
    bool otherWasLoop = other->isLoop_;
    if (isLoop_) breakLoop();
    if (other->isLoop_) other->breakLoop();
    
    // 连 接两个列车
    tail_->next_ = other->head_->next_;
    other->head_->next_->prev_ = tail_;
    other->tail_->next_ = head_;
    head_->prev_ = other->tail_;
    tail_ = other->tail_;
    
    size_ += other->size_;
    
    if (thisWasLoop || otherWasLoop) {
        makeLoop();
    }

    other->head_->next_ = other->head_;
    other->head_->prev_ = other->head_;
    other->tail_ = other->head_;
    other->size_ = 0;
    delete other;
}

bool Train::reverse(int startId, int endId) {
    Car* start = findById(startId);
    Car* end = findById(endId);
    
    if (!start || !end) {
        return false;
    }
    
    Car* current = start;
    bool found = false;
    do {
        if (current == end) {
            found = true;
            break;
        }
        current = current->next_;
    } while (current != head_ && current != start);
    
    if (!found) {
        return false;
    }

    Car* beforeStart = start->prev_;
    Car* afterEnd = end->next_;
    current = start;
    Car* prev = nullptr;
    Car* next = nullptr;
    
    do {
        next = current->next_;
        current->next_ = prev;
        current->prev_ = next;
        prev = current;
        current = next;
    } while (prev != end);
    
    beforeStart->next_ = end;
    end->prev_ = beforeStart;
    start->next_ = afterEnd;
    afterEnd->prev_ = start;

    if (tail_ == end) {
        tail_ = start;
    }
    
    return true;
}

void Train::sort(SortType type, SortOrder order) {
    if (size_ <= 1) {
        return;
    }

    std::vector<Car*> cars;
    Car* current = head_->next_;
    while (current != head_) {
        cars.push_back(current);
        current = current->next_;
    }

    auto comparator = [type, order](Car* a, Car* b) {
        double valueA = (type == SortType::WEIGHT) ? a->getWeight() : a->getLength();
        double valueB = (type == SortType::WEIGHT) ? b->getWeight() : b->getLength();
        return (order == SortOrder::ASC) ? (valueA < valueB) : (valueA > valueB);
    };
    std::sort(cars.begin(), cars.end(), comparator);
    
    head_->next_ = cars[0];
    cars[0]->prev_ = head_;
    
    for (size_t i = 0; i < cars.size() - 1; ++i) {
        cars[i]->next_ = cars[i + 1];
        cars[i + 1]->prev_ = cars[i];
    }
    
    cars.back()->next_ = head_;
    head_->prev_ = cars.back();
    tail_ = cars.back();
}

nlohmann::json Train::toJson() const {
    nlohmann::json j;
    j["size"] = size_;
    j["isLoop"] = isLoop_;
    
    nlohmann::json cars = nlohmann::json::array();
    Car* current = head_->next_;
    while (current != head_) {
        cars.push_back(current->toJson());
        current = current->next_;
    }
    j["cars"] = cars;
    
    return j;
}

Train Train::fromJson(const nlohmann::json& j) {
    Train train;
    
    for (const auto& carJson : j["cars"]) {
        Car* car = new Car(Car::fromJson(carJson));
        train.addBack(car);
    }
    
    if (j["isLoop"].get<bool>()) {
        train.makeLoop();
    }
    
    return train;
} 
