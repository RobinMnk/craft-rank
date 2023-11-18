#include <vector>

struct ZipCodeInfo {
    int extraDistance{0};
    float lat{0}; // Latitude in radians
    float lon{0}; // Longitude in radians
    int distance{40}; // Neighbour distance
};

namespace db {
    void getNeighboringZips(int zipCode, const std::vector<int>& neighbors);

    ZipCodeInfo getZipInfo(int zipCode);


}