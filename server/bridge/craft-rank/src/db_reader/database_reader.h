#include <vector>

struct ZipCodeInfo {
    int extraDistance{0};
    float lat{0}; // Latitude in radians
    float lon{0}; // Longitude in radians
    int distance{40}; // Neighbour distance
};

struct WorkerInfo {
    float lat{0}; // Latitude in radians
    float lon{0}; // Longitude in radians
    int maxDrivingDistance{40}; // Neighbour distance
    float profileScore{0};
};

namespace db {

    void getNeighboringZips(int zipCode, const std::vector<int>& neighbors);

    void allWorkersForZips(const std::vector<int>& zips, const std::vector<int>& workers);

    ZipCodeInfo getZipInfo(int zipCode);

    WorkerInfo getWorkerInfo(int workerId);

}