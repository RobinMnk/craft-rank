#include "database_reader.h"
#include <cstdlib> 
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

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

    double queryDatabaseForDistance(int zipCode1, int zipCode2)
    {
        sqlite3 *db;
        const char* dbShortName = "check24-profis.db";
        const char* homeDir = getenv("HOME");
        std::string dbFilePath = std::string(homeDir) + "/craft-rank/server/database/" + std::string(dbShortName);
        const char *dbName = dbFilePath.c_str();
        int width = 5;
        std::ostringstream oss;
        oss << std::setw(width) << std::setfill('0') << zipCode1;
        std::ostringstream oss2;
        oss2 << std::setw(width) << std::setfill('0') << zipCode2;
        std::string numStr = oss.str();
        std::string numStr2 = oss2.str();
        std::string query = "SELECT lat, lon FROM postcode WHERE postcode IN ('" + numStr + "', '" + numStr2 + "');";
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

    void getNeighboringZips(int zipCode, std::vector<int>& neighbors){
        sqlite3 *db;
        const char* dbShortName = "check24-profis.db";
        const char* homeDir = getenv("HOME");
        std::string dbFilePath = std::string(homeDir) + "/craft-rank/server/database/" + std::string(dbShortName);
        const char *dbName = dbFilePath.c_str();
        int width = 5;
        std::ostringstream oss;
        oss << std::setw(width) << std::setfill('0') << zipCode;
        std::string numStr = oss.str();  

        std::string query = "SELECT neighbours FROM postcode WHERE postcode='" + numStr + "';";
        const char *queryStr = query.c_str();

        int rc = sqlite3_open_v2(dbName, &db, SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE ,nullptr);

        if (rc)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            throw std::runtime_error("Can't open database");
        }

        // Execute the SQL query and get the result
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, queryStr, -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            throw std::runtime_error("SQL error");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the value from the first column (assuming it's a string)
            const char *neighborsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

            // Now, you have the neighbors string, and you can split and process it as shown in the previous response
            std::string neighborsStrCpp(neighborsStr);

            // Create a string stream to split the string
            std::stringstream ss(neighborsStrCpp);
            std::string token;

            while (std::getline(ss, token, ',')) {
                // Convert the token (substring) to an integer and add it to the vector
                int neighborZip = std::stoi(token);
                neighbors.push_back(neighborZip);
            }
        }

        // Finalize the statement and close the database
        sqlite3_finalize(stmt);
        sqlite3_close(db);   
    }

    void allWorkersForZips(const std::vector<int>& zips, std::vector<int>& workers){
        sqlite3 *db;
        const char* dbShortName = "check24-profis.db";
        const char* homeDir = getenv("HOME");
        std::string dbFilePath = std::string(homeDir) + "/craft-rank/server/database/" + std::string(dbShortName);
        const char *dbName = dbFilePath.c_str();
        int width = 5;
        std::ostringstream oss;

        std::string query = "SELECT id FROM service_provider_profile WHERE postcode IN (";
        for (int i = 0; i < zips.size(); i++) {
            oss << std::setw(width) << std::setfill('0') << zips[i];
            query += "'" + oss.str() + "'";  
            if (i < zips.size() - 1) {
                query += ", ";
            }
            oss.str("");
        }
        query += ");";
        const char *queryStr = query.c_str();

        int rc = sqlite3_open_v2(dbName, &db, SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE ,nullptr);

        if (rc)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            throw std::runtime_error("Can't open database");
        }

        // Execute the SQL query and get the result
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, queryStr, -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            throw std::runtime_error("SQL error");
        }

        // Fetch the result (assuming there is only one row in the result set)
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the value from the first column (assuming it's a string)
            int workerId = sqlite3_column_int(stmt, 0);
            workers.push_back(workerId);
        }

        // Finalize the statement and close the database connection
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    void getZipInfo(int zipCode, ZipCodeInfo& zipInfo){
        sqlite3 *db;
        const char* dbShortName = "check24-profis.db";
        const char* homeDir = getenv("HOME");
        std::string dbFilePath = std::string(homeDir) + "/craft-rank/server/database/" + std::string(dbShortName);
        const char *dbName = dbFilePath.c_str();

        int width = 5;
        std::ostringstream oss;
        oss << std::setw(width) << std::setfill('0') << zipCode;
        std::string numStr = oss.str();  
        std::cout << "Zip Code: " << numStr << std::endl;
        std::string query = "SELECT lon, lat, postcode_extension_distance_group FROM postcode WHERE postcode='" + numStr + "';";
        const char *queryStr = query.c_str();

        int rc = sqlite3_open_v2(dbName, &db, SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE ,nullptr);

        if (rc)
        {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            throw std::runtime_error("Can't open database");
        }

        // Execute the SQL query and get the result
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, queryStr, -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            throw std::runtime_error("SQL error");
        }

        // Fetch the result (assuming there is only one row in the result set)
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get the values from the columns
            zipInfo.lon = sqlite3_column_double(stmt, 0);
            zipInfo.lat = sqlite3_column_double(stmt, 1);
            const char * tmp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            if (tmp != nullptr) {
                std::string group(tmp);
                if (group == "group_a") {
                    zipInfo.extraDistance = 0;
                } else if (group == "group_b") {
                    zipInfo.extraDistance = 2000;
                } else if (group == "group_c") {
                    zipInfo.extraDistance = 5000;
                } else {
                    // Handle the case when group is something other than the expected values
                }
            } else {
                throw std::runtime_error("Error: postcode_extension_distance_group is NULL");
            }
        }

        // Finalize the statement and close the database connection
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    void getWorkerInfo(int workerId, WorkerInfo& workerInfo){
        sqlite3 *db;
        const char* dbShortName = "check24-profis.db";
        const char* homeDir = getenv("HOME");
        std::string dbFilePath = std::string(homeDir) + "/craft-rank/server/database/" + std::string(dbShortName);
        const char *dbName = dbFilePath.c_str();

        int rc = sqlite3_open_v2(dbName, &db, SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE, nullptr);

        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            throw std::runtime_error("Can't open database");
        }

        // Prepare the SQL query with a JOIN clause
        std::string querySt = "SELECT spp.lon, spp.lat, spp.max_driving_distance, qfs.profile_picture_score, qfs.profile_description_score "
                            "FROM service_provider_profile spp "
                            "JOIN quality_factor_score qfs ON spp.id = qfs.profile_id "
                            "WHERE spp.id =" + std::to_string(workerId) + ";";
        const char* query = querySt.c_str();

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            throw std::runtime_error("SQL error");
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Fetch values and store them in the struct
            workerInfo.lon = sqlite3_column_double(stmt, 0);
            workerInfo.lat = sqlite3_column_double(stmt, 1);
            workerInfo.maxDrivingDistance = sqlite3_column_int(stmt, 2);
            workerInfo.profileScore = (0.4 * sqlite3_column_double(stmt, 3)) + (0.6 * sqlite3_column_double(stmt, 4));
        }

        // Finalize the prepared statement
        sqlite3_finalize(stmt);
        // Close the database
        sqlite3_close(db);
    }

}
