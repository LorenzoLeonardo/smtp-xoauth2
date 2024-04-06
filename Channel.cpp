#include "pch.h"

#include "Channel.h"

template <typename T> void Channel<T>::Send(const T &value) {
    std::unique_lock<std::mutex> lock(_mutex);
    _queue.push(value);
    _condition.notify_one();
}

template <typename T> T Channel<T>::Receive() {
    std::unique_lock<std::mutex> lock(_mutex);
    _condition.wait(lock, [this] { return !_queue.empty(); });
    T value = _queue.front();
    _queue.pop();
    return value;
}
