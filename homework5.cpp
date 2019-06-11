/* 
 * File:   homework5.cpp
 * Copyright (C) 2019 sorkinmp@miamioh.edu
 *
 */

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include "Movie.h"

// A couple of namespaces to streamline code
using namespace std;
using namespace boost::asio::ip;
using namespace boost::asio;

// Alias to a vector-of-strings
using StrVec = std::vector<std::string>;

/** Helper method to split a given string into separate words based on spaces.
 * Note that this method does the following 1 extra operations:
 *    2. Removes all commas (',') characters
 */
StrVec split(std::string str) {
    // Change all ',' to spaces
    std::replace(str.begin(), str.end(), ',', ' ');
    // Now use a istringstream to extract words into a vector
    std::istringstream is(str);
    StrVec wordList;
    std::string word;
    while (is >> std::quoted(word)) {
        wordList.push_back(word);
    }
    // Return the list of words back to the caller.
    return wordList;
}

/**
 * Helper method to find the index of a certain string and return it to user
 * Given a string to find, return the index of that string in a vector
 * @param str the string to find
 * @param wordList StrVec containing entire query split up
 * @return 
 */
size_t find(std::string str, StrVec wordList) {
    int index = 0;
    // create std::string variable to be able to use stream
    for (size_t i = 0; i < wordList.size(); i++) {
        if (wordList.at(i) == str) {
            // if we find the word
            index = i;
            break;
        }
    }
    return index;
}

/**
 * Method to print lines
 * 
 * @param numCol number of columns to print
 * @param wordList StrVec containing entire query split up
 * @param m movie object
 */
void printLines(size_t numCol, StrVec& wordList, Movie& m) {
    // if only one column requested
    if (numCol == 1) {
        if (wordList.at(1) == "title" || wordList.at(1) == "genres") {
            std::cout << std::quoted(m.getCol(wordList.at(1))) << std::endl;
        } else {
            std::cout << m.getCol(wordList.at(1)) << std::endl;
        }
    } else {  // if 2+ columns requested
        for (size_t i = 1; (i < numCol + 1); i++) {
            if (wordList.at(i) == "title" || wordList.at(i) == "genres") {
                std::cout << std::quoted(m.getCol(wordList.at(i))) << " ";
            } else {
                std::cout << m.getCol(wordList.at(i)) << " ";
            }
            if (i == numCol) {  // you have reached the last column
                std::cout << std::endl;
            }
        }
    }
}

/**
 * Method to process and print lines from website
 * @param is input client stream
 * @param os output client stream
 * @param dbName name of database
 * @param dbNameLocation location of database in StrVec wordList
 * @param wordList StrVec containing entire query split up
 * @param m Movie object
 * @param fileName name of requested file
 */
void process(std::istream& is, std::ostream& os, std::string& dbName,
    size_t& dbNameLocation, StrVec wordList, Movie m, std::string& fileName) {
    // send get request to server
    // "GET /raodm/movies_db_20.txt HTTP/1.1\r\n"
    // "Host: users.miamioh.edu\r\n";
    os << "GET /" << fileName << " HTTP/1.1\r\n";
    os << "Host: " << dbName << "\r\n";
    os << "Connection: Close\r\n\r\n";
    // process lines
    std::string line;
    // Skip over the http header lines until we get a blank line
    while (std::getline(is, line), (!line.empty() && line != "\r")) { }
    // read and ignore the first header line in the CSV file exactly
    // how it was done in homework #3
    std::getline(is, line);
    while (is >> m) {
        // only print out requested headers
        // for one column
        if (dbNameLocation - 2 == 1) {
            printLines(1, wordList, m);
        } else {
            // for multiple columns
            size_t numCol = dbNameLocation - 2;
            printLines(numCol, wordList, m);
        } 
    }
}

/**
 * Method to process a query that contains where
 * @param is input client stream
 * @param os output client stream
 * @param dbName name of database
 * @param dbNameLocation location of database in StrVec wordList
 * @param wordList StrVec containing entire query split up
 * @param m Movie object
 * @param fileName name of requested file
 */
void whereProcess(std::istream& is, std::ostream& os, std::string& dbName,
    size_t& dbNameLocation, StrVec wordList, Movie m, std::string& fileName) {
    // send get request to server
    // "GET /raodm/movies_db_20.txt HTTP/1.1\r\n"
    // "Host: users.miamioh.edu\r\n";
    os << "GET /" << fileName << " HTTP/1.1\r\n";
    os << "Host: " << dbName << "\r\n";
    os << "Connection: Close\r\n\r\n";
    // create variables for where [col] and like [string]
    std::string whereCol = wordList.at(dbNameLocation + 2);
    std::string likeStr = wordList.at(dbNameLocation + 4);
    // process lines
    std::string line;
    // Skip over the http header lines until we get a blank line
    while (std::getline(is, line), (!line.empty() && line != "\r")) { }
    // read and ignore the first header line in the CSV file exactly
    // how it was done in homework #3
    std::getline(is, line);
    while (is >> m) {
        // put code here to only pull where columns
        // dbNameLocation is one index before "where" [col] "like" ["string"]
        // 21 lines so far
        // make sure to do std::quoted for title and genre
        // != 0 or std::string::npos
        if (m.getCol(whereCol).find(likeStr) != std::string::npos) {
            if (dbNameLocation - 2 == 1) {
                printLines(1, wordList, m);
            } else {  // if multiple columns
                size_t numCol = dbNameLocation - 2;
                printLines(numCol, wordList, m);
            }
        }
    }
}

/** Main method for processing requests
 * 
 * Example request (all on one line):
 * select year, title 
 * from "http://ceclnx01.cec.miamioh.edu/~raodm/hw5/movies_db_10.txt"
 * 
 * split changes query to:
 * select year title from "[url]"
 * to ensure all columns found for select, go until "from" found
 */
int main(int argc, char *argv[]) {
    // create std::string and StrVec variable for processing
    std::string query;
    StrVec wordList;
    Movie m;
    // print message to user
    std::cout << "query> ";
    // continuously process until user enters "exit"
    while (getline(std::cin, query) && query != "exit") {
        // process query
        wordList = split(query);
        // connect to database (find the name of database in wordList)
        size_t dbNameLocation = find("from", wordList) + 1;
        // name goes from position 7 to before slash
        // ex. http://ceclnx01.cec.miamioh.edu/~raodm/hw5/movies_db_10.txt"
        // want to return ceclnx01.cec.miamioh.edu
        size_t endUrl = wordList.at(dbNameLocation).find(".edu") - 3;
        std::string dbName = wordList.at(dbNameLocation).substr(7, endUrl);
        std::string fileName = wordList.at(dbNameLocation).substr(endUrl + 8);
        ip::tcp::iostream database(dbName, "80");
        // process and print out columns
        // if its a where, do where method
        if (std::find(std::begin(wordList), std::end(wordList), "where") 
                != std::end(wordList)) {            
            whereProcess(database, database, dbName, dbNameLocation, wordList,
                        m, fileName);
        } else { 
        process(database, database, dbName, dbNameLocation, wordList, 
                m, fileName);
        }
        // after query request complete, another query will be printed
        // print message to user
        std::cout << "query> ";
        // need to insert where request somewhere
    }
    return 0;
}
