#pragma once

#include <iostream>
#include <numeric>
#include <cassert>
#include <stdexcept>
#include <utility>

#include "simple_vector.h"

void TestInitialization() {
    std::cout << "TestInitialization" << std::endl;
    // Инициализация конструктором по умолчанию
    {
        SimpleVector<int> v;
        assert(v.size() == 0u);
        assert(v.empty());
        assert(v.capacity() == 0u);
    }

    // Инициализация вектора указанного размера
    {
        SimpleVector<int> v(5);
        assert(v.size() == 5u);
        assert(v.capacity() == 5u);
        assert(!v.empty());
        for (size_t i = 0; i < v.size(); ++i) {
            assert(v[i] == 0);
        }
    }

    // Инициализация вектора, заполненного заданным значением
    {
        SimpleVector<int> v(3, 42);
        assert(v.size() == 3);
        assert(v.capacity() == 3);
        for (size_t i = 0; i < v.size(); ++i) {
            assert(v[i] == 42);
        }
    }

    // Инициализация вектора при помощи initializer_list
    {
        SimpleVector<int> v{1, 2, 3};
        assert(v.size() == 3);
        assert(v.capacity() == 3);
        assert(v[2] == 3);
    }
    std::cout << "Done!" << std::endl;
}

void TestMethods() {
    std::cout << "TestMethods" << std::endl;
    // Доступ к элементам при помощи at
    {
        SimpleVector<int> v(3);
        assert(&v.at(2) == &v[2]);
        try {
            v.at(3);
            assert(false);  // Ожидается выбрасывание исключения
        } catch (const std::out_of_range &) {
        } catch (...) {
            assert(false);  // Не ожидается исключение, отличное от out_of_range
        }
    }

    // Очистка вектора
    {
        SimpleVector<int> v(10);
        const size_t old_capacity = v.capacity();
        v.clear();
        assert(v.size() == 0);
        assert(v.capacity() == old_capacity);
    }

    // Изменение размера
    {
        SimpleVector<int> v(3);
        v[2] = 17;
        v.resize(7);
        assert(v.size() == 7);
        assert(v.capacity() >= v.size());
        assert(v[2] == 17);
        assert(v[3] == 0);
    }
    {
        SimpleVector<int> v(3);
        v[0] = 42;
        v[1] = 55;
        const size_t old_capacity = v.capacity();
        v.resize(2);
        assert(v.size() == 2);
        assert(v.capacity() == old_capacity);
        assert(v[0] == 42);
        assert(v[1] == 55);
    }
    {
        const size_t old_size = 3;
        SimpleVector<int> v(3);
        v.resize(old_size + 5);
        v[3] = 42;
        v.resize(old_size);
        v.resize(old_size + 2);
        assert(v[3] == 0);
    }

    // Итерирование по SimpleVector
    {
        // Пустой вектор
        {
            SimpleVector<int> v;
            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }

        // Непустой вектор
        {
            SimpleVector<int> v(10, 42);
            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.size());
        }
    }

    // push_back
    {
        SimpleVector<int> v(1);
        v.push_back(42);
        assert(v.size() == 2);
        assert(v.capacity() >= v.size());
        assert(v[0] == 0);
        assert(v[1] == 42);
    }

    // Если хватает места, push_back не увеличивает Capacity
    {
        SimpleVector<int> v(2);
        v.resize(1);
        const size_t old_capacity = v.capacity();
        v.push_back(123);
        assert(v.size() == 2);
        assert(v.capacity() == old_capacity);
    }

    // pop_back
    {
        SimpleVector<int> v{0, 1, 2, 3};
        const size_t old_capacity = v.capacity();
        const auto old_begin = v.begin();
        v.pop_back();
        assert(v.capacity() == old_capacity);
        assert(v.begin() == old_begin);
        assert((v == SimpleVector<int>{0, 1, 2}));
    }

    // Конструктор копирования
    {
        SimpleVector<int> numbers{1, 2};
        auto numbers_copy(numbers);
        assert(&numbers_copy[0] != &numbers[0]);
        assert(numbers_copy.size() == numbers.size());
        for (size_t i = 0; i < numbers.size(); ++i) {
            assert(numbers_copy[i] == numbers[i]);
            assert(&numbers_copy[i] != &numbers[i]);
        }
    }

    // Сравнение
    {
        assert((SimpleVector{1, 2, 3} == SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 3} != SimpleVector{1, 2, 2}));

        assert((SimpleVector{1, 2, 3} < SimpleVector{1, 2, 3, 1}));
        assert((SimpleVector{1, 2, 3} > SimpleVector{1, 2, 2, 1}));

        assert((SimpleVector{1, 2, 3} >= SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 4} >= SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 3}));
        assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 4}));
    }

    // Обмен значений векторов
    {
        SimpleVector<int> v1{42, 666};
        SimpleVector<int> v2;
        v2.push_back(0);
        v2.push_back(1);
        v2.push_back(2);
        const int *const begin1 = &v1[0];
        const int *const begin2 = &v2[0];

        const size_t capacity1 = v1.capacity();
        const size_t capacity2 = v2.capacity();

        const size_t size1 = v1.size();
        const size_t size2 = v2.size();

        static_assert(noexcept(v1.swap(v2)));
        v1.swap(v2);
        assert(&v2[0] == begin1);
        assert(&v1[0] == begin2);
        assert(v1.size() == size2);
        assert(v2.size() == size1);
        assert(v1.capacity() == capacity2);
        assert(v2.capacity() == capacity1);
    }

    // Присваивание
    {
        SimpleVector<int> src_vector{1, 2, 3, 4};
        SimpleVector<int> dst_vector{1, 2, 3, 4, 5, 6};
        dst_vector = src_vector;
        assert(dst_vector == src_vector);
    }

    // Вставка элементов
    {
        SimpleVector<int> v{1, 2, 3, 4};
        v.insert(v.begin() + 2, 42);
        assert((v == SimpleVector<int>{1, 2, 42, 3, 4}));
    }

    // Удаление элементов
    {
        SimpleVector<int> v{1, 2, 3, 4};
        v.erase(v.cbegin() + 2);
        assert((v == SimpleVector<int>{1, 2, 4}));
    }

    // Резервирование памяти
    {
        SimpleVector<int> v(Reserve(5));
        assert(v.capacity() == 5);
        assert(v.empty());
    }

    // Добавление элементов и изменение capacity
    {
        SimpleVector<int> v;
        // зарезервируем 5 мест в векторе
        v.reserve(5);
        assert(v.capacity() == 5);
        assert(v.empty());

        // попытаемся уменьшить capacity до 1
        v.reserve(1);
        // capacity должно остаться прежним
        assert(v.capacity() == 5);
        // поместим 10 элементов в вектор
        for (int i = 0; i < 10; ++i) {
            v.push_back(i);
        }
        assert(v.size() == 10);
        // увеличим capacity до 100
        v.reserve(100);
        // проверим, что размер не поменялся
        assert(v.size() == 10);
        assert(v.capacity() == 100);
        // проверим, что элементы на месте
        for (int i = 0; i < 10; ++i) {
            assert(v[i] == i);
        }
    }
    std::cout << "Done!" << std::endl;
}


class X {
public:
    X() : X(5) {
    }

    explicit X(size_t num) : x_(num) {
    }

    X(const X &other) = delete;

    X &operator=(const X &other) = delete;

    X(X &&other)  noexcept {
        x_ = std::exchange(other.x_, 0);
    }

    X &operator=(X &&other)  noexcept {
        x_ = std::exchange(other.x_, 0);
        return *this;
    }

    [[nodiscard]] size_t GetX() const {
        return x_;
    }

private:
    size_t x_;
};

SimpleVector<int> GenerateVector(size_t size) {
    SimpleVector<int> v(size);
    std::iota(v.begin(), v.end(), 1);
    return v;
}

void TestTemporaryObjConstructor() {
    const size_t size = 1000000;
    std::cout << "TestTemporaryObjConstructor" << std::endl;
    SimpleVector<int> moved_vector(GenerateVector(size));
    assert(moved_vector.size() == size);
    std::cout << "Done!" << std::endl;
}

void TestTemporaryObjOperator() {
    const size_t size = 1000000;
    std::cout << "TestTemporaryObjOperator" << std::endl;
    SimpleVector<int> moved_vector;
    assert(moved_vector.size() == 0);
    moved_vector = GenerateVector(size);
    assert(moved_vector.size() == size);
    std::cout << "Done!" << std::endl;
}

void TestNamedMoveConstructor() {
    const size_t size = 1000000;
    std::cout << "TestNamedMoveConstructor" << std::endl;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.size() == size);

    SimpleVector<int> moved_vector(std::move(vector_to_move));
    assert(moved_vector.size() == size);
    assert(vector_to_move.size() == 0);
    std::cout << "Done!" << std::endl;
}

void TestNamedMoveOperator() {
    const size_t size = 1000000;
    std::cout << "TestNamedMoveOperator" << std::endl;
    SimpleVector<int> vector_to_move(GenerateVector(size));
    assert(vector_to_move.size() == size);

    SimpleVector<int> moved_vector = std::move(vector_to_move);
    assert(moved_vector.size() == size);
    assert(vector_to_move.size() == 0);
    std::cout << "Done!" << std::endl;
}

void TestNoncopiableMoveConstructor() {
    const size_t size = 5;
    std::cout << "TestNoncopiableMoveConstructor" << std::endl;
    SimpleVector<X> vector_to_move;
    for (size_t i = 0; i < size; ++i) {
        vector_to_move.push_back(X(i));
    }

    SimpleVector<X> moved_vector = std::move(vector_to_move);
    assert(moved_vector.size() == size);
    assert(vector_to_move.size() == 0);

    for (size_t i = 0; i < size; ++i) {
        assert(moved_vector[i].GetX() == i);
    }
    std::cout << "Done!" << std::endl;
}

void TestNoncopiablePushBack() {
    const size_t size = 5;
    std::cout << "TestNoncopiablePushBack" << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.push_back(X(i));
    }

    assert(v.size() == size);

    for (size_t i = 0; i < size; ++i) {
        assert(v[i].GetX() == i);
    }
    std::cout << "Done!" << std::endl;
}

void TestNoncopiableInsert() {
    const size_t size = 5;
    std::cout << "TestNoncopiableInsert" << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.push_back(X(i));
    }

    // в начало
    v.insert(v.begin(), X(size + 1));
    assert(v.size() == size + 1);
    assert(v.begin()->GetX() == size + 1);
    // в конец
    v.insert(v.end(), X(size + 2));
    assert(v.size() == size + 2);
    assert((v.end() - 1)->GetX() == size + 2);
    // в середину
    v.insert(v.begin() + 3, X(size + 3));
    assert(v.size() == size + 3);
    assert((v.begin() + 3)->GetX() == size + 3);
    std::cout << "Done!" << std::endl;
}

void TestNoncopiableErase() {
    const size_t size = 3;
    std::cout << "TestNoncopiableErase" << std::endl;
    SimpleVector<X> v;
    for (size_t i = 0; i < size; ++i) {
        v.push_back(X(i));
    }

    auto it = v.erase(v.begin());
    assert(it->GetX() == 1);
    std::cout << "Done!" << std::endl;
}