#ifndef MAPLOADER_H
#define MAPLOADER_H
#include <iostream>
#include <fstream>
#include "Map.h"
class MapLoader
{
private:
      Map *map; // Pointer to a Map object where the loaded map data will be stored

public:
      // Default constructor to create an empty MapLoader object
      MapLoader();

      // Constructor to create a MapLoader object with a specified map name
      MapLoader(string name);

      // Copy constructor for creating a deep copy of a MapLoader object
      MapLoader(const MapLoader &loader);

      // Destructor to release dynamically allocated memory
      ~MapLoader();

      // Getter method to retrieve the Map object
      Map *getMap();

      // Method to add continents to the map from an input file stream
      void addContinents(ifstream &file);

      // Method to add territories to the map from an input file stream
      void addTerritories(ifstream &file);

      // Overloaded stream insertion operator to output a MapLoader object
      friend ostream &operator<<(ostream &out, const MapLoader &mloader);

      // Overloaded assignment operator to perform a deep copy of a MapLoader object
      MapLoader &operator=(const MapLoader &mLoader);
};

// Function to split a string into a vector of substrings using a delimiter
vector<string> split(string s, string delimiter);

#endif // MAPLOADER_H
