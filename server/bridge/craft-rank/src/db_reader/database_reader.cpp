#include "database_reader.h"

namespace db {

    // Callback function to retrieve latitude and longitude values from the database
    int Callback(void *data, int argc, char **argv, char **colNames)
    {
        auto* latLonList = static_cast<std::vector<LatLon>*>(data);
        if (argc != 2)
        {
            std::cerr << "Error: Not enough rows returned from the database." << std::endl;
            return 1;
        }

        // Extract latitude and longitude values
        double lat1 = std::stod(argv[0]);
        double lon1 = std::stod(argv[1]);
        latLonList->push_back({lat1, lon1});

        return 0;
    }

    double queryDatabaseForDistance(const std::string& zipCode1, const std::string& zipCode2)
    {
        sqlite3 *db;
        const char *dbName = "/home/theresa/craft-rank/server/database/check24-profis.db";
        const char *query = "SELECT lat, lon FROM postcode WHERE postcode IN ('01067', '01069');";

        int rc = sqlite3_open(dbName, &db);

        if (rc)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            throw std::runtime_error("Can't open database");
        }

        std::vector<LatLon> latLonList;
        // Execute the SQL query and provide the callback function
        rc = sqlite3_exec(db, query, Callback, &latLonList, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            throw std::runtime_error("SQL error");
        }

        // Close the database
        sqlite3_close(db);

        // Calculate the distance
        double distance = calculateDistance(latLonList[0].lat, latLonList[0].lon, latLonList[1].lat, latLonList[1].lat);

        // Output the calculated distance
        std::cout << "Distance between the two zip codes: " << distance << " kilometers" << std::endl;

        return distance;
    }

}