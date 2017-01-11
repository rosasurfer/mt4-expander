#include "common.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>


/**
 *
 */
std::istream& getline(std::istream &is, string &line) {
   // The characters in the stream are read one-by-one using std::streambuf. This is faster than reading them one-by-one using
   // std::istream. Code that uses streambuf this way must be guarded by a sentry object. The sentry object performs various
   // tasks, such as thread synchronization and updating the stream state.
   //
   // @see  http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf/6089413#6089413

   // CR     = 0D     = 13       = \r       Mac
   // LF     = 0A     = 10       = \n       Linux
   // CRLF   = 0D0A   = 13,10    = \r\n     Windows
   // CRCRLF = 0D0D0A = 13,13,10 = \r\r\n   Netscape, Windows XP Notepad bug (not yet tested)

   std::istream::sentry se(is, true);
   std::streambuf* sb = is.rdbuf();
   line.clear();

   for (;;) {
      int ch = sb->sbumpc();
      switch (ch) {
         case '\n':
            goto endloop;

         case '\r':
            if (sb->sgetc() == '\n')
               sb->sbumpc();
            goto endloop;

         case EOF:
            // handle the case when the last line has no line ending
            if (line.empty())
               is.setstate(std::ios::eofbit);
            goto endloop;

         default:
            line += (char)ch;
      }
   }
   endloop:
   return(is);

   /*
   string   fileName = GetTerminalPath() +"\\tester\\"+ ec->programName +".ini";
   std::ifstream fs(fileName.c_str());
   if (!fs) return(error(ERR_FILE_CANNOT_OPEN, "cannot open file %s", DoubleQuoteStr(fileName.c_str())));

   string line;
   uint n = 0;

   debug("reading file %s...", DoubleQuoteStr(fileName.c_str()));
   while (!getline(fs, line).eof()) {
      ++n;
      debug("line %d: %s (%d)", n, line.c_str(), line.length());
   }
   fs.close();
   debug("file contains %d line(s)", n);
   */
}
