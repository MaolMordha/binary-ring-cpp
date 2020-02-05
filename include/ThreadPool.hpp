#ifndef THREAD_POOL_HPP_
#define THREAD_POOL_HPP_

#include <condition_variable>
#include <cstddef>
#include <future>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

class ThreadPool {
    public:
        ThreadPool(std::size_t threadNumber);
        ~ThreadPool();
    
        template <typename F, class ... Args>
        auto enqueue(F && function, Args && ... args)
            -> std::future<typename std::result_of<F(Args ...)>::type>
        {
            using return_type = typename std::result_of<F(Args ...)>;

            std::packaged_task<return_type> task(
                std::bind(std::forward<F>(function), std::forward(args) ...)
            );
        }

    private:
        void workerRoutine();

        std::vector<std::thread> workers_;
        std::condition_variable conditionVariable_;
        std::mutex mutex_;
        bool stop_ = false;
}; /* class ThreadPool */

#endif /* THREAD_POOL_HPP_ */
