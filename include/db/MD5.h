#pragma once
#include <vector>
#include <QString>

class MD5
{
private:
    unsigned int tempA, tempB, tempC, tempD, strlength;
    MD5();
    // F函数
    inline unsigned int F(unsigned int b, unsigned int c, unsigned int d);
    // G函数
    inline unsigned int G(unsigned int b, unsigned int c, unsigned int d);
    // H函数
    inline unsigned int H(unsigned int b, unsigned int c, unsigned int d);
    // I函数
    inline unsigned int I(unsigned int b, unsigned int c, unsigned int d);
    // 移位操作函数
    inline unsigned int shift(unsigned int a, unsigned int n);
    // 循环压缩
    void iterateFunc(unsigned int *X, int size = 16);
    // 填充字符串
    std::vector<unsigned int> padding(const std::string &src);
    // 整理输出
    std::string format(unsigned int num);
    // 编码函数
    std::string encoding(const std::string &src);

public:
    // 接口
    static std::string encode(const std::string &src);
    static QString encode(const QString &src);
};