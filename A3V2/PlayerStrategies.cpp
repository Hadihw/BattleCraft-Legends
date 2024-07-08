#include <sstream>
#include "PlayerStrategies.h"
#include "GameEngine.h"
#include "Cards.h"

using namespace std;

PlayerStrategy::PlayerStrategy()
{
    this->player = nullptr;
}
PlayerStrategy::PlayerStrategy(Player *player)
{
    this->player = player;
}
PlayerStrategy::PlayerStrategy(const PlayerStrategy &strategy)
{
    this->player = strategy.player;
}
PlayerStrategy::~PlayerStrategy() {}
Player *PlayerStrategy::getPlayer()
{
    return this->player;
}

HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}
HumanPlayerStrategy::HumanPlayerStrategy(Player *player) : PlayerStrategy(player) {}
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy &humanStrategy) : PlayerStrategy(humanStrategy) {}
HumanPlayerStrategy::~HumanPlayerStrategy() {}
vector<Command> *HumanPlayerStrategy::toAttack()
{
    Hand *hand = this->player->getHand();
    cout << "Enter the list of territories/move to attack in the foramt: NumberOfArimes, FromTerritory, ToTerritory\n";
    if (this->player->getHasCapturedATerritory() && hand->size() > 0)
        cout << "Or enter \"Play Card, Card Type\" to play a card\n";
    else
        cout << "You did not capture a territory in the last round so you cannot play a card this round\n";
    cout << "Enter q to indicate that you are done adding territories.\n";
    vector<Command> *attack = new vector<Command>();
    while (true)
    {
        cout << "Enter the territories";
        if (this->player->getHasCapturedATerritory())
            cout << " or command to play a card: ";
        else
            cout << ": ";
        Command com = *GameEngine::getComProc()->getCommand();
        string s = com.getCommandString();
        if (s == "q")
            break;
        else if (s == "Play Card")
        {
            if (!this->player->getHasCapturedATerritory() || !(hand->size() > 0))
            {
                cout << "You cannot a play a card because you did not capture a territory!";
                com.saveEffect("Play a card command is Invalid!");
            }
            else
            {
                cout << "You have the following cards available:\n";
                hand->printCards();
                cout << "To play a card in the above list enter the associated info such as:\n";
                cout << "Bomb Order: destinationTerritory; Blockade Order: destinationTerritory; AirLift Order: Armies, SourceTerritory, DestinationTerritory; Negotiate Order: PlayerToNegotiateWith";
                cout << "Enter the index of the card(as an integer) to play it(in the format: index, info): ";
                Command card = *GameEngine::getComProc()->getCommand();
                int index;
                stringstream ss;
                ss << card.getCommandString();
                string holder;
                getline(ss, holder, ',');
                try
                {
                    index = stoi(holder);
                }
                catch (const exception &exp)
                {
                    std::cout << "You provided an incorrect integer\n";
                    card.saveEffect("Invalid card played.");
                    continue;
                }
                if (index < 0 && index < hand->size())
                {
                    Card::Type type = hand->getCardList().at(index)->getEnumType();
                    switch (type)
                    {
                    case Card::Type::Bomb:
                        this->player->getOrdersList()->addOrder(new BombOrder(&card, this->player));
                        break;
                    case Card::Type::Blockade:
                        this->player->getOrdersList()->addOrder(new BlockadeOrder(&card, this->player));
                        break;
                    case Card::Type::Airlift:
                        this->player->getOrdersList()->addOrder(new AirliftOrder(&card, this->player));
                        break;
                    case Card::Type::Diplomacy:
                    {
                        getline(ss, holder, ',');
                        this->player->getOrdersList()->addOrder(new NegotiateOrder(&card, GameEngine::getPlayerByName(holder), this->player));
                        break;
                    }
                    default:
                    {
                        std::cout << "Invalid card type. Not creating an order.";
                        card.saveEffect("Invalid Card type.");
                        break;
                    }
                    }
                }
                else
                {
                    std::cout << "Index is larger than the size. Invalidating order...\n";
                    card.saveEffect("Index entered larger than the size of ordersList. Order invalidated.");
                }
            }
        }
        int commas = std::count_if(s.begin(), s.end(),
                                   [](unsigned char c)
                                   { return c == ','; });
        if (commas != 2)
        {
            std::cout << "INVALID number of inputs! Input discarded!";
            com.saveEffect("Invalid to defend command!");
            continue;
        }
        attack->push_back(com);
    }
    return attack;
}

vector<Command> *HumanPlayerStrategy::toDefend()
{
    cout << "\nEnter the list of territories to defend in the format:  NumberOfArmies, TerritoryName\n";
    cout << "Enter q to indicate that you are done adding territories.\n";
    vector<Command> *deploy = new vector<Command>();
    cout << "You have " << this->player->getArmiesPool() << " to deploy this round\n";
    do
    {
        cout << "Enter the territories";
        if (this->player->getHasCapturedATerritory())
            cout << " or command to play a card: ";
        else
            cout << ": ";
        Command com = *GameEngine::getComProc()->getCommand();
        string s = com.getCommandString();
        if (s == "q")
            break;

        int commas = std::count_if(s.begin(), s.end(),
                                   [](unsigned char c)
                                   { return c == ','; });
        if (commas != 1)
        {
            cout << "INVALID number of inputs! Input discarded!\n";
            com.saveEffect("Invalid to defend command!");
            continue;
        }
        deploy->push_back(com);
    } while (deploy->size() == 0 || deploy->size() < this->player->getTerritoryList().size());

    cout << "Territories to defend recorded.\n";
    return deploy;
}

void HumanPlayerStrategy::issueOrder(vector<Command> *toDefend, vector<Command> *toAttack)
{
    if (this->player->getHasCapturedATerritory())
    {
        cout << "You captured a territory last round. Drawing a card...";
        Card *temp = GameEngine::getDeck()->draw();
        this->player->getHand()->addCard(temp);
        cout << "Card of type " << temp->getType() << " drawn";
    }
    int numOfarmies = this->player->getArmiesPool();
    this->player->printTerritories();
    cout << "Enter the list of territories that you will like to defend this round\n";
    vector<Command> *deploy = toDefend;
    cout << "Creating deploy orders based on the territories entered.\n";
    int i = 0;
    while (numOfarmies > 0)
    {
        stringstream ss;

        ss << deploy->at(i).getCommandString();

        string holder;

        getline(ss, holder, ',');
        int a = stoi(holder);
        numOfarmies -= a;
        if (numOfarmies < 0)
        {
            getline(ss, holder);
            deploy->at(i).setCommandString(to_string(a + numOfarmies) + "," + holder);
            cout << "Number of armies deployed exceeded armies available. Reduced the armies\n";
        }

        if (i < deploy->size())
        {
            this->player->getOrdersList()->addOrder(new DeployOrder(&deploy->at(i), this->player));
        }
        else
        {
            ss << deploy->at(i - 1).getCommandString();
            holder;
            getline(ss, holder, ',');
            a = stoi(holder);
            deploy->at(i).setCommandString(to_string(numOfarmies + a) + "," + holder);
            cout << "Number of armies deployed in all territories is less then the armies of available. Increased the armies.";
        }
        i++;
    }
    cout << "Enter the list of territories that you will like to attack/move armies to this round\n";
    vector<Command> *attack = toAttack;
    cout << "Creating attack/move orders based on the territories entered in the previous phase.\n";
    i = 0;
    for (auto comm : *attack)
    {
        this->player->getOrdersList()->addOrder(new AdvanceOrder(&attack->at(i), this->player));
        i++;
    }
    this->player->setGotAttacked(false);
}
string HumanPlayerStrategy::getStrategy()
{
    return "Human";
}
AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy()
{
    this->strongestTerritories = new vector<pair<Territories *, Territories *>>();
}
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) : PlayerStrategy(player)
{
    this->strongestTerritories = new vector<pair<Territories *, Territories *>>();
}
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &aggressiveStrategy) : PlayerStrategy(aggressiveStrategy.player)
{
    this->strongestTerritories = aggressiveStrategy.strongestTerritories;
}
AggressivePlayerStrategy::~AggressivePlayerStrategy()
{
    this->strongestTerritories->clear();
    delete this->strongestTerritories;
}

void AggressivePlayerStrategy::adjacentToEnemyTerritories()
{
    this->strongestTerritories->clear();
    int playerid = this->player->getPlayerId();
    for (int i = 0; i < this->player->getTerritoryList().size(); i++)
    {
        Territories *t = this->player->getTerritoryList().at(i);
        for (int j = 0; j < t->adjTerritories->size(); j++)
        {
            if (GameEngine::getMap()->getTerritoryById(t->adjTerritories->at(j))->getPlayer().getPlayerId() != playerid)
            {
                this->strongestTerritories->push_back(pair<Territories *, Territories *>(t, GameEngine::getMap()->getTerritoryById(t->adjTerritories->at(j))));
                break;
            }
        }
        if (strongestTerritories->size() == 5)
            break;
    }
    for (int i = 1; i < strongestTerritories->size(); i++)
    {
        pair t = strongestTerritories->at(i);
        int j = i;
        while (j > 0 && t.first->getArmies() > strongestTerritories->at(j - 1).first->getArmies())
        {
            pair temp = t;
            strongestTerritories->at(i) = strongestTerritories->at(j - 1);
            strongestTerritories->at(j - 1) = t;
            --j;
        }
    }
}

vector<Command> *AggressivePlayerStrategy::toDefend()
{
    adjacentToEnemyTerritories();
    vector<Command> *deploy = new vector<Command>();
    int armiesToDeploy = this->player->getArmiesPool() / this->strongestTerritories->size();
    for (int i = 0; i < this->strongestTerritories->size(); i++)
    {
        deploy->push_back(*(new Command(to_string(armiesToDeploy) + "," + strongestTerritories->at(i).first->getName())));
    }
    return deploy;
}
vector<Command> *AggressivePlayerStrategy::toAttack()
{
    vector<Command> *attack = new vector<Command>();
    int armiesToDeploy = player->getArmiesPool() / strongestTerritories->size();
    for (int i = 0; i < strongestTerritories->size(); i++)
    {
        attack->push_back(*(new Command(to_string(armiesToDeploy) + "," + strongestTerritories->at(i).first->getName() + "," + strongestTerritories->at(i).second->getName())));
    }
    return attack;
}
void AggressivePlayerStrategy::issueOrder(vector<Command> *toAttack,vector<Command> *toDefend)
{
    this->adjacentToEnemyTerritories();
    if (player->getHand()->cardExists("Bomb"))
        player->getOrdersList()->addOrder(new BombOrder(new Command(strongestTerritories->at(0).second->getName()), player));

    vector<Command> *deploy = toDefend;
    for (int i = 0; i < deploy->size(); i++)
    {
        player->getOrdersList()->addOrder(new DeployOrder(&deploy->at(i), this->player));
    }
    vector<Command> *attack = toAttack;
    for (int i = 0; i < attack->size(); i++)
    {
        player->getOrdersList()->addOrder(new DeployOrder(&attack->at(i), this->player));
    }
}

string AggressivePlayerStrategy::getStrategy()
{
    return "Aggressive";
}
BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy()
{
    weakestTerritories = new vector<Territories *>();
}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player, vector<Territories *> *weakest) : PlayerStrategy(player)
{
    weakestTerritories = weakest;
}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &strategy) : PlayerStrategy(strategy)
{
    weakestTerritories = strategy.weakestTerritories;
}
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

vector<Command> *BenevolentPlayerStrategy::toDefend()
{
    vector<Territories *> *t = new vector<Territories *>(player->getTerritoryList());
    // sort in increasing order
    for (int i = 1; i < t->size(); i++)
    {
        Territories *cur = t->at(i);
        int j = i;
        while (j > 0 && cur->getArmies() < t->at(j - 1)->getArmies())
        {
            Territories *temp = cur;
            t->at(i) = t->at(j - 1);
            t->at(j - 1) = cur;
            --j;
        }
    }
    delete weakestTerritories;
    weakestTerritories = t;
    int numTerrToDeployTo = __min(5, t->size());
    int armiesToDeploy = player->getArmiesPool() / numTerrToDeployTo;
    vector<Command> *deploy = new vector<Command>();
    for (int i = 0; i < numTerrToDeployTo; i++)
    {
        deploy->push_back(*(new Command(to_string(armiesToDeploy) + "," + t->at(i)->getName())));
    }
    return deploy;
}
// No need for an attack method as this strategy does not issue attack orders.
vector<Command> *BenevolentPlayerStrategy::toAttack()
{
    // returns a local vector to save memory.
    vector<Command> attack = vector<Command>();
    return &attack;
}

void BenevolentPlayerStrategy::issueOrder(vector<Command> *toAttack, vector<Command> *toDefend)
{
    vector<Command> *defend = toDefend;
    if (player->getHand()->cardExists("Airlift") && weakestTerritories->size() > 1)
        player->getOrdersList()->addOrder(new AirliftOrder(new Command(to_string(weakestTerritories->back()->getArmies() / 2) + "," + weakestTerritories->back()->getName() + "," + weakestTerritories->at(0)->getName()), player));
    for (int i = 0; i < defend->size(); i++)
    {
        player->getOrdersList()->addOrder(new DeployOrder(&defend->at(i), player));
    }
}
string BenevolentPlayerStrategy::getStrategy()
{
    return "Benevolent";
}

NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy() {}

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) : PlayerStrategy(player) {}

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &neutralStrategy) : PlayerStrategy(neutralStrategy) {}

NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

vector<Command> *NeutralPlayerStrategy::toAttack()
{
    vector<Command> defend = vector<Command>();
    return &defend;
}

vector<Command> *NeutralPlayerStrategy::toDefend()
{
    vector<Command> attack = vector<Command>();
    return &attack;
}

void NeutralPlayerStrategy::issueOrder(vector<Command> *toDefend, vector<Command> *toAttack)
{
    if (player->getGotAttacked())
    {
        player->setGotAttacked(false);
        player->setPlayerStrategy(new AggressivePlayerStrategy(player));
        delete this;
    }
}

string NeutralPlayerStrategy::getStrategy()
{
    return "Neutral";
}

CheaterPlayerStrategy::CheaterPlayerStrategy() : PlayerStrategy() {}
CheaterPlayerStrategy::CheaterPlayerStrategy(Player *player) : PlayerStrategy(player) {}
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy &strategy) : PlayerStrategy(strategy.player) {}
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}
vector<Command> *CheaterPlayerStrategy::toAttack()
{
    vector<Command> attack = vector<Command>();
    return &attack;
}
vector<Command> *CheaterPlayerStrategy::toDefend()
{
    vector<Command> defend = vector<Command>();
    return &defend;
}
void CheaterPlayerStrategy::issueOrder(vector<Command> *toDefend, vector<Command> *toAttack)
{
    winsTerritory();
}
void CheaterPlayerStrategy::winsTerritory()
{
    Territories *t = nullptr;
    for (int i = 0; i < this->player->getTerritoryList().size(); i++)
    {
        Territories *t = this->player->getTerritoryList().at(i);
        for (int j = 0; j < t->adjTerritories->size(); j++)
        {
            if (GameEngine::getMap()->getTerritoryById(t->adjTerritories->at(j))->getPlayer().getPlayerId() != player->getPlayerId())
            {
                t = GameEngine::getMap()->getTerritoryById(t->adjTerritories->at(j));
                t->setPlayer(player);
                cout << "Cheater Player conquered " + t->getName() + "\n";
                break;
            }
        }
        if (t != nullptr)
            break;
    }
}
string CheaterPlayerStrategy::getStrategy()
{
    return "Cheater";
}
