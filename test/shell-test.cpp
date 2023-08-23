// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"
#include "shell-test.h"
#include<sstream>
#include <fstream>


// The fixture for testing class Shell.
//
class ShellTest : public ::testing::Test {
protected:
    ShellTest() {}
    virtual ~ShellTest() {}
    void SetUp(const std::string & prompt, const int size) {
        cmdHistory = CommandHistory(size);
        shell = Shell(prompt, cmdHistory);
    }
    virtual void TearDown() {
    }

    CommandHistory cmdHistory;
    Shell shell;
};

/*
class ShellTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  CommandHistory cmdHistory;
  Shell :shell;
  ShellTest() {
     // You can do set-up work for each test here.
  }

  ~ShellTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }
  
  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
     cmdHistory = CommandHistory(10);
     shell = Shell("no prompt", cmdHistory);
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Shell.
};
*/

TEST_F(ShellTest, oneWordResult){
    // arrange
    SetUp();
    vector<string> exResult = {"hi"};
    // act
    // assert 
    EXPECT_EQ(shell.Tokenise("hi", ' '), exResult);
}

TEST_F(ShellTest, tooLongInput){
    SetUp();
    string tooLong(MAX_INPUT+1, 'a');
    string justRight(MAX_INPUT, 'a');
    shell.CheckLength(tooLong);
    EXPECT_EQ(tooLong, justRight);
}

TEST_F(ShellTest, multipleWords){
    SetUp();
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.Tokenise("Dimpy loves mice ", ' '), exResult);
}


TEST_F(ShellTest, initialSpace){
    SetUp();
    vector<string> exResult = {"hi"};
    EXPECT_EQ(shell.Tokenise(" hi", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpace){
    SetUp();
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.Tokenise(" Dimpy   loves    mice", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpaceAndEndSpace){
    SetUp();
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.Tokenise("   Dimpy   loves    mice    ", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens){
    SetUp();
    vector<string> exResult = {"Dimpy", ">", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy > loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_2){
    SetUp();
    vector<string> exResult = {"Dimpy", ">", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy>loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_3){
    SetUp();
    vector<string> exResult = {"Dimpy", ">>", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy >> loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_4){
    SetUp();
    vector<string> exResult = {"Dimpy", ">>", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy>>loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens){
    SetUp();
    vector<string> exResult = {"Dimpy", "<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy < loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens2){
    SetUp();
    vector<string> exResult = {"Dimpy", "<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy<loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens3){
    SetUp();
    vector<string> exResult = {"Dimpy", "<<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy << loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens4){
    SetUp();
    vector<string> exResult = {"Dimpy", "<<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy<<loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokensThreeInput){
    SetUp();
    vector<string> exResult = {"Dimpy", "<<<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy<<<loves", ' '), exResult);
}

TEST_F(ShellTest, testEmptyInitially){
    SetUp();
    // arrange
    auto cmdHistory = shell.GetCommandHistory(); 
    ASSERT_EQ(cmdHistory->GetCmdHistorySize(), 0);
}

TEST_F(ShellTest, addToCmdHistory){
    SetUp();
    // arrange
    string newCmd = "/bin/ls -la" ;
    string oldCmd1 = "cat Makefile";
    deque<string> cmdList;
    cmdList.push_back(oldCmd1);
    // act
    auto cmdHistory = shell.GetCommandHistory();
    cmdHistory->AddCmdToHistory(newCmd, cmdList);

    // assert
    // act
    EXPECT_EQ(cmdList.back(), newCmd);
}

TEST_F(ShellTest, inputTestNewline){
    SetUp();
    //Note that std::unique_ptr is better that raw pointers
    std::istringstream is("ls -la\n");
    std::ostringstream os("");
    EXPECT_EQ(shell.GetInput(is, os), "ls -la");

}

TEST_F(ShellTest, cinTestBackspace)
{
    SetUp();
    // Create payload
    string part1 = "Dimpj";
    string part2 = "y loves Mice\n";
    string s = part1 + (char) 127 + part2;
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "Dimpy loves Mice");
    cout << "strings equal" << endl;
}

TEST_F(ShellTest, cinTestMultipleBackspace)
{
    SetUp();
    // Create payload
    string part1 = "Dimpjsdjk";
    string backSpaces(5, (char) 127);
    string part2 = "y loves Mice\n";
    string s = part1 + backSpaces + part2;
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "Dimpy loves Mice");
}

TEST_F(ShellTest, inputTestUpArrow){
    SetUp();
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHist = shell.GetCommandHistory();
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
    EXPECT_EQ(shell.GetInput(iss, oss), "/bin/ls -la");
}

TEST_F(ShellTest, inputTestMultipleArrow){
    SetUp();
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell.GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "cat Makefile");
}

TEST_F(ShellTest, inputTestDownArrow){
    SetUp();
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell.GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "/bin/ls");
}

TEST_F(ShellTest, inputTestDownArrow2){
    SetUp();
    string oldCmd =  "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell.GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += " -la\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "/bin/ls -la");
}

TEST_F(ShellTest, inputTestDownArrowPressDownTooManyTimesThenPressUp){
    SetUp();
    string oldCmd = "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell.GetCommandHistory();
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
    EXPECT_EQ(shell.GetInput(iss, oss), "/bin/ls");
}

TEST_F(ShellTest, inputTestDownArrowPressDownEmptyHistory){
    SetUp();
    auto cmdHistory = shell.GetCommandHistory();
    string s = "Dimpy";
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "Dimpy");
}

TEST_F(ShellTest, inputTestDownArrowPressDownTooManyTimesWithInputText){
    SetUp();
    string oldCmd = "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell.GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "Dimpy");
}

TEST_F(ShellTest, UpArrow25timesEmpty){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    auto cmdHistory = shell.GetCommandHistory();
    string s = "Dimpy";
    for(int i = 0; i < 25; i++){
        s += UP_ARROW_SEQ;
    }
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "Dimpy");
}

TEST_F(ShellTest, UpArrow25times){
    Shell shell("no prompt");
    string oldCmd = "cat Makefile";
    string newCmd = "/bin/ls";
    auto cmdHistory = shell.GetCommandHistory();
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    for(int i = 0; i < 25; i++){
        s += UP_ARROW_SEQ;
    }
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "cat Makefile");
}

TEST_F(ShellTest, CheckRedirParamsInitState){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
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
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "echo \"hello\"\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell.Tokenise(s, ' '), Cmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, Cmd);
    EXPECT_EQ(redirParams.cmd, Cmd);
}


TEST_F(ShellTest, WithRedirectInputCmd){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "echo \"hello\">test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell.Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(redirParams.cmd, Cmd);
    EXPECT_EQ(err, RedirErrNone);
}

TEST_F(ShellTest, PostTokeniseProcessingBoundsTest){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "echo \"hello\">test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell.Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.cmd.size(), 2);
    EXPECT_EQ(redirParams.cmd, Cmd);
    EXPECT_EQ(redirParams.cmdStart, 0);
    EXPECT_EQ(redirParams.cmdEnd, 2);
    EXPECT_EQ(redirParams.outputFileIndex, 3);
}

TEST_F(ShellTest, TestTriggerRedirection){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "echo \"hello\">test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell.Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.outputRedirectionType, OutputCreate);
    EXPECT_EQ(redirParams.inputRedirectionType, RedirNone);
}

TEST_F(ShellTest, TestOutputfilename){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "echo \"hello\">>test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"echo", "\"hello\"", ">>", "test.txt"};
    vector<string> Cmd = {"echo", "\"hello\""};
    EXPECT_EQ(shell.Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.outfilename, "test.txt");
}
TEST_F(ShellTest, InputRedirectionArgsTest){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "cat < cmd.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "<", "cmd.txt"};
    vector<string> cutCmd = {"cat"};
    EXPECT_EQ(shell.Tokenise(s, ' '), fullCmd);

    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, fullCmd);

    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.cmd, cutCmd);
    EXPECT_EQ(redirParams.infilename, "cmd.txt");
}
 
TEST_F(ShellTest, MultipleRedirectionArgsTest){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "cat < cmd.txt > test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", "<", "cmd.txt", ">" , "test.txt"};
    vector<string> cutCmd = {"cat"};
    EXPECT_EQ(shell.Tokenise(s, ' '), fullCmd);
    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrNone);
    EXPECT_EQ(redirParams.infilename, "cmd.txt");
    EXPECT_EQ(redirParams.outfilename, "test.txt");
    EXPECT_EQ(redirParams.cmd, cutCmd);
}

// This tests that the shell returns an error when 
TEST_F(ShellTest, WrongOrderRedirectionTest){
    CommandHistory cmdHist(10);
    Shell shell("no prompt", cmdHist);
    string s = "cat > cmd.txt < test.txt\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    vector<string> fullCmd = {"cat", ">", "cmd.txt", "<" , "test.txt"};
    vector<string> cutCmd = {"cat"};
    EXPECT_EQ(shell.Tokenise(s, ' '), fullCmd);

    RedirectionParams redirParams = {0};
    RedirErr err = shell.PostTokeniseProcessing(redirParams, fullCmd);
    EXPECT_EQ(err, RedirErrWrongOrder);
}
   
