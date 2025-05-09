#pragma once

#include <functional>
#include <utility>

template <typename ResultType>
class TypeSafeTask {
public:
    template <typename Callable>
    explicit TypeSafeTask(Callable&& func)
        : task(std::forward<Callable>(func)) {
        static_assert(std::is_invocable_r<ResultType, Callable>::value,
                      "Callable does not match expected signature!");
    }

    ResultType execute() {
        return task();
    }

private:
    std::function<ResultType()> task;
};

