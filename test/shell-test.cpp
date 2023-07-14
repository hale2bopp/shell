// write unit tests here
#include "gtest/gtest.h"
#include "shell.h"
TEST(gtestTest, test1){
    EXPECT_EQ(1, 1);
}

TEST(tokeniseTest, test1){
    vector<string> exResult = {"hi"};
    EXPECT_EQ(tokenise("hi", ' '), exResult);
}

TEST(tokeniseTest, test3){
    string tooLong(MAX_INPUT+1, 'a');
    string justRight(MAX_INPUT, 'a');
    checkLength(tooLong);
    EXPECT_EQ(tooLong, justRight);
}

TEST(tokeniseTest, test4){
    vector<string> exResult1 = {"hi", "hi"};
    EXPECT_EQ(tokenise("hi hi ", ' '), exResult1);
}

TEST(tokeniseTest, test5){
    vector<string> exResult1 = {"hi"};
    EXPECT_EQ(tokenise(" hi", ' '), exResult1);
}

TEST(tokeniseTest, test6){
    vector<string> exResult1 = {"hi", "hi"};
    EXPECT_EQ(tokenise(" hi   hi", ' '), exResult1);
}

