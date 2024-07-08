#include <sstream>
#include <filesystem>
#include "CommandProcessing.h"
#include "GameEngine.h"

// Command class

Command::Command(const std::string &str_cmd) : commandText(new std::string(str_cmd)), effect(nullptr), valid(nullptr) {}

void Command::saveEffect(const std::string &eff)
{
    if (!effect)
    {
        effect = new std::string(eff);
    }
}
void Command::setCommandString(string commandString)
{
    this->commandText = new string(commandString);
}

std::string Command::getCommandString()
{
    return *commandText;
}

std::string Command::getCommandEffect()
{
    return *effect;
}

//Used to return processed information about mode
Mode* Command::getMode()
{
    return this->mode;
}

bool Command::isValid()
{
    return *(this->valid);
}

// Command Processor class

CommandProcessor::CommandProcessor() {}

CommandProcessor::~CommandProcessor()
{
    for (Command *cmd : commands)
    {
        delete cmd;
    }
    commands.clear();
}

Command *CommandProcessor::readCommand()
{

    std::string input;             // string that stores the commandText
    std::getline(std::cin, input); // User input extraction

    return new Command(input);
}

void CommandProcessor::saveCommand(Command *command)
{
    commands.push_back(command);
}

bool CommandProcessor::validate(Command *command)
{
    stringstream ss;

    std::string inputCommand;

    ss << (command->getCommandString());

    getline(ss, inputCommand, ' ');

    switch (GameEngine::getGameState())
    {
    case GameEngine::GameState::START:
    {
        if (inputCommand == "loadmap")
        {
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing map field");
                command->valid = new bool(false);
                return false;
            }
            getline(ss, inputCommand, ' ');

            if (ss.eof() != true)
            {
                command->saveEffect("Invalid_command: Too many fields");
                command->valid = new bool(false);
                return false;
            }
            // Instantiate a path
            std::filesystem::path path{"./MapFiles/" + inputCommand + ".map"};
            // Check if path exists
            if (!std::filesystem::exists(path))
            {
                command->saveEffect("Invalid_command: File does not exist in MapFiles");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("loadmap " + inputCommand);
            command->valid = new bool(true);
            return true;
        }

        else if (inputCommand == "tournament")
        {
            //Initialize variables that are going to hold information about the tournament mode
            string* maps = new string[SIZE1]; //is going to store (1-5) different maps
            string* computerPlayerStrats = new string[SIZE2]; //is going to store (2-4) different computer player strategies
            int* numberOfGames = new int(-1); // is going to store the number of games to be played per map (1-5)
            int* numberOfTurns = new int(-1); // is going to store the number of turns before a game draws (10-50)


            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing -M, -P, -G, -D options");
                command->valid = new bool(false);
                return false;
            }

            //Extract -M option token
            getline(ss, inputCommand, ' ');

            //Validate token
            if(inputCommand != "-M")
            {
                command->saveEffect("Invalid_command: Invalid -M option");
                command->valid = new bool(false);
                return false;
            }

            //Check for end of file
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing -M option input");
                command->valid = new bool(false);
                return false;
            }

            //Extract -M option input token
            getline(ss, inputCommand, ' ');

            //Count how many fields the user has entered : fields = nbOfCommas + 1
            int fields = 1;
            for(int i = 0; i < inputCommand.size(); i++)
            {
                if(inputCommand[i] == ',')
                fields++;
            }

            //If the number of fields is greater than 5 invalidate the command
            if(fields > 5)
            {
                command->saveEffect("Invalid_command: Map fields greater than 5 in -M option input");
                command->valid = new bool(false);
                return false;
            }

            //Store fields if field is present in the input
            stringstream tokens(inputCommand);
            int startIndex = 0;
            int foundIndex;
            string token;

            for(int i = 0; i < SIZE1; i++)
            {
                //Get the index of the i + 1 th comma character
                foundIndex = inputCommand.find(',', startIndex);

                //Check if the ith map field is missing
                if (foundIndex == startIndex)
                {
                    command->saveEffect("Invalid_command: " + to_string(i + 1) + ((i == 0)? "st": (i == 1)? "nd" : (i == 2)? "rd": "th") + " map field is missing in -M option input");
                    command->valid = new bool(false);
                    return false;
                }

                //Move starting index past the index that was found
                startIndex = foundIndex + 1;

                //Extract the ith map field
                getline(tokens, token, ',');

                //Set the corresponding element in the array of maps to the map field
                maps[i] = token;

                //Update the number of fields left to extract
                fields--;

                //Stop extracting if there are no more fields left to extract
                if(fields == 0){
                    break;
                }
            }

            //Check for end of file
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing -P, -G, -D options");
                command->valid = new bool(false);
                return false;
            }

            //Extract -P option token
            getline(ss, inputCommand, ' ');

            if(inputCommand != "-P")
            {
                command->saveEffect("Invalid_command: Invalid -P option");
                command->valid = new bool(false);
                return false;
            }

            //Check for end of file
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing -P option input");
                command->valid = new bool(false);
                return false;
            }

            //Extract -P option input token
            getline(ss, inputCommand, ' ');

            //Reset field variable to count the number of fields in the player strategy input
            fields = 1;

            //Count how many fields the user has entered : fields = nbOfCommas + 1
            for(int i = 0; i < inputCommand.size(); i++)
            {
                if(inputCommand[i] == ',')
                fields++;
            }

            //If the number of fields is greater than 5 invalidate the command
            if(fields > 4)
            {
                command->saveEffect("Invalid_command: Player strategy fields greater than 4 in -P option input");
                command->valid = new bool(false);
                return false;
            }
            //If the number of fields is less than 2 invalidate the command
            else if(fields < 2)
            {
                command->saveEffect("Invalid_command: Player strategy fields less than 2 in -P option input");
                command->valid = new bool(false);
                return false;
            }

            //Reset start index
            startIndex = 0;

            //Instantiate a new stringstream object (bc the state of the old one is moved past the first index)
            stringstream tokens2(inputCommand);
            
            //Store player strategy if strategy field is present in input
            for(int i = 0; i < SIZE2; i++)
            {
                //Get the index of the i + 1 th comma character
                foundIndex = inputCommand.find(',', startIndex);

                //Check if the ith strategy field is missing
                if (foundIndex == startIndex)
                {
                    command->saveEffect("Invalid_command: " + to_string(i + 1) + ((i == 0)? "st": (i == 1)? "nd" : (i == 2)? "rd": "th") + " player strategy field is missing in -P option input");
                    command->valid = new bool(false);
                    return false;
                }

                //Move starting index past the index that was found
                startIndex = foundIndex + 1;

                //Extract the ith strategy field
                getline(tokens2, token, ',');

                //Set the corresponding element in the array of computer player strategies to the strategy field
                computerPlayerStrats[i] = token;

                //Update the number of fields left to extract
                fields--;

                //Stop extracting if there are no more fields left to extract
                if(fields == 0){
                    break;
                }
            }

            //Check for end of file
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing -G, -D options");
                command->valid = new bool(false);
                return false;
            }

             //Extract -G option token
            getline(ss, inputCommand, ' ');

            if(inputCommand != "-G")
            {
                command->saveEffect("Invalid_command: Invalid -G option");
                command->valid = new bool(false);
                return false;
            }

            //Extract number of games to be played
            getline(ss, inputCommand, ' ');

            //Check if length of the string is not equal to 1 (only digits are allowed)
            if(inputCommand.size() != 1)
            {
                command->saveEffect("Invalid_command: String length != 1 in -G option input");
                command->valid = new bool(false);
                return false;
            }
            
            //Attempt string conversion to integer and assignment
            try
            {
                *numberOfGames = stoi(inputCommand);
            }
            catch(invalid_argument iA)
            {
                command->saveEffect("Invalid_command: Not a number in -G option input");
                command->valid = new bool(false);
                return false;
            }

            //Check for end of file
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing -D option");
                command->valid = new bool(false);
                return false;
            }

             //Extract -D option token
            getline(ss, inputCommand, ' ');

            if(inputCommand != "-D")
            {
                command->saveEffect("Invalid_command: Invalid -D option");
                command->valid = new bool(false);
                return false;
            }

            //Extract the max number of turns
            getline(ss, inputCommand, ' ');

            //Check if length of the string is not equal to 2 (numbers from 10 to 50 are allowed)
            if(inputCommand.size() != 2)
            {
                command->saveEffect("Invalid_command: String length != 2 in -G option input");
                command->valid = new bool(false);
                return false;
            }
            
            //Attempt string conversion to integer and assignment
            try
            {
                *numberOfTurns = stoi(inputCommand);
            }
            catch(invalid_argument iA)
            {
                command->saveEffect("Invalid_command: Not a number in -D option input");
                command->valid = new bool(false);
                return false;
            }

            //Check for end of file
            if (ss.eof() != true)
            {
                command->saveEffect("Invalid_command: Too many arguments in tournament command");
                command->valid = new bool(false);
                return false;
            }

            //Validate the semantics of the extracted data

            //Maps
            for(int i = 0; i < SIZE1; i++)
            {
                //Idk if this is how you are supposed to check for empty fields in maps
                if(maps[i] != "")
                {
                    // Instantiate a path
                    std::filesystem::path path{"./MapFiles/" + maps[i] + ".map"};
                    // Check if path does not exist
                    if (!std::filesystem::exists(path))
                    {
                        command->saveEffect("Invalid_command: File '" + maps[i] + ".map' does not exist in MapFiles");
                        command->valid = new bool(false);
                        return false;
                    }
                    //Check if map already exists at a previous map index
                    if(i > 0)
                    {
                        for(int j = 0; j < i; j++)
                        {
                            if(maps[i] == maps[j])
                            {
                                command->saveEffect("Invalid_command: File '" + maps[i] + ".map' is duplicate in -M option input");
                                command->valid = new bool(false);
                                return false;
                            }
                        }
                    }
                }
            }

            //Computer player strategies
            for(int i = 0; i < SIZE2; i++)
            {   
                if(computerPlayerStrats[i] != "")
                {
                    if(computerPlayerStrats[i] != "Aggressive" && computerPlayerStrats[i] != "Benevolent" && computerPlayerStrats[i] != "Neutral" && computerPlayerStrats[i] != "Cheater")
                    {
                        command->saveEffect("Invalid_command: Player strategy '" + computerPlayerStrats[i] + "' is not recognized in -P option input");
                        command->valid = new bool(false);
                        return false;
                    }

                    //Check if player strategy already exists at a previous player strategy index
                    if(i > 0)
                    {
                        for(int j = 0; j < i; j++)
                        {
                            if(computerPlayerStrats[i] == computerPlayerStrats[j])
                            {
                                command->saveEffect("Invalid_command: Player strategy '" + computerPlayerStrats[i] + "' is duplicate in -P option input");
                                command->valid = new bool(false);
                                return false;
                            }
                        }
                    }
                }
            }

            //Number of games
            if(*numberOfGames > 5 || *numberOfGames < 1)
            {
                command->saveEffect("Invalid_command: Number of games '" + to_string(*numberOfGames) + "' is not within the accepted range in -G option input");
                command->valid = new bool(false);
                return false;
            }

            //Number of max turns
            if(*numberOfTurns > 50 || *numberOfTurns < 10)
            {
                command->saveEffect("Invalid_command: Number of turns '" + to_string(*numberOfTurns) + "' is not within the accepted range in -D option input");
                command->valid = new bool(false);
                return false;
            }

            command->saveEffect(command->getCommandString());

            command->mode = new TournamentMode(maps,computerPlayerStrats, numberOfGames,numberOfTurns);

            command->valid = new bool(true);
            return true;
        }
        
        command->saveEffect("Invalid_command: Unrecognized command specifier");
        command->valid = new bool(false);
        return false;
    }
    case GameEngine::GameState::MAP_LOADED:
    {
        if (inputCommand == "loadmap")
        {
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing map field");
                command->valid = new bool(false);
                return false;
            }
            getline(ss, inputCommand, ' ');

            if (ss.eof() != true)
            {
                command->saveEffect("Invalid_command: Too many input fields");
                command->valid = new bool(false);
                return false;
            }
            // Instantiate a path
            std::filesystem::path path{"./MapFiles/" + inputCommand + ".map"};
            // Check if path exists
            if (!std::filesystem::exists(path))
            {
                command->saveEffect("Invalid_command: File does not exist in MapFiles");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("loadmap " + inputCommand);
            command->valid = new bool(true);
            return true;
        }
        else if (inputCommand == "validatemap")
        {
            if (ss.eof() != true)
            {
                command->saveEffect("Invalid_command: Too many input fields");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("validatemap");
            command->valid = new bool(true);
            return true;
        }
        command->saveEffect("Invalid_command: Unrecognized command specifier");
        command->valid = new bool(false);
        return false;
    }
    case GameEngine::GameState::MAP_VALIDATED:
    {
        if (inputCommand == "addplayer")
        {
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing player input field");
                command->valid = new bool(false);
                return false;
            }
            getline(ss, inputCommand, ' ');

            if (ss.eof() != true)
            {
                command->saveEffect("Invalid_command: Too many input fields");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("addplayer " + inputCommand);
            command->valid = new bool(true);
            return true;
        }
        command->saveEffect("Invalid_command: Unrecognized command specifier");
        command->valid = new bool(false);
        return false;
    }
    case GameEngine::GameState::PLAYERS_ADDED:
    {
        if (inputCommand == "addplayer")
        {
            if (ss.eof() == true)
            {
                command->saveEffect("Invalid_command: Missing player input field");
                command->valid = new bool(false);
                return false;
            }
            getline(ss, inputCommand, ' ');

            if (ss.eof() != true)
            {
                command->saveEffect("Invalid_command: Too many input fields");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("addplayer " + inputCommand);
            command->valid = new bool(true);
            return true;
        }
        else if (inputCommand == "gamestart")
        {
            if (ss.eof() != true)
            {
                command->saveEffect("Invalid command: Too many input fields");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("assigncountries");
            command->valid = new bool(true);
            return true;
        }
        command->saveEffect("Invalid command: Unrecognized command specifier");
        command->valid = new bool(false);
        return false;
    }
    case GameEngine::GameState::ASSIGN_REINFORCEMENT:
        return true;
        break;
    case GameEngine::GameState::ISSUE_ORDERS:
        return true;
        break;
    case GameEngine::GameState::EXECUTE_ORDERS:
        return true;
        break;
    case GameEngine::GameState::WIN:
    {
        if (inputCommand == "quit")
        {
            if (ss.eof() != true)
            {
                command->saveEffect("Invalid command: Too many input fields");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("end");
            command->valid = new bool(true);
            return true;
        }
        else if (inputCommand == "replay")
        {
            if (ss.eof() != true)
            {
                command->saveEffect("Invalid command: Too many input fields");
                command->valid = new bool(false);
                return false;
            }
            command->saveEffect("play");
            command->valid = new bool(true);
            return true;
        }
        command->saveEffect("Invalid command: Unrecognized command specifier");
        command->valid = new bool(false);
        return false;
    }
    }
    std::cout << "Invalid game state, exiting program...";
    exit(1);
    return false;
}

Command *CommandProcessor::getCommand()
{
    Command *cmd = readCommand();
    saveCommand(cmd);
    validate(cmd);
    return cmd;
}

// FileCommandProcessorAdapter class
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string &filename)
{
    this->file = new ifstream();
    file->open(filename);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{
    if (file->is_open())
    {
        file->close();
    }
    delete file;
}

Command *FileCommandProcessorAdapter::readCommand()
{
    std::string input;
    getline(*file, input);

    return new Command(input);
}
