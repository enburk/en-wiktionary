template <class Task> struct ioqueue
{
    std::int64_t cargo = 0;
    std::int64_t cargo_upper_bound = 1024*1024;
    // to do: bandwidth statistics

    std::queue<Task> queue;
    std::condition_variable cv;
    std::mutex mutex;
    bool stopped = false;

    bool pop (Task & task) {
        std::unique_lock lock {mutex};
        while (queue.empty()) { if (stopped) return false; cv.wait(lock); }
        task = std::move(queue.front()); queue.pop();
        cargo -= task.size();
        return true;
    }

    void push (Task task) {
        std::unique_lock lock {mutex};
        // this task could exceed limit by itself, so...
        while (cargo > cargo_upper_bound) // w/o + task.size()
        {
            lock.unlock();
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
            lock.lock();
        }
        cargo += task.size(); // before std::move
        queue.push (std::move(task));
        cv.notify_one();
    }

    void stop () {
        std::lock_guard lock {mutex};
        stopped = true;
        cv.notify_all();
    }

    struct iterator
    {
        Task task;
        ioqueue<Task> * queue;
        void operator ++ () {}
        Task operator *  () { return task; }
        bool operator != (iterator) { return queue->pop(task); }
    };

    iterator begin () { return iterator{Task{}, this}; }
    iterator end   () { return iterator{Task{}, nullptr}; }
};
