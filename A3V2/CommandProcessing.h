#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H

#include "GameEngine.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class Command;

class Mode;

class CommandProcessor
{
public:
    // Constructors/Destructor
    CommandProcessor();
    // CommandProcessor(GameEngine* eng);

    CommandProcessor(const CommandProcessor &other);

    ~CommandProcessor();

    /// @brief Calls readCommand and saveCommand to process the user's command
    /// @return The processed command object
    Command *getCommand();

    void saveCommand(Command *command);

    bool validate(Command *command);

    // Operators
    // Assignment
    const CommandProcessor &operator=(const CommandProcessor &other);
    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const CommandProcessor &cp);
;

private:
    virtual Command *readCommand();
    std::vector<Command *> commands;
;
};

class FileCommandProcessorAdapter
{
public:
    FileCommandProcessorAdapter(const std::string &filename);
    ~FileCommandProcessorAdapter();
;

private:
    Command *readCommand();
    std::ifstream *file;
;
};

class Command
{
    friend CommandProcessor;

public:
    // Constructors/Destructor
    Command(const std::string &str_cmd);

    // Methods
    void saveEffect(const std::string &eff);
    void setCommandString(std::string commandString);
    // Getters
    std::string getCommandString();

    std::string getCommandEffect();

    Mode* getMode();

    bool isValid();
;

private:
    std::string *commandText;
    std::string *effect;
    bool *valid;

    Mode* mode;
;
};

#endif