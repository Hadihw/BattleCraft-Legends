#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include "Orders.h"
#include "Map.h"
#include "GameEngine.h"
// #include "Command.h"
using namespace std;

// class Command;

// Node Constructors/Destructor
OrdersList::Node::Node()
{
    this->prev = nullptr;
    this->next = nullptr;
}
OrdersList::Node::Node(Order *o, Node *next, Node *prev)
{
    this->o = o;
    this->next = next;
    this->prev = prev;
}

OrdersList::Node::Node(Node *n)
{
    this->o = n->o->clone();
    this->prev = n->prev;
    this->next = n->next;
}

OrdersList::Node::~Node()
{
}

OrdersList::Node *OrdersList::Node::clone()
{
    return new Node(this);
}

// OrdersList methods
int OrdersList::getIndex(Node *p)
{
    Node *temp = head;

    if (temp == nullptr)
    {
        return -1;
    }

    else
    {
        int i = 1;
        while (temp != nullptr)
        {
            if (p == temp)
            {
                break;
            }
            temp = temp->next;
            i++;
        }

        if (!(i <= *size))
            return -1;

        return i;
    }
}

void OrdersList::swap(int index1, int index2)
{
    Node *temp1;
    Node *temp2;

    temp2 = head;

    // check if they are valid indices
    if (!validIndex(index1) || !validIndex(index2))
    {
        cout << "Invalid swap" << endl;
        return;
    }
    // Return from method if we are trying to swap the same Node
    if (index1 == index2)
    {
        return;
    }

    // Otherwise continue with operation

    // Iterate to find the second specified node
    for (int i = 1; i < index2; i++)
    {
        temp2 = temp2->next;
    }

    // Remove and extract the first specified node
    temp1 = remove(index1);

    // Replace first specified node with second specified node

    // If first specified node is first in this list
    if (index1 == 1)
    {
        head = new Node(temp2->o, head);
        head->next->prev = head;

        (*size)++;
    }
    // If first specified node is last in the list
    else if (index1 == *(this->size) + 1)
    {
        tail = new Node(temp2->o, nullptr, tail);

        tail->prev->next = tail;

        (*size)++;
    }

    // If first specified node is neither first or last in the list
    else
    {
        Node *temp3 = head;

        // Iterate through list until you reach the node that is before the node your removed
        for (int i = 1; i < index1 - 1; i++)
        {
            temp3 = temp3->next;
        }

        temp3->next = new Node(temp2->o, temp3->next, temp3);

        temp3->next->next->prev = temp3->next;

        (*size)++;
    }

    // Now do the same for the second specified node

    // Remove and extract the second specified node
    temp2 = remove(index2);

    // Replace second specified node with first specified node

    // If second specified node is first in this list
    if (index2 == 1)
    {
        head = new Node(temp1->o, head);
        head->next->prev = head;

        (*size)++;
    }
    // If second specified node is last in the list
    else if (index2 == (*(this->size) + 1))
    {
        tail = new Node(temp1->o, nullptr, tail);

        tail->prev->next = tail;

        (*size)++;
    }

    // If second specified node is neither first or last in the list
    else
    {
        Node *temp3 = head;

        // Iterate through list until you reach the node that is before the node you removed
        for (int i = 1; i < index2 - 1; i++)
        {
            temp3 = temp3->next;
        }

        temp3->next = new Node(temp1->o, temp3->next, temp3);

        temp3->next->next->prev = temp3->next;

        (*size)++;
    }

    delete temp1;
    delete temp2;
}

// Helper method for swap() & move()
bool OrdersList::validIndex(int index) const
{
    return (index >= 1) && (index <= *size);
}

// Methods inherited from interfaces
//  string OrdersList::stringToLog(){
//      return *(this->effect);
//  }

// OrdersList Constructors/Destructor
OrdersList::OrdersList()
{
    this->head = nullptr;
    this->tail = nullptr;
    this->size = new int(0);
    this->effect = new string("Empty orders list was created.");
}

OrdersList::OrdersList(Order *const o)
{
    head = tail = new Node(o);
    (*size)++;
    this->effect = new string("Orders list with 1 order was created");
}

OrdersList::~OrdersList()
{
    Node *temp = head;

    while (temp != nullptr)
    {
        temp = temp->next;
        delete temp->prev;
    }

    delete tail;
}

// Functionality methods for the OrdersList linked list
void OrdersList::addOrder(Order *o)
{
    Node *temp;
    childrenTypes type = o->getType();

    temp = head;
    // If list is empty
    if (head == nullptr)
    {
        head = new Node(o);
        tail = head;
        (*size)++;
        return;
    }
    // Iteratre until you reach the correct hierarchy
    while (temp != nullptr && temp->o->getType() <= type)
    {
        temp = temp->next;
    }

    if (temp == nullptr)
    {
        // add to tail
        tail->next = new Node(o, nullptr, tail);
        tail = tail->next;
    }
    else if (temp == head)
    {
        // if we stopped at the head
        head = new Node(o, head, nullptr);
        head->next->prev = head;
    }
    else
    {
        // In any other case, we insert before the node we stopped at
        temp->prev = new Node(o, temp, temp->prev);
        temp->prev->prev->next = temp->prev;
    }

    (*size)++;

    delete effect;
    effect = new string(o->getName() + ": " + o->toString() + " was added to the orders list of " + o->getPlayer()->getName());

    // notify(this);

    return;
}

OrdersList::Node *OrdersList::remove(int index)
{
    Node *temp;
    temp = head;

    if (!validIndex(index))
    {
        return nullptr;
    }

    // Iterate through the OrdersList
    for (int i = 1; i < index; i++)
    {
        temp = temp->next;
    }

    // If there is only 1 node in the list
    if (temp == head && head == tail)
    {
        head = tail = nullptr;
        (*size)--;
        return temp;
    }

    // If the node is at the beginning of the list
    else if (temp == head)
    {
        head = head->next;
        head->prev = nullptr;
        temp->next = nullptr;
        (*size)--;
        return temp;
    }

    // If the node is at the end of the list
    else if (temp == tail)
    {
        tail = temp->prev;
        tail->next = nullptr;
        temp->prev = nullptr;

        (*size)--;

        return temp;
    }

    // If the node is somewhere in between two nodes that are not the head or tail or the list
    else
    {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;

        temp->prev = nullptr;
        temp->next = nullptr;

        (*size)--;

        return temp;
    }
}

Order *OrdersList::removeOrder(int index)
{
    return remove(index)->o;
}

Order *OrdersList::getOrder(int index)
{
    Node *temp = head;

    if (!validIndex(index))
    {
        return nullptr;
    }

    for (int i = 1; i < index; i++)
    {
        temp = temp->next;
    }

    return temp->o;
}

void OrdersList::move(int indexOfOrder, int option)
{

    // Validate specified index of Order Object
    if (!validIndex(indexOfOrder))
    {
        cout << "Not valid index!: in move" << endl;
        return;
    }

    Node *temp = head; // we already know head cannot be null because we validated that the index is between 1 and the size of the orderlist => there is at least 1 order in the list

    // Iterate through list
    for (int i = 1; i < indexOfOrder; i++)
    {
        temp = temp->next;
    }

    string type = temp->o->getName();

    switch (option)
    {
    // MoveDown
    case 1:
        // If specified node is tail
        if (temp->next == nullptr)
        {
            cout << type << " is already at the bottom of the list." << endl;
        }
        // If node before specified node is not of the same type
        else if (temp->next->o->getName() != type)
        {
            cout << type << " cannot be moved further down." << endl;
        }
        else
        {
            swap(indexOfOrder, indexOfOrder + 1);
        }
        break;
    // MoveUp
    case 2:
        // If specified node is head
        if (temp->prev == nullptr)
        {
            cout << type << " is already at the top of the list." << endl;
        }
        // If node before specified node is not of the same type
        else if (temp->prev->o->getName() != type)
        {
            cout << type << " cannot be moved further up." << endl;
        }
        else
        {
            swap(indexOfOrder, indexOfOrder - 1);
        }
        break;
    // MoveToBottom
    case 3:

        // If specified node is tail
        if (temp->next == nullptr)
        {
            cout << type << " is already at the bottom of the list." << endl;
            return;
        }

        // While node after specified node is of the same type keep iterating
        while (temp->next != nullptr)
        {
            if (temp->next->o->getName() != type)
                break;
            temp = temp->next;
        }

        if (getIndex(temp) == indexOfOrder)
        {
            cout << type << " cannot be moved further down." << endl;
        }

        else
        {
            swap(indexOfOrder, getIndex(temp));
        }

        break;
    // MoveToTop
    case 4:
        // If specified node is head
        if (temp->prev == nullptr)
        {
            cout << type << " is already at the top of the list." << endl;
            return;
        }
        // While node before specified node is of the same type keep iterating
        while (temp->prev != nullptr)
        {
            if (temp->prev->o->getName() != type)
                break;
            temp = temp->prev;
        }

        if (getIndex(temp) == indexOfOrder)
        {
            cout << type << " cannot be moved further up." << endl;
        }

        else
        {
            swap(indexOfOrder, getIndex(temp));
        }

        break;

    default:
        cout << "Invalid option." << endl;
    }
}

string OrdersList::toString() const
{
    // If there are no orders in the OrdersList
    if (head == nullptr)
    {
        return "There are no orders to display!\n";
    }
    // If there are orders in the OrdersList
    Node *temp;
    temp = head;
    int i = 1;
    string orders;

    while (temp != nullptr)
    {
        orders += to_string(i) + ". " + temp->o->toString() + "\n";
        i++;
        temp = temp->next;
    }

    return orders;
}
int OrdersList::Size()
{
    return *this->size;
}
// Order Constructors/Destructors
string OrdersList::stringToLog() const
{
    return *this->effect;
}
childrenTypes operator++(childrenTypes &c)
{

    using IntType = typename std::underlying_type<childrenTypes>::type;
    c = static_cast<childrenTypes>(static_cast<IntType>(c) + 1);
    if (c > childrenTypes::NegotiateOrder)
        c = static_cast<childrenTypes>(0);
    return c;
}
// Order Constructors/Destructors

Order::Order() : type(new childrenTypes(childrenTypes::Order))
{
    currentPlayer = nullptr;
}

Order::Order(Player *currentPlayer) : type(new childrenTypes(childrenTypes::Order))
{
    this->currentPlayer = currentPlayer;
}

Order::Order(Order *orderObj) : type(orderObj->type)
{
    this->currentPlayer = orderObj->currentPlayer;
}

Order::~Order()
{
}

// Order Methods
string Order::getName() const
{
    return "Order";
}

childrenTypes Order::getType() const
{
    return *(this->type);
}

Player *Order::getPlayer() const
{
    return this->currentPlayer;
}

// DeployOrder methods

DeployOrder::DeployOrder(Command *cmd, Player *currentPlayer) : type(new childrenTypes(childrenTypes::DeployOrder))
{

    stringstream ss;

    ss << (cmd->getCommandString());

    string holder;

    getline(ss, holder, ',');

    this->nbOfArmies = new int(stoi(holder));

    ss >> holder;

    this->destination = GameEngine::getMap()->getTerritoryByName(holder);

    this->currentPlayer = currentPlayer;
}

DeployOrder::DeployOrder(DeployOrder *dO) : type(dO->type)
{
    this->nbOfArmies = new int(*(dO->nbOfArmies));
    this->destination = dO->destination;
    this->currentPlayer = dO->currentPlayer;
}

DeployOrder::~DeployOrder()
{
    delete this->nbOfArmies;
    delete this->type;
}

string DeployOrder::toString() const
{
    if (currentPlayer == nullptr || destination == nullptr || nbOfArmies == nullptr)
    {
        return "INVALID ORDER";
    }

    return "Player " + currentPlayer->getName() + ": Deploy " + to_string(*nbOfArmies) + " armies to " + destination->getName();
}

bool DeployOrder::validate()
{

    if (destination == nullptr)
    {
        cout << "!//INVALID ORDER//\nDestination territory does not exist!\n//End of Message//!" << endl;

        return false;
    }

    else if (&destination->getPlayer() != currentPlayer)
    {

        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot deploy to a territory you do not own\n//End of Message//!" << endl;

        return false;
    }

    else if (*nbOfArmies < 0)
    {

        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot deploy a negative amount of units.\n//End of Message//!" << endl;

        return false;
    }

    else if (*nbOfArmies == 0)
    {

        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot deploy 0 army units.\n//End of Message//!" << endl;

        return false;
    }

    else if (currentPlayer->getArmiesPool() < *nbOfArmies)
    {

        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot deploy more armies than the number of armies available in your pool.\nYour pool: " + to_string(currentPlayer->getArmiesPool()) + " armies\n//End of Message//!" << endl;

        return false;
    }

    return true;
}

void DeployOrder::execute()
{
    if (!validate())
    {
        return;
    }

    // Add the number of armies to the destination territory and subtract from the player's pool
    destination->setNumArmies(destination->getArmies() + *nbOfArmies);
    currentPlayer->setArmiesPool(currentPlayer->getArmiesPool() - *nbOfArmies);

    // Print the action to the console
    cout << this->toString() << endl;
}

DeployOrder *DeployOrder::clone() { return new DeployOrder(this); }

string DeployOrder::getName() const
{
    return "DeployOrder";
}

childrenTypes DeployOrder::getType() const
{
    return *(this->type);
}

// AdvanceOrder methods

AdvanceOrder::AdvanceOrder(Command *cmd, Player *currentPlayer) : type(new childrenTypes(childrenTypes::AdvanceOrder))
{
    stringstream ss;

    ss << (cmd->getCommandString());

    string holder;

    getline(ss, holder, ',');

    this->nbOfArmies = new int(stoi(holder));

    getline(ss, holder, ',');

    this->source = GameEngine::getMap()->getTerritoryByName(holder);

    ss >> holder;

    this->destination = GameEngine::getMap()->getTerritoryByName(holder);

    this->currentPlayer = currentPlayer;
}

AdvanceOrder::AdvanceOrder(AdvanceOrder *adO) : type(new childrenTypes(childrenTypes::AdvanceOrder))
{
    this->nbOfArmies = new int(*(adO->nbOfArmies));
    this->source = adO->source;
    this->destination = adO->destination;
    this->currentPlayer = adO->currentPlayer;
}

AdvanceOrder::~AdvanceOrder()
{
    delete this->nbOfArmies;
    delete this->type;
}

string AdvanceOrder::toString() const
{
    if (currentPlayer == nullptr || destination == nullptr || nbOfArmies == nullptr || source == nullptr)
    {
        return "INVALID ORDER";
    }

    return "Player " + currentPlayer->getName() + ": " + to_string(*nbOfArmies) + " armies from " + source->getName() + " will attack/transfer " + destination->getName();
}

bool AdvanceOrder::validate()
{

    if (destination == nullptr)
    {
        cout << "!//INVALID ORDER//\nDestination territory does not exist!\n//End of Message//!" << endl;

        return false;
    }

    else if (source == nullptr)
    {
        cout << "!//INVALID ORDER//\nSource territory does not exist!\n//End of Message//!" << endl;

        return false;
    }

    else if (&source->getPlayer() != currentPlayer)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou do not own territory: " + source->getName() + "\n//End of Message//!" << endl;
        return false;
    }

    else if (!destination->isBorder(source))
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nDestination territory: " + destination->getName() + " does not border " + source->getName() + "\n//End of Message//!" << endl;
        return false;
    }

    else if (*nbOfArmies < 0)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot advance a negative amount of armies\n//End of Message//!" << endl;
        return false;
    }

    else if (*nbOfArmies == 0)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot advance 0 army units\n//End of Message//!" << endl;
        return false;
    }

    else if (source->getArmies() < *nbOfArmies)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou do not have enough armies to advance " << *nbOfArmies << "army units from: " + source->getName() + " to: " + destination->getName() + " !\nYou have: " << source->getArmies() << " army units in " + source->getName() + "\n//End of Message//!" << endl;
        return false;
    }
    // Checks if current player is in a treaty with the player who owns the destination territory
    else if (currentPlayer->getPlayerInTreaty() == &destination->getPlayer())
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot attack " + currentPlayer->getPlayerInTreaty()->getName() + " because you are still in a treaty with them! Treaty will end in: " + to_string(currentPlayer->getTurnsUntilEndOfTreaty()) + " turns\n//End of Message//!" << endl;
        return false;
    }

    return true;
}

void AdvanceOrder::execute()
{
    if (!validate())
    {
        return;
    }
    // Case where the destination territory owner is a neutral player;
    if (destination->getPlayer().getPlayerStrategy()->getStrategy() == "Neutral")
        destination->getPlayer().setGotAttacked(true);
    // Case where the destination is an enemy player's territory
    if (&destination->getPlayer() != currentPlayer)
    {
        // Simulate the battle
        // Send the appropriate number of armies to battle
        source->setNumArmies(source->getArmies() - (*nbOfArmies));
        // Extract the armies
        int attackingArmies = *nbOfArmies;
        int deffendingArmies = destination->getArmies();
        int numOfDeffendingArmiesKilled = 0;
        int numOfAttackingArmiesKilled = 0;
        // Setup random number generation between 1 and 100
        random_device *rd = new random_device();
        mt19937 *mersenne = new mt19937{(*rd)()};
        binomial_distribution<> *b1 = new binomial_distribution{attackingArmies, 0.60};  // Returns the number of attacking armies that have succeeded in killing a deffending army(60% chance/army)
        binomial_distribution<> *b2 = new binomial_distribution{deffendingArmies, 0.70}; // Returns the number of deffending armies that have succeeded in killing an attacking army(70% chance/army)
        // Calculate how many armies were killed
        numOfDeffendingArmiesKilled = (*b1)(*mersenne);
        numOfAttackingArmiesKilled = (*b2)(*mersenne);

        // Killing simulation
        if (numOfDeffendingArmiesKilled >= deffendingArmies)
        {
            destination->setNumArmies(0);
        }
        else
        {
            destination->setNumArmies(destination->getArmies() - numOfDeffendingArmiesKilled);
        }

        if (numOfAttackingArmiesKilled >= attackingArmies)
        {
            attackingArmies = 0; // No more attacking armies
        }
        else
        {
            attackingArmies = attackingArmies - numOfAttackingArmiesKilled;
        }

        // Decide if territory transfer needs to happen
        if (attackingArmies != 0 && destination->getArmies() == 0)
        {
            // Switch the territories in the players' inventory
            // Attacking player
            currentPlayer->getTerritoryList().push_back(destination);
            // Defending player
            // Find the destination in the deffending player's territory list
            for (auto i = destination->getPlayer().getTerritoryList().begin(); i != destination->getPlayer().getTerritoryList().end(); i++)
            {
                if ((*i) == destination)
                {
                    destination->getPlayer().getTerritoryList().erase(i);
                    break;
                }
            }

            // Update the attacking player's hasCapturedATerritory variable
            currentPlayer->setHasCapturedATerritory(true);

            // Update the territories correctly
            destination->setPlayer(currentPlayer);      // transfers control of territory to attacking player
            destination->setNumArmies(attackingArmies); // transfers remaining armies onto newly acquired territory

            delete rd;
            delete mersenne;
            delete b1;
            delete b2;
        }
        else
        {
            // Return the nb of attacking armies to the source territory if capture is not successful
            source->setNumArmies(source->getArmies() + attackingArmies);
        }
    }

    // Case where the destination is the current player's territory
    else
    {
        // Transfer armies from one territory to the other
        source->setNumArmies(source->getArmies() - (*nbOfArmies));
        destination->setNumArmies(destination->getArmies() + *nbOfArmies);
    }

    cout << this->toString() << endl;
}

AdvanceOrder *AdvanceOrder::clone()
{
    return new AdvanceOrder(this);
}

string AdvanceOrder::getName() const
{
    return "AdvanceOrder";
}

childrenTypes AdvanceOrder::getType() const
{
    return *(this->type);
}

// BombOrder Methods

// Constructors

BombOrder::BombOrder(Command *cmd, Player *currentPlayer) : type(new childrenTypes(childrenTypes::BombOrder))
{
    this->destination = GameEngine::getMap()->getTerritoryByName((cmd->getCommandString()));
    this->currentPlayer = currentPlayer;
}

BombOrder::BombOrder(BombOrder *bO) : type(new childrenTypes(childrenTypes::BombOrder))
{
    this->destination = bO->destination;
    this->currentPlayer = bO->currentPlayer;
}

BombOrder::~BombOrder()
{
    delete this->type;
}

string BombOrder::toString() const
{
    if (currentPlayer == nullptr || destination == nullptr)
    {
        return "INVALID ORDER";
    }

    return "Player " + currentPlayer->getName() + ": Bombing " + destination->getName() + "...";
}

bool BombOrder::validate()
{
    if (destination == nullptr)
    {
        cout << "!//INVALID ORDER//\nDestination territory does not exist!\n//End of Message//!" << endl;

        return false;
    }

    else if (&destination->getPlayer() == currentPlayer)
    {

        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot bomb your own territory!\n//End of Message//!" << endl;

        return false;
    }

    // Extract vector of territories
    vector<Territories *> *territories = &currentPlayer->getTerritoryList();

    for (vector<Territories *>::iterator p = territories->begin(); p != territories->end(); p++)
    {
        if ((*p)->isBorder(destination))
            return true;
    }

    cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot bomb territory \"" + destination->getName() + "\" because it does not border any of your territories!\n//End of Message//!" << endl;

    return false;
}

void BombOrder::execute()
{
    if (!validate())
    {
        return;
    }
    if (destination->getPlayer().getPlayerStrategy()->getStrategy() == "Neutral")
        destination->getPlayer().setGotAttacked(true);
    // Bomb the destination territory and half the number of armies on that territory (rounded down)
    destination->setNumArmies(destination->getArmies() / 2);

    //! There is no transfer of territories!//

    cout << this->toString() << endl;
}

BombOrder *BombOrder::clone()
{
    return new BombOrder(this);
}

string BombOrder::getName() const
{
    return "BombOrder";
}

childrenTypes BombOrder::getType() const
{
    return *(this->type);
}

// BlockadeOrder methods

BlockadeOrder::BlockadeOrder(Command *cmd, Player *currentPlayer) : type(new childrenTypes(childrenTypes::BlockadeOrder))
{
    this->destination = GameEngine::getMap()->getTerritoryByName((cmd->getCommandString()));
    this->secretFactor = new double(calculateSecretFactor());
    this->currentPlayer = currentPlayer;
}

BlockadeOrder::BlockadeOrder(BlockadeOrder *blO) : type(new childrenTypes(childrenTypes::BlockadeOrder))
{
    this->destination = blO->destination;
    this->secretFactor = blO->secretFactor;
    this->currentPlayer = blO->currentPlayer;
}

BlockadeOrder::~BlockadeOrder()
{
    delete this->type;
}

string BlockadeOrder::toString() const
{
    if (currentPlayer == nullptr || destination == nullptr)
    {
        return "INVALID ORDER";
    }

    return "Player " + currentPlayer->getName() + ": Blockading " + destination->getName() + "...";
}
bool BlockadeOrder::validate()
{

    if (destination == nullptr)
    {
        cout << "!//INVALID ORDER//\nDestination territory does not exist!\n//End of Message//!" << endl;

        return false;
    }

    else if (&destination->getPlayer() != currentPlayer)
    {

        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot blockade a territory you do not own!\n//End of Message//!" << endl;

        return false;
    }

    return true;
}

void BlockadeOrder::execute()
{
    if (!validate())
    {
        return;
    }

    // Multiply the number of army units on the destination territory by the secret factor
    destination->setNumArmies(destination->getArmies() * (int)(*(this->secretFactor)));

    // Yield the control of the territory to the neutral player

    // Remove the territory from the player's territory list
    for (auto i = currentPlayer->getTerritoryList().begin(); i != currentPlayer->getTerritoryList().end(); i++)
    {
        if ((*i) == destination)
        {
            currentPlayer->getTerritoryList().erase(i);
            break;
        }
    }

    // Give the territory to the neutral player
    GameEngine::nPlayer->getTerritoryList().push_back(destination);

    // Update the territory to reflect the changes
    destination->setPlayer(GameEngine::nPlayer);

    cout << this->toString() << endl;
}

BlockadeOrder *BlockadeOrder::clone()
{
    return new BlockadeOrder(this);
}

double BlockadeOrder::calculateSecretFactor()
{
    return 2.0; // This number can be anything within reason (:O)-|----<
}

string BlockadeOrder::getName() const
{
    return "BlockadeOrder";
}

childrenTypes BlockadeOrder::getType() const
{
    return *(this->type);
}

// AirliftOrder methods

AirliftOrder::AirliftOrder(Command *cmd, Player *currentPlayer) : type(new childrenTypes(childrenTypes::AirliftOrder))
{
    stringstream ss;

    ss << (cmd->getCommandString());

    string holder;

    getline(ss, holder, ',');

    this->nbOfArmies = new int(stoi(holder));

    getline(ss, holder, ',');

    this->source = GameEngine::getMap()->getTerritoryByName(holder);

    ss >> holder;

    this->destination = GameEngine::getMap()->getTerritoryByName(holder);

    this->currentPlayer = currentPlayer;
}

AirliftOrder::AirliftOrder(AirliftOrder *alO) : type(new childrenTypes(childrenTypes::AirliftOrder))
{
    this->destination = alO->destination;
    this->source = alO->source;
    this->nbOfArmies = new int(*(this->nbOfArmies));
    this->currentPlayer = alO->currentPlayer;
}

AirliftOrder::~AirliftOrder()
{
    delete this->nbOfArmies;
    delete this->type;
}

string AirliftOrder::toString() const
{
    if (currentPlayer == nullptr || destination == nullptr || source == nullptr || nbOfArmies == nullptr)
    {
        return "INVALID ORDER";
    }

    return "Player " + currentPlayer->getName() + ": " + to_string(*nbOfArmies) + " armies from " + source->getName() + " will airlift to " + destination->getName();
}

bool AirliftOrder::validate()
{

    if (destination == nullptr)
    {
        cout << "!//INVALID ORDER//\nDestination territory does not exist!\n//End of Message//!" << endl;

        return false;
    }

    else if (source == nullptr)
    {
        cout << "!//INVALID ORDER//\nSource territory does not exist!\n//End of Message//!" << endl;

        return false;
    }

    else if (&source->getPlayer() != currentPlayer)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot airlift from a territory you do not own territory\n//End of Message//!" << endl;
        return false;
    }
    else if (&destination->getPlayer() != currentPlayer)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot airlift to a territory you do not own!\n//End of Message//!" << endl;
        return false;
    }

    else if (*nbOfArmies < 0)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot airlift a negative amount of armies\n//End of Message//!" << endl;
        return false;
    }

    else if (*nbOfArmies == 0)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot airlift 0 army units\n//End of Message//!" << endl;
        return false;
    }

    else if (source->getArmies() < *nbOfArmies)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou do not have enough armies to airlift " << *nbOfArmies << " army units from: " + source->getName() + " to: " + destination->getName() + "!\nYou have: " << source->getArmies() << " units in " + source->getName() + "\n//End of Message//!" << endl;
        return false;
    }

    return true;
}

void AirliftOrder::execute()
{
    if (!validate())
    {
        return;
    }

    // Move the specified number of armies from the source territory to the destination territory
    source->setNumArmies(source->getArmies() - (*nbOfArmies));
    destination->setNumArmies(destination->getArmies() + *nbOfArmies);

    cout << this->toString() << endl;
}

AirliftOrder *AirliftOrder::clone()
{
    return new AirliftOrder(this);
}

string AirliftOrder::getName() const
{
    return "AirliftOrder";
}

childrenTypes AirliftOrder::getType() const
{
    return *(this->type);
}

// NegotiateOrder methods

NegotiateOrder::NegotiateOrder(Command *cmd, Player *targetPlayer, Player *currentPlayer) : type(new childrenTypes(childrenTypes::NegotiateOrder))
{
    this->targetPlayer = targetPlayer;
    this->secretFactor = new int(calculateSecretFactor());
    this->currentPlayer = currentPlayer;
}

NegotiateOrder::NegotiateOrder(NegotiateOrder *nO) : type(new childrenTypes(childrenTypes::NegotiateOrder))
{
    this->targetPlayer = nO->targetPlayer;
    this->currentPlayer = nO->currentPlayer;
}

NegotiateOrder::~NegotiateOrder()
{
    delete this->type;
}

string NegotiateOrder::toString() const
{
    if (currentPlayer == nullptr || targetPlayer == nullptr)
    {
        return "INVALID ORDER";
    }

    return "Player " + currentPlayer->getName() + ": A treaty was signed for " + to_string(*secretFactor) + " days with player " + targetPlayer->getName();
}

bool NegotiateOrder::validate()
{
    if (targetPlayer == nullptr)
    {
        cout << "!//INVALID ORDER//\nInvalid index specified in order.\n//End of Message//!" << endl;
        return false;
    }

    else if (targetPlayer == currentPlayer)
    {
        cout << "!//INVALID ORDER//\n" + this->toString() + "\nYou cannot sign a treaty with yourself!\n//End of Message//!" << endl;
        return false;
    }

    return true;
}

void NegotiateOrder::execute()
{
    if (!validate())
    {
        return;
    }

    // Set a Treaty between the target player and the current player for a number of turns determined by the secret factor
    currentPlayer->setPlayerInTreaty(targetPlayer, this->secretFactor);
    targetPlayer->setPlayerInTreaty(currentPlayer, this->secretFactor);

    cout << this->toString() << endl;
}

NegotiateOrder *NegotiateOrder::clone()
{
    return new NegotiateOrder(this);
}

int NegotiateOrder::calculateSecretFactor()
{
    return rand() % 4 + 1;
}

string NegotiateOrder::getName() const
{
    return "NegotiateOrder";
}

childrenTypes NegotiateOrder::getType() const
{
    return *(this->type);
}

// Global stream insertion operator for orders
ostream &operator<<(ostream &out, const Order *o)
{
    out << o->toString();
    return out;
}

// Global stream insertion operator for OrdersList
ostream &operator<<(ostream &out, const OrdersList *ordersList)
{
    out << ordersList->toString();
    return out;
}

// Methods in PlayersInGame
vector<Player *> *PlayersInGame::players = nullptr;

void PlayersInGame::setPlayers(vector<Player *> *players)
{
    if (PlayersInGame::players != nullptr)
    {
        delete PlayersInGame::players;
        PlayersInGame::players = players;
        return;
    }
    PlayersInGame::players = players;
}

vector<Player *> *PlayersInGame::getPlayers()
{
    return PlayersInGame::players;
}

Player *PlayersInGame::getPlayer(int index)
{
    return PlayersInGame::getPlayers()->at(index - 1);
}

void PlayersInGame::displayPlayers()
{
    vector<Player *> *players = PlayersInGame::getPlayers();
    for (auto player = players->begin(); player != players->end(); player++)
    {
        cout << "Player " << (distance(players->begin(), player) + 1) << ": " << (*player)->getName() << endl;
    }
}