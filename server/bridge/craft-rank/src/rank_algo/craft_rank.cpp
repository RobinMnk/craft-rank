#include "craft_rank.h"

// Constructor
CraftRankHandler::CraftRankHandler() {
    // Initialize any necessary resources here
}

// Destructor
CraftRankHandler::~CraftRankHandler() {
    // Cleanup or release resources here, if needed
}

/**
// Function to query a database and fill the ZipCodeInfo struct
void CraftRankHandler::queryDatabase(ZipCodeInfo& zipCode, BoundingCoordinates& boundingC) {
    // Replace this with your actual database query logic
    // For demonstration purposes, we'll set some example values.

    // Example: Query extraDistance, latitude, longitude, and max_driving_dist from the database
    zipCode.extraDistance = 0;
    zipCode.lat = 1.3963; // Example latitude in radians
    zipCode.lon = -0.6981; // Example longitude in radians
    zipCode.distance = 1000;

    computeBoundingCoordinates(zipCode, boundingC);
    std::cout << "Calculate distance" << calculateDistance(40.6892, -74.0444, 48.8583, 2.2945) << std::endl;

} */

void CraftRankHandler::generateRelevantZipCodes(int startZip, std::vector<int> relevantZips) {
    std::unordered_set<int> visited;
    relevantZips.clear();

    std::queue<int> q{};
    q.push(startZip);

    std::vector<int> neighbors;

    while(!q.empty()) {
        int currentZip = q.front();
        q.pop();
        if(visited.contains(currentZip) || distanceBetweenZips(startZip, currentZip) > MAX_ZIP_DISTANCE) {
            continue;
        }
        visited.insert(currentZip);
        relevantZips.push_back(currentZip);

        db::getNeighboringZips(currentZip, neighbors);
        for(int nb: neighbors) {
            q.push(nb);
        }
    }
}

void CraftRankHandler::getListOfWorkers(int zipCode) {
    std::vector<int> relevantZips;
    generateRelevantZipCodes(zipCode, relevantZips);

    std::vector<int> workers;
    db::allWorkersForZips(relevantZips, workers);

    // TODO: Give all workers to ParallelRank for ranking
}


double rank(int zipCode, int workerId) {
    WorkerInfo workerInfo = db::getWorkerInfo(workerId);
    ZipCodeInfo zipCodeInfo = db::getZipInfo(zipCode);

    auto adjustedMaxDriveDistance = static_cast<double>(workerInfo.maxDrivingDistance + zipCodeInfo.extraDistance);
    double distance = distanceBetweenZipAndWorker(zipCode, workerId);

    if (distance > adjustedMaxDriveDistance) {
        return -10;
    }

    double distance_score = 1 - (distance / adjustedMaxDriveDistance);
    double distance_weight = distance > 80 ? 0.01 : 0.15;

    return distance_weight * distance_score + (1 - distance_weight) * workerInfo.profileScore;
}


void ParallelRank::init(int zip, std::shared_ptr<std::vector<int>>& workerIds, std::shared_ptr<std::vector<Result>>& res) {
    baseZip = zip;
    queue = workerIds;
    results = res;
}

void ParallelRank::insert(int value) {
    std::lock_guard<std::mutex> lock(mtx_queue);
    queue->push_back(value);
    cv.notify_one();
}

void ParallelRank::safeWrite(Result res) {
    std::lock_guard<std::mutex> lock(mtx_results);
    results->push_back(res);
}

void ParallelRank::process() {
    while (!queue->empty()) {
        std::unique_lock<std::mutex> lock(mtx_queue);
        cv.wait(lock, [this] { return !queue->empty(); });
        int workerId = queue->back();
        queue->pop_back();
        lock.unlock();

        double rk = rank(baseZip, workerId);
        if (rk > 0) {
            Result res{workerId, rk};
            safeWrite(res);
        }
    }
}

std::thread ParallelRank::start() {
    std::thread t(&ParallelRank::process, this);
    return t;
}




