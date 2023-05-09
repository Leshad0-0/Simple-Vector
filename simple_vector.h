#pragma once

#include <cassert>
#include <algorithm>
#include <initializer_list>

#include "array_ptr.h"

class ReserveProxyObj {
public:
    explicit ReserveProxyObj(size_t size_to_reserve) : size_to_reserve_(size_to_reserve) {}
    operator size_t() const {
        return size_to_reserve_;
    }
private:
    size_t size_to_reserve_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}


template <typename Type>
class SimpleVector {
public:
    using Iterator = Type *;
    using ConstIterator = const Type *;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size) : size_(size), capacity_(size), items_(size) {
        std::fill(begin(), end(), Type{});
    }

    SimpleVector(size_t size, const Type &value) : size_(size), capacity_(size), items_(size) {
        std::fill(begin(), end(), value);
    }

    SimpleVector(const SimpleVector &other) : size_(other.size_), capacity_(other.capacity_), items_(other.capacity_) {
        std::copy(other.begin(), other.end(), begin());
    }

    SimpleVector(std::initializer_list<Type> init) : size_(init.size()), capacity_(init.size()), items_(init.size()) {
        std::copy(init.begin(), init.end(), begin());
    }

    explicit SimpleVector(ReserveProxyObj size_to_reserve) : SimpleVector() {
        reserve(size_to_reserve);
    }

    SimpleVector(SimpleVector &&other) noexcept: size_(std::exchange(other.size_, 0)),
                                                 capacity_(std::exchange(other.capacity_, 0)),
                                                 items_(std::move(other.items_)) {}

    SimpleVector &operator=(const SimpleVector &rhs) {
        if (this != &rhs) {
            SimpleVector temp(rhs);
            swap(temp);
        }
        return *this;
    }

    SimpleVector &operator=(SimpleVector &&rhs) noexcept {
        if (this != &rhs) {
            swap(rhs);
        }
        return *this;
    }

    void push_back(const Type &item) {
        emplace_back(item);
    }

    void push_back(Type &&item) {
        emplace_back(std::move(item));
    }

    void pop_back() noexcept {
        assert(!empty());
        --size_;
    }

    Iterator erase(ConstIterator pos) {
        assert(pos >= cbegin() && pos <= cend());
        size_t index = pos - begin();
        std::move(begin() + index + 1, end(), begin() + index);
        --size_;
        return begin() + index;
    }

    Iterator insert(ConstIterator pos, const Type &value) {
        return emplace(pos, value);
    }

    Iterator insert(ConstIterator pos, Type &&value) {
        return emplace(pos, std::move(value));
    }

    void swap(SimpleVector &other) noexcept {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    void clear() noexcept {
        size_ = 0;
    }

    [[nodiscard]] size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] size_t capacity() const noexcept {
        return capacity_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    Type &operator[](size_t index) noexcept {
        assert(index < size_);
        return items_[index];
    }

    const Type &operator[](size_t index) const noexcept {
        assert(index < size_);
        return items_[index];
    }

    Type &at(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return items_[index];
    }

    const Type &at(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return items_[index];
    }

    void resize(size_t new_size) {
        if (new_size > capacity_) {
            reserve(std::max(new_size, capacity_ * 2));
            fill(begin() + size_, begin() + new_size);
        } else if (new_size > size_) {
            fill(begin() + size_, begin() + new_size);
        }
        size_ = new_size;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), end(), new_items.get());
            items_.swap(new_items);
            capacity_ = new_capacity;
        }
    }

    Iterator begin() noexcept {
        return items_.get();
    }

    Iterator end() noexcept {
        return items_.get() + size_;
    }

    ConstIterator begin() const noexcept {
        return items_.get();
    }

    ConstIterator end() const noexcept {
        return items_.get() + size_;
    }

    ConstIterator cbegin() const noexcept {
        return items_.get();
    }

    ConstIterator cend() const noexcept {
        return items_.get() + size_;
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<Type> items_;

    template<typename T>
    Iterator emplace(ConstIterator pos, T &&items) {
        assert(pos >= cbegin() && pos <= cend());
        size_t index_pos = pos - begin();
        if (size_ == capacity_) {
            size_t new_capacity;
            if (capacity_ == 0) {
                new_capacity = 1;
            } else {
                new_capacity = capacity_ * 2;
            }
            ArrayPtr<Type> new_items(new_capacity);
            std::move(begin(), begin() + index_pos, new_items.get());
            new_items[index_pos] = Type(std::forward<T>(items));
            std::move(begin() + index_pos, end(), new_items.get() + index_pos + 1);
            items_.swap(new_items);
            capacity_ = new_capacity;
            ++size_;
        } else {
            ++size_;
            std::move_backward(begin() + index_pos, begin() + size_ - 1, end());
            items_[index_pos] = Type(std::forward<T>(items));
        }
        return begin() + index_pos;
    }

    template<typename T>
    void emplace_back(T &&items) {
        if (size_ == capacity_) {
            reserve(std::max(capacity_ * 2, static_cast<size_t>(1)));
        }
        items_[size_++] = Type(std::forward<T>(items));
    }

    void fill(Type *begin, Type *end) {
        std::generate(begin, end, [] { return Type{}; });
    }
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs <= lhs;
}