#include "MemoryArena.h"
#include <gtest/gtest.h>


const int MB = 1 * 1024 * 1024;

struct A {
    int a;
    bool q;
    bool n;
    int m;

    ~A() { std::cout << "delete A" << std::endl; }
};

struct C {
    int a;
    bool q;
    int m;
    bool n;

    ~C() { std::cout << "delete C" << std::endl; }
};

class B {
public:
    B() { std::cout << "create B" << std::endl; }
    ~B() { std::cout << "delete B" << std::endl; }
};


TEST(MemoryArenaTest, SimpleTask1) {
    auto arena = MemoryArena(MB);

    // auto* a = arena.insertData<int>(10);        // 4
    // auto* b = arena.insertData<A>();            // 16
    // auto* q = arena.insertData<bool>(true);     // 1
    // auto* c = arena.insertData<double>(5.5);    // 8
    // auto* d = arena.insertData<char>('X');      // 1
    // auto* e = arena.insertData<short>(42);      // 2
    // auto* f = arena.insertData<long long>(99);  // 8
    // auto* cl = arena.insertData<B>();           // 8

    // arena.printMemoryRegion("int A", a);        // 4
    // arena.printMemoryRegion("A struct B", b);   // 16
    // arena.printMemoryRegion("bool Q", q);       // 1
    // arena.printMemoryRegion("double C", c);     // 8
    // arena.printMemoryRegion("char D", d);       // 1
    // arena.printMemoryRegion("short E", e);      // 2
    // arena.printMemoryRegion("long long F", f);  // 8

    // arena.printMemoryRegion("class B", cl);  // 8 байт

    auto* a = arena.insertData<A>();  // 4 + (1 + 1 + _ + _)  + 4 -> 12
    auto* b =
        arena
            .insertData<C>();  // 4 + (1 + _ + _ + _)  + 4 + (1 + _ + _ + _) -> 16

    auto* c = arena.insertData<B>();  // 4 + (1 + 1 + _ + _)  + 4 -> 12

    arena.printMemoryRegion("Struct A", a);
    arena.printMemoryRegion("Struct C", b);
    arena.printMemoryRegion("class B", c);

    // std::cout << "Begin: " << static_cast<void*>(arena.start) << std::endl;
    // std::cout << "End:   " << static_cast<void*>(arena.end) << std::endl;
}
