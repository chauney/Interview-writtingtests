#include <iostream>
#include <cstring>
#include <assert>

namespace interview
{

class string
{
public:
    string(const char* str = "") { // 默认构造
        if (str == nullptr) {
            mData = new char[1];
            *mData = '\0';
        }
        else {
            mLen = strlen(str);
            mCapcacity = mLen;
            mData  = new char[mLen + 1];
            strcpy(mData, str);
        }
    }

    string(const string& s) { // 拷贝构造
        mLen = s.mLen;
        mCapacity = s.mCapacity;
        mData = new char[mCapacity + 1];
        strcpy(mData, s.mData);
    }

    string& operator=(const string& s) { // 赋值构造
        if (this != &s) {
            delete[] mData;
            mLen = s.mLen;
            mCapacity = s.mCapacity;
            mData = new char[mCapacity + 1];
            strcpy(mData, s.mData);
        }
        return *this;
    }

    ~string(){ // 析构
        delete[] mData;
    }

private:
    char* mData;
    size_t mLen;
    size_t mCapacity;
};

}
