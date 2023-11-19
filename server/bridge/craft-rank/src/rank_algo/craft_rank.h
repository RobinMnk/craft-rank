#ifndef CRAFT_RANK_H
#define CRAFT_RANK_H

#include "core.h"
#include "../db_reader/database_reader.h"
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <queue>
#include <algorithm>

const double MAX_ZIP_DISTANCE = 25'000;

struct Result {
    int workerId;
    double rank;
};

template<typename T>
class ThreadSafeList {
    std::mutex mtx{};
    std::condition_variable cv{};
public:
    std::vector<T> list{};

    void insert(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        list.push_back(value);
        cv.notify_one();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mtx);
        list.clear();
        cv.notify_one();
    }

    void notify() {
        cv.notify_all();
    }

    T get() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !list.empty(); });
        T value = list.back();
        list.pop_back();
        lock.unlock();
        return value;
    }

    bool empty() {
        return list.empty();
//        std::unique_lock<std::mutex> lock(mtx);
//        cv.wait(lock, [this] { return !list.empty(); });
//        bool isEmpty = list.empty();
//        list.pop_back();
//        lock.unlock();
//        return isEmpty;
    }
};

using ResultList = std::shared_ptr<ThreadSafeList<Result>>;
using IDList = std::shared_ptr<ThreadSafeList<int>>;


class ParallelRank {
    ResultList results;
    IDList queue;
    int baseZip;
    bool active{true};

public:
    ParallelRank(int zip, IDList& q, ResultList& res);

    std::thread start();

    void stop();

    void process();
};


class CraftRankHandler {
public:
    static void generateRankedListOfWorkers(int zipCode, ResultList& res);

private:
    static double rank(int zipCode, int workerId);

    static void generateRelevantZipCodes(int startingZip, IDList& relevantZips);

    friend ParallelRank;
};


#endif // CRAFT_RANK_H
