#include <mutex>
#include <atomic>
#include <iostream>

/*
*   单例模式实现的三个要点
*   1、拷贝构造、移动构造，拷贝赋值、移动赋值要禁用
*   2、构造和析构要私有化
*   3、静态成员变量和静态返回单例的成员函数
*
*/

// 懒加载
class SingletonStatic
{
public:
    static SingletonStatic* getInstance()
    {
        static SingletonStatic instance;
        return &instance;
    }
private:
    SingletonStatic() {
        std::cout << "SingletonStatic" << std::endl;
    }
    ~SingletonStatic() {
        std::cout << "~SingletonStatic" << std::endl;
    }
    SingletonStatic(const SingletonStatic&) = delete;
    SingletonStatic& operator=(const SingletonStatic&) = delete;
    SingletonStatic(SingletonStatic&&) = delete;
    SingletonStatic& operator=(SingletonStatic&&) = delete;
};

// 带模板的懒加载
template<typename T>
class SingletonTemplate
{
public:
    static T* getInstance() {
        static T instance;
        return &instance;
    }
protected:
    SingletonTemplate() {
        std::cout << "SingletonTemplate" << std::endl;
    } // 保证子类可以继承基类的构造和析构
    virtual ~SingletonTemplate() {
        std::cout << "~SingletonTemplate" << std::endl;
    }
private:
    SingletonTemplate(const T&) = delete;
    SingletonTemplate& operator=(const T&) = delete;
    SingletonTemplate(T&&) = delete;
    SingletonTemplate& operator=(T&&) = delete;
};

class DisgnPattern : public SingletonTemplate<DisgnPattern>
{
    friend class SingletonTemplate<DisgnPattern>; // 保证基类可以访问子类的私有构造函数
private:
    DisgnPattern(){
        std::cout << "DisgnPattern" << std::endl;
    }
    ~DisgnPattern(){
        std::cout << "~DisgnPattern" << std::endl;
    }

};

class SingletonAtomic
{
public:
    // 双重加锁是为了放在多线程环境下，一个线程判断当前为null，去创建实例；但此时正好另一个线程已经创建出实例，会导致实例多次创建，所以需要再加一次判断保护
    // 锁的作用是保护判断为null之后，后续的操作只有单个线程执行
    // 内存屏障：acquire保证了加载语句下面的读写语句不会优化到上面，release保证存储操作前，之前的写操作语句对线程可见，防止之前的语句重排到屏障之后。
    // atomic本身保证操作的原子性，不会有多部操作
    static SingletonAtomic* getInstance() {
        SingletonAtomic* tmp = _instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            std::lock_guard<std::mutex> lock(_mutex);
            tmp = _instance.load(std::memory_order_acquire);
            if (tmp == nullptr) {
                tmp = new SingletonAtomic();
                std::atomic_thread_fence(std::memory_order_release);
                _instance.store(tmp, std::memory_order_relaxed);
                atexit(Destructor);
            }
        }
        return tmp;
    }
private:
    static void Destructor() {
        SingletonAtomic* tmp = _instance.load(std::memory_order_relaxed);
        if (tmp != nullptr) {
            delete tmp;
            tmp = nullptr;
        }
    }
    SingletonAtomic() {
        std::cout << "SingletonAtomic" << std::endl;
    }
    ~SingletonAtomic() {
        std::cout << "~SingletonAtomic" << std::endl;
    }
    SingletonAtomic(const SingletonAtomic&) = delete;
    SingletonAtomic& operator=(const SingletonAtomic&) = delete;
    SingletonAtomic(SingletonAtomic&&) = delete;
    SingletonAtomic& operator=(SingletonAtomic&&) = delete;

    static std::mutex _mutex;
    static std::atomic<SingletonAtomic*> _instance;
};

// 定义静态成员变量
std::mutex SingletonAtomic::_mutex;
std::atomic<SingletonAtomic*> SingletonAtomic::_instance{nullptr};

int main()
{
    SingletonAtomic* at = SingletonAtomic::getInstance();
    SingletonStatic* st = SingletonStatic::getInstance();
    DisgnPattern* dt = DisgnPattern::getInstance();
}