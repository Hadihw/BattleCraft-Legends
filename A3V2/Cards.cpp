#include "Cards.h"
#include <random>
#include <iostream>

// Card class

// Constructor to initialize card type
Card::Card(Type type) : type(new Type(type)) {}

// Copy constructor
Card::Card(const Card &other) : type(new Type(*other.type)) {}

// Assignment operator
Card &Card::operator=(const Card &other)
{
    if (this == &other)
        return *this;             // Self-assignment guard
    delete type;                  // Delete the existing type
    type = new Type(*other.type); // Copy the type from the other object
    return *this;
}

// Destructor to free the dynamically allocated type
Card::~Card()
{
    delete type;
}

// Function to play the card
void Card::play()
{
    std::string cardType;
    // Convert card type enum to string for display
    switch (*type)
    {
    case Type::Bomb:
        cardType = "Bomb";
        break;
    case Type::Reinforcement:
        cardType = "Reinforcement";
        break;
    case Type::Blockade:
        cardType = "Blockade";
        break;
    case Type::Airlift:
        cardType = "Airlift";
        break;
    case Type::Diplomacy:
        cardType = "Diplomacy";
        break;
    default:
        cardType = "Unknown";
        break;
    }
    std::cout << "Playing card: " << cardType << std::endl;
}
Card::Type Card::getEnumType()
{
    return *type;
}
// Getter for card type
std::string Card::getType() const
{
    std::string cardType;
    // Convert card type enum to string for display
    switch (*type)
    {
    case Type::Bomb:
        cardType = "Bomb";
        break;
    case Type::Reinforcement:
        cardType = "Reinforcement";
        break;
    case Type::Blockade:
        cardType = "Blockade";
        break;
    case Type::Airlift:
        cardType = "Airlift";
        break;
    case Type::Diplomacy:
        cardType = "Diplomacy";
        break;
    default:
        cardType = "Unknown";
        break;
    }
    return cardType;
}

// Stream insertion operator to display card info
std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << "Card Type: " << static_cast<int>(*card.type);
    return os;
}

// Implementations for Deck class

// Constructor to initialize deck with number of cards
Deck::Deck(int nCards) : cards(new std::vector<Card *>())
{
    for (int i = 0; i < nCards; ++i)
    {
        cards->push_back(new Card(static_cast<Card::Type>(i % 5)));
    }
}

// Copy constructor
Deck::Deck(const Deck &other) : cards(new std::vector<Card *>(*other.cards)) {}

// Assignment operator
Deck &Deck::operator=(const Deck &other)
{
    if (this == &other)
        return *this;                              // Self-assignment guard
    delete cards;                                  // Delete existing cards
    cards = new std::vector<Card *>(*other.cards); // Copy cards from the other object
    return *this;
}

// Destructor to free the dynamically allocated cards
Deck::~Deck()
{
    for (auto card : *cards)
    {
        delete card;
    }
    delete cards;
}

// Function to draw a card from the deck
Card *Deck::draw()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, cards->size() - 1);
    int index = distrib(gen);
    Card *drawnCard = cards->at(index);
    delete cards->at(index);
    cards->erase(cards->begin() + index);
    return drawnCard;
}

// Function to get the size of the deck
int Deck::size() const
{
    return cards->size();
}

// Display deck info
std::ostream &operator<<(std::ostream &os, const Deck &deck)
{
    os << "Deck size: " << deck.cards->size();
    return os;
}

// Implementations for Hand class
Hand::Hand() : cards(new std::vector<Card *>()) {}

Hand::Hand(const Hand &other) : cards(new std::vector<Card *>(*other.cards)) {}

Hand &Hand::operator=(const Hand &other)
{
    if (this == &other)
        return *this;
    delete cards;
    cards = new std::vector<Card *>(*other.cards);
    return *this;
}

Hand::~Hand()
{
    for (auto card : *cards)
    {
        delete card;
    }
    delete cards;
}

void Hand::addCard(Card *card)
{
    cards->push_back(card);
}
std::vector<Card *> Hand::getCardList()
{
    return *this->cards;
}
void Hand::printCards()
{
    for (int i = 0; i < cards->size(); i++)
    {
        std::cout << i << ". " << cards->at(i)->getType();
        if (i < cards->size() - 1)
            std::cout << ", ";
    }
    std::cout << "\n";
}
void Hand::playAll()
{
    for (auto card : *cards)
    {
        card->play();
        delete card;
    }
    cards->clear();
}

int Hand::size() const
{
    return cards->size();
}
bool Hand::cardExists(std::string cardType)
{
    for (int i = 0; i < this->cards->size(); i++)
    {
        if (cardType == cards->at(i)->getType())
            return true;
    }
    return false;
}
std::ostream &operator<<(std::ostream &os, const Hand &hand)
{
    os << "Hand size: " << hand.cards->size();
    return os;
}

// Free function to test cards
void testCards()
{
    Deck deck(30); // Create a deck with 30 cards
    Hand hand;     // Create an empty hand

    // Display initial sizes
    std::cout << "Initial " << deck << std::endl;
    std::cout << "Initial hand size: " << hand.size() << std::endl
              << std::endl;

    std::cout << "Drawing 10 Cards..." << std::endl
              << std::endl;
    // Draw 10 cards
    for (int i = 0; i < 10; ++i)
    {
        hand.addCard(deck.draw());
    }

    // Display sizes after playing all cards
    std::cout << "Remaining " << deck << std::endl;
    std::cout << "Remaining hand size: " << hand.size() << std::endl
              << std::endl;

    std::cout << "Playing All cards:" << std::endl
              << std::endl;
    // Play all cards in the hand
    hand.playAll();
}
