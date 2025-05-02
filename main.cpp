#include "LockFreeQueue.h"
#include "ThreadPool.h"

std::chrono::milliseconds getTime() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}
void fooPriority10() {
  std::cout << getTime().count() << " Priority 10" << std::endl;
}

void fooPriority1() {
  std::cout << getTime().count() << " Priority 1" << std::endl;
}

int x = 0;

void foo() { x++; }
int main() {
  // ThreadPool pool;

  // // for (int i = 0; i < 20000; ++i) {
  // //   pool.addTask(10, fooPriority10);
  // // }
  // // std::this_thread::sleep_for(std::chrono::milliseconds(2));
  // // for (int i = 0; i < 20000; ++i) {
  // //   pool.addTask(1, fooPriority1);
  // // }

  // for (int i = 0; i < 50000; ++i) {
  //   pool.addTask<void>(1, foo);
  // }

  // while (!pool.isQueueEmpty()) {
  //   std::this_thread::sleep_for(std::chrono::milliseconds(2));
  // }
  // std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // std::cout << x << std::endl;

  LockFreeQueue<int> a;

  int q = 10;
  a.push(q);
  a.push(++q);
  a.push(++q);
  a.push(++q);

  a.printAllData();
  return 0;
}
