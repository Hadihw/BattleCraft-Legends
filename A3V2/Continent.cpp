#include "Continent.h"
using namespace std;
// Default constructor to create an empty Continent object
Continent::Continent()
{
      // Initialize bonus, name, and continentId with default values
      this->bonus = new int(0);
      this->name = new string("No name");
      this->continentId = new int(0);

      // Initialize the territories vector as empty
      this->territories = new vector<Territories *>();
}

// Constructor to create a Continent object with specified bonus, name, and continentId
Continent::Continent(int bonus, string name, int continentId) : Continent()
{
      // Set bonus, name, and continentId to the provided values
      this->bonus = new int(bonus);
      this->name = new string(name);
      this->continentId = new int(continentId);
}

// Copy constructor for creating a deep copy of a Continent object
Continent::Continent(const Continent &c)
{
      // Perform a deep copy of bonus, name, and territories vector
      this->bonus = new int(*c.bonus);
      this->name = new string(*c.name);

      // Copy the territories pointers without creating new Territory objects
      for (auto t : *c.territories)
      {
            territories->push_back(t);
      }
}

// Destructor to release dynamically allocated memory
Continent::~Continent()
{
      // Delete dynamically allocated bonus, name, and continentId
      delete this->bonus;
      delete this->name;
      delete this->continentId;

      // Delete the Territory objects within the territories vector
      for (auto t : *this->territories)
      {
            delete t;
      }

      // Clear the territories vector and then delete it
      territories->clear();
      delete territories;
}

// Setter method to set the bonus value for the continent
void Continent::setBonus(int bonus)
{
      *this->bonus = bonus;
}

// Setter method to set the name of the continent
void Continent::setName(string name)
{
      *this->name = name;
}

// Setter method to set the continentId for the continent
void Continent::setContinentId(int continentId)
{
      *this->continentId = continentId;
}

// Getter method to retrieve the bonus value of the continent
int Continent::getBonus()
{
      return *this->bonus;
}

// Getter method to retrieve the name of the continent
string Continent::getName()
{
      return *this->name;
}

// Getter method to retrieve the unique continentId
int Continent::getContinentId()
{
      return *this->continentId;
}

// Method to check if the continent contains a specific territory
bool Continent::containsTerritory(Territories *t)
{
      for (auto a : *this->territories)
      {
            if (*t == *a)
                  return true;
      }
      return false;
}

// Overloaded stream insertion operator to output Continent information
ostream &operator<<(ostream &out, const Continent &c)
{
      return out << "Name: " << c.name << "\nContinent Id: " << c.continentId << "\nBonus: " << c.bonus << "\n";
}

// Overloaded stream insertion operator to output a pointer to a Continent object
ostream &operator<<(ostream &out, Continent *c)
{
      if (c == nullptr)
            return out << "Null Continent object pointer";
      return out << "Name: " << c->getName() << "\nContinent Id: " << c->getContinentId() << "\nBonus: " << c->getBonus() << "\n";
}

// Overloaded equality operator to compare two Continent objects
bool operator==(const Continent &C1, const Continent &C2)
{
      return (*C1.name == *C2.name) && (*C1.bonus == *C2.bonus) && (*C1.continentId == *C2.continentId);
}

// Overloaded assignment operator to perform a deep copy of a Continent object
Continent &Continent::operator=(const Continent &C)
{
      *this->name = *C.name;
      *this->bonus = *C.bonus;
      *this->continentId = *C.continentId;

      // Clear the territories vector and then copy the pointers from the source
      this->territories->clear();
      for (auto t : *C.territories)
      {
            this->territories->push_back(t);
      }
      return *this;
}
