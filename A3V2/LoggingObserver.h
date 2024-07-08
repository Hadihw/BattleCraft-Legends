#ifndef LOGGING_OBSERVER_H
#define LOGGING_OBSERVER_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

// ILoggable: Interface for objects that can be logged

class ILoggable
{
public:
    virtual ~ILoggable() = default;
    virtual std::string stringToLog() const = 0;
};

class Observer;

class Subject
{
private:
    std::vector<Observer *> observers;

public:
    Subject() = default;
    virtual ~Subject(); // does not delete observers

    void attach(Observer *observer);
    void detach(Observer *observer);
    void notify() const;
};

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void update(const Subject *subject) = 0;
};

class LogObserver : public Observer
{
private:
    std::ofstream logFile;

public:
    explicit LogObserver(const std::string &filename);
    ~LogObserver();
    void update(const Subject *subject) override;
};

#endif // LOGGING_OBSERVER_H