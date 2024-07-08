#ifndef TERRITORIES_H
#define TERRITORIES_H
#include <string>
#include <cmath>
#include <memory>
#include "Continent.h"
#include "Player.h"
using namespace std;
class Player;
// Territories class represents a territory on a map.

class Territories
{
private:
      string *name;          // Pointer to store the name of the territory
      int *numArmies;        // Pointer to store the number of armies on the territory
      int *territoryId;      // Pointer to store a unique identifier for the territory
      int *continentId;      // Pointer to store the continent identifier to which the territory belongs
      string *continentName; // Pointer to store the name of the continent to which the territory belongs
      Player *player;        // Pointer to store the player identifier controlling the territory

public:
      vector<int> *adjTerritories; // Pointer to a vector storing the IDs of adjacent territories

      // Default constructor to create an empty Territories object
      Territories();

      // Constructor to create a Territories object with specified attributes
      Territories(string name, int territoryId, int continentId, string continentName);

      // Copy constructor for creating a deep copy of a Territories object
      Territories(const Territories &T);

      // Destructor to release dynamically allocated memory
      ~Territories();

      // Setter method to set the name of the territory
      void setTerritoryName(string name);

      // Setter method to set the number of armies on the territory
      void setNumArmies(int armies);

      // Setter method to set the unique territory identifier
      void setTerritoryId(int id);

      // Setter method to set the continent identifier to which the territory belongs
      void setContinentId(int continentId);

      // Setter method to set the name of the continent to which the territory belongs
      void setContinentName(string ContinentName);

      // Setter method to set the player identifier controlling the territory
      void setPlayer(Player *player);

      // Getter method to retrieve the name of the territory
      string getName();

      // Getter method to retrieve the number of armies on the territory
      int getArmies();

      // Getter method to retrieve the unique territory identifier
      int getTerritoryId();

      // Getter method to retrieve the continent identifier to which the territory belongs
      int getContinentId();

      // Getter method to retrieve the name of the continent to which the territory belongs
      string getContinentName();

      // Getter method to retrieve the player identifier controlling the territory
      Player &getPlayer();

      // Method to add armies to the territory
      bool addArmies(int armies);

      // Method to subtract armies from the territory
      bool subArmies(int armies);

      // Method to check if the territory
      bool isBorder(Territories *t1);
      // Overloaded equality operator to compare two Territories objects
      friend bool operator==(const Territories &t1, const Territories &t2);

      // Overloaded stream insertion operator to output a pointer to a Territories object
      friend ostream &operator<<(ostream &out, Territories *t);

      // Overloaded assignment operator to perform a deep copy of a Territories object
      Territories &operator=(const Territories &t2);

      // Overloaded stream insertion operator to output Territories information
      friend ostream &operator<<(ostream &out, const Territories &t);
};

#endif // TERRITORIES_H
