#ifndef CONTINENT_H
#define CONTINENT_H
#include <string>
#include <vector>
#include <iostream>
#include "Territories.h"
using namespace std;
// Forward declaration for the Territories class
class Territories;

// Continent class represents a geographical continent in a map.
class Continent
{
private:
      int *bonus;       // Pointer to store the bonus value for controlling the continent
      string *name;     // Pointer to store the name of the continent
      int *continentId; // Pointer to store a unique identifier for the continent

public:
      vector<Territories *> *territories; // Pointer to a vector of Territories objects within the continent

      // Default constructor to create an empty Continent object
      Continent();

      // Constructor to create a Continent object with specified bonus, name, and continentId
      Continent(int bonus, string name, int continentId);

      // Copy constructor for creating a deep copy of a Continent object
      Continent(const Continent &);

      // Destructor to release dynamically allocated memory
      ~Continent();

      // Setter method to set the bonus value for the continent
      void setBonus(int bonus);

      // Setter method to set the name of the continent
      void setName(string name);

      // Setter method to set the continentId for the continent
      void setContinentId(int continentId);

      // Getter method to retrieve the bonus value of the continent
      int getBonus();

      // Getter method to retrieve the name of the continent
      string getName();

      // Getter method to retrieve the unique continentId
      int getContinentId();

      // Method to check if the continent contains a specific territory
      bool containsTerritory(Territories *t);

      // Overloaded equality operator to compare two Continent objects
      friend bool operator==(const Continent &C1, const Continent &C2);

      // Overloaded stream insertion operator to output Continent information
      friend std::ostream &operator<<(std::ostream &out, const Continent &c);

      // Overloaded stream insertion operator to output a pointer to a Continent object
      friend std::ostream &operator<<(std::ostream &out, Continent *c);

      // Overloaded assignment operator to perform a deep copy of a Continent object
      Continent &operator=(const Continent &C);
};

#endif // CONTINENT_H
