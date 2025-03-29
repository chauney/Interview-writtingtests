/*
描述：手写string类
完整剖析：https://cloud.tencent.com/developer/article/2456266

*/


#include <iostream>
#include <cstring>

namespace interview
{

class mystring
{
public:
    mystring(const char* str = "") { // 默认构造
        std::cout << "default constructor" << std::endl;
        if (str == nullptr) {
            mData = new char[1];
            *mData = '\0';
        }
        else {
            mLen = strlen(str);
            mCapacity = mLen;
            mData  = new char[mLen + 1];
            strcpy(mData, str);
        }
    }

    mystring(const mystring& s) { // 拷贝构造
        mLen = s.mLen;
        mCapacity = s.mCapacity;
        mData = new char[mCapacity + 1];
        strcpy(mData, s.mData);
    }

    mystring& operator=(const mystring& s) { // 赋值构造
        if (this != &s) {
            delete[] mData;
            mLen = s.mLen;
            mCapacity = s.mCapacity;
            mData = new char[mCapacity + 1];
            strcpy(mData, s.mData);
        }
        return *this;
    }

    char* get_mystring()
    {
        return mData;
    }

    ~mystring(){ // 析构
        std::cout << "default de constructor" << std::endl;
        delete[] mData;
    }

    // 迭代器
    typedef char* iterator;
    typedef const char* const_iterator;

    iterator begin() {
        return mData;
    }

    const_iterator begin() const {
        return mData;
    }

    const_iterator end() const {
        return mData + mLen;
    }

    iterator end() {
        return mData + mLen;
    }

    // find
    size_t find(const char c, size_t pos = 0) const {
        if (pos >= mLen) {
            return -1;
        }
        for (int i = pos; i < mLen; i++) {
            if (mData[i] == c) {
                return i;
            }
        }
        return -1;
    }

    size_t find(const char* str, size_t pos = 0) const {
        if (str == nullptr) {
            return -1;
        }
        size_t len = strlen(str);
        if (len == 0) { // 字符串本身检查
            return (pos <= mLen) ? pos : -1;
        }

        if (pos >= mLen || pos + len > mLen) {  // 边界检查
            return -1;
        }

        for (int i = pos; i < mLen; i++) {
            if (mData[i] == str[0]) {
                int j = 0;
                while (j < len && mData[i + j] == str[j]) {
                    j++;
                }
                if (j == len) {
                    return i;
                }
            }
        }

        // char * p = strstr(mData + pos, str);
        // if (p != nullptr) {
        //     return p - mData;
        // }

        return -1;
    }

    // insert
    mystring& insert(const char c, size_t pos = 0) {
        if (pos > mLen) return *this;

        if (mLen == mCapacity) {
            reserve(mCapacity * 2);
        }

        for (size_t i = mLen; i > pos; i--) {
            mData[i] = mData[i - 1];
        }

        mData[pos] = c;
        mLen++;
        mCapacity *= 2;
        mData[mLen] = '\0';

        return *this;
    }

    mystring& insert(const char* str, size_t pos = 0) {
        if (pos > mLen) return *this;
        size_t strLen = strlen(str);
        if (str == nullptr || strLen == 0) {
            return *this;
        }

        if (mLen + strLen > mCapacity) {
            reserve(mLen + strLen);
        }

        for (size_t i = mLen; i + 1 > pos; i--) {
            mData[i + strLen] = mData[i];
        }

        memcpy(mData + pos, str, strLen);
        mLen += strLen;
        mCapacity = mLen + strLen;
        mData[mLen] = '\0';

        return *this;
    }

    // delete
    mystring erase(size_t pos, size_t len = -1) {
        if (pos > mLen) return *this;

        if (len == 0 || pos == mLen) {
            return *this;
        }

        if (len == -1 || pos + len > mLen) {
            len = mLen - pos;
        }

        for (size_t i = pos; i + len < mLen; i++) {
            mData[i] = mData[i + len];
        }

        mLen -= len;
        mData[mLen] = '\0';

        return *this;
    }

    mystring& operator+(const char* str) {
        if (str == nullptr) {
            return *this;
        }

        size_t len = strlen(str);
        if (len == 0) {
            return *this;
        }

        size_t newLen = mLen + len;

        if (newLen + 1 > mCapacity) {
            reserve(newLen + 1);
        }

        memcpy(mData + mLen, str, len);
        mLen = newLen;
        mData[mLen] = '\0';

        return *this;
    }
    
    size_t size() {
        return mLen;
    }

private:
    void reserve (const size_t capacity) {
        if (capacity <= mCapacity) return;

        char* newStr = new char[capacity + 1];
        memcpy(newStr, mData, mLen + 1);

        delete[] mData;
        mData = newStr;
        mCapacity = capacity;
    }

    char* mData;
    size_t mLen;
    size_t mCapacity;
};

}

int main()
{
    interview::mystring s1("hello");
    std::cout << "my mystring test" << std::endl;

    for (interview::mystring::iterator it = s1.begin(); it != s1.end(); it++) {
        *it = *it + 1;
    }
    std::cout << "mystring " << s1.get_mystring() << std::endl;

    size_t pos = s1.find('m');
    std::cout << "find pos " << pos << std::endl;
    pos = s1.find("m", 3);
    std::cout << "find pos " << pos << std::endl;

    size_t pos1 = s1.find("ifmmp");
    std::cout << "find pos " << pos1 << std::endl;

    // insert test
    s1.insert('a', 2);
    std::cout << "mystring " << s1.get_mystring() << std::endl;
    s1.insert("bbbbbbbb", 3);
    std::cout << "mystring " << s1.get_mystring() << " size: " << s1.size() << std::endl;

    s1.erase(2, 3);
    std::cout << "mystring " << s1.get_mystring() << " size: " << s1.size() << std::endl;

    interview::mystring s2 = s1 + "1111";
    std::cout << "mystring " << s2.get_mystring() << " size: " << s2.size() << std::endl;

    return 0;
}
