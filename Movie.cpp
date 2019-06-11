#ifndef MOVIE_CPP
#define MOVIE_CPP

/** This file contains the implementation for various methods in the 
 * movie class.
 * 
 * Copyright (C) 2019 raodm@miamioh.edu
 */

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "Movie.h"

// The default (and only) constructor for this class.
Movie::Movie() {
    // The instance variables are initialized in the header file. Consequently,
    // this constructor is empty.
}

// The destructor
Movie::~Movie() {
    // Currently, the destructor does not have any special tasks to do.
}

// Stream-extraction operator.
std::istream& operator>>(std::istream& is, Movie& m) {
    // Read the values from an entry in the stream/file in the correct order.
    is >> m.movieID >> std::quoted(m.title) >> m.year >> std::quoted(m.genres)
       >> m.imdbId  >> m.rating >> m.numRaters;
    // Return the stream as per the API requirement.
    return is;
}

// The stream insertion operator.
std::ostream& operator<<(std::ostream& os, const Movie& m) {
    // Read the values from an entry in the stream/file in the correct order.
    os << m.movieID << " " << std::quoted(m.title) << " " << m.year   << " " 
       << std::quoted(m.genres) << " " << m.imdbId << " " << m.rating << " "
       << m.numRaters;
    // Return the stream as per the API requirement.
    return os;    
}

// A simple to-string method.
std::string to_string(const Movie& m) {
    std::ostringstream os;
    os << m;
    return os.str();
}

// Return value for a given column
std::string
Movie::getCol(const std::string& col) const {
    if (col == "movieid") {
        return std::to_string(movieID);
    } else if (col == "title") {
        return title;
    } else if (col == "year") {
        return std::to_string(year);
    } else if (col == "genres") {
        return genres;
    } else if (col == "imdbid") {
        return std::to_string(imdbId);
    } else if (col == "rating") {
        return std::to_string(rating);
    } else if (col == "raters") {
        return std::to_string(numRaters);
    }
    // Unknown column name.
    return "";
}

#endif
