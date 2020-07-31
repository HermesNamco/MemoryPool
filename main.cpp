#include <QCoreApplication>
#include <iostream>
#include "memorypool.h"

using namespace std;

struct test{
    int a;
    char b;
    long long c;
    vector<int> d;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    test data;
    test* data2;
    test* data3;
    cout << sizeof(data) << endl;
    data2 = (test *)MemoryPool::getInstance().alloc(sizeof(test));
    data3 = (test *)MemoryPool::getInstance().alloc(sizeof(test));
    cout << sizeof (*data2) << endl;
    data2->a = 3;
    cout << data2->a << endl;
    cout << "data2:" << data2 << endl;
    cout << "data3:" << data3 << endl;
    MemoryPool::getInstance().getInfo();

    return a.exec();
}
