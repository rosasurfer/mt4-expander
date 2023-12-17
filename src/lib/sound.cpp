#include "expander.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/terminal.h"

#include <mmsystem.h>


/**
 * Return a readable version of an MCI error code.
 *
 * @param  DWORD error
 *
 * @return char*
 */
const char* WINAPI mciErrorToStr(const DWORD error) {
   WORD lowWord  = (WORD) error;
   WORD highWord = (WORD)(error >> 16);

   switch (lowWord) {
      case MCIERR_INVALID_DEVICE_ID       : return("MCIERR_INVALID_DEVICE_ID"       );
      case MCIERR_UNRECOGNIZED_KEYWORD    : return("MCIERR_UNRECOGNIZED_KEYWORD"    );
      case MCIERR_UNRECOGNIZED_COMMAND    : return("MCIERR_UNRECOGNIZED_COMMAND"    );
      case MCIERR_HARDWARE                : return("MCIERR_HARDWARE"                );
      case MCIERR_INVALID_DEVICE_NAME     : return("MCIERR_INVALID_DEVICE_NAME"     );
      case MCIERR_OUT_OF_MEMORY           : return("MCIERR_OUT_OF_MEMORY"           );
      case MCIERR_DEVICE_OPEN             : return("MCIERR_DEVICE_OPEN"             );
      case MCIERR_CANNOT_LOAD_DRIVER      : return("MCIERR_CANNOT_LOAD_DRIVER"      );
      case MCIERR_MISSING_COMMAND_STRING  : return("MCIERR_MISSING_COMMAND_STRING"  );
      case MCIERR_PARAM_OVERFLOW          : return("MCIERR_PARAM_OVERFLOW"          );
      case MCIERR_MISSING_STRING_ARGUMENT : return("MCIERR_MISSING_STRING_ARGUMENT" );
      case MCIERR_BAD_INTEGER             : return("MCIERR_BAD_INTEGER"             );
      case MCIERR_PARSER_INTERNAL         : return("MCIERR_PARSER_INTERNAL"         );
      case MCIERR_DRIVER_INTERNAL         : return("MCIERR_DRIVER_INTERNAL"         );
      case MCIERR_MISSING_PARAMETER       : return("MCIERR_MISSING_PARAMETER"       );
      case MCIERR_UNSUPPORTED_FUNCTION    : return("MCIERR_UNSUPPORTED_FUNCTION"    );
      case MCIERR_FILE_NOT_FOUND          : return("MCIERR_FILE_NOT_FOUND"          );
      case MCIERR_DEVICE_NOT_READY        : return("MCIERR_DEVICE_NOT_READY"        );
      case MCIERR_INTERNAL                : return("MCIERR_INTERNAL"                );
      case MCIERR_DRIVER                  : return("MCIERR_DRIVER"                  );
      case MCIERR_CANNOT_USE_ALL          : return("MCIERR_CANNOT_USE_ALL"          );
      case MCIERR_MULTIPLE                : return("MCIERR_MULTIPLE"                );
      case MCIERR_EXTENSION_NOT_FOUND     : return("MCIERR_EXTENSION_NOT_FOUND"     );
      case MCIERR_OUTOFRANGE              : return("MCIERR_OUTOFRANGE"              );
      case MCIERR_FLAGS_NOT_COMPATIBLE    : return("MCIERR_FLAGS_NOT_COMPATIBLE"    );
      case MCIERR_FILE_NOT_SAVED          : return("MCIERR_FILE_NOT_SAVED"          );
      case MCIERR_DEVICE_TYPE_REQUIRED    : return("MCIERR_DEVICE_TYPE_REQUIRED"    );
      case MCIERR_DEVICE_LOCKED           : return("MCIERR_DEVICE_LOCKED"           );
      case MCIERR_DUPLICATE_ALIAS         : return("MCIERR_DUPLICATE_ALIAS"         );
      case MCIERR_BAD_CONSTANT            : return("MCIERR_BAD_CONSTANT"            );
      case MCIERR_MUST_USE_SHAREABLE      : return("MCIERR_MUST_USE_SHAREABLE"      );
      case MCIERR_MISSING_DEVICE_NAME     : return("MCIERR_MISSING_DEVICE_NAME"     );
      case MCIERR_BAD_TIME_FORMAT         : return("MCIERR_BAD_TIME_FORMAT"         );
      case MCIERR_NO_CLOSING_QUOTE        : return("MCIERR_NO_CLOSING_QUOTE"        );
      case MCIERR_DUPLICATE_FLAGS         : return("MCIERR_DUPLICATE_FLAGS"         );
      case MCIERR_INVALID_FILE            : return("MCIERR_INVALID_FILE"            );
      case MCIERR_NULL_PARAMETER_BLOCK    : return("MCIERR_NULL_PARAMETER_BLOCK"    );
      case MCIERR_UNNAMED_RESOURCE        : return("MCIERR_UNNAMED_RESOURCE"        );
      case MCIERR_NEW_REQUIRES_ALIAS      : return("MCIERR_NEW_REQUIRES_ALIAS"      );
      case MCIERR_NOTIFY_ON_AUTO_OPEN     : return("MCIERR_NOTIFY_ON_AUTO_OPEN"     );
      case MCIERR_NO_ELEMENT_ALLOWED      : return("MCIERR_NO_ELEMENT_ALLOWED"      );
      case MCIERR_NONAPPLICABLE_FUNCTION  : return("MCIERR_NONAPPLICABLE_FUNCTION"  );
      case MCIERR_ILLEGAL_FOR_AUTO_OPEN   : return("MCIERR_ILLEGAL_FOR_AUTO_OPEN"   );
      case MCIERR_FILENAME_REQUIRED       : return("MCIERR_FILENAME_REQUIRED"       );
      case MCIERR_EXTRA_CHARACTERS        : return("MCIERR_EXTRA_CHARACTERS"        );
      case MCIERR_DEVICE_NOT_INSTALLED    : return("MCIERR_DEVICE_NOT_INSTALLED"    );
      case MCIERR_GET_CD                  : return("MCIERR_GET_CD"                  );
      case MCIERR_SET_CD                  : return("MCIERR_SET_CD"                  );
      case MCIERR_SET_DRIVE               : return("MCIERR_SET_DRIVE"               );
      case MCIERR_DEVICE_LENGTH           : return("MCIERR_DEVICE_LENGTH"           );
      case MCIERR_DEVICE_ORD_LENGTH       : return("MCIERR_DEVICE_ORD_LENGTH"       );
      case MCIERR_NO_INTEGER              : return("MCIERR_NO_INTEGER"              );

      case MCIERR_WAVE_OUTPUTSINUSE       : return("MCIERR_WAVE_OUTPUTSINUSE"       );
      case MCIERR_WAVE_SETOUTPUTINUSE     : return("MCIERR_WAVE_SETOUTPUTINUSE"     );
      case MCIERR_WAVE_INPUTSINUSE        : return("MCIERR_WAVE_INPUTSINUSE"        );
      case MCIERR_WAVE_SETINPUTINUSE      : return("MCIERR_WAVE_SETINPUTINUSE"      );
      case MCIERR_WAVE_OUTPUTUNSPECIFIED  : return("MCIERR_WAVE_OUTPUTUNSPECIFIED"  );
      case MCIERR_WAVE_INPUTUNSPECIFIED   : return("MCIERR_WAVE_INPUTUNSPECIFIED"   );
      case MCIERR_WAVE_OUTPUTSUNSUITABLE  : return("MCIERR_WAVE_OUTPUTSUNSUITABLE"  );
      case MCIERR_WAVE_SETOUTPUTUNSUITABLE: return("MCIERR_WAVE_SETOUTPUTUNSUITABLE");
      case MCIERR_WAVE_INPUTSUNSUITABLE   : return("MCIERR_WAVE_INPUTSUNSUITABLE"   );
      case MCIERR_WAVE_SETINPUTUNSUITABLE : return("MCIERR_WAVE_SETINPUTUNSUITABLE" );

      case MCIERR_SEQ_DIV_INCOMPATIBLE    : return("MCIERR_SEQ_DIV_INCOMPATIBLE"    );
      case MCIERR_SEQ_PORT_INUSE          : return("MCIERR_SEQ_PORT_INUSE"          );
      case MCIERR_SEQ_PORT_NONEXISTENT    : return("MCIERR_SEQ_PORT_NONEXISTENT"    );
      case MCIERR_SEQ_PORT_MAPNODEVICE    : return("MCIERR_SEQ_PORT_MAPNODEVICE"    );
      case MCIERR_SEQ_PORT_MISCERROR      : return("MCIERR_SEQ_PORT_MISCERROR"      );
      case MCIERR_SEQ_TIMER               : return("MCIERR_SEQ_TIMER"               );
      case MCIERR_SEQ_PORTUNSPECIFIED     : return("MCIERR_SEQ_PORTUNSPECIFIED"     );
      case MCIERR_SEQ_NOMIDIPRESENT       : return("MCIERR_SEQ_NOMIDIPRESENT"       );

      case MCIERR_NO_WINDOW               : return("MCIERR_NO_WINDOW"               );
      case MCIERR_CREATEWINDOW            : return("MCIERR_CREATEWINDOW"            );
      case MCIERR_FILE_READ               : return("MCIERR_FILE_READ"               );
      case MCIERR_FILE_WRITE              : return("MCIERR_FILE_WRITE"              );

      case MCIERR_NO_IDENTITY             : return("MCIERR_NO_IDENTITY"             );
   }
   return(asformat("%hu:%hu", highWord, lowWord));
}


/**
 * Play a soundfile asynchronously and immediately returns (non-blocking). Plays all sound types currently supported on the
 * system. Allows mixing of sounds (except midi files). Also plays sounds if the terminal doesn't support it in the current
 * context (e.g. in tester).
 *
 * @param  char* soundfile - either an absolute filename or a filename relative to "sounds" (terminal or data directory)
 *
 * @return BOOL - success status
 */
BOOL WINAPI PlaySoundA(const char* soundfile) {
   if ((uint)soundfile < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter soundfile: 0x%p (not a valid pointer)", soundfile));

   string filepath(soundfile);

   // test absolute path
   if (!IsFileA(filepath, MODE_SYSTEM)) {
      // test data dir path
      filepath = string(GetTerminalDataPathA()).append("\\sounds\\").append(soundfile);
      if (!IsFileA(filepath, MODE_SYSTEM)) {
         // test terminal dir path
         filepath = string(GetTerminalPathA()).append("\\sounds\\").append(soundfile);
         if (!IsFileA(filepath, MODE_SYSTEM)) {
            return(!error(ERR_FILE_NOT_FOUND, "invalid parameter soundfile: \"%s\" (file not found)", soundfile));
         }
      }
   }

   // For installed devices and supported file types see:
   //  HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\MCI32
   //  HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\MCI Extensions
   //
   // @see  https://learn.microsoft.com/en-us/windows/win32/Multimedia/windows-multimedia-start-page
   // @see  https://learn.microsoft.com/en-us/windows/win32/multimedia/opening-a-device

   // open sound without defining an alias (complicates managing uniqueness and device re-usage)
   string cmd = string("open \"").append(filepath).append("\"");
   MCIERROR error = mciSendStringA(cmd.c_str(), NULL, 0, NULL);
   if (error) {
      if      ((WORD)error == MCIERR_DEVICE_OPEN) {}      // if played again in the same thread: continue and re-use the device
      else if ((WORD)error == MCIERR_INVALID_DEVICE_NAME) return(!error(ERR_RUNTIME_ERROR, "unsupported file type or codec not available (MCIERR_INVALID_DEVICE_NAME)"));
      else                                                return(!error(ERR_RUNTIME_ERROR, "mciSendString(%s) => %s", cmd.c_str(), mciErrorToStr(error)));
   }

   // play sound
   cmd.replace(0, 4, "play").append(" from 0");           // reset play position to start (in case sound is played again)
   error = mciSendStringA(cmd.c_str(), NULL, 0, NULL);
   if (error) {                                           // midi files can't be mixed with the MCI extension
      if ((WORD)error == MCIERR_SEQ_PORT_INUSE) return(!error(ERR_RUNTIME_ERROR, "MIDI sequencer already in use (MCIERR_SEQ_PORT_INUSE)"));
      else                                      return(!error(ERR_RUNTIME_ERROR, "mciSendString(%s) => %s", cmd.c_str(), mciErrorToStr(error)));
   }

   // intentionally leave sound open for faster re-use
   return(TRUE);
   #pragma EXPANDER_EXPORT
}


/**
 *
 */
BOOL WINAPI PlaySoundW(const wchar* soundfile) {
   return(!error(ERR_FILE_NOT_FOUND, "invalid parameter soundfile: \"%s\"", unicodeToAnsi(soundfile)));
}


/**
 *
 */
BOOL WINAPI TestSound(const char* soundfile) {
   debug("format %%S, unicode param: %S", L"arg");
   return(TRUE);
   #pragma EXPANDER_EXPORT
}
