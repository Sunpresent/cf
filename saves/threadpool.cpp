#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <unistd.h>
#include <vector>
#include <memory>

using namespace std;

class _ThreadPool
{
public:
    friend class ThreadPool;

    _ThreadPool()
    {
        this->threads = 0;
    }

    ~_ThreadPool()
    {
    }

    void join()
    {
        std::unique_lock<std::mutex> lk(mtx);
        cv.wait(lk, [this]() { return threads == 0; });
    }

    void lock()
    {
        mtx.lock();
    }

    void unlock()
    {
        mtx.unlock();
    }

    void inc_threads()
    {
        threads++;
    }

    void dec_threads()
    {
        threads--;
    }

    unsigned int get_threads()
    {
        return threads;
    }

private:
    void threadFunc()
    {
        while (true) {
            lock();
            if (tasks.empty()) {
                dec_threads();
                unlock();
                cv.notify_one();
                break;
            } else {
                auto task = std::move(tasks.front());
                tasks.pop();
                unlock();
                task();
            }
        }
    }

private:
    unsigned int threads;
    std::mutex mtx;
    std::queue<std::function<void()>> tasks;
    std::condition_variable cv;
};

class ThreadPool
{
public:
    ThreadPool(unsigned int size) : _thread_pool(new _ThreadPool())
    {
        this->stop = false;
        this->size = size;
    }

    ~ThreadPool()
    {
        std::cout << "destructor called" << std::endl;
    }

    void join()
    {
        stop = true;
        _thread_pool->join();
    }

    template <class F, class... Args>
    std::future<std::result_of_t<F(Args...)>> push_action(F &&f, Args &&...args)
    {
        using return_type = std::result_of_t<F(Args...)>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        lock();

        if (stop) {
            throw std::runtime_error("The thread pool has been stop.");
        }

        if (_thread_pool->get_threads() < size) {
            _thread_pool->inc_threads();
            std::thread([task](std::shared_ptr<_ThreadPool> threadPool) {
                (*task)();
                threadPool->threadFunc();
            }, _thread_pool).detach();
        } else {
            _thread_pool->tasks.emplace([task]() { (*task)(); });
        }

        unlock();

        return res;
    }

private:
    void lock()
    {
        _thread_pool->lock();
    }

    void unlock()
    {
        _thread_pool->unlock();
    }

private:
    bool stop;
    unsigned int size;
    shared_ptr<_ThreadPool> _thread_pool;
};

int main(int argc, char **argv)
{
    {
        ThreadPool threadpool(5);

        auto func = [](int id) {
            for (int i = 0; i < 10; i++) {
                std::cout << "do work " << id << std::endl;
                sleep(1);
            }
        };

        for (int i = 0; i < 5; i++) {
            threadpool.push_action(func, i + 1);
        }
    }

    sleep(20);

    return 0;
}
