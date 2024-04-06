#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

template <typename T> class Channel {
  public:
    void Send(const T &value);

    T Receive();

  private:
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _condition;
};
