#include "Territories.h"
using namespace std;
// Default constructor to create an empty Territories object
Territories::Territories()
{
      // Initialize member variables with default values
      this->name = new string("");
      this->numArmies = new int(0);
      this->territoryId = new int(0);
      this->continentId = new int(0);
      this->continentName = new string("");
      this->player = new Player();

      // Initialize the vector of adjacent territories as empty
      this->adjTerritories = new vector<int>();
}

// Constructor to create a Territories object with specified attributes
Territories::Territories(string name, int territoryId, int continentId, string continentName) : Territories()
{
      // Set member variables with provided values
      *this->name = name;
      *this->territoryId = territoryId;
      *this->continentId = continentId;
      *this->continentName = continentName;
}

// Copy constructor for creating a deep copy of a Territories object
Territories::Territories(const Territories &T)
{
      // Perform deep copies of member variables
      this->name = new string(*T.name);
      this->numArmies = new int(*T.numArmies);
      this->territoryId = new int(*T.territoryId);
      this->continentId = new int(*T.continentId);
      this->continentName = new string(*T.continentName);
      this->player = new Player(*T.player);

      // Copy the vector of adjacent territories
      for (auto a : *T.adjTerritories)
      {
            adjTerritories->push_back(a);
      }
}

// Destructor to release dynamically allocated memory
Territories::~Territories()
{
      // Delete dynamically allocated member variables
      delete this->name;
      delete this->numArmies;
      delete this->territoryId;
      delete this->continentId;
      delete this->continentName;
      delete this->player;

      // Clear the vector of adjacent territories and then delete it
      adjTerritories->clear();
      delete adjTerritories;
}

// Setter method to set the name of the territory
void Territories::setTerritoryName(string name)
{
      *this->name = name;
}

// Setter method to set the number of armies on the territory
void Territories::setNumArmies(int armies)
{
      *this->numArmies = armies;
}

// Setter method to set the unique territory identifier
void Territories::setTerritoryId(int id)
{
      *this->territoryId = id;
}

// Setter method to set the continent identifier to which the territory belongs
void Territories::setContinentId(int continentId)
{
      *this->continentId = continentId;
}

// Setter method to set the name of the continent to which the territory belongs
void Territories::setContinentName(string ContinentName)
{
      *this->continentName = ContinentName;
}

// Setter method to set the player identifier controlling the territory
void Territories::setPlayer(Player *player)
{
      this->player = player;
}

// Getter method to retrieve the number of armies on the territory
int Territories::getArmies()
{
      return *this->numArmies;
}

// Getter method to retrieve the name of the territory
string Territories::getName()
{
      return *this->name;
}

// Getter method to retrieve the unique territory identifier
int Territories::getTerritoryId()
{
      return *this->territoryId;
}

// Getter method to retrieve the continent identifier to which the territory belongs
int Territories::getContinentId()
{
      return *this->continentId;
}

// Getter method to retrieve the name of the continent to which the territory belongs
string Territories::getContinentName()
{
      return *this->continentName;
}

// Getter method to retrieve the player identifier controlling the territory
Player &Territories::getPlayer()
{
      return *this->player;
}

// Method to add armies to the territoryj
bool Territories::addArmies(int armies)
{
      if (armies <= 0)
            return false;

      *this->numArmies += armies;
      return true;
}

// Method to subtract armies from the territory
bool Territories::subArmies(int armies)
{
      if (armies >= 0)
            return false;

      *this->numArmies -= armies;
      return true;
}

bool Territories::isBorder(Territories *t1)
{
      vector<int> *adjTerritories = t1->adjTerritories;
      for (auto adjT = adjTerritories->begin(); adjT != adjTerritories->end(); adjT++)
      {
            if (*adjT == this->getTerritoryId())
            {
                  return true;
            }
      }
      return false;
}

// Overloaded equality operator to compare two Territories objects
bool operator==(const Territories &t1, const Territories &t2)
{
      return (*t1.numArmies == *t2.numArmies) && (*t1.name == *t2.name) && (*t1.continentId == *t2.continentId) && (*t1.player == *t2.player);
}

// Overloaded stream insertion operator to output Territories information
ostream &operator<<(ostream &out, const Territories &t)
{
      return out << "Territory Name: " << *t.name << "\nTerritory Id: " << *t.territoryId << "\nNumber of Armies: " << *t.numArmies << "\nContinent: " << *t.continentName << "\nContinent Id:" << *t.continentId
                 << "\nPlayer Name:" << t.player->getName() << "\n";
}

// Overloaded stream insertion operator to output a pointer to a Territories object
ostream &operator<<(ostream &out, Territories *t)
{
      if (t == nullptr)
            return out << "Null Territories object pointer";

      return out << "Territory Name: " << t->getName() << "\nNumber of Armies: " << t->getArmies() << "\nContinent: " << t->getContinentName() << "\nPlayer Id:" << t->getPlayer() << "\n";
}

// Overloaded assignment operator to perform a deep copy of a Territories object
Territories &Territories::operator=(const Territories &t2)
{
      // Perform deep copies of member variables
      this->numArmies = t2.numArmies;
      this->name = t2.name;
      this->territoryId = t2.territoryId;
      this->continentId = t2.continentId;
      this->continentName = t2.continentName;
      this->player = t2.player;

      // Clear the vector of adjacent territories and then copy values from the source
      this->adjTerritories->clear();
      for (int i = 0; i < t2.adjTerritories->size(); i++)
      {
            adjTerritories[i] = t2.adjTerritories[i];
      }
      return *this;
}
