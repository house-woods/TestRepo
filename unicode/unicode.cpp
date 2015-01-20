#include <iostream>
#include <string>
#include <wchar.h>
#include <locale.h>

using namespace std;

wchar_t* utf8_to_wchar_t(
  char*  begin,
  int len,
  wchar_t* result
  ) {
   int init = 0;
  for (; init < len; ++begin, ++result)
    {
    init++;
    int      count = 0;       // the number of bytes in the UTF-8 sequence
    unsigned c     = (unsigned char)*begin;
    unsigned i     = 0x80;

    // Skip the stupid UTF-8 BOM that Windows programs add
    //
    // (And yes, we have to do it here like this due to problems
    // that iostream iterators have with multiple data accesses.)
    //
    // Note that 0xEF is an illegal UTF-8 code, so it is safe to have
    // this check in the loop.
    //
    if (c == 0xEF)
      c = (unsigned char)* ++ ++ ++begin;

    // Resynchronize after errors (which shouldn't happen)
    while ((c & 0xC0) == 0x80)
      c = (unsigned char)*++begin;

    // Now we count the number of bytes in the sequence...
    for (; c & i; i >>= 1) ++count;
    // ...and strip the high-code-bits from the character value
    c &= i - 1;

    // Now we build the resulting wchar_t by
    // appending all the character bits together
    for (; count > 1; --count)
      {
      c <<= 6;
      c |=  (*++begin) & 0x3F;
      }

    // And we store the result in the output container
    *result = c;
    }

  // The usual generic stuff
  return result;
  };
int Uni2UTF(wchar_t wchar, char *utf8)
{
 if (utf8 == NULL) {
  return -1;
 }
 int len = 0;
 int size_d = 8;
 
 if (wchar < 0x80)
 {  //
  //length = 1;
  utf8[len++] = (char)wchar;
 }
 else if(wchar < 0x800)
 {
  //length = 2;
  
  if (len + 1 >= size_d)
   return -1;
  
  utf8[len++] = 0xc0 | ( wchar >> 6 );
  utf8[len++] = 0x80 | ( wchar & 0x3f );
 }
 else if(wchar < 0x10000 )
 {
  //length = 3;
  if (len + 2 >= size_d)
   return -1;
  
  utf8[len++] = 0xe0 | ( wchar >> 12 );
  utf8[len++] = 0x80 | ( (wchar >> 6) & 0x3f );
  utf8[len++] = 0x80 | ( wchar & 0x3f );
 }
 else if( wchar < 0x200000 ) 
 {
  //length = 4;
  if (len + 3 >= size_d)
   return -1;
  
  utf8[len++] = 0xf0 | ( (int)wchar >> 18 );
  utf8[len++] = 0x80 | ( (wchar >> 12) & 0x3f );
  utf8[len++] = 0x80 | ( (wchar >> 6) & 0x3f );
  utf8[len++] = 0x80 | ( wchar & 0x3f );
 }
 return len;
};
int main(int argc, char* argv[])
{
    char *loc = setlocale(LC_ALL,NULL);
    cout<<"loc is:"<<loc<<endl;
    setlocale(LC_ALL,"en_US.UTF-8");
    wchar_t latin = 20063;
    cout<<sizeof(latin)<<endl;
    char utf[10];
    int len = Uni2UTF(latin, utf); 
    cout<<"..."<<sizeof(utf)<<endl;
    printf("length is :%d\n",len);
    utf[len]='\0';
    wchar_t res[200];
    utf8_to_wchar_t(utf,len,res);
    printf("Output:\n.");
    printf("%ls.\n",res);
    return 0;
}
