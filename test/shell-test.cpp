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

TEST_F(ShellTest, testEmptyInitially){
    Shell shell;
    // arrange 
    ASSERT_EQ(shell.getCmdHistorySize(), 0);
}

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

// The fixture for testing class Cin.

class CinTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  int fildes[2];
  Shell shell;

  CinTest() {
     // You can do set-up work for each test here.
  }

  ~CinTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
     int status = pipe(fildes);
     ASSERT_NE(status, -1);

     // Swap `stdin` fd with the "read" end of the pipe
     status = dup2(fildes[0], STDIN_FILENO);
     ASSERT_NE(status, -1);

  }

  void TearDown() override {
      // Code here will be called immediately after each test (right
      // before the destructor).
      close(fildes[0]);
      close(fildes[1]);
  }

  // Class members declared here can be used by all tests in the test suite
  // for Shell.
};


TEST_F(CinTest, cinTestNewline)
{
    // Create payload
    const char buf[] = "Hi\n";
    const int bsize  = strlen(buf);

    // Send payload through pipe
    ssize_t nbytes = write(fildes[1], buf, bsize);
    ASSERT_EQ(nbytes, bsize);

    ASSERT_EQ(shell.getInput(), "Hi");
}


TEST_F(CinTest, cinTestBackspace)
{
    // Create payload
    string part1 = "Dimpj";
    string part2 = "y loves Mice\n";
    string buf = part1 + (char) 127 + part2;
    int bsize = buf.size();
    ssize_t nbytes = write(fildes[1], buf.c_str(), bsize);
    EXPECT_EQ(nbytes, bsize);
    cout << "nbytes equal to bsize" << endl;
    EXPECT_EQ(shell.getInput(), "Dimpy loves Mice");
    cout << "strings equal" << endl;
}

TEST_F(CinTest, cinTestMultipleBackspace)
{
    // Create payload
    string part1 = "Dimpjsdjk";
    string backSpaces(5, (char) 127);
    string part2 = "y loves Mice\n";
    string buf = part1 + backSpaces + part2;
    int bsize = buf.size();
    ssize_t nbytes = write(fildes[1], buf.c_str(), bsize);
    EXPECT_EQ(nbytes, bsize);
    EXPECT_EQ(shell.getInput(), "Dimpy loves Mice");
}

TEST_F(CinTest, testUpArrow){
    // arrange
    vector<string> newCmd = {"/bin/ls"};
    vector<string> oldCmd1 =  {"cat", "Makefile"};
    queue<vector<string>> cmdList;
    cmdList.push(oldCmd1);
    cmdList.push(newCmd);
     
    // act
    string fullString = "Dimpy"; 
    fullString += (char)27+(char)65 + (char)65+ (char)65;
    int bsize = fullString.size();
    ssize_t nbytes = write(fildes[1], fullString.c_str(), bsize);

    string restOfString = " -la\n";
    ssize_t nbytesRest = write(fildes[1], restOfString.c_str(), restOfString.size());
    string lastCmd;
    for (auto s: cmdList.back()){
        lastCmd += s+" ";
    }
    lastCmd += "-la";
    EXPECT_EQ(shell.getInput(), lastCmd);
}

