#ifndef CARDS_H
#define CARDS_H

#include <vector>
#include <string>
#include <iostream>

// Forward declaration
class Card;
class Deck;
class Hand;

// Card class declaration
class Card
{
public:
    enum class Type
    {
        Bomb = 0,
        Reinforcement,
        Blockade,
        Airlift,
        Diplomacy
    };
    Card(Type type);
    Card(const Card &other);
    Card &operator=(const Card &other);
    ~Card(); // Destructor
    void play();
    std::string getType() const;
    Type getEnumType();
    friend std::ostream &operator<<(std::ostream &os, const Card &card);

private:
    Type *type;
};

// Deck class declaration
class Deck
{
public:
    Deck(int nCards);
    Deck(const Deck &other); // Copy constructor
    Deck &operator=(const Deck &other);
    ~Deck(); // Destructor
    int size() const;
    Card *draw();
    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);

private:
    std::vector<Card *> *cards;
};

// Hand class declaration
class Hand
{
public:
    Hand();
    Hand(const Hand &other);            // Copy constructor
    Hand &operator=(const Hand &other); // Assignment operator
    ~Hand();                            // Destructor
    void addCard(Card *card);
    std::vector<Card *> getCardList();
    void printCards();
    void playAll();
    bool cardExists(std::string);
    int size() const;
    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);

private:
    std::vector<Card *> *cards;
};

// function to test cards
void testCards();
#endif