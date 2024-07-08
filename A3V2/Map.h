#ifndef MAP_H
#define MAP_H
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include "Territories.h"
#include "Continent.h"

using std::string;
using std::vector;
using std::ostream;

class Continent;
class Territories;
class Map
{
private:
      string *mapName; // Pointer to store the name of the map

public:
      // Constructor to create an empty map
      Map();

      // Constructor to create a map with a specified name
      Map(string name);

      // Copy constructor for creating a copy of a map
      Map(const Map &map);

      // Destructor to free memory and clean up resources
      ~Map();

      // Pointer to a vector storing pointers to Territories objects
      vector<Territories *> *territories;

      // Pointer to a vector storing pointers to Continent objects
      vector<Continent *> *continents;

      // Setter method to set the name of the map
      void setName(string name);

      // Getter method to retrieve the name of the map
      string getName();

      // Method to validate the map's structure and connections
      bool validate();

      // Method to check if the map contains unique territories
      bool containsUniqueTerritories();

      // Method to check if the map is fully connected
      bool isConnected();

      // Method to check if the map is a connected subgraph
      bool isConnectedSubGraph();

      // Method to get a Territory object by its ID
      Territories *getTerritoryById(int id);

      // Method to get a Territory object by its name
      Territories *getTerritoryByName(string name);

      // Method to get a Continent object by its ID
      Continent *getContinentById(int id);

      // Method to get a Continent object by its name
      Continent *getContinentByName(string name);

      // Method to print the details of the map
      void printMap();

      // Method to overloading stream insertion
      friend ostream &operator<<(ostream &out, const Map &mp);

      // Method overloading the assignment operator
      Map &operator=(const Map &mp);
};

#endif // MAP_H
