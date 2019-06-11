#ifndef MOVIE_H
#define MOVIE_H

/* 
 * File:   Movie.h
 * Author: raodm
 *
 * Copyright (C) 2019 raodm@miamioh.edu
 * 
 * Created on February 13, 2019, 10:33 AM
 */

#include <iostream>
#include <string>

/** A simple class to encapsulate information about a movie entry.
 * Each movie entry has the following information:
 * 
 * 1. moveID (int): Unique ID for a movie
 * 2. title (std::string): A double-quoted title for the movie
 * 3. year (int): The year when the movie was released
 * 4. genres (std::string): A double-quoted list of genres for the movie
 * 5. imdbId (int): The IMDB identifier for the movies
 * 6. rating (float): An average movie rating (assigned by many reviewers)
 * 7. numRaters (int): Number of reviewers who contributed to rating.
 * 
 * The class provides a suitable stream-insertion and stream-extraction 
 * operator.
 */
class Movie {
    // The stream extraction operator
    friend std::istream& operator>>(std::istream& is, Movie& m);
    // The stream insertion operator.
    friend std::ostream& operator<<(std::ostream& os, const Movie& m);

public:
    /** The constructor.
     * 
     * The constructor initializes all the instance variables to default
     * initial values.
     */
    Movie();
    
    /**
     * The destructor. The destructor does not have any functionality but
     * is present to adhere to coding conventions.
     */
    virtual ~Movie();
    
    /** Convenience method to return the ID of a movie object.
     * 
     * @return The unique ID associated with this movie object.
     */
    int getID() const { return movieID; }
           
    /** Return the value for the specified column. 
     * 
     * @param [in] colName A predefined column name, one of: "movieid",
     * "title", "year", "genres", "imdbid", "rating", "raters"
     * 
     * @return The value associated with the given column.
     */
    std::string getCol(const std::string& colName) const;
    
private:
    /** The unique ID for this movie. */
    int movieID = -1;
    
    /** The title for this movie. The title is automatically initialized to
     *  an empty string. */
    std::string title;
    
    /** The year when the movie was released */
    int year;
    
    /** The list of genres associated with a movie. */
    std::string genres;
    
    /** The unique IMBD ID for this movie. */
    int imdbId;
    
    /** The rating (0 through 5) associated with this movie. */
    float rating;
    
    /** The number of people who rated this movie. */
    int numRaters;
};

/** A convenience method to get movie information as a string.
 * 
 * This method uses a std::ostringstream to convert a given movie
 * object to a string. 
 * 
 * @return A string representation of the movie object.
 */
std::string to_string(const Movie& m);

#endif /* MOVIE_H */
