#include "craft_rank.h"

void CraftRankHandler::generateRelevantZipCodes(int startZip, IDList& relevantZips) {
    std::unordered_set<int> visited;
    relevantZips->clear();

    std::queue<int> q{};
    q.push(startZip);

    std::vector<int> neighbors;

    // Breadth-first search
    while(!q.empty()) {
        int currentZip = q.front();
        q.pop();
        if(visited.contains(currentZip) || distanceBetweenZips(startZip, currentZip) > MAX_ZIP_DISTANCE) {
            continue;
        }
        visited.insert(currentZip);
        relevantZips->insert(currentZip);

        db::getNeighboringZips(currentZip, neighbors);
        for(int nb: neighbors) {
            q.push(nb);
        }
    }
}

bool compareResults(const Result& a, const Result& b) {
    return a.rank > b.rank;
}

/**
 * Main function for calculating the ranks of all relevant workers
 */
void CraftRankHandler::generateRankedListOfWorkers(int zipCode, ResultList& res) {
    IDList relevantZips;

    // In Parallel: find all workers and calculate the ranks for every found zipcode
    ParallelRank pr(zipCode, relevantZips, res);
    std::thread t = pr.start();

    // Obtain all zip codes that are close via BFS
    generateRelevantZipCodes(zipCode, relevantZips);
    pr.stop();
    t.join();


    // all workers found and ranked -> now sort
    if (relevantZips->list.size() > 40) {
        // QuickSelect 20 times to make first 20 entries sorted
        for(int i{0}; i < 20; i++) {
            std::nth_element(res->list.begin() + i, res->list.begin() + i + 1, res->list.end(), compareResults);
        }
    } else {
        // List not long enough -> sorting is quicker
        std::sort(res->list.begin(), res->list.end(), compareResults);
    }
}


double CraftRankHandler::rank(int zipCode, int workerId) {
    WorkerInfo workerInfo = db::getWorkerInfo(workerId);
    ZipCodeInfo zipCodeInfo = db::getZipInfo(zipCode);

    auto adjustedMaxDriveDistance = static_cast<double>(workerInfo.maxDrivingDistance + zipCodeInfo.extraDistance);
    double distance = CraftRankHandler::distanceBetweenZipAndWorker(zipCode, workerId);

    if (distance > adjustedMaxDriveDistance) {
        // worker would not drive here, return negative number and sort out
        return -10;
    }

    double distance_score = 1 - (distance / adjustedMaxDriveDistance);
    double distance_weight = distance > 80 ? 0.01 : 0.15;

    return distance_weight * distance_score + (1 - distance_weight) * workerInfo.profileScore;
}

ParallelRank::ParallelRank(int zip, IDList& workerIds, ResultList& res) {
    baseZip = zip;
    queue = workerIds;
    results = res;
}

void ParallelRank::process() {
    while (active || !queue->empty()) {
        int zipcode = queue->get();

        std::vector<int> workers;
        db::allWorkersForSingleZip(zipcode, workers);

        for(int workerId: workers) {
            double rk = CraftRankHandler::rank(baseZip, workerId);
            if (rk > 0) {
                Result res{workerId, rk};
                results->insert(res);
            }
        }
    }
}

std::thread ParallelRank::start() {
    std::thread t(&ParallelRank::process, this);
    return t;
}

void ParallelRank::stop() {
    active = false;
    queue->notify();
}
