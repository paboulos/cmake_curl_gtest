#include "libapi.hpp"


class AbcImpl : public IAbc
{
    letters_t GreekIndex(greek_t greek);
    letters_t GreekName(greek_t greek);
    void Release();
    
    private:
    static static_t letters[4];

};

static_t AbcImpl::letters[] = {"ALPHA","BETA","GAMMA","NIL"};

letters_t AbcImpl::GreekIndex(greek_t greek)
{
     switch(greek){
        case ALPHA: return letters[ALPHA];
        case BETA: return  letters[BETA];
        case GAMMA: return  letters[GAMMA];
        default: return  letters[NIL];
    }
};
letters_t AbcImpl::GreekName(greek_t greek){
      return letters[greek];
}
void AbcImpl::Release()
{
    delete this;
};

#if !defined(_WIN64) // 32-bit builds
#pragma comment(linker,"/export:CreateAbc=_CreateAbc@0")
#endif
#  ifdef _DLL
EXPORT AbcHANDLE WINAPI CreateAbc()
{
    return  new AbcImpl;
};
#endif
