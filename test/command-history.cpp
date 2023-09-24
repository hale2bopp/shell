// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"
#include<sstream>
#include <fstream>

/* Functions to test 
 *
 *   int GetCmdHistorySize(void);
 *   int GetCurrentHistoryIndex(void);
 *   string GetSavedCurrentInput(void);
 *   void SetCurrentHistoryIndex(const int& val);
 *   void SaveCurrentEnteredString(const string& s);
 *   void SetSavedCurrentInput(const string& val);
 *   void AddCmdToHistory(const string &cmd, deque<string> &cmdList);
 *   void MainWrapperAddCmdToHistory(const string &cmd);
 *
 */

// The fixture for testing class Shell.
class CHTest : public ::testing::Test {
protected:
    CHTest() {}
    virtual ~CHTest() {}
    void SetUp(const int size) {
        cmdHistory = new CommandHistory(10);
    }
    void SetUp(){
    // Code here will be called immediately after the constructor (right
    // before each test).
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
    }
    CommandHistory* cmdHistory;
};

TEST_F(CHTest, emptyCheckSize){
    // arrange
    SetUp(10);
    EXPECT_EQ(cmdHistory->GetCmdHistorySize(), 0);
}

TEST_F(CHTest, addEmptyCmd){
    SetUp(10);
    cmdHistory->MainWrapperAddCmdToHistory("");
    EXPECT_EQ(cmdHistory->GetCmdHistorySize(), 0);
}

TEST_F(CHTest, addOneCheckSize){
    // arrange
    SetUp(10);
    string cmd = "Hi There";
    cmdHistory->MainWrapperAddCmdToHistory(cmd);
    EXPECT_EQ(cmdHistory->GetCmdHistorySize(), 1);
}


TEST_F(CHTest, overFlowCmdHistory){
    // arrange
    int maxCmdHistorySize = 10;
    SetUp(maxCmdHistorySize);
    for (int i = 0; i < maxCmdHistorySize; i++){
        string cmd = to_string(i);
        cmdHistory->MainWrapperAddCmdToHistory(cmd);
    }
    EXPECT_EQ(cmdHistory->GetCmdHistorySize(), maxCmdHistorySize);
    string cmd = to_string(maxCmdHistorySize+1);
    cmdHistory->MainWrapperAddCmdToHistory(cmd);
    EXPECT_EQ(cmdHistory->GetCmdHistorySize(), maxCmdHistorySize);
}

TEST_F(CHTest, overFlowCmdHistoryCheckFirstCmd){
    // arrange
    int maxCmdHistorySize = 10;
    SetUp(maxCmdHistorySize);
    for (int i = 0; i < maxCmdHistorySize; i++){
        string cmd = to_string(i);
        cmdHistory->MainWrapperAddCmdToHistory(cmd);
    }
    EXPECT_EQ(cmdHistory->GetCmdHistorySize(), maxCmdHistorySize);
    string cmd = to_string(maxCmdHistorySize+1);
    cmdHistory->MainWrapperAddCmdToHistory(cmd);
    EXPECT_EQ(cmdHistory->GetCmdHistorySize(), maxCmdHistorySize);
    EXPECT_EQ(cmdHistory->cmdHistoryList[0], "1");
}
