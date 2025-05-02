#include <iostream>
#include <memory>

template <typename T>
class LockFreeQueue {
 private:
  struct Node {
    std::shared_ptr<T> data;
    std::atomic<Node*> next;

    Node() : next(nullptr) {}
    Node(T const& data_) : data(std::make_shared<T>(data_)), next(nullptr) {}
  };
  std::atomic<Node*> head;
  std::atomic<Node*> tail;

 public:
  LockFreeQueue() {
    Node* dummy = new Node();
    head.store(dummy);
    tail.store(dummy);
  }

  void printAllData() {
    Node* tmpHead = head.load()->next;
    while (tmpHead != tail.load()->next) {
      std::cout << *(tmpHead->data) << std::endl;
      tmpHead = tmpHead->next;
    }
  }

  void push(T const& data) {
    Node* new_node = new Node(data);
    Node* old_tail = tail.load();

    while (true) {
      Node* next = old_tail->next.load();

      if (next != nullptr) {
        tail.compare_exchange_weak(old_tail, next);
        continue;
      }

      if (old_tail->next.compare_exchange_weak(next, new_node)) {
        tail.compare_exchange_weak(old_tail, new_node);
        break;
      }
    }
  }

  std::shared_ptr<T> pop() {
    Node* old_head;
    Node* old_tail;

    while (true) {
      old_head = head.load();
      old_tail = tail.load();
      Node* next = old_head->next.load();

      if (old_head == old_tail) {
        if (next == nullptr) {
          return std::shared_ptr<T>();
        }
        tail.compare_exchange_weak(old_tail, next);
      } else {
        if (head.compare_exchange_weak(old_head, next)) {
          std::shared_ptr<T> res = next->data;
          delete old_head;
          return res;
        }
      }
    }
  }
};
