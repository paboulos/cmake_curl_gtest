#include "libapi.hpp"
#include <gtest/gtest.h>

class AbcApiTestSuite : public testing::Test{
 protected:
  AbcApiTestSuite() : pAbc_(::CreateAbc()){};
  AbcHANDLE pAbc_;
  ~AbcApiTestSuite(){
     pAbc_->Release();
  };

};

TEST_F(AbcApiTestSuite, CreateAbcAPI) {
    ASSERT_NE(pAbc_, nullptr);
}

TEST_F( AbcApiTestSuite, AbcIndexAPI) {
    greek_t greekBeta = BETA;
    greek_t invalidGreek = static_cast<greek_t>(100);
    const char * betaString = pAbc_->GreekIndex(greekBeta);
    int fooResult1 = strcmp (betaString, pAbc_->GreekName(greekBeta));
    const char * invalidString = pAbc_->GreekIndex(invalidGreek);
    int fooResult2 = strcmp (invalidString, pAbc_->GreekName(NIL));
    EXPECT_EQ(  fooResult1, 0 );
    EXPECT_EQ(  fooResult2, 0 );
}

