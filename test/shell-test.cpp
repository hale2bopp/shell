// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"

TEST(tokeniseTest, oneWordResult){
    // arrange
    vector<string> exResult = {"hi"};
    // act
    // assert 
    EXPECT_EQ(tokenise("hi", ' '), exResult);
}

TEST(tokeniseTest, tooLongInput){
    string tooLong(MAX_INPUT+1, 'a');
    string justRight(MAX_INPUT, 'a');
    checkLength(tooLong);
    EXPECT_EQ(tooLong, justRight);
}

TEST(tokeniseTest, multipleWords){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(tokenise("Dimpy loves mice ", ' '), exResult);
}

TEST(tokeniseTest, initialSpace){
    vector<string> exResult = {"hi"};
    EXPECT_EQ(tokenise(" hi", ' '), exResult);
}

TEST(tokeniseTest, initialSpaceAndMultipleSpace){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(tokenise(" Dimpy   loves    mice", ' '), exResult);
}

TEST(tokeniseTest, initialSpaceAndMultipleSpaceAndEndSpace){
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(tokenise("   Dimpy   loves    mice    ", ' '), exResult);
}

TEST(tokeniseTest, testSingleBackspace){
    // arrange
    string part1 = "Dimpj";
    string part2 = "y loves mice";
    string fullString = part1 + '^' + '?' + part2;    
    // act
    // assert 
    //
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(tokenise(fullString, ' '), exResult);
}

TEST(tokeniseTest, testMultipleBackspace){
    // arrange
    string part1 = "Dimpjs";
    string part2 = "y loves mice";
    string fullString = part1 + "^?" + "^?" + part2;
    // act
    vector<string> outputString = tokenise(fullString, ' ');
    printTokens(outputString);
    // assert 
    //
    vector<string> exResult = {"Dimpy", "loves", "mice"};
    EXPECT_EQ(outputString, exResult);
}

TEST(executeCmd, executeCmdStatus){
    // arrange 
    vector<string> cmd = {"/bin/s" , "-la"};
    
    // act
    // assert
    EXPECT_LT(executeProgram(cmd),0);
}

TEST(cmdHistoryTest, addToCmdHistory){
    // arrange
    vector<string> newCmd = {"/bin/ls", "-la" };
    vector<string> oldCmd1 =  {"cat", "Makefile" };
    queue<vector<string>> cmdList;
    cmdList.push(oldCmd1);
    // act
    addCmdToHistory(newCmd, cmdList); 

    // assert
    // act
    EXPECT_EQ(cmdList.back(), newCmd);
}

TEST(cmdHistoryTest, noHistory){
    // arrange
    queue<vector<string>> cmdList;
    // act
    EXPECT_EQ(replaceInput(cmdList), "");
}

/*
TEST(cmdHistoryTest, testUpArrow){
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

