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
    EXPECT_EQ(shell.tokenise("hi", ' '), exResult);
}

TEST_F(ShellTest, tooLongInput){
    string tooLong(MAX_INPUT+1, 'a');
    string justRight(MAX_INPUT, 'a');
    shell.checkLength(tooLong);
    EXPECT_EQ(tooLong, justRight);
}

TEST_F(ShellTest, multipleWords){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.tokenise("Dimpy loves mice ", ' '), exResult);
}


TEST_F(ShellTest, initialSpace){
    vector<string> exResult = {"hi"};
    EXPECT_EQ(shell.tokenise(" hi", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpace){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.tokenise(" Dimpy   loves    mice", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpaceAndEndSpace){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.tokenise("   Dimpy   loves    mice    ", ' '), exResult);
}

TEST_F(ShellTest, testEmptyInitially){
    // arrange 
    ASSERT_EQ(shell.getCmdHistorySize(), 0);
}

TEST_F(ShellTest, addToCmdHistory){
    // arrange
    vector<string> newCmd = {"/bin/ls", "-la" };
    vector<string> oldCmd1 =  {"cat", "Makefile" };
    deque<vector<string>> cmdList;
    cmdList.push_back(oldCmd1);
    // act
    shell.addCmdToHistory(newCmd, cmdList); 

    // assert
    // act
    EXPECT_EQ(cmdList.back(), newCmd);
}

TEST_F(ShellTest, noHistory){
    // arrange
    deque<vector<string>> cmdList;
    // act
    EXPECT_EQ(shell.replaceInput(cmdList), "");
}

TEST_F(ShellTest, inputTestNewline){
    Shell shell;
    //Note that std::unique_ptr is better that raw pointers
    std::istringstream is("ls -la\n");
    EXPECT_EQ(shell.getInput(is), "ls -la");

}

TEST_F(ShellTest, cinTestBackspace)
{
    // Create payload
    string part1 = "Dimpj";
    string part2 = "y loves Mice\n";
    string s = part1 + (char) 127 + part2;
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "Dimpy loves Mice");
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
    EXPECT_EQ(shell.getInput(iss), "Dimpy loves Mice");
}

TEST_F(ShellTest, inputTestUpArrow){
    vector<string> newCmd = {"/bin/ls"};
    vector<string> oldCmd =  {"cat", "Makefile"};
    shell.mainWrapperAddCmdToHistory(oldCmd);
    shell.mainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += " -la\n"; 
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "/bin/ls -la");
}

TEST_F(ShellTest, inputTestMultipleArrow){
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
    shell.mainWrapperAddCmdToHistory(oldCmd);
    shell.mainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "cat Makefile");
}

TEST_F(ShellTest, inputTestDownArrow){
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
    shell.mainWrapperAddCmdToHistory(oldCmd);
    shell.mainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "/bin/ls");
}

TEST_F(ShellTest, inputTestDownArrow2){
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
    shell.mainWrapperAddCmdToHistory(oldCmd);
    shell.mainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += " -la\n";
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "/bin/ls -la");
}

TEST_F(ShellTest, inputTestDownArrowPressDownManyTimesEmpty){
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
    shell.mainWrapperAddCmdToHistory(oldCmd);
    shell.mainWrapperAddCmdToHistory(newCmd);
    string s = "";
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "");
}


TEST_F(ShellTest, inputTestDownArrowPressDownTooManyTimesThenPressUp){
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
    shell.mainWrapperAddCmdToHistory(oldCmd);
    shell.mainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += UP_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += UP_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "/bin/ls");
}

TEST_F(ShellTest, inputTestDownArrowPressDownTooManyTimesWithInputText){
    vector<string> oldCmd =  {"cat", "Makefile"};
    vector<string> newCmd = {"/bin/ls"};
    shell.mainWrapperAddCmdToHistory(oldCmd);
    shell.mainWrapperAddCmdToHistory(newCmd);
    string s = "Dimpy";
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += DOWN_ARROW_SEQ;
    s += "\n";
    std::istringstream iss(s);
    EXPECT_EQ(shell.getInput(iss), "Dimpy");
}
