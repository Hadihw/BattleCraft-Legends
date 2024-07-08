#include "Player.h"
#include <sstream>
#include <typeinfo>
#include <exception>
#include "CommandProcessing.h"
#include "GameEngine.h"


Player::Player()
{
    this->name = new string("");
    this->playerId = new int(0);
    this->territoryList = new vector<Territories *>();
    this->hand = new Hand();
    this->ordersList = new OrdersList();
    this->armiesPool = new int(0);
    this->playerInTreaty = nullptr;
    this->turnsUntilEndOfTreaty = new int(0);
    this->hasCapturedATerritory = new bool(false);
    this->gotAttacked = new bool(false);
    this->strategy = new HumanPlayerStrategy(this);
}

Player::Player(string name, int playerId, vector<Territories *> *t, Hand *h, OrdersList *o, int *armiesPool, Player *playerInTreaty, int *turnsUntilEndOfTreaty, bool *hasCapturedATerritory, bool *gotAttacked, PlayerStrategy *strategy)
{
    this->name = new string(name);
    this->playerId = new int(playerId);
    this->territoryList = t;
    this->hand = h;
    this->ordersList = o;
    this->armiesPool = armiesPool;
    this->hasCapturedATerritory = hasCapturedATerritory;
    this->gotAttacked = gotAttacked;
    this->playerInTreaty = playerInTreaty;
    this->turnsUntilEndOfTreaty = turnsUntilEndOfTreaty;
    this->strategy = strategy;
}
Player::Player(const Player &p)
{
    this->name = new string(*p.name);
    this->playerId = new int(*p.playerId);
    this->territoryList = new vector<Territories *>(*p.territoryList);
    this->hand = new Hand(*p.hand);
    this->ordersList = new OrdersList(*p.ordersList);
    this->armiesPool = new int(*p.armiesPool);
    this->playerInTreaty = p.playerInTreaty;
    this->turnsUntilEndOfTreaty = new int(*(p.turnsUntilEndOfTreaty));
    this->hasCapturedATerritory = new bool(p.hasCapturedATerritory);
    this->gotAttacked = new bool(p.gotAttacked);
    if (p.strategy->getStrategy() == "Human")
    {
        this->strategy = dynamic_cast<HumanPlayerStrategy *>(p.strategy);
    }
    else if (p.strategy->getStrategy() == "Aggressive")
    {
        this->strategy = dynamic_cast<AggressivePlayerStrategy *>(p.strategy);
    }
    else if (p.strategy->getStrategy() == "Benevolent")
    {
        this->strategy = dynamic_cast<BenevolentPlayerStrategy *>(p.strategy);
    }
    else if (p.strategy->getStrategy() == "Neutral")
    {
        this->strategy = dynamic_cast<NeutralPlayerStrategy *>(p.strategy);
    }
    else if (p.strategy->getStrategy() == "Cheater")
    {
        this->strategy = dynamic_cast<CheaterPlayerStrategy *>(p.strategy);
    }
}

Player::~Player()
{
    delete name;
    delete playerId;
    territoryList->clear();
    delete territoryList;
    delete hand;
    delete ordersList;
    delete armiesPool;
    delete hasCapturedATerritory;
    delete gotAttacked;
    delete turnsUntilEndOfTreaty;
    delete strategy;
}
string Player::getName()
{
    return *this->name;
}
void Player::setName(string *name)
{
    this->name = name;
}
vector<Territories *> &Player::getTerritoryList()
{
    return *this->territoryList;
}

void Player::setTerritoryList(vector<Territories *> *territorylist)
{
    this->territoryList = territoryList;
}

int Player::getArmiesPool()
{
    return *this->armiesPool;
}

void Player::setArmiesPool(int armies)
{
    this->armiesPool = new int(armies);
}
int Player::getPlayerId()
{
    return *this->playerId;
}

void Player::setPlayerId(int id)
{
    *this->playerId = id;
}
bool Player::getHasCapturedATerritory()
{
    return *(this->hasCapturedATerritory);
}
void Player::setHasCapturedATerritory(bool b)
{
    *this->hasCapturedATerritory = b;
}
bool Player::getGotAttacked()
{
    return *this->gotAttacked;
}
void Player::setGotAttacked(bool b)
{
    *this->gotAttacked = b;
}
Hand *Player::getHand()
{
    return this->hand;
}
void Player::setHand(Hand *hand)
{
    this->hand = hand;
}
Player *Player::getPlayerInTreaty()
{
    return this->playerInTreaty;
}
void Player::setPlayerInTreaty(Player *player, int *numberofTurns)
{
    this->playerInTreaty = player;
    this->turnsUntilEndOfTreaty = numberofTurns;
}
int Player::getTurnsUntilEndOfTreaty()
{
    return *this->turnsUntilEndOfTreaty;
}
PlayerStrategy *Player::getPlayerStrategy()
{
    return this->strategy;
}
void Player::setPlayerStrategy(PlayerStrategy *strategy)
{
    this->strategy = strategy;
}
bool Player::setPlayerStrategy(string strategy)
{
    if (strategy == "Human")
    {
        this->strategy = new HumanPlayerStrategy(this);
        return true;
    }
    else if (strategy == "Aggressive")
    {
        this->strategy = new AggressivePlayerStrategy(this);
        return true;
    }
    else if (strategy == "Benevolent")
    {
        this->strategy = new BenevolentPlayerStrategy(this, territoryList);
        return true;
    }
    else if (strategy == "Neutral")
    {
        this->strategy = new NeutralPlayerStrategy(this);
        return true;
    }
    else if (strategy == "Cheater")
    {
        this->strategy = new CheaterPlayerStrategy(this);
        return true;
    }
    else
        return false;
}
void Player::addToPoolOfArmies(int armies)
{
    *this->armiesPool = *this->armiesPool + armies;
}
void Player::printTerritories()
{
    cout << "\n"
         << *this->name << " owns the following territories\n";
    for (auto t : *this->territoryList)
    {
        cout << t->getName() << ", " << t->getArmies() << "\n";
    }
}

vector<Command> *Player::toAttack()
{
    return this->strategy->toAttack();
}
vector<Command> *Player::toDefend()
{
    return this->strategy->toDefend();
}
// Only DeployOrder and AdvanceOrder options should be here, since the other orders are given by playing cards
void Player::issueOrder()
{
    this->strategy->issueOrder(this->toAttack(), this->toDefend());
}

OrdersList *Player::getOrdersList()
{
    return this->ordersList;
}
bool Player::operator==(const Player &rhs)
{
    return this->name == rhs.name;
}
ostream &operator<<(ostream &out, const Player &p)
{
    return cout << "Name";
}