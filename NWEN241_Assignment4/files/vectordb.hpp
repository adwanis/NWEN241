/**
 * vectordb.hpp
 * C++ header file that should contain declaration for
 * - VectorDbTable class
 * 
 * You need to modify this file to declare VectorDbTable class 
 * as specified in the hand-out (Task 2)
 */ 
#ifndef __VECTOR_DB_HPP__
#define __VECTOR_DB_HPP__ 

#include "abstractdb.hpp"
#include <vector>
#include <algorithm>

namespace nwen{
	// The VectorDbTable class inherits from AbstractDbTable and implements its pure virtual functions.
    // This class uses a vector to store movie records.
	class VectorDbTable: public AbstractDbTable {
	public:
		VectorDbTable(); // Constructor declaration
		

		 // Overridden member function declarations
		virtual int rows() const override;
        virtual movie* get(int index) const override;
        virtual bool add(const movie& m) override;
        virtual bool update(unsigned long id, const movie& m) override;
        virtual bool remove(unsigned long id) override;

    private:
    	std::vector<movie> movies;
	};


}
#endif /* __VECTOR_DB_HPP__ */