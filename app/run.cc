#include "libapi.hpp"
#include <iostream>
#include <curl/curl.h>
/**
 * 
 */
int run(int argc, char** argv) {
    AbcHANDLE pAbc = ::CreateAbc();
    greek_t greek = BETA;
    if(pAbc)
    {
        std::cout << "ABC Foo is " << pAbc->GreekIndex(greek) << std::endl;
        pAbc->Release();
    }else{
        std::cout << "ABC ERR" << std::endl;
    }
    return 0;
}