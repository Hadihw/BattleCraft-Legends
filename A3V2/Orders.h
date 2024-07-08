#ifndef ORDERS_H
#define ORDERS_H
#include <string>
#include <iostream>
#include <vector>
#include "Territories.h"
#include "Player.h"
#include "CommandProcessing.h"
#include "LoggingObserver.h"

class Territories;
class Player;
class Order;
class Command;

using namespace std;

class OrdersList : public Subject, public ILoggable
{
private:
    // Node class
    class Node
    {
    public:
        // Constructors
        Node();

        Node(Order *o, Node *next = nullptr, Node *prev = nullptr);

        Node(Node *n);

        // Destructor
        ~Node();

        // Methods
        Node *clone();

        // Attributes
        Node *prev;
        Node *next;
        Order *o;
        ;
    };

    // Pointer attributes
    Node *head;
    Node *tail;

    // Size of the list
    int *size;

    // Variable used to keep track of the effect of ordersList methods
    string *effect;

    // Helper method

    /// @brief Returns the index of the specified node
    /// @param p Pointer to a node
    /// @return Integer representing the position of the specified node
    int getIndex(Node *p);

    // Helper method for move()

    /// @brief Swaps two nodes in the list
    /// @param index1 Integer representing the index of the first node
    /// @param index2 Integer representing the index of the second node
    void swap(int index1, int index2);

    // Helper method for swap() & move()

    /// @brief Returns true if the index is valid
    /// @param index Integer representing the index of a node
    /// @return True or false
    bool validIndex(int index) const;

    // Methods inherited from interfaces
    // string stringToLog(); // this needs to be defined for every ILoggable child

    // all of these methods need to be virtual in the Subject class (no need for an implementation here)
    // void notify(ILoggable* iL);

    // void attach();

    // void dettach();
    ;

public:
    // Constructors
    OrdersList();
    OrdersList(Order *const o);

    // Destructor
    ~OrdersList();

    // Functionality methods for the linked list

    /// @brief Moves an order in 1 of 4 ways (1down, 2up, 3all the way down, 4all the way up)
    /// @param indexOfOrder Integer representing the index of the specified order
    /// @param option Integer representing the corresponding choices
    /// @return A message display
    void move(int indexOfOrder, int option);

    /// @brief Removes the node from the list at the given index
    /// @param index Integer representing the index of the node
    /// @return A pointer to the node that was removed from the list
    Node *remove(int index);

    /// @brief Removes the order in the orders list at the given index
    /// @param index Integer representing the index of the order
    /// @return A pointer to the order that was removed from the list
    Order *removeOrder(int index);

    /// @brief Returns the order at the given index
    /// @param index Integer representing the index of the order
    /// @return A pointer to the order that was specified
    Order *getOrder(int index);

    /// @brief Returns the contents of the OrderList as a string
    string toString() const;
    int Size();
    // Static methods

    /// @brief Adds an order to the player's orderList
    /// @param order Order object that is going to be added to the player's orderList
    void addOrder(Order *order);

    virtual string stringToLog() const override;
};

// Enumerated data type to determine the class' type **DO NOT SWITCH THE ORDER OF THIS INITIALIZATION BC IT WILL AFFECT THE LOGIC OF addOrder() and move()
enum class childrenTypes : char
{
    Order = 0,
    DeployOrder,
    BombOrder,
    AirliftOrder,
    AdvanceOrder,
    BlockadeOrder,
    NegotiateOrder
};
childrenTypes operator++(childrenTypes &c);
class Order
{ // This class needs to be abstract
public:
    // Constructors
    Order();

    Order(Player *currentPlayer);

    Order(Order *orderObj);

    // Destructors
    virtual ~Order();

    // Returns a string representation of the order
    virtual string toString() const = 0;
    // Validates the order
    virtual bool validate() = 0; // This method should be called isValidObject : /
    // Executes the order
    virtual void execute() = 0;
    // Returns a deep copy instance of the order object
    virtual Order *clone() = 0;
    // Returns the name of the order's class
    virtual string getName() const;
    // Returns the type of the order's class
    virtual childrenTypes getType() const;
    // Returns the player object of the order
    virtual Player *getPlayer() const;
    ;

private:
    // Type of the order at runtime
    const childrenTypes *const type;
    ;

protected:
    Player *currentPlayer;
    ;
};

class DeployOrder : public Order
{
public:
    // Constructors

    DeployOrder(Command *cmd, Player *currentPlayer);

    DeployOrder(DeployOrder *dO);

    // Destructor
    ~DeployOrder();

    string toString() const;

    bool validate();

    void execute();

    DeployOrder *clone();

    string getName() const;

    childrenTypes getType() const;
    ;

private:
    int *nbOfArmies;
    Territories *destination;
    const childrenTypes *const type;
    ;
};

class AdvanceOrder : public Order
{
public:
    // Constructors
    AdvanceOrder(Command *cmd, Player *currentPlayer);

    AdvanceOrder(AdvanceOrder *adO);

    // Destructor
    ~AdvanceOrder();

    string toString() const;

    bool validate();

    void execute();

    AdvanceOrder *clone();

    string getName() const;

    childrenTypes getType() const;
    ;

private:
    // Attributes
    int *nbOfArmies;
    Territories *source;
    Territories *destination;
    const childrenTypes *const type = new childrenTypes(childrenTypes::AdvanceOrder);
    ;
};

class BombOrder : public Order
{
public:
    // Constructors
    BombOrder(Command *cmd, Player *currentPlayer);

    BombOrder(BombOrder *bO);

    // Destructor
    ~BombOrder();

    // Methods
    string toString() const;

    bool validate();

    void execute();

    BombOrder *clone();

    string getName() const;

    childrenTypes getType() const;
    ;

private:
    Territories *destination;
    const childrenTypes *const type = new childrenTypes(childrenTypes::BombOrder);
    ;
};

class BlockadeOrder : public Order
{
public:
    // Constructors
    BlockadeOrder(Command *cmd, Player *currentPlayer);

    BlockadeOrder(BlockadeOrder *blO);

    // Destructor
    ~BlockadeOrder();

    string toString() const;

    bool validate();

    void execute();

    BlockadeOrder *clone();

    string getName() const;

    childrenTypes getType() const;
    ;

private:
    Territories *destination;
    double *secretFactor;
    const childrenTypes *const type = new childrenTypes(childrenTypes::BlockadeOrder);
    double calculateSecretFactor();
    ;
};

class AirliftOrder : public Order
{
public:
    AirliftOrder(Command *cmd, Player *currentPlayer);

    AirliftOrder(AirliftOrder *alO);

    // Destructor
    ~AirliftOrder();

    string toString() const;

    bool validate();

    void execute();

    AirliftOrder *clone();

    string getName() const;

    childrenTypes getType() const;
    ;

private:
    Territories *source;
    Territories *destination;
    int *nbOfArmies;
    const childrenTypes *const type = new childrenTypes(childrenTypes::AirliftOrder);
    ;
};

class NegotiateOrder : public Order
{
public:
    NegotiateOrder(Command *cmd, Player *targetPlayer, Player *currentPlayer);

    NegotiateOrder(NegotiateOrder *nO);

    // Destructor
    ~NegotiateOrder();

    string toString() const;

    bool validate();

    void execute();

    NegotiateOrder *clone();

    string getName() const;

    childrenTypes getType() const;
    ;

private:
    Player *targetPlayer;
    int *secretFactor;
    const childrenTypes *const type = new childrenTypes(childrenTypes::NegotiateOrder);
    int calculateSecretFactor();
};

// Helper class for assignment 1
class PlayersInGame
{
private:
    static vector<Player *> *players;
    ;

public:
    // Sets players that are going to play
    static void setPlayers(vector<Player *> *players);

    // Returns players that are playing in the game
    static vector<Player *> *getPlayers();

    // Displays the current players in the game in order
    static void displayPlayers();

    // Returns the player specified by their index
    static Player *getPlayer(int index);
    ;
};

// Global stream insertion operators
ostream &operator<<(ostream &out, const Order *o);

ostream &operator<<(ostream &out, const OrdersList *o);

void testOrdersLists();
#endif
