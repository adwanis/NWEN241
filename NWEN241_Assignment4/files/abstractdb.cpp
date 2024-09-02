/**
 * abstractdb.cpp
 * C++ source file that should contain implementation for member functions
 * - loadCSV()
 * - saveCSV()
 * 
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Tasks 4 and 5)
 */ 

#include "abstractdb.hpp"
#include <fstream>
#include <sstream>
#include <iostream> // For debugging, remove in production
#include <cstring>

namespace nwen {
/*
 * Save the table contents to a CSV file.
 * 
 * @param filename This is the name of the CSV file where the table data will be saved.
 * @return This function returns true if the data was successfully saved to the file. 
 *         If there was an error opening the file or writing to it, the function returns false.
 */

    bool AbstractDbTable::saveCSV(const std::string& filename) {
        // Open the file for writing (truncate mode to empty the file)
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false; // Cannot open file for writing
        }

        // Write each record to the file
        for (int i = 0; i < rows(); ++i) {
            movie* m = get(i);
            if (m == nullptr) {
                file.close();
                return false; // Error in retrieving a record
            }

            // Create a CSV line for the current movie record
            std::ostringstream oss;
            oss << m->id << ",\"" << m->title << "\"," << m->year << ",\"" << m->director << "\"";
            
            // Write the line to the file
            file << oss.str() << std::endl;
            if (file.fail()) {
                file.close();
                return false; // Error encountered while writing to the file
            }
        }

        // Close the file
        file.close();
        return true; // Successfully written to the file
    }

/**
 * Load table contents from a CSV file.
 * 
 * @param filename This is the name of the CSV file from which the table data will be loaded.
 * @return This function returns true if the data was successfully loaded from the file.
 *         If there was an error opening the file or if the file is not in a valid format, the function returns false.
 */
    bool AbstractDbTable::loadCSV(const std::string& filename) {
        // Open the file for reading
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false; // Cannot open file for reading
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string id_str, title, year_str, director;

            if (!std::getline(iss, id_str, ',')) break;
            if (!std::getline(iss, title, ',')) break;
            if (!std::getline(iss, year_str, ',')) break;
            if (!std::getline(iss, director, ',')) break;

            // Remove quotes from title and director
            if (title.front() == '"') title = title.substr(1, title.size() - 2);
            if (director.front() == '"') director = director.substr(1, director.size() - 2);

            // Convert id and year to appropriate types
            unsigned long id = std::stoul(id_str);
            unsigned short year = static_cast<unsigned short>(std::stoi(year_str));

            // Create a movie record
            movie m = { id, "", year, "" };
            strncpy(m.title, title.c_str(), sizeof(m.title) - 1);
            m.title[sizeof(m.title) - 1] = '\0'; // Ensure null-termination
            strncpy(m.director, director.c_str(), sizeof(m.director) - 1);
            m.director[sizeof(m.director) - 1] = '\0'; // Ensure null-termination

            // Add the movie to the table
            if (!add(m)) {
                file.close();
                return false; // Error adding record
            }
        }

        // Check for reading errors (e.g., incorrect format)
        if (file.bad()) {
            file.close();
            return false;
        }

        // Close the file
        file.close();
        return true; // Successfully loaded from the file
    }

} 

