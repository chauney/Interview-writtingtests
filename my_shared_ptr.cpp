#include <iostream>
#include <memory>
#include <thread>
#include <vector>

template<typename T>
class SharedPtr
{
public:
    SharedPtr();
    // explicit 防止隐式类型转换
    // 例子：shared_ptr<int> a = new int(100); 这里会进行类型转换，explicit可以杜绝这种转换
    explicit SharedPtr(T* ptr = nullptr);
    ~SharedPtr();
    SharedPtr(const SharedPtr<T>& p);
    SharedPtr(SharedPtr<T>&& p) noexcept;
    SharedPtr<T> operator=(SharedPtr<T>&& p) noexcept;
    SharedPtr<T> operator=(const SharedPtr<T>& p);
    T* operator->() const;
    T& operator*() const;

    T* get() const;
    void reset(T* p = nullptr);
    int32_t use_count();

private:
    T* _ptr;
    std::atomic<int32_t>* _count;
};

template<typename T>
SharedPtr<T>::SharedPtr()
    : _ptr(nullptr)
    , _count(nullptr)
{}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr)
    : _ptr(ptr)
    , _count(ptr ? (new std::atomic<int32_t>(1)) : nullptr)
{}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
    // 内存序，保证释放资源的同步
    if (_count && _count->fetch_sub(1, std::memory_order_acq_rel) == 1) {
        delete _ptr;
        delete _count;
        _ptr = nullptr;
        _count = nullptr;
    }
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& p)
{
    _ptr = p._ptr;
    _count = p._count;
    if (_count != nullptr) {
        _count->fetch_add(1, std::memory_order_relaxed); // 引用计数增加，不需要强内存序
    }
}

template<typename T>
SharedPtr<T> SharedPtr<T>::operator=(const SharedPtr<T>& p)
{
    if (p != *this) {
        this->~SharedPtr();
        _ptr = p._ptr;
        _count = p._count;
        if (_count) {
            _count->fetch_add(1, std::memory_order_relaxed); // 引用计数增加，不需要强内存序
        }
    }

    return *this;
}

// noexcept 告诉编译器不需要生成额外的异常相关的代码，提升运行的效率
// 标准库的某些操作要求移动操作是noexcept的。
template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& p) noexcept
    : _ptr(p._ptr)
    , _count(p._count)
{
    p._ptr = nullptr;
    p._count = nullptr;
}

template<typename T>
SharedPtr<T> SharedPtr<T>::operator=(SharedPtr<T>&& p) noexcept
{
    if (p != *this) {
        this->~SharedPtr();
        _ptr = p._cptr;
        _count = p._count;
        // count 不增加
        p._ptr = nullptr;
        p._count = nullptr;
    }
    return *this;
}

// const表示不会修改成员的值
template<typename T>
T* SharedPtr<T>::operator->() const
{
    return _ptr;
}

template<typename T>
T& SharedPtr<T>::operator*() const
{
    return *_ptr;
}

template<typename T>
T* SharedPtr<T>::get() const
{
    return _ptr;
}

template<typename T>
void SharedPtr<T>::reset(T* p)
{
    this->~SharedPtr();
    _ptr = p;
    _count = p ? (new std::atomic<int32_t>(1)) : nullptr;
}

template<typename T>
int32_t SharedPtr<T>::use_count()
{
    return _count ? _count->load(std::memory_order_acquire) : 0;
}

void test_shared_ptr()
{
    SharedPtr<int> ptr(new int(200));

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&ptr]() -> void {
            for (int j = 0; j < 10000; j++) {
                SharedPtr<int> local_ptr(ptr);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });
    }

    for (int i = 0; i < 10; i++) {
        threads[i].join();
    }

    if (ptr.use_count() == 1) {
        std::cout << "Test passed !!!" << std::endl;
    }
    else {
        std::cout << "Test unpassed !!! use count: " << ptr.use_count() << std::endl;
    }
}

int main()
{
    SharedPtr<int> ptr(new int(32));
    std::cout << "ptr use count " << ptr.use_count() << std::endl;
    test_shared_ptr();
    return 0;
}