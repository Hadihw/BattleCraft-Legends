#include "Map.h"
using namespace std;

// Constructor to create a Map object with default values
Map::Map()
{
      // Initialize the mapName pointer with "No name"
      this->mapName = new string("No name");

      // Initialize the territories and continents vectors as empty
      this->territories = new vector<Territories *>();
      this->continents = new vector<Continent *>();
}

// Constructor to create a Map object with a specified name
Map::Map(string name) : Map()
{
      // Set the mapName to the provided name
      *this->mapName = name;
}

// Copy constructor for creating a deep copy of a Map object
Map::Map(const Map &map)
{
      // Copy the mapName string using dynamic memory allocation
      this->mapName = new string(*map.mapName);

      // Copy the territories by creating new Territories objects
      for (auto t : *(map.territories))
      {
            this->territories->push_back(new Territories(*t));
      }

      // Copy the continents by creating new Continent objects
      for (auto c : *map.continents)
      {
            this->continents->push_back(new Continent(*c));
      }
}

// Destructor to release dynamically allocated memory
Map::~Map()
{
      delete mapName;

      // Delete the Territories objects and Continent objects within the vectors
      for (auto t : *territories)
      {
            delete t;
      }
      for (auto c : *continents)
      {
            delete c;
      }
}

// Setter method to set the name of the Map
void Map::setName(string name)
{
      *this->mapName = name;
}

// Getter method to retrieve the name of the Map
string Map::getName()
{
      return *this->mapName;
}

// Method to validate the Map's structure and connections
bool Map::validate()
{
      // Check if territories and continents vectors are empty
      if (territories->empty() || continents->empty())
      {
            return false;
      }

      // Check if each territory has adjacent territories (minimum two territories)
      if (territories->size() > 1)
      {
            for (auto t : *territories)
            {
                  if (t->adjTerritories->empty())
                        return false;
            }
      }

      // Check for uniqueness of territories, connectivity, and subgraph connectivity
      if (containsUniqueTerritories() && isConnected() && isConnectedSubGraph())
            return true;
      else
            return false;
}

// Method to check if all territories belong to exactly one continent
bool Map::containsUniqueTerritories()
{
      for (auto t : *territories)
      {
            int count = 0;
            for (auto c : *continents)
            {
                  if (c->containsTerritory(t))
                  {
                        count++;
                  }
            }
            if (count != 1)
                  return false;
      }
      return true;
}
// Method to check if the Map is fully connected
bool Map::isConnected()
{
      map<int, bool> visited;
      vector<int> IdStack;
      for (auto t : *territories)
      {
            visited.insert(pair<int, bool>(t->getTerritoryId(), false));
      }
      visited.at((*territories)[0]->getTerritoryId()) = true;
      IdStack.push_back((*territories)[0]->getTerritoryId());
      while (!IdStack.empty())
      {
            Territories *cur = getTerritoryById(IdStack.back());
            IdStack.pop_back();
            for (auto t : *cur->adjTerritories)
            {
                  if (visited.find(t) != visited.end())
                  {
                        if (!visited.at(t))
                        {
                              visited[t] = true;
                              IdStack.push_back(t);
                        }
                  }
            }
      }
      for (auto v : visited)
            if (!v.second)
                  return false;
      return true;
}
// Method to check if the Map is a connected subgraph
bool Map::isConnectedSubGraph()
{
      map<int, bool> visited;
      vector<int> IdStack;
      for (auto c : *continents)
      {
            for (auto t : *c->territories)
            {
                  visited.insert(pair<int, bool>(t->getTerritoryId(), false));
            }
            IdStack.push_back((*c->territories)[0]->getTerritoryId());
            while (!IdStack.empty())
            {
                  Territories *cur = getTerritoryById(IdStack.back());
                  IdStack.pop_back();
                  for (auto t : *cur->adjTerritories)
                  {
                        if (visited.find(t) != visited.end())
                        {
                              if (!visited.at(t))
                              {
                                    visited[t] = true;
                                    IdStack.push_back(t);
                              }
                        }
                  }
            }
            for (auto v : visited)
                  if (!v.second)
                        return false;
            visited.clear();
            IdStack.clear();
      }
      return true;
}
// Method to get a Territory object by its ID
Territories *Map::getTerritoryById(int id)
{
      // Search for a Territory with the specified ID in the territories vector
      for (auto t : *territories)
      {
            if (t->getTerritoryId() == id)
                  return t;
      }
      return nullptr;
}

// Method to get a Territory object by its name
Territories *Map::getTerritoryByName(string name)
{
      // Search for a Territory with the specified name in the territories vector
      for (auto t : *territories)
      {
            if (t->getName() == name)
                  return t;
      }
      return nullptr;
}

// Method to get a Continent object by its ID
Continent *Map::getContinentById(int id)
{
      // Search for a Continent with the specified ID in the continents vector
      for (auto c : *continents)
      {
            if (c->getContinentId() == id)
                  return c;
      }
      return nullptr;
}

// Method to get a Continent object by its name
Continent *Map::getContinentByName(string name)
{
      // Search for a Continent with the specified name in the continents vector
      for (auto c : *continents)
      {
            if (c->getName() == name)
                  return c;
      }
      return nullptr;
}

// Method to print the details of the Map
void Map::printMap()
{
      map<int, bool> visited;
      vector<int> IdStack;
      for (auto t : *territories)
      {
            visited.insert(pair<int, bool>(t->getTerritoryId(), false));
      }
      visited.at((*territories)[0]->getTerritoryId()) = true;
      IdStack.push_back((*territories)[0]->getTerritoryId());
      while (!IdStack.empty())
      {
            Territories *cur = getTerritoryById(IdStack.back());
            cout << *cur;
            IdStack.pop_back();
            for (auto t : *cur->adjTerritories)
            {
                  if (visited.find(t) != visited.end())
                  {
                        if (!visited.at(t))
                        {
                              visited[t] = true;
                              IdStack.push_back(t);
                        }
                  }
            }
      }
}
// Method to overload the stream insertion operator
ostream &operator<<(ostream &out, const Map &mp)
{
      return out << "Map Name: " << mp.mapName << "\nNumber of Continent" << mp.continents->size() << "\nNumber of territories: " << mp.territories->size();
}
// Method to overload the assignment operator
Map &Map::operator=(const Map &mp)
{
      *this->mapName = *mp.mapName;
      this->continents->clear();
      this->territories->clear();
      for (auto c : *mp.continents)
            this->continents->push_back(c);
      for (auto t : *mp.territories)
            this->territories->push_back(t);
      return *this;
}
