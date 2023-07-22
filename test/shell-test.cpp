// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"

// The fixture for testing class Shell.
class ShellTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

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
    Shell shell;
    // arrange
    vector<string> exResult = {"hi"};
    // act
    // assert 
    EXPECT_EQ(shell.tokenise("hi", ' '), exResult);
}

TEST_F(ShellTest, tooLongInput){
    Shell shell;
    string tooLong(MAX_INPUT+1, 'a');
    string justRight(MAX_INPUT, 'a');
    shell.checkLength(tooLong);
    EXPECT_EQ(tooLong, justRight);
}

TEST_F(ShellTest, multipleWords){
    Shell shell;
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.tokenise("Dimpy loves mice ", ' '), exResult);
}


TEST_F(ShellTest, initialSpace){
    Shell shell;
    vector<string> exResult = {"hi"};
    EXPECT_EQ(shell.tokenise(" hi", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpace){
    Shell shell;
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.tokenise(" Dimpy   loves    mice", ' '), exResult);
}

TEST_F(ShellTest, initialSpaceAndMultipleSpaceAndEndSpace){
    Shell shell;
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.tokenise("   Dimpy   loves    mice    ", ' '), exResult);
}

/*
TEST_F(ShellTest, testSingleBackspace){
    Shell shell;
    // arrange
    string part1 = "Dimpj";
    string part2 = "y loves mice";
    string fullString = part1 + (char) 8 + part2;    
    // act
    // assert 
    //
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(shell.tokenise(fullString, ' '), exResult);
}


TEST_F(tokeniseTest, testMultipleBackspace){
    // arrange
    string part1 = "Dimpjs";
    string part2 = "y loves mice";
    string fullString = part1 + (char)8 + (char)8 + part2;
    // act
    vector<string> outputString = tokenise(fullString, ' ');
    printTokens(outputString);
    // assert 
    //
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(outputString, exResult);
}
*/
/*
TEST_F(ShellTest, executeCmdStatus){
    // arrange 
    vector<string> cmd = {"/bin/s" , "-la"};
    
    // act
    // assert
    EXPECT_LT(executeProgram(cmd),0);
}
*/
TEST_F(ShellTest, addToCmdHistory){
    Shell shell;
    // arrange
    vector<string> newCmd = {"/bin/ls", "-la" };
    vector<string> oldCmd1 =  {"cat", "Makefile" };
    queue<vector<string>> cmdList;
    cmdList.push(oldCmd1);
    // act
    shell.addCmdToHistory(newCmd, cmdList); 

    // assert
    // act
    EXPECT_EQ(cmdList.back(), newCmd);
}

TEST_F(ShellTest, noHistory){
    Shell shell;
    // arrange
    queue<vector<string>> cmdList;
    // act
    EXPECT_EQ(shell.replaceInput(cmdList), "");
}


/*
TEST_F(cmdHistoryTest, testUpArrow){
    // arrange
    vector<string> newCmd = {"/bin/ls", "-la" };
    vector<string> oldCmd1 =  {"cat", "Makefile" };
    queue<vector<string>> cmdList;
    cmdList.push(oldCmd1);
    cmdList.push(newCmd);
        
    // act
    string fullString = ""; 
    fullString += char(27)+(char)27+(char)65;
    vector<string> inputStringVec = cmdList.back();
    inputStringVec[inputStringVec.size()-1] += "h";
    int retVal = executeProgram(inputStringVec);
    // assert
    EXPECT_LT(retVal, 0);
}
*/

