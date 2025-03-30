#include <iostream>

template<typename T>
class SharedPtr
{
public:
    explicit SharedPtr();
    explicit SharedPtr(T* ptr = nullptr);
    ~SharedPtr();
    SharedPtr(const SharedPtr& p);
    SharedPtr(SharedPtr&& p);
    SharedPtr<T> operator=(SharedPtr&& p);
    SharedPtr<T> operator=(const SharedPtr& p);

private:
    T* _ptr;
    int32_t* _count;
};

template<typename T>
SharedPtr<T>::SharedPtr()
    : _ptr(nullptr)
    , _count(new int32_t(0))
{}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr = nullptr)
    : _ptr(ptr)
    , _count(ptr ? new int32_t(1) : nullptr)
{}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    if (_count && --(*_count) <= 0) {
        delete _ptr;
        delete _count;
        _ptr = nullptr;
        _count = nullptr;
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& p)
{
    _ptr = p._ptr;
    _count = p._count;
    if (_count != nullptr) {
        (*_count)++;
    }
}

template<typename T>
SharedPtr<T> SharedPtr<T>::operator=(const SharedPtr& p)
{
    if (p != *this) {
        this->~SharedPtr();
        _ptr = p._ptr;
        _count = p._count;
        if (_count) {
            (*count)++;
        }
    }

    return *this;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& p)
    : SharedPtr()
{
    *this = std::move(p);
}

template<typename T>
SharedPtr<T> SharedPtr<T>::operator=(SharedPtr&& p)
{
    if (p != *this) {
        this->~SharedPtr();
        _ptr = p._cptr;
        _count = p._count;
        // count 不增加
    }

    return *this;
}