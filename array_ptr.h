#pragma once

#include <iostream>

template <typename Type>
class ArrayPtr {
public:
    ArrayPtr() = default;

    explicit ArrayPtr(size_t size) : raw_ptr_(size ? (new Type[size]) : nullptr) {}

    explicit ArrayPtr(Type *raw_ptr) noexcept: raw_ptr_(raw_ptr) {
    }

    ArrayPtr(const ArrayPtr &) = delete;

    ~ArrayPtr() {
        delete[] raw_ptr_;
        raw_ptr_ = nullptr;
    }

    ArrayPtr &operator=(const ArrayPtr &) = delete;

    ArrayPtr(ArrayPtr &&other) noexcept: raw_ptr_(std::exchange(other.raw_ptr_, nullptr)) {}

    ArrayPtr &operator=(ArrayPtr &&other) noexcept {
        swap(other);
        return *this;
    }

    [[nodiscard]] Type *release() noexcept {
        return std::exchange(raw_ptr_, nullptr);
    }

    Type &operator[](size_t index) noexcept {
        assert(raw_ptr_ != nullptr);
        return raw_ptr_[index];
    }

    const Type &operator[](size_t index) const noexcept {
        assert(raw_ptr_ != nullptr);
        return raw_ptr_[index];
    }

    explicit operator bool() const {
        return raw_ptr_ != nullptr;
    }

    Type *get() const noexcept {
        return raw_ptr_;
    }

    void swap(ArrayPtr &other) noexcept {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:
    Type *raw_ptr_ = nullptr;
};