# Simple Vector
This is a C++ implementation of a simple dynamic array (vector) with some basic operations, including insertion, deletion, resizing and etc.



## Implementation details
The implementation of Simple Vector is based on a dynamic array with a template type parameter Type. The array is managed by an ArrayPtr class, which is a simple implementation of a smart pointer that owns the array it points to and automatically frees the memory when the pointer goes out of scope.

The Simple Vector class provides several constructors for creating and initializing an instance of the vector. It also provides methods for pushing and popping items from the back of the vector, inserting and erasing items at arbitrary positions, and resizing and reserving memory.



## Usage
### Creating a Simple Vector
To create a Simple Vector, you can declare an instance of the class and provide the desired size and/or initial value:
```cpp
SimpleVector<int> vec(10, 42); // creates a vector of size 10 with all elements initialized to 42
```
You can also create a vector with an initializer list:
```cpp
SimpleVector<std::string> vec{"foo", "bar", "baz"}; // creates a vector of size 3 with the given elements
```


### Modifying the Vector
You can add and remove elements from the vector using the push_back, pop_back, insert, and erase methods:
```cpp
vec.push_back(123); // adds an element to the back of the vector
vec.pop_back(); // removes the last element from the vector
vec.insert(vec.begin() + 1, "123"); // inserts an element at position 1 (after the first element)
vec.erase(vec.begin() + 2); // removes the element at position 2 (the third element).
```


### Accessing Elements
You can access elements of the vector using the subscript ([]) operator or the at method:
```cpp
int x = vec[0]; // access the first element of the vector
int y = vec.at(1); // access the second element of the vector with bounds checking.
```


### Resizing and Reserving Memory
You can resize the vector using the resize method, which will add or remove elements from the back of the vector as needed:
```cpp
vec.resize(20); // resize the vector to size 20, adding default-constructed elements if necessary
```
You can also reserve memory for the vector using the reserve method, which will allocate memory for the specified number of elements but not actually construct any objects:
```cpp
vec.reserve(100); // reserve memory for at least 100 elements
```



## Testing

The Simple Vector implementation includes a set of tests in the tests directory. The tests cover basic functionality of the vector, including adding and removing elements, resizing and reserving memory, and copying and moving the vector.