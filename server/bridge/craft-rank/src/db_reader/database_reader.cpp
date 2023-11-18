#include "database_reader.h"
#include <cstdlib> 

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
        const char* dbShortName = "check24-profis.db";
        const char* homeDir = getenv("HOME");
        std::string dbFilePath = std::string(homeDir) + "/craft-rank/server/database/" + std::string(dbShortName);
        const char *dbName = dbFilePath.c_str();
        std::string query = "SELECT lat, lon FROM postcode WHERE postcode IN ('" + zipCode1 + "', '" + zipCode2 + "');";
        const char *queryStr = query.c_str();

        int rc = sqlite3_open_v2(dbName, &db, SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE ,nullptr);

        if (rc)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            throw std::runtime_error("Can't open database");
        }

        std::vector<LatLon> latLonList;
        // Execute the SQL query and provide the callback function
        rc = sqlite3_exec(db, queryStr, Callback, &latLonList, nullptr);

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