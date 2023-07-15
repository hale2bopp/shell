// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"
TEST(gtestTest, test1){
    EXPECT_EQ(1, 1);
}

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

