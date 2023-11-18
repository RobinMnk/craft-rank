#include "craft_rank.h"

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


void CraftRankHandler::generateRankedListOfWorkers(int zipCode, ResultList& res) {
    // Obtain all zip codes that are close enough via BFS
    IDList relevantZips;
    generateRelevantZipCodes(zipCode, relevantZips);

    ParallelRank pr(zipCode, relevantZips, res);
    std::thread t = pr.start();
    t.join();


    // TODO: Give result list to QuickSelect
//    std::nth_element(v.begin(), v.begin() + 1, v.end());
}


double CraftRankHandler::rank(int zipCode, int workerId) {
    WorkerInfo workerInfo = db::getWorkerInfo(workerId);
    ZipCodeInfo zipCodeInfo = db::getZipInfo(zipCode);

    auto adjustedMaxDriveDistance = static_cast<double>(workerInfo.maxDrivingDistance + zipCodeInfo.extraDistance);
    double distance = CraftRankHandler::distanceBetweenZipAndWorker(zipCode, workerId);

    if (distance > adjustedMaxDriveDistance) {
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
    while (!queue->empty()) {
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
