// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"
#include "shell-test.h"
#include<sstream>

// The fixture for testing class Shell.
class ShellTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  Shell shell;
  ShellTest() {
     // You can do set-up work for each test here.
  }

  ~ShellTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Shell.
};

TEST_F(ShellTest, oneWordResult){
    // arrange
    vector<string> exResult = {"hi"};
    // act
    // assert 
    EXPECT_EQ(shell.Tokenise("hi", ' '), exResult);
}

TEST_F(ShellTest, tooLongInput){
    string tooLong(MAX_INPUT+1, 'a');
    string justRight(MAX_INPUT, 'a');
    shell.CheckLength(tooLong);
    EXPECT_EQ(tooLong, justRight);
}

TEST_F(ShellTest, multipleWords){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.Tokenise("Dimpy loves mice ", ' '), exResult);
}


TEST_F(ShellTest, initialSpace){
    vector<string> exResult = {"hi"};
    EXPECT_EQ(shell.Tokenise(" hi", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpace){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.Tokenise(" Dimpy   loves    mice", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpaceAndEndSpace){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.Tokenise("   Dimpy   loves    mice    ", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens){
    vector<string> exResult = {"Dimpy", ">", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy > loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_2){
    vector<string> exResult = {"Dimpy", ">", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy>loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_3){
    vector<string> exResult = {"Dimpy", ">>", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy >> loves", ' '), exResult);
}

TEST_F(ShellTest, outputRedirectTokens_4){
    vector<string> exResult = {"Dimpy", ">>", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy>>loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens){
    vector<string> exResult = {"Dimpy", "<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy < loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens2){
    vector<string> exResult = {"Dimpy", "<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy<loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens3){
    vector<string> exResult = {"Dimpy", "<<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy << loves", ' '), exResult);
}

TEST_F(ShellTest, inputRedirectTokens4){
    vector<string> exResult = {"Dimpy", "<<", "loves"};
    EXPECT_EQ(shell.Tokenise("Dimpy<<loves", ' '), exResult);
}

TEST_F(ShellTest, testEmptyInitially){
    // arrange
    auto cmdHistory = shell.GetCommandHistory(); 
    ASSERT_EQ(cmdHistory->GetCmdHistorySize(), 0);
}

TEST_F(ShellTest, addToCmdHistory){
    // arrange
    vector<string> newCmd = {"/bin/ls", "-la" };
    vector<string> oldCmd1 =  {"cat", "Makefile" };
    deque<vector<string>> cmdList;
    cmdList.push_back(oldCmd1);
    // act
    auto cmdHistory = shell.GetCommandHistory();
    cmdHistory->AddCmdToHistory(newCmd, cmdList);

    // assert
    // act
    EXPECT_EQ(cmdList.back(), newCmd);
}

TEST_F(ShellTest, inputTestNewline){
    Shell shell;
    //Note that std::unique_ptr is better that raw pointers
    std::istringstream is("ls -la\n");
    std::ostringstream os("");
    EXPECT_EQ(shell.GetInput(is, os), "ls -la");

}

TEST_F(ShellTest, cinTestBackspace)
{
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
    Shell shell("no prompt");
    vector<string> newCmd = {"/bin/ls"};
    vector<string> oldCmd =  {"cat", "Makefile"};
    auto cmdHistory = shell.GetCommandHistory();
    cout <<"step1" << endl;
    cmdHistory->MainWrapperAddCmdToHistory(oldCmd);
    cout <<"step2" << endl;
    cmdHistory->MainWrapperAddCmdToHistory(newCmd);
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
    Shell shell("no prompt");
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
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
    Shell shell("no prompt");
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
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
    Shell shell("no prompt");
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
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
    Shell shell("no prompt");
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
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
    Shell shell("no prompt");
    auto cmdHistory = shell.GetCommandHistory();
    string s = "Dimpy";
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    std::ostringstream oss("");
    EXPECT_EQ(shell.GetInput(iss, oss), "Dimpy");
}

TEST_F(ShellTest, inputTestDownArrowPressDownTooManyTimesWithInputText){
    Shell shell("no prompt");
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
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
    Shell shell("no prompt");
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
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
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
