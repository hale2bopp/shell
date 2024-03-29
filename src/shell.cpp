/* \file shell.cpp 
 * \brief defines a simple shell
 *
 */
#include <iostream>
#include <fstream>
#include "shell.h"
#include "shellDriverInterface.h"
#include "fileDef.h"
#include "shellSignal.h"
#include "redirection.h"
#include <termios.h>
#include <string.h>
#include <vector>
#include <algorithm>

string prompt = "penn-shredder# ";
static struct termios oldt, newt;


std::unique_ptr<Shell> createShellWithDriver(const std::string& mainPrompt,  ShellDriverInterface &shellDriverIntf){
    // Use std::make_unique to create a unique_ptr managing a Shell instance.
    auto shellPtr = std::make_unique<Shell>(mainPrompt, CMD_HISTORY_SIZE, shellDriverIntf);
    return shellPtr;
}
/*
std::unique_ptr<Shell> createShell(const std::string& mainPrompt) {
    // Assuming ShellDriver can be default constructed or however it needs to be initialized.
    ShellDriverInterface shellDriverIntf;
    // Use std::make_unique to create a unique_ptr managing a Shell instance.
    auto shellPtr = std::make_unique<Shell>(mainPrompt, CMD_HISTORY_SIZE, shellDriverIntf);
    return shellPtr;
}
*/
/*
   Shell createShell(const string& mainPrompt){
   ShellDriver shellDriverIntf;
   Shell shell(mainPrompt, CMD_HISTORY_SIZE, shellDriverIntf);
   return &shell;
   }
 */

void shellRunWrapper(Shell& shell){
    shell.shellRun();
}

void Shell::shellRun(){
    registerSignals();
    PutTerminalInPerCharMode();

    while (1) {
        DisplayPrompt(cout);
        string shellInput = GetInput(cin, cout);
        vector<string> tokens = Tokenise(shellInput, ' ');
        GetCommandHistory()->MainWrapperAddCmdToHistory(shellInput);
        fflush(stdout);
        Command command;
        PipesErr pipesErr = ParsePipes(tokens, command);
        if (pipesErr!=PipesErrNone){
            perror("error in parsing pipes");
            continue;
        }
        pipesErr = HandlePipes(command);
        if (pipesErr!=PipesErrNone){
            perror("error in handling pipes");
            continue;
        } 
    }
    PutTerminalBackInNormalMode();
}

CommandHistory::CommandHistory(int maxCmdHistorySize): maxCmdHistorySize(maxCmdHistorySize){
}


/**
 * \brief Adds every entered command to history of commands, wrapper
 * for main 
 * @param vector of strings describing a command to enter 
 */
void CommandHistory::MainWrapperAddCmdToHistory(const string &cmd){
    if (cmd.size() !=0){
        AddCmdToHistory(cmd, cmdHistoryList);
        currentHistoryIndex = cmdHistoryList.size();
        savedCurrentInput = "";
    }
}

/**
 * \brief Small helper to display prompt
 */
void Shell::DisplayPrompt(ostream& ofs){
    ofs << prompt;
}

/**
 * \brief Adds every entered command to history of commands.
 * Works as a FIFO, if number of commands exceeds a maximum
 * defined value, first value entered is discarded.
 * @param vector of strings describing a command to enter 
 * @param vector of vector of strings with current command history
 */
void CommandHistory::AddCmdToHistory(const string &cmd, deque<string> &cmdList){
    if (cmdList.size() >= maxCmdHistorySize){
        cmdList.pop_front();
    } 
    cmdList.push_back(cmd);
}

/**
 * \brief Getter for current history index
 */
int CommandHistory::GetCurrentHistoryIndex(void){
    return currentHistoryIndex;
}

/**
 * \brief Getter for currently saved input string
 */
string CommandHistory::GetSavedCurrentInput(void){
    return savedCurrentInput;
}

/**
 * \brief Save currently entered string
 */
void CommandHistory::SaveCurrentEnteredString(const string& s){
    if ((SetSavedCurrentInputFlag) && (s.size()>0)){
        SetSavedCurrentInput(s);
        SetSavedCurrentInputFlag = false;
    }
}

/**
 * \brief Sets index in the command history vector
 */
void CommandHistory::SetCurrentHistoryIndex(const int& val){
    currentHistoryIndex = val;
}

/**
 * \brief Saves current input string before being replaced by up arrow
 */
void CommandHistory::SetSavedCurrentInput(const string& s){
    savedCurrentInput = s;
}

/**
 * \brief Handles up arrow press
 */
string Shell::handleUpArrow(const string& s, ostream& ofs){
    auto cmdHistory = GetCommandHistory();
    cmdHistory->SaveCurrentEnteredString(s);
    int chi = cmdHistory->GetCurrentHistoryIndex();
    if (chi > 0){
        chi--;        
        cmdHistory->SetCurrentHistoryIndex(chi);
    }
    return replaceInput(ofs);
}

/**
 * \brief Handles down arrow press
 */
string Shell::handleDownArrow(const string& s, ostream& ofs){
    auto cmdHistory = GetCommandHistory();
    cmdHistory->SaveCurrentEnteredString(s);
    int chi = cmdHistory->GetCurrentHistoryIndex();
    if (chi < cmdHistory->cmdHistoryList.size()){
        chi++;
        cmdHistory->SetCurrentHistoryIndex(chi);
    }
    return replaceInput(ofs);
}

/**
 * \brief Ensures that entered command is not larger than a max
 * input value. Truncates input string if larger.
 * @param input string 
 */
void Shell::CheckLength(string &shellInput){
    if (shellInput.size() > maxInputLength){
        shellInput = shellInput.substr(0, maxInputLength);
    }
}

/**
 * \brief Handles up arrow press. Replaces currently typed string 
 * with the string from command history. Allows for 
 * continued input after replacing currently typed string
 * with command from history.
 * @param vector of vector of strings (Command History)
 */
string Shell::replaceInput(ostream& ofs){
    auto cmdHistory = GetCommandHistory();

    int chi = cmdHistory->GetCurrentHistoryIndex();
    int msize = cmdHistory->GetCmdHistorySize();
    if (chi == msize || ((chi == 0) && (msize==0))){
        string s = cmdHistory->GetSavedCurrentInput();
        ofs << s;
        return s;
    }
    ofs << cmdHistory->cmdHistoryList[chi];
    return cmdHistory->cmdHistoryList[chi];
}

/**
 * \brief get Shell input. Immediately handles backspace, arrows
 * passes to tokeniser 
 */
string Shell::GetInput(istream& ifs, ostream& ofs){ 
    cursorPosition = 0;
    auto cmdHistory = GetCommandHistory();
    cmdHistory->SetSavedCurrentInputFlag = true;

    string shellInput;
    char c = 0;
    while(c!=ENTER){
        ifs.get(c);
        switch(c){
            case (char)ESCAPE_SEQ:
                // get 2 more characters
                ifs.get(c);
                ifs.get(c);
                if (c == (char)UP_ARROW){
                    moveCursorToBackDisplayPrompt(ofs);
                    shellInput = handleUpArrow(shellInput, ofs);
                    cursorPosition = shellInput.size();
                }
                if (c == (char) DOWN_ARROW){
                    moveCursorToBackDisplayPrompt(ofs);
                    shellInput = handleDownArrow(shellInput, ofs);
                    cursorPosition = shellInput.size();
                }
                if (c == (char) LEFT_ARROW){
                    cursorOneLeft(shellInput, ofs);
                }
                if (c == (char) RIGHT_ARROW){
                    cursorOneRight(shellInput, ofs);
                } 
                break;
            case (char) DELETE: 
                eraseCharacter(shellInput, ofs);
                break;
            case (char)ENTER:
                break;
            default:
                insertCharacter(shellInput, c, cursorPosition, ofs);
                break;
        }
    }
    // limit length of terminal input
    CheckLength(shellInput);
    return shellInput;
}

/**
 * \brief Executes currently entered command with arguments.
 * converts vector of strings into a format understandable
 * by execvp. 
 * @param vector of strings with command to be run
 */
int Shell::ExecuteProgram(const vector<string>& cmd){
    // need to handle empty input separately 
    if (cmd.size() == 0){
        return 0;
    }
    vector<char *> vec_cp;
    vec_cp.reserve(cmd.size() + 1);
    for (auto s : cmd){
        vec_cp.push_back(strdup(s.c_str()));
    }
    vec_cp.push_back(NULL);
    return shellDriverIntf.execute(cmd[0].c_str(), const_cast<char* const*>(vec_cp.data()));
}


void Shell::tokenHelper(vector<string>& tokens, string& temp, bool& wordBoundary){
    if (!wordBoundary){
        // add to tokens
        tokens.push_back(temp);
        // empty temp string
        temp = "";
        wordBoundary = true;
    }
}

void Shell::detectDoubleChar(const char& charDetect, int& numChar, vector<string>& tokens, string& temp, bool& wordBoundaryFlag, bool& multipleChar){
    if (multipleChar){
        string str(numChar+1,charDetect);
        tokens.pop_back();
        tokens.push_back(str);
    } else {
        tokenHelper(tokens, temp, wordBoundaryFlag);
        string str(1,charDetect);
        tokens.push_back(str);
    }
    multipleChar = true;
    numChar++;
}



/**
 * \brief Tokenise entered input string from shell to extract command
 * and arguments
 * @param s input string from shell
 * @param delimiter character to split string into command and args. 
 *        in normal operation, this should be ' '.
 */
vector<string> Shell::Tokenise(const string& s, const char& delimiter){
    // ignore whitespaces 
    // end on enter 
    vector<string> tokens;
    bool wordBoundaryFlag = true;
    typedef struct _TemporaryCounts{
        int numberOfRedirect = 0;
        int numberOfPipe = 0;
        int numberOfBg = 0;
        bool multiplePipe = false;
        bool multipleRedirect = false;
        bool multipleBg = false;
    } TemporaryCounts;
    string temp;
    TemporaryCounts temporaryCounts = {0};
    for(int i = 0; i < s[i]; i++){
        switch(s[i]){
            case '>':
            case '<':
                detectDoubleChar(s[i], temporaryCounts.numberOfRedirect, tokens, temp, wordBoundaryFlag, temporaryCounts.multipleRedirect);
                break;
            case ' ':
                // if previous state was false
                // this is a transition from finding a word
                // to finding a space
                tokenHelper(tokens, temp, wordBoundaryFlag);
                temporaryCounts = {0};
                break;
            case '|':
                detectDoubleChar(s[i], temporaryCounts.numberOfPipe, tokens, temp, wordBoundaryFlag, temporaryCounts.multiplePipe);
                break;
            case '&':
                detectDoubleChar(s[i], temporaryCounts.numberOfBg, tokens, temp, wordBoundaryFlag, temporaryCounts.multipleBg);
                break;
            default: 
                temp.push_back(s[i]);
                temporaryCounts = {0};
                wordBoundaryFlag = false;
                break;
        }
    }
    if (!wordBoundaryFlag){
        // if the last word was not a space,
        // need to add the temp at the end
        tokens.push_back(temp);
    }
    return tokens;
}


/**
 * \brief Simple helper to print out a vector of strings 
 * @param input vector of strings to print
 */
void Shell::printTokens(const vector<string> &input, ostream& ofs){
    for(int i = 0; i < input.size(); i++){
        ofs << input[i];
        ofs << " ";
    }
    ofs << "\n"; 
}

/**
 * \brief Simple helper to set the end of a commnand sent to execvp
 * @param input redirection Param struct, index
 */
void Shell::setCmdEnd(RedirectionParams& redirParams, const int& index){
    if (!redirParams.foundRedirectionParam){
        redirParams.cmdEnd = index;
        redirParams.foundRedirectionParam = true;
    }
}

/**
 * \brief Separate tokens out into pipes
 * @param input vector of strings 
 * @param pipeline struct
 */
PipesErr Shell::ParsePipes(vector<string> tokens, Command& command){
    vector<string> temp;
    for (int i = 0; i < tokens.size(); i++){
        if (tokens[i] == "|"){
            if (i == tokens.size()-1){
                perror("invalid pipe");
                return PipesEndsWithPipe;
            }
            // create a new set of commands 
            // open input and output pipes
            command.pipeline.pipes.push_back(temp);
            temp.clear();
            command.pipeline.numPipes++;
        } else if (count(tokens[i].begin(), tokens[i].end(), '|') > 1) {
            return PipesDoublePipe;
        } else {
            temp.push_back(tokens[i]);
        }
    }
    command.pipeline.pipes.push_back(temp);

    int lastPipeSize = command.pipeline.pipes[command.pipeline.numPipes].size();
    if (lastPipeSize >0){
        if (command.pipeline.pipes[command.pipeline.numPipes][lastPipeSize-1] == "&"){
            command.pipeline.pipes[command.pipeline.numPipes].pop_back();
            command.SetIsBackground(true);
        }
    }

    return PipesErrNone;
}

/**
 * \brief Handle Pipes if any, and execute program
 * @param pipeline struct 
 * @param redirParams redirection parameters
 */
PipesErr Shell::HandlePipes(Command& command){
    if (command.pipeline.numPipes > 0){
        int pipefd[2*command.pipeline.numPipes];
        for (int i = 0; i < command.pipeline.numPipes; i++){
            // create a pipe using pipe(2)         
            if(pipe(pipefd+(i*2)) < 0 ) {
                perror("Pipe failed");
                return PipesExecErr;
            }
        }
        // pid of root of pipe
        pid_t rootPid;
        for (int i = 0; i < command.pipeline.numPipes+1; i++){
            pid_t cpid = shellDriverIntf.processFork();
            if (cpid < 0) {
                perror("fork error");
                return PipesExecErr;
            } else if (cpid == 0){
                // set process group to itself
                setpgrp();
                command.cpid.push_back(cpid);
                // dup2 stdin from previous pipe 
                if (i == 0){
                    // if it is the first command in the pipe, store the pid
                    rootPid = getpid();
                    setpgid(rootPid, 0);
                } else {
                    // set all further pipes to the same pgid as the root
                    setpgid(rootPid, 0);
                    if (shellDriverIntf.dupFile(pipefd[(i-1)*2], stdin)<0){
                        perror("unable to open stdin from previous pipe");
                        return PipesExecErr;
                    }
                }

                // dup2 stdout to next pipe
                if (i != command.pipeline.numPipes) {
                    if (shellDriverIntf.dupFile(pipefd[(i*2)+1], stdout)<0){
                        perror("unable to open stdout to next pipe");
                        return PipesExecErr;
                    }
                }
                for( int j = 0; j < 2*command.pipeline.numPipes; j++){
                    shellDriverIntf.fileClose(pipefd[j]);
                }
                // reset redirection params 
                command.redirParams = {0};
                PostTokeniseProcessingErr err = PostTokeniseProcessing(command.redirParams, command.pipeline.pipes[i]);
                if (err!=PostTokeniseProcessingErrNone){
                    perror("Wrong Redirection");
                    return PipesExecErr;
                }
                HandleRedirection(command.redirParams);
                auto ret = ExecuteProgram(command.redirParams.cmd);
                if (ret < 0 ){
                    perror("unable to execute");           
                }
            } else {
                // PARENT
                // set process group of child
                setpgid(cpid, cpid);
            }
        }
        // parent closes all of its copies at the end
        for( int i = 0; i < 2 * command.pipeline.numPipes; i++ ){
            shellDriverIntf.fileClose( pipefd[i] );
        }

        // waits for children
        // Check whether the last token of last pipe
        // is a background symbol. if so, do not wait        
        if (!command.GetIsBackground()){
            for(int i = command.pipeline.numPipes; i >= 0; i--){
                int retVal = 0;
                if(shellDriverIntf.pWaitpid(command.cpid[i], &retVal, 0)){
                    //Report child exited with return status 'return'
                    //Remove child (linked list style)
                    command.cpid.pop_back();
                }
            }
        } 
    } else {
        // no pipes
        pid_t cpid = shellDriverIntf.processFork();
        if (cpid == 0){
            command.redirParams = {0};
            PostTokeniseProcessingErr err = PostTokeniseProcessing(command.redirParams, command.pipeline.pipes[0]);
            if (err!=PostTokeniseProcessingErrNone){
                perror("Wrong Redirection");
                return PipesExecErr;
            }
            HandleRedirection(command.redirParams);
            auto ret = ExecuteProgram(command.redirParams.cmd);
            if (ret < 0){
                perror("unable to execute");
            }
        } 
        else {
            int retVal;
            if (!command.GetIsBackground()){                
                if(shellDriverIntf.pWaitpid(cpid, &retVal, 0) < 0){
                    perror("waitpid error");
                    return PipesExecErr;
                }                
            }        
        }
    }
    return PipesErrNone;
}

/**
 * \brief Check for Redirection and split out command
 * @param input vector of strings
 */
PostTokeniseProcessingErr Shell::PostTokeniseProcessing(RedirectionParams& redirParams, const vector<string>& cmd){
    redirParams.cmdEnd = cmd.size();
    ofstream outputFile;
    ifstream inputFile;
    for (int i = 0; i < cmd.size(); i++){
        if (cmd[i] == ">"){
            redirParams.outputRedirectionType = OutputCreate;
            redirParams.outputFileIndex = i+1;
            setCmdEnd(redirParams,i);
            redirParams.outfilename = cmd[redirParams.outputFileIndex];
        } else if (cmd[i] == ">>"){
            redirParams.outputRedirectionType = OutputAppend;
            redirParams.outputFileIndex = i+1;
            setCmdEnd(redirParams,i);
            redirParams.outfilename = cmd[redirParams.outputFileIndex];
        } else if (cmd[i] == "<") {
            redirParams.inputRedirectionType = Input;
            redirParams.inputFileIndex = i+1;
            setCmdEnd(redirParams,i);
            redirParams.infilename = cmd[redirParams.inputFileIndex];
        } else if (cmd[i] == "<<"){
            redirParams.inputRedirectionType = Input;
            redirParams.inputFileIndex = i+1;
            setCmdEnd(redirParams,i);
            redirParams.infilename = cmd[redirParams.inputFileIndex];
        } 
        if ((cmd[i] == "&" ) &&( i!=cmd.size()-1)){
            return BgErrWrongPosition;
        }
        if (count(cmd[i].begin(), cmd[i].end(), '&') > 1) {
            return BgErrDoubleBg;
        }

    }
    if ((redirParams.outputFileIndex != 0) && (redirParams.inputFileIndex != 0) &&(redirParams.outputFileIndex <= redirParams.inputFileIndex)){
        return RedirErrWrongOrder; 
    }
    vector<string> inputCmd;
    redirParams.cmd.assign(cmd.begin()+redirParams.cmdStart, cmd.begin()+redirParams.cmdEnd);
    return PostTokeniseProcessingErrNone; 
}

/**
 * \brief Handle Redirection 
 * @param input command, redirectionType
 */
void Shell::HandleRedirection(const RedirectionParams& redirParams){
    // Note there could be multiple redirection flags in a single command 
    switch(redirParams.outputRedirectionType){
        case (OutputCreate):
            {
                int newstdout = shellDriverIntf.fileOpen(redirParams.outfilename, S_CREAT);
                shellDriverIntf.dupFile(newstdout, stdout);                
                shellDriverIntf.fileClose(newstdout);
            }
            break;
        case(OutputAppend):
            {
                int newstdout = shellDriverIntf.fileOpen(redirParams.outfilename, S_APPEND);
                shellDriverIntf.dupFile(newstdout, stdout);                
                shellDriverIntf.fileClose(newstdout);
            }
            break;
        default:
            break;

    }
    switch (redirParams.inputRedirectionType){
        case(Input):
            {
                int newstdin = shellDriverIntf.fileOpen(redirParams.infilename, S_RDONLY);
                shellDriverIntf.dupFile(newstdin, stdin);
                shellDriverIntf.fileClose(newstdin);
            }
            break;
        default:
            break;

    }
}

/*
 * \brief erases whole line and moves cursor to beginning of line
 * @param ofs output stream
 */
void Shell::moveCursorToBackDisplayPrompt(ostream& ofs){
    cursorPosition = 0; 
    ofs << eraseTillStartOfLine + moveCursorToBeginningOfLine;
    DisplayPrompt(ofs);
}

/*
 * \brief move cursor right (wrapper)
 * @param s input string
 * @param ofs output stream
 */
void Shell::cursorOneRight(string&s, ostream& ofs){
    incrementCursorPosition(s, cursorPosition);
    moveCursorLeftRight(s, ofs);
}

/*
 * \brief move cursor left (wrapper)
 * @param s input string
 * @param ofs output stream
 */
void Shell::cursorOneLeft(string&s, ostream& ofs){
    decrementCursorPosition(s, cursorPosition);
    moveCursorLeftRight(s, ofs);
}

/*
 * \brief move cursor left or right
 * @param s input string
 * @param ofs output stream
 */
void Shell::moveCursorLeftRight(string&s, ostream& ofs){
    for (int i = 0; i < 4; i++){
        ofs << moveCursorOneLeft << eraseCursorTillEndOfLine;
    }
    ofs << moveCursorTillStart;
    ofs << shellPrompt << s;
    ofs << moveCursorTillStart;
    for (int i = 0; i < shellPrompt.size() + cursorPosition; i++){
        ofs << moveCursorOneRight;
    }
}

/*
 * \brief safely decrement cursor position
 * @param s input string
 * @param cursor cursorPosition
 */
void Shell::decrementCursorPosition(const string&s, int& cursor){
    cursor = max(0,cursor-1);
}

/*
 * \brief safely increment cursor position
 * @param s input string
 * @param cursor cursorPosition
 */
void Shell::incrementCursorPosition(const string&s, int& cursor){
    cursor = min((int)s.size(), cursor+1);
}

/*
 * \brief erase character and move the cursor one
 * step back
 * @param s input string
 * @param ofs output stream
 */
void Shell::eraseCharacter(string&s, ostream& ofs){
    // remove the 'erase sequence' printed to the screen 
    for (int i = 0; i < 2; i++){
        ofs << moveCursorOneLeft << eraseCursorTillEndOfLine; 
    }
    if (cursorPosition > 0){
        // move it one more left to emulate an erased character
        ofs << moveCursorOneLeft << eraseCursorTillEndOfLine;
        decrementCursorPosition(s, cursorPosition);
        s = s.substr(0, cursorPosition) + s.substr(cursorPosition+1);     
        ofs << moveCursorTillStart;
        ofs << shellPrompt << s;
        ofs << moveCursorTillStart;
        for (int i = 0; i < shellPrompt.size() + cursorPosition; i++){
            ofs << moveCursorOneRight;
        }
    }
}

/*
 * \brief insert character at cursorPosition
 * @param s input string
 * @param c input character
 * @param cursor cursorPosition
 * @param ofs output stream
 */
void Shell::insertCharacter(string& s, const char&c, int& cursor, ostream& ofs){
    if (cursor == s.size()){
        s += c;
    } else {
        s = s.substr(0,cursor) + c + s.substr(cursor);
    }
    ofs << moveCursorTillStart;
    ofs << shellPrompt << s;
    ofs << moveCursorTillStart;
    for (int i = 0; i < shellPrompt.size()+cursorPosition+1; i++){
        ofs << moveCursorOneRight;
    }
    incrementCursorPosition(s, cursorPosition);
}

/*
 * \brief Puts terminal in per character mode
 * so allows us to respond immediately as soon 
 * as a character is entered
 *
 */
void Shell::PutTerminalInPerCharMode(void){
    /*tcgetattr gets the parameters of the current terminal
      STDIN_FILENO will tell tcgetattr that it should write the settings
      of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
      that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);          

    /*Those new settings will be set to STDIN
      TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
}

/*
 * \brief Restore normal terminal
 *
 */ 
void Shell::PutTerminalBackInNormalMode(void){
    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
