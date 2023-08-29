// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"
#include "shell-test.h"
#include<sstream>
#include <fstream>


// The fixture for testing class Shell.
class ShellTest : public ::testing::Test {
protected:
    ShellTest() {}
    virtual ~ShellTest() {}
    void SetUp(const std::string & prompt, const int size) {
        shell = new Shell(prompt, size);
    }
    void SetUp(){
    // Code here will be called immediately after the constructor (right
    // before each test).
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    }

    Shell* shell;
};

TEST_F(ShellTest, oneWordResult){
    // arrange
    SetUp("no prompt", 10);
    vector<string> exResult = {"hi"};
    // act
    // assert 
    EXPECT_EQ(shell->Tokenise("hi", ' '), exResult);
}

TEST_F(ShellTest, tooLongInput){
    SetUp("no prompt", 10);
    string tooLong(MAX_INPUT+1, 'a');
    string justRight(MAX_INPUT, 'a');
    shell->CheckLength(tooLong);
    EXPECT_EQ(tooLong, justRight);
}

TEST_F(ShellTest, multipleWords){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell->Tokenise("Dimpy loves mice ", ' '), exResult);
}


TEST_F(ShellTest, initialSpace){
    SetUp("no prompt", 10);
    vector<string> exResult = {"hi"};
    EXPECT_EQ(shell->Tokenise(" hi", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpace){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell->Tokenise(" Dimpy   loves    mice", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpaceAndEndSpace){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell->Tokenise("   Dimpy   loves    mice    ", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", ">", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy > loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_2){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", ">", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy>loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_3){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", ">>", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy >> loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_4){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", ">>", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy>>loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "<", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy < loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens2){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "<", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy<loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens3){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "<<", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy << loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens4){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "<<", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy<<loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokensThreeInput){
    SetUp("no prompt", 10);
    vector<string> exResult = {"Dimpy", "<<<", "loves"};
    EXPECT_EQ(shell->Tokenise("Dimpy<<<loves", ' '), exResult);
}

TEST_F(ShellTest, testEmptyInitially){
    SetUp("no prompt", 10);
    // arrange
    auto cmdHistory = shell->GetCommandHistory(); 
    ASSERT_EQ(cmdHistory->GetCmdHistorySize(), 0);
}

TEST_F(ShellTest, addToCmdHistory){
    SetUp("no prompt", 10);
    // arrange
    string newCmd = "/bin/ls -la" ;
    string oldCmd1 = "cat Makefile";
    deque<string> cmdList;
    cmdList.push_back(oldCmd1);
    // act
    auto cmdHistory = shell->GetCommandHistory();
    cmdHistory->AddCmdToHistory(newCmd, cmdList);

    // assert
    // act
    EXPECT_EQ(cmdList.back(), newCmd);
}

TEST_F(ShellTest, inputTestNewline){
    SetUp("no prompt", 10);
    //Note that std::unique_ptr is better that raw pointers
    std::istringstream is("ls -la\n");
    std::ostringstream os("");
    EXPECT_EQ(shell->GetInput(is, os), "ls -la");

}

TEST_F(ShellTest, cinTestBackspace)
{
    SetUp("no prompt", 10);
    // Create payload
    string part1 = "Dimpj";
    string part2 = "y loves Mice\n";
    string s = part1 + (char) 127 + part2;
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "Dimpy loves Mice");
    cout << "strings equal" << endl;
}

TEST_F(ShellTest, cinTestMultipleBackspace)
{
    SetUp("no prompt", 10);
    // Create payload
    string part1 = "Dimpjsdjk";
    string backSpaces(5, (char) 127);
    string part2 = "y loves Mice\n";
    string s = part1 + backSpaces + part2;
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "Dimpy loves Mice");
}

TEST_F(ShellTest, inputTestUpArrow){
    SetUp("no prompt", 10);
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHist = shell->GetCommandHistory();
    cout <<"step1" << endl;
    cmdHist->MainWrapperAddCmdToHistory(oldCmd);
    cout <<"step2" << endl;
    cmdHist->MainWrapperAddCmdToHistory(newCmd);
    cout <<"step3" << endl;
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += " -la\n"; 
    std::istringstream iss(s);
    std::ostringstream oss("");
    cout <<"step4" << endl;
    EXPECT_EQ(shell->GetInput(iss, oss), "/bin/ls -la");
}

TEST_F(ShellTest, inputTestMultipleArrow){
    SetUp("no prompt", 10);
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell->GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "cat Makefile");
}

TEST_F(ShellTest, inputTestDownArrow){
    SetUp("no prompt", 10);
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell->GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "/bin/ls");
}

TEST_F(ShellTest, inputTestDownArrow2){
    SetUp("no prompt", 10);
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell->GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += " -la\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "/bin/ls -la");
}

TEST_F(ShellTest, inputTestDownArrowPressDownTooManyTimesThenPressUp){
    SetUp("no prompt", 10);
    string oldCmd = "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell->GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "/bin/ls");
}

TEST_F(ShellTest, inputTestDownArrowPressDownEmptyHistory){
    SetUp("no prompt", 10);
    auto cmdHistory = shell->GetCommandHistory();
    string s = "Dimpy";
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "Dimpy");
}

TEST_F(ShellTest, inputTestDownArrowPressDownTooManyTimesWithInputText){
    SetUp("no prompt", 10);
    string oldCmd = "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell->GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "Dimpy");
}

TEST_F(ShellTest, UpArrow25timesEmpty){
    SetUp("no prompt", 10);
    auto cmdHistory = shell->GetCommandHistory();
    string s = "Dimpy";
    for(int i = 0; i < 25; i++){
        s += UP_ARROW_SEQ;
    }
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "Dimpy");
}

TEST_F(ShellTest, UpArrow25times){
    SetUp("no prompt", 10);
    string oldCmd = "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell->GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    for(int i = 0; i < 25; i++){
        s += UP_ARROW_SEQ;
    }
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell->GetInput(iss, oss), "cat Makefile");
}

TEST_F(ShellTest, CheckRedirParamsInitState){
    SetUp("no prompt", 10);
    RedirectionParams redirParams = {0};
    EXPECT_EQ(redirParams.cmdStart, 0);
    EXPECT_EQ(redirParams.cmdEnd, 0);
    EXPECT_EQ(redirParams.outputFileIndex, 0);
    EXPECT_EQ(redirParams.inputFileIndex, 0);
    EXPECT_EQ(redirParams.cmd.size(), 0);
    vector<string> cmd = {"hello", "world"};
    redirParams.cmd = cmd;
    EXPECT_EQ(redirParams.cmd.size(), cmd.size());
    EXPECT_EQ(redirParams.cmd, cmd);
}

TEST_F(ShellTest, NoRedirectInputCmdFullCommand){
    SetUp("no prompt", 10);
    string s = "echo \"hello\"\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell->Tokenise(s, ' '), Cmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, Cmd);
    EXPECT_EQ(redirParams.cmd, Cmd);
}


TEST_F(ShellTest, WithRedirectInputCmd){
    SetUp("no prompt", 10);
    string s = "echo \"hello\">test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(redirParams.cmd, Cmd);
    EXPECT_EQ(err, RedirErrNone);
}

TEST_F(ShellTest, PostTokeniseProcessingBoundsTest){
    SetUp("no prompt", 10);
    string s = "echo \"hello\">test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.cmd.size(), 2);
    EXPECT_EQ(redirParams.cmd, Cmd);
    EXPECT_EQ(redirParams.cmdStart, 0);
    EXPECT_EQ(redirParams.cmdEnd, 2);
    EXPECT_EQ(redirParams.outputFileIndex, 3);
}

TEST_F(ShellTest, TestTriggerRedirection){
    SetUp("no prompt", 10);
    string s = "echo \"hello\">test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.outputRedirectionType, OutputCreate);
    EXPECT_EQ(redirParams.inputRedirectionType, RedirNone);
}

TEST_F(ShellTest, TestOutputfilename){
    SetUp("no prompt", 10);
    string s = "echo \"hello\">>test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">>", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.outfilename, "test.txt");
}
TEST_F(ShellTest, InputRedirectionArgsTest){
    SetUp("no prompt", 10);
    string s = "cat < cmd.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "<", "cmd.txt"};
    vector<string> cutCmd = {"cat"};
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);

    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, fullCmd);

    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.cmd, cutCmd);
    EXPECT_EQ(redirParams.infilename, "cmd.txt");
}
 
TEST_F(ShellTest, MultipleRedirectionArgsTest){
    SetUp("no prompt", 10);
    string s = "cat < cmd.txt > test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "<", "cmd.txt", ">" , "test.txt"};
    vector<string> cutCmd = {"cat"};
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.infilename, "cmd.txt");
    EXPECT_EQ(redirParams.outfilename, "test.txt");
    EXPECT_EQ(redirParams.cmd, cutCmd);
}

// This tests that the shell returns an error when 
// multiple redirections are involved
TEST_F(ShellTest, WrongOrderRedirectionTest){
    SetUp("no prompt", 10);
    string s = "cat > cmd.txt < test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", ">", "cmd.txt", "<" , "test.txt"};
    vector<string> cutCmd = {"cat"};
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);

    RedirectionParams redirParams = {0};
    RedirErr err = shell->PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrWrongOrder);
}
 

TEST_F(ShellTest, PipeProcessingTestTokenise){

    SetUp("no prompt", 10);
    string s = "cat README.md | sort\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "|", "sort"};

    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 1);
}
 
TEST_F(ShellTest, MultiplePipeProcessingTestTokenise){

    SetUp("no prompt", 10);
    string s = "cat README.md | sort | give \n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "|", "sort", "|" , "give"};

    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 2);
}
 
TEST_F(ShellTest, NoSpacesPipeTokenise){

    SetUp("no prompt", 10);
    string s = "cat README.md|sort|give\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "|", "sort", "|" , "give"};

    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 2);
}

TEST_F(ShellTest, ManyPipesTokenise){

    SetUp("no prompt", 10);
    string s = "cat README.md|sort|give|take|help\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "|", "sort", "|" , "give", "|", "take" , "|","help"};

    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 4);
}

TEST_F(ShellTest, DoublePipeTokenise){

    SetUp("no prompt", 10);
    string s = "cat README.md||\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "||"};

    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 2);
}

TEST_F(ShellTest, DoublePipe){

    SetUp("no prompt", 10);
    string s = "cat README.md ||\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "||"};

    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 2);
    vector<vector<string>> pipes;
    EXPECT_EQ(shell->ParsePipes(fullCmd, pipes), PipesDoublePipe);
}


TEST_F(ShellTest, InvalidPipeConfigError){

    SetUp("no prompt", 10);
    string s = "cat README.md | sort | \n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "|", "sort", "|"};
    vector<vector<string>> pipes;
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 2);
    EXPECT_EQ(shell->ParsePipes(fullCmd, pipes), PipesEndsWithPipe);
}

TEST_F(ShellTest, TriplePipe){

    SetUp("no prompt", 10);
    string s = "cat README.md |||\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "|||"};

    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 3);
    vector<vector<string>> pipes;
    EXPECT_EQ(shell->ParsePipes(fullCmd, pipes), PipesDoublePipe);
}

TEST_F(ShellTest, ManyPipesProcessTest){

    SetUp("no prompt", 10);
    string s = "cat README.md|sort|give|take|help\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "README.md", "|", "sort", "|" , "give", "|", "take" , "|","help"};
    
    EXPECT_EQ(shell->Tokenise(s, ' '), fullCmd);
    EXPECT_EQ(shell->getNumPipes(), 4);
    vector<vector<string>> pipes;
    vector<vector<string>> correctPipes = {{"cat", "README.md"}, {"sort"}, {"give"}, {"take"}, {"help"}};
    EXPECT_EQ(shell->ParsePipes(fullCmd, pipes), PipesErrNone);
    EXPECT_EQ(pipes, correctPipes);
}

