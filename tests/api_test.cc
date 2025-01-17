#include "libapi.hpp"
#include <gtest/gtest.h>

class AbcApiTestSuite {
 public:
 AbcApiTestSuite(){};
 AbcApiTestSuite(AbcHANDLE pAbc) : pAbc_(pAbc){};
  AbcHANDLE pAbc_ = NULL;
  ~AbcApiTestSuite(){
    pAbc_->Release();
    delete this;
  };
};

TEST(AbcApiTestSuite, CreateAbcAPI) {
    const AbcApiTestSuite* AbcApi = new AbcApiTestSuite(::CreateAbc());
   // AbcApi.pAbc_ = ::CreateAbc();
    EXPECT_TRUE(  AbcApi->pAbc_ != NULL );
}

TEST( AbcApiTestSuite, AbcFooAPI) {
     const AbcApiTestSuite* AbcApi = new AbcApiTestSuite(::CreateAbc());
    greek_t greekBeta = BETA;
    greek_t invalidGreek = static_cast<greek_t>(100);
    const char * betaString = AbcApi->pAbc_->GreekIndex(greekBeta);
    int fooResult1 = strcmp (betaString, AbcApi->pAbc_->GreekName(greekBeta));
    const char * invalidString = AbcApi->pAbc_->GreekIndex(invalidGreek);
    int fooResult2 = strcmp (invalidString, AbcApi->pAbc_->GreekName(NIL));
    EXPECT_EQ(  fooResult1, 0 );
    EXPECT_EQ(  fooResult2, 0 );
}

