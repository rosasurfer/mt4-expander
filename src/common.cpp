#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>


/**
 *
 */
std::istream& getLine(std::istream &is, std::string &str) {
   str.clear();

   // The characters in the stream are read one-by-one using a std::streambuf. This is faster than reading them one-by-one
   // using the std::istream. Code that uses streambuf this way must be guarded by a sentry object. The sentry object performs
   // various tasks, such as thread synchronization and updating the stream state.

   std::istream::sentry se(is, true);
   std::streambuf* sb = is.rdbuf();

   for (;;) {
      int c = sb->sbumpc();
      switch (c) {
         case '\n':
            break;

         case '\r':
            if (sb->sgetc() == '\n')
               sb->sbumpc();
            break;

         case EOF:
            // handle the case when the last line has no line ending
            if (str.empty())
               is.setstate(std::ios::eofbit);
            break;

         default:
            str += (char)c;
      }
   }
   return(is);
}
