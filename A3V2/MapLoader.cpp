#include "MapLoader.h"
// The `split` function takes a string `s` and a delimiter `delimiter` and splits the string into
// substrings based on the delimiter. It returns a vector containing the resulting substrings.
vector<string> split(string s, string delimiter)
{
      size_t startPos = 0, endPos, delimLength = delimiter.length();
      string token;
      vector<string> res;

      // Loop through the string, finding occurrences of the delimiter
      while ((endPos = s.find(delimiter, startPos)) != string::npos)
      {
            // Extract the substring between startPos and endPos
            token = s.substr(startPos, endPos - startPos);
            startPos = endPos + delimLength; // Move the start position to the character after the delimiter
            res.push_back(token);            // Add the extracted substring to the result vector
      }

      // Add the remaining part of the string (after the last delimiter)
      res.push_back(s.substr(startPos));

      return res; // Return the vector of substrings
}

// The `MapLoader` class is responsible for loading map data from a file into a `Map` object.

// Default constructor to create an empty `MapLoader` object
MapLoader::MapLoader()
{
      this->map = new Map(); // Create a new Map object
}

// Constructor to create a `MapLoader` object with a specified map name
MapLoader::MapLoader(string name)
{
      this->map = new Map(name); // Create a new Map object with the given name
}

// Copy constructor for creating a deep copy of a `MapLoader` object
MapLoader::MapLoader(const MapLoader &loader)
{
      this->map = new Map(*loader.map); // Create a new Map object as a deep copy of the source Map
}

// Destructor to release dynamically allocated memory
MapLoader::~MapLoader()
{
      //delete this->map; // Delete the dynamically allocated Map object
      //I do not think that map should be deleted by MapLoader b/c map needs to exist throughout the game
}

// Getter method to retrieve the Map object
Map *MapLoader::getMap()
{
      return this->map;
}

// Method to add continents to the map from an input file stream
void MapLoader::addContinents(ifstream &file)
{
      string line = "";
      int id = 0;

      while (file)
      {
            getline(file, line);

            if (line == "[Continents]")
            {
                  getline(file, line);

                  while (line.empty())
                        getline(file, line);

                  while (!line.empty())
                  {
                        vector<string> v = split(line, "=");
                        map->continents->push_back(new Continent((stoi(v[1])), v[0], ++id));
                        getline(file, line);
                  }

                  return;
            }
      }
}

// Method to add territories to the map from an input file stream
void MapLoader::addTerritories(ifstream &file)
{
      std::map<string, int> idMap;
      string line = "";
      int ids = 0;

      while (file)
      {
            getline(file, line);

            if (line == "[Territories]")
            {
                  getline(file, line);

                  while (file)
                  {
                        if (!line.empty())
                        {
                              vector<string> v = split(line, ",");
                              Territories *t = new Territories();
                              t->setTerritoryName(v[0]);
                              t->setContinentName(v[3]);
                              t->setContinentId(map->getContinentByName(v[3])->getContinentId());

                              // Handle territory IDs
                              if (idMap.empty() || idMap.find(v[0]) == idMap.end())
                              {
                                    idMap.insert(pair<string, int>(v[0], ++ids));
                                    t->setTerritoryId(ids);
                              }
                              else
                              {
                                    t->setTerritoryId(idMap.at(v[0]));
                              }

                              // Handle adjacent territories
                              for (int i = 4; i < v.size(); i++)
                              {
                                    if (idMap.find(v[i]) == idMap.end())
                                    {
                                          idMap.insert(pair<string, int>(v[i], ++ids));
                                          t->adjTerritories->push_back(ids);
                                    }
                                    else
                                    {
                                          t->adjTerritories->push_back(idMap.at(v[i]));
                                    }
                              }

                              map->getContinentByName(v[3])->territories->push_back(t);
                              map->territories->push_back(t);
                        }
                        getline(file, line);
                  }

                  return;
            }
      }
}

// Overloaded stream insertion operator to output a `MapLoader` object
ostream &operator<<(ostream &out, const MapLoader &mloader)
{
      return out << *mloader.map;
}

// Overloaded assignment operator to perform a deep copy of a `MapLoader` object
MapLoader &MapLoader::operator=(const MapLoader &mLoader)
{
      *this->map = *mLoader.map; // Perform a deep copy of the Map object
      return *this;
}
