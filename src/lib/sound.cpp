#include "expander.h"
#include "lib/file.h"
#include "lib/string.h"
#include "lib/sound.h"
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
   return(asformat("%hu", lowWord));
}


/**
 * Plays a soundfile asynchronously and immediately returns (non-blocking). Plays all sound types supported on the system and
 * supports simultaneous playback of multiple sounds (except MIDI files). Also plays sounds if the terminal doesn't support it
 * in the current context (e.g. in tester).
 *
 * @param  char* soundfile - an absolute filename or a filename relative to directory "sounds" of the terminal directory or
 *                           the data directory (both are searched)
 *
 * @return DWORD - error status
 */
DWORD WINAPI PlaySoundA(const char* soundfile) {
   if ((uint)soundfile < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter soundfile: 0x%p (not a valid pointer)", soundfile));
   wstring s = ansiToUnicode(string(soundfile));
   return(PlaySoundW(s.c_str()));
   #pragma EXPANDER_EXPORT
}


/**
 * Plays a soundfile asynchronously and immediately returns (non-blocking). Plays all sound types supported on the system and
 * supports simultaneous playback of multiple sounds (except MIDI files). Also plays sounds if the terminal doesn't support it
 * in the current context (e.g. in tester).
 *
 * @param  wchar* soundfile - an absolute filename or a filename relative to directory "sounds" of the terminal directory or
 *                            the data directory (both are searched)
 *
 * @return DWORD - error status (MCI errors are mapped to ERR_MCI_ERROR + error)
 */
DWORD WINAPI PlaySoundW(const wchar* soundfile) {
   if ((uint)soundfile < MIN_VALID_POINTER) return(error(ERR_INVALID_PARAMETER, "invalid parameter soundfile: 0x%p (not a valid pointer)", soundfile));

   // test absolute path
   wstring filepath(soundfile);
   if (!IsFileW(filepath.c_str(), MODE_SYSTEM)) {
      // test data dir path
      filepath = wstring(GetTerminalDataPathW()).append(L"\\sounds\\").append(soundfile);
      if (!IsFileW(filepath.c_str(), MODE_SYSTEM)) {
         // test terminal dir path
         filepath = wstring(GetTerminalPathW()).append(L"\\sounds\\").append(soundfile);
         if (!IsFileW(filepath.c_str(), MODE_SYSTEM)) {
            return(warn(ERR_FILE_NOT_FOUND, "invalid parameter soundfile: \"%S\" (file not found)", soundfile));
         }
      }
   }

   // For installed devices and supported file types see:
   //  HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\MCI32
   //  HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\MCI Extensions
   //
   // @see  https://learn.microsoft.com/en-us/windows/win32/Multimedia/windows-multimedia-start-page
   // @see  https://learn.microsoft.com/en-us/windows/win32/multimedia/opening-a-device

   // open sound (without alias as it complicates managing uniqueness and re-usage)
   wstring cmd = wstring(L"open \"").append(filepath).append(L"\"");
   MCIERROR error = mciSendStringW(cmd.c_str(), NULL, 0, NULL);
   if (error) {
      switch ((WORD)error) {
         case MCIERR_DEVICE_OPEN:            break;            // if played again in the same thread: continue and re-use the device
         case MCIERR_WAVE_OUTPUTSUNSUITABLE: return(NULL);     // no sound device installed, quite common in VMs without RDP connection
         case MCIERR_INVALID_DEVICE_NAME:    return(notice(ERR_MCI_ERROR+(WORD)error, "unsupported file type or codec not available"));
         default:                            return( error(ERR_MCI_ERROR+(WORD)error, "mciSendString(%S) => %s", cmd.c_str(), mciErrorToStr(error)));
      }
   }

   // play sound
   cmd.replace(0, 4, L"play").append(L" from 0");              // reset play position to start (in case sound is played again)
   error = mciSendStringW(cmd.c_str(), NULL, 0, NULL);
   if (error) {
      switch ((WORD)error) {                                   // MIDI files can't be mixed with the MCI extension
         case MCIERR_SEQ_PORT_INUSE: return(notice(ERR_MCI_ERROR+(WORD)error, "MIDI sequencer already in use"));
         default:                    return( error(ERR_MCI_ERROR+(WORD)error, "mciSendString(%S) => %s", cmd.c_str(), mciErrorToStr(error)));
      }
   }

   // intentionally leave sound open for faster re-use
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 *
 */
BOOL WINAPI TestSound(const char* soundfile) {
   debug("format %%S, unicode param: %S", L"arg");
   return(TRUE);
   //#pragma EXPANDER_EXPORT

   // @see  API description of winmm::PlaySound()
   //  http://www.jasinskionline.com/windowsapi/ref/p/playsound.html
   //  https://learn.microsoft.com/en-us/previous-versions/dd743680(v=vs.85)#  (update flag description)
}
