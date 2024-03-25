#include <iostream>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <type_traits>
#include <atomic>
#include <cassert>
#include <utility>
#include <functional>
#include <any>

class thread_pool {
public:

    thread_pool(uint32_t num_threads) {
        threads.reserve(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(std::thread(&thread_pool::run, this));
        }
    }

    template <typename Func, typename ...Args>
    bool add_task(const Func& func, Args&&... args) {

        std::unique_lock<std::mutex> lock(task_mtx);
        queueFrames.push(std::bind(func, args...));
        task_cv.notify_one();
        return true;
    }


    ~thread_pool() {
        quite = true;
        task_cv.notify_all();
        for (int i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }
    }

private:

    void run() {
        while (!quite) {
            std::unique_lock<std::mutex> lock(task_mtx);
            task_cv.wait(lock, [this]()->bool {
                return !queueFrames.empty() || quite;
                });

            if (!queueFrames.empty()) {
                auto func = std::move(queueFrames.front());
                queueFrames.pop();
                lock.unlock();
                func();
            }
        }
    }

    std::queue<std::function<std::any()>> queueFrames;
    std::vector<std::thread> threads;
    std::condition_variable task_cv;
    std::mutex task_mtx;
    std::atomic<bool> quite{ false };
};

