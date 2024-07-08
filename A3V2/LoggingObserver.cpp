#include <iostream>
#include "LoggingObserver.h"

//destructor for Subject. Does not delete observers as ownership is not assumed
Subject::~Subject() {
    observers.clear();
}

//attaches an observer to the subject.
void Subject::attach(Observer* observer) {
    observers.push_back(observer);
}

//detaches an observer from the subject
void Subject::detach(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

//notifies all attached observers of a change
void Subject::notify() const {
    for (Observer* observer : observers) {
        if (observer) {
            observer->update(this);
        }
    }
}

//constructor for LogObserver
LogObserver::LogObserver(const std::string& filename) {
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile) {
            throw std::runtime_error("Unable to open log file: " + filename);
        }

}

//destructor for LogObserver
LogObserver::~LogObserver() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

//updates the log file with the loggable information from the subject
void LogObserver::update(const Subject* subject) {
    const ILoggable* loggable = dynamic_cast<const ILoggable*>(subject);
    if (loggable && logFile.is_open()) {
        logFile << loggable->stringToLog() << std::endl;
        if (!logFile) {
            std::cerr << "Failed to write to log file." << std::endl;
        }
    }
}