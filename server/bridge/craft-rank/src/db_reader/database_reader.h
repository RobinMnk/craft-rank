#include <vector>

struct ZipCodeInfo {
    int extraDistance{0};
    float lat{0}; // Latitude in radians
    float lon{0}; // Longitude in radians
    int distance{40}; // Neighbour distance
};

namespace db {

    void getNeighboringZips(int zipCode, const std::vector<int>& neighbors);

    void allWorkersForZips(const std::vector<int>& zips, const std::vector<int>& workers);

    ZipCodeInfo getZipInfo(int zipCode);

}