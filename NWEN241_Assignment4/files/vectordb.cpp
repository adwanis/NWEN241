/**
 * vectordb.cpp
 * C++ source file that should contain implementation for member functions
 * - rows()
 * - get()
 * - add()
 * - update()
 * - remove()
 * 
 * You need to modify this file to implement the above-mentioned member functions
 * as specified in the hand-out (Task 3)
 */ 

#include "vectordb.hpp"
#include <cstring>

namespace nwen {

	VectorDbTable::VectorDbTable() {
    // Default constructor
	}

	int VectorDbTable::rows() const {
	    return movies.size();
	}

	movie* VectorDbTable::get(int index) const {
	    if (index >= 0 && index < movies.size()) {
	        return const_cast<movie*>(&movies[index]); //return A pointer to the movie record at the given index

	    }
	    return nullptr; //else nullptr if the index is out of bounds
	}

	bool VectorDbTable::add(const movie& m) {
	    // Check if a movie with the same id already exists
	    auto it = std::find_if(movies.begin(), movies.end(), [&m](const movie& existingMovie) {
	        return existingMovie.id == m.id;
	    });
	    if (it != movies.end()) {
	        return false; // Movie with the same id exists
	    }
	    movies.push_back(m);
	    return true; // Successfully added
	}

	bool VectorDbTable::update(unsigned long id, const movie& m) {
	    auto it = std::find_if(movies.begin(), movies.end(), [id](const movie& existingMovie) {
	        return existingMovie.id == id;
	    });
	    if (it != movies.end()) {
	        *it = m;
	        return true; // Update successful
	    }
	    return false; // Movie with the given id not found
	}

	bool VectorDbTable::remove(unsigned long id) {
	    auto it = std::find_if(movies.begin(), movies.end(), [id](const movie& existingMovie) {
	        return existingMovie.id == id;
	    });
	    if (it != movies.end()) {
	        movies.erase(it);
	        return true; // Removal successful
	    }
	    return false; // Movie with the given id not found
	}

} 
