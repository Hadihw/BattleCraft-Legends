#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H
#include <vector>
#include <string>
#include "Territories.h"
#include "CommandProcessing.h"
// #include "Player.h"
class Player;
class PlayerStrategy
{

public:
    PlayerStrategy();
    PlayerStrategy(Player *);
    PlayerStrategy(const PlayerStrategy &);
    virtual ~PlayerStrategy();
    virtual std::vector<Command> *toAttack() = 0;
    virtual std::vector<Command> *toDefend() = 0;
    virtual void issueOrder(std::vector<Command> *toDefend, std::vector<Command> *toAttack) = 0;
    virtual std::string getStrategy() = 0;
    Player *getPlayer();

protected:
    Player *player;
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(Player *);
    HumanPlayerStrategy(const HumanPlayerStrategy &);
    virtual ~HumanPlayerStrategy();
    std::vector<Command> *toAttack() override;
    std::vector<Command> *toDefend() override;
    void issueOrder(std::vector<Command> *toDefend, std::vector<Command> *toAttack) override;
    std::string getStrategy() override;
};

class AggressivePlayerStrategy : public PlayerStrategy
{
public:
    AggressivePlayerStrategy();
    AggressivePlayerStrategy(Player *);
    AggressivePlayerStrategy(const AggressivePlayerStrategy &);
    ~AggressivePlayerStrategy() override;
    std::vector<Command> *toAttack() override;
    std::vector<Command> *toDefend() override;
    void issueOrder(std::vector<Command> *toDefend, std::vector<Command> *toAttack) override;
    std::string getStrategy() override;

private:
    void adjacentToEnemyTerritories();
    std::vector<std::pair<Territories *, Territories *>> *strongestTerritories;
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:
    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(Player *, std::vector<Territories *> *);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy &);
    ~BenevolentPlayerStrategy();
    std::vector<Command> *toAttack() override;
    std::vector<Command> *toDefend() override;
    void issueOrder(std::vector<Command> *toDefend, std::vector<Command> *toAttack) override;
    std::string getStrategy() override;

private:
    std::vector<Territories *> *weakestTerritories;
};
class NeutralPlayerStrategy : public PlayerStrategy
{
public:
    NeutralPlayerStrategy();
    NeutralPlayerStrategy(Player *);
    NeutralPlayerStrategy(const NeutralPlayerStrategy &);
    ~NeutralPlayerStrategy();
    std::vector<Command> *toAttack() override;
    std::vector<Command> *toDefend() override;
    void issueOrder(std::vector<Command> *toDefend, std::vector<Command> *toAttack) override;
    std::string getStrategy() override;
};

class CheaterPlayerStrategy : public PlayerStrategy
{
public:
    CheaterPlayerStrategy();
    CheaterPlayerStrategy(Player *);
    CheaterPlayerStrategy(const CheaterPlayerStrategy &);
    ~CheaterPlayerStrategy() override;
    std::vector<Command> *toAttack() override;
    std::vector<Command> *toDefend() override;
    void issueOrder(std::vector<Command> *toDefend, std::vector<Command> *toAttack) override;
    void winsTerritory();
    std::string getStrategy() override;
};
#endif