#include "expander.h"
#include "util/format.h"


/**
 * Convert a BOOL value to the string "TRUE" or "FALSE".
 *
 * @param  BOOL value
 *
 * @return char*
 */
const char* WINAPI BoolToStr(BOOL value) {
   return(value ? "TRUE":"FALSE");
   #pragma EXPANDER_EXPORT
}


/**
 * Wrap a std::string in double quote characters and return a std::string.
 *
 * @param  string& value
 *
 * @return string - new string
 */
string WINAPI doubleQuoteStr(const string& value) {
   return(string("\"").append(value).append("\""));                  // Visual Assist bug
}


/**
 * Wrap a C string in double quote characters and return a std::string.
 *
 * @param  char* value
 *
 * @return string - new string; the string "NULL" if a NULL pointer was specified
 */
string WINAPI doubleQuoteStr(const char* value) {
   if (!value)
      return(string("NULL"));
   return(doubleQuoteStr(string(value)));
}


/**
 * Wrap a C string in double quote characters and return a C string.
 *
 * @param  char* value
 *
 * @return char* - new C string; the string "NULL" if a NULL pointer was specified
 */
const char* WINAPI DoubleQuoteStr(const char* value) {
   if (value && (uint)value < MIN_VALID_POINTER) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter value: 0x%p (not a valid pointer)", value));
   if (!value) return("NULL");

   uint size = strlen(value) + 3;                                    // +2 for the quotes, +1 for the terminating '\0'
   char* buffer = new char[size];                                    // TODO: close memory leak
   sprintf_s(buffer, size, "\"%s\"", value);

   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of an MQL error code.
 *
 * @param  int error
 *
 * @return char*
 */
const char* WINAPI ErrorToStr(int error) {
   switch (error) {
      case NO_ERROR                                         : return("NO_ERROR"                             );    //      0

      // trade server errors
      case ERR_NO_RESULT                                    : return("ERR_NO_RESULT"                        );    //      1
      case ERR_COMMON_ERROR                                 : return("ERR_COMMON_ERROR"                     );    //      2
      case ERR_INVALID_TRADE_PARAMETERS                     : return("ERR_INVALID_TRADE_PARAMETERS"         );    //      3
      case ERR_SERVER_BUSY                                  : return("ERR_SERVER_BUSY"                      );    //      4
      case ERR_OLD_VERSION                                  : return("ERR_OLD_VERSION"                      );    //      5
      case ERR_NO_CONNECTION                                : return("ERR_NO_CONNECTION"                    );    //      6
      case ERR_NOT_ENOUGH_RIGHTS                            : return("ERR_NOT_ENOUGH_RIGHTS"                );    //      7
      case ERR_TOO_FREQUENT_REQUESTS                        : return("ERR_TOO_FREQUENT_REQUESTS"            );    //      8
      case ERR_MALFUNCTIONAL_TRADE                          : return("ERR_MALFUNCTIONAL_TRADE"              );    //      9
      case ERR_ACCOUNT_DISABLED                             : return("ERR_ACCOUNT_DISABLED"                 );    //     64
      case ERR_INVALID_ACCOUNT                              : return("ERR_INVALID_ACCOUNT"                  );    //     65
      case ERR_TRADE_TIMEOUT                                : return("ERR_TRADE_TIMEOUT"                    );    //    128
      case ERR_INVALID_PRICE                                : return("ERR_INVALID_PRICE"                    );    //    129
      case ERR_INVALID_STOP                                 : return("ERR_INVALID_STOP"                     );    //    130
      case ERR_INVALID_TRADE_VOLUME                         : return("ERR_INVALID_TRADE_VOLUME"             );    //    131
      case ERR_MARKET_CLOSED                                : return("ERR_MARKET_CLOSED"                    );    //    132
      case ERR_TRADE_DISABLED                               : return("ERR_TRADE_DISABLED"                   );    //    133
      case ERR_NOT_ENOUGH_MONEY                             : return("ERR_NOT_ENOUGH_MONEY"                 );    //    134
      case ERR_PRICE_CHANGED                                : return("ERR_PRICE_CHANGED"                    );    //    135
      case ERR_OFF_QUOTES                                   : return("ERR_OFF_QUOTES"                       );    //    136
      case ERR_BROKER_BUSY                                  : return("ERR_BROKER_BUSY"                      );    //    137
      case ERR_REQUOTE                                      : return("ERR_REQUOTE"                          );    //    138
      case ERR_ORDER_LOCKED                                 : return("ERR_ORDER_LOCKED"                     );    //    139
      case ERR_LONG_POSITIONS_ONLY_ALLOWED                  : return("ERR_LONG_POSITIONS_ONLY_ALLOWED"      );    //    140
      case ERR_TOO_MANY_REQUESTS                            : return("ERR_TOO_MANY_REQUESTS"                );    //    141
      case ERR_ORDER_QUEUED                                 : return("ERR_ORDER_QUEUED"                     );    //    142
      case ERR_ORDER_ACCEPTED                               : return("ERR_ORDER_ACCEPTED"                   );    //    143
      case ERR_ORDER_DISCARDED                              : return("ERR_ORDER_DISCARDED"                  );    //    144
      case ERR_TRADE_MODIFY_DENIED                          : return("ERR_TRADE_MODIFY_DENIED"              );    //    145
      case ERR_TRADE_CONTEXT_BUSY                           : return("ERR_TRADE_CONTEXT_BUSY"               );    //    146
      case ERR_TRADE_EXPIRATION_DENIED                      : return("ERR_TRADE_EXPIRATION_DENIED"          );    //    147
      case ERR_TRADE_TOO_MANY_ORDERS                        : return("ERR_TRADE_TOO_MANY_ORDERS"            );    //    148
      case ERR_TRADE_HEDGE_PROHIBITED                       : return("ERR_TRADE_HEDGE_PROHIBITED"           );    //    149
      case ERR_TRADE_PROHIBITED_BY_FIFO                     : return("ERR_TRADE_PROHIBITED_BY_FIFO"         );    //    150

      // runtime errors
      case ERR_NO_MQLERROR                                  : return("ERR_NO_MQLERROR"                      );    //   4000
      case ERR_WRONG_FUNCTION_POINTER                       : return("ERR_WRONG_FUNCTION_POINTER"           );    //   4001
      case ERR_ARRAY_INDEX_OUT_OF_RANGE                     : return("ERR_ARRAY_INDEX_OUT_OF_RANGE"         );    //   4002
      case ERR_NO_MEMORY_FOR_CALL_STACK                     : return("ERR_NO_MEMORY_FOR_CALL_STACK"         );    //   4003
      case ERR_RECURSIVE_STACK_OVERFLOW                     : return("ERR_RECURSIVE_STACK_OVERFLOW"         );    //   4004
      case ERR_NOT_ENOUGH_STACK_FOR_PARAM                   : return("ERR_NOT_ENOUGH_STACK_FOR_PARAM"       );    //   4005
      case ERR_NO_MEMORY_FOR_PARAM_STRING                   : return("ERR_NO_MEMORY_FOR_PARAM_STRING"       );    //   4006
      case ERR_NO_MEMORY_FOR_TEMP_STRING                    : return("ERR_NO_MEMORY_FOR_TEMP_STRING"        );    //   4007
      case ERR_NOT_INITIALIZED_STRING                       : return("ERR_NOT_INITIALIZED_STRING"           );    //   4008
      case ERR_NOT_INITIALIZED_ARRAYSTRING                  : return("ERR_NOT_INITIALIZED_ARRAYSTRING"      );    //   4009
      case ERR_NO_MEMORY_FOR_ARRAYSTRING                    : return("ERR_NO_MEMORY_FOR_ARRAYSTRING"        );    //   4010
      case ERR_TOO_LONG_STRING                              : return("ERR_TOO_LONG_STRING"                  );    //   4011
      case ERR_REMAINDER_FROM_ZERO_DIVIDE                   : return("ERR_REMAINDER_FROM_ZERO_DIVIDE"       );    //   4012
      case ERR_ZERO_DIVIDE                                  : return("ERR_ZERO_DIVIDE"                      );    //   4013
      case ERR_UNKNOWN_COMMAND                              : return("ERR_UNKNOWN_COMMAND"                  );    //   4014
      case ERR_WRONG_JUMP                                   : return("ERR_WRONG_JUMP"                       );    //   4015
      case ERR_NOT_INITIALIZED_ARRAY                        : return("ERR_NOT_INITIALIZED_ARRAY"            );    //   4016
      case ERR_DLL_CALLS_NOT_ALLOWED                        : return("ERR_DLL_CALLS_NOT_ALLOWED"            );    //   4017
      case ERR_CANNOT_LOAD_LIBRARY                          : return("ERR_CANNOT_LOAD_LIBRARY"              );    //   4018
      case ERR_CANNOT_CALL_FUNCTION                         : return("ERR_CANNOT_CALL_FUNCTION"             );    //   4019
      case ERR_EX4_CALLS_NOT_ALLOWED                        : return("ERR_EX4_CALLS_NOT_ALLOWED"            );    //   4020
      case ERR_NO_MEMORY_FOR_RETURNED_STR                   : return("ERR_NO_MEMORY_FOR_RETURNED_STR"       );    //   4021
      case ERR_SYSTEM_BUSY                                  : return("ERR_SYSTEM_BUSY"                      );    //   4022
      case ERR_DLL_EXCEPTION                                : return("ERR_DLL_EXCEPTION"                    );    //   4023
      case ERR_INTERNAL_ERROR                               : return("ERR_INTERNAL_ERROR"                   );    //   4024
      case ERR_OUT_OF_MEMORY                                : return("ERR_OUT_OF_MEMORY"                    );    //   4025
      case ERR_INVALID_POINTER                              : return("ERR_INVALID_POINTER"                  );    //   4026
      case ERR_FORMAT_TOO_MANY_FORMATTERS                   : return("ERR_FORMAT_TOO_MANY_FORMATTERS"       );    //   4027
      case ERR_FORMAT_TOO_MANY_PARAMETERS                   : return("ERR_FORMAT_TOO_MANY_PARAMETERS"       );    //   4028
      case ERR_ARRAY_INVALID                                : return("ERR_ARRAY_INVALID"                    );    //   4029
      case ERR_CHART_NOREPLY                                : return("ERR_CHART_NOREPLY"                    );    //   4030
      case ERR_INVALID_FUNCTION_PARAMSCNT                   : return("ERR_INVALID_FUNCTION_PARAMSCNT"       );    //   4050
      case ERR_INVALID_PARAMETER                            : return("ERR_INVALID_PARAMETER"                );    //   4051
      case ERR_STRING_FUNCTION_INTERNAL                     : return("ERR_STRING_FUNCTION_INTERNAL"         );    //   4052
      case ERR_ARRAY_ERROR                                  : return("ERR_ARRAY_ERROR"                      );    //   4053
      case ERR_SERIES_NOT_AVAILABLE                         : return("ERR_SERIES_NOT_AVAILABLE"             );    //   4054
      case ERR_CUSTOM_INDICATOR_ERROR                       : return("ERR_CUSTOM_INDICATOR_ERROR"           );    //   4055
      case ERR_INCOMPATIBLE_ARRAYS                          : return("ERR_INCOMPATIBLE_ARRAYS"              );    //   4056
      case ERR_GLOBAL_VARIABLES_PROCESSING                  : return("ERR_GLOBAL_VARIABLES_PROCESSING"      );    //   4057
      case ERR_GLOBAL_VARIABLE_NOT_FOUND                    : return("ERR_GLOBAL_VARIABLE_NOT_FOUND"        );    //   4058
      case ERR_FUNC_NOT_ALLOWED_IN_TESTER                   : return("ERR_FUNC_NOT_ALLOWED_IN_TESTER"       );    //   4059
      case ERR_FUNCTION_NOT_CONFIRMED                       : return("ERR_FUNCTION_NOT_CONFIRMED"           );    //   4060
      case ERR_SEND_MAIL_ERROR                              : return("ERR_SEND_MAIL_ERROR"                  );    //   4061
      case ERR_STRING_PARAMETER_EXPECTED                    : return("ERR_STRING_PARAMETER_EXPECTED"        );    //   4062
      case ERR_INTEGER_PARAMETER_EXPECTED                   : return("ERR_INTEGER_PARAMETER_EXPECTED"       );    //   4063
      case ERR_DOUBLE_PARAMETER_EXPECTED                    : return("ERR_DOUBLE_PARAMETER_EXPECTED"        );    //   4064
      case ERR_ARRAY_AS_PARAMETER_EXPECTED                  : return("ERR_ARRAY_AS_PARAMETER_EXPECTED"      );    //   4065
      case ERS_HISTORY_UPDATE                               : return("ERS_HISTORY_UPDATE"                   );    //   4066      status, no error
      case ERR_TRADE_ERROR                                  : return("ERR_TRADE_ERROR"                      );    //   4067
      case ERR_RESOURCE_NOT_FOUND                           : return("ERR_RESOURCE_NOT_FOUND"               );    //   4068
      case ERR_RESOURCE_NOT_SUPPORTED                       : return("ERR_RESOURCE_NOT_SUPPORTED"           );    //   4069
      case ERR_RESOURCE_DUPLICATED                          : return("ERR_RESOURCE_DUPLICATED"              );    //   4070
      case ERR_INDICATOR_CANNOT_INIT                        : return("ERR_INDICATOR_CANNOT_INIT"            );    //   4071
      case ERR_INDICATOR_CANNOT_LOAD                        : return("ERR_INDICATOR_CANNOT_LOAD"            );    //   4072
      case ERR_NO_HISTORY_DATA                              : return("ERR_NO_HISTORY_DATA"                  );    //   4073
      case ERR_NO_MEMORY_FOR_HISTORY                        : return("ERR_NO_MEMORY_FOR_HISTORY"            );    //   4074
      case ERR_NO_MEMORY_FOR_INDICATOR                      : return("ERR_NO_MEMORY_FOR_INDICATOR"          );    //   4075
      case ERR_END_OF_FILE                                  : return("ERR_END_OF_FILE"                      );    //   4099
      case ERR_FILE_ERROR                                   : return("ERR_FILE_ERROR"                       );    //   4100
      case ERR_WRONG_FILE_NAME                              : return("ERR_WRONG_FILE_NAME"                  );    //   4101
      case ERR_TOO_MANY_OPENED_FILES                        : return("ERR_TOO_MANY_OPENED_FILES"            );    //   4102
      case ERR_CANNOT_OPEN_FILE                             : return("ERR_CANNOT_OPEN_FILE"                 );    //   4103
      case ERR_INCOMPATIBLE_FILEACCESS                      : return("ERR_INCOMPATIBLE_FILEACCESS"          );    //   4104
      case ERR_NO_TICKET_SELECTED                           : return("ERR_NO_TICKET_SELECTED"               );    //   4105
      case ERR_SYMBOL_NOT_AVAILABLE                         : return("ERR_SYMBOL_NOT_AVAILABLE"             );    //   4106
      case ERR_INVALID_PRICE_PARAM                          : return("ERR_INVALID_PRICE_PARAM"              );    //   4107
      case ERR_INVALID_TICKET                               : return("ERR_INVALID_TICKET"                   );    //   4108
      case ERR_TRADE_NOT_ALLOWED                            : return("ERR_TRADE_NOT_ALLOWED"                );    //   4109
      case ERR_LONGS_NOT_ALLOWED                            : return("ERR_LONGS_NOT_ALLOWED"                );    //   4110
      case ERR_SHORTS_NOT_ALLOWED                           : return("ERR_SHORTS_NOT_ALLOWED"               );    //   4111
      case ERR_AUTOMATED_TRADING_DISABLED                   : return("ERR_AUTOMATED_TRADING_DISABLED"       );    //   4112
      case ERR_OBJECT_ALREADY_EXISTS                        : return("ERR_OBJECT_ALREADY_EXISTS"            );    //   4200
      case ERR_UNKNOWN_OBJECT_PROPERTY                      : return("ERR_UNKNOWN_OBJECT_PROPERTY"          );    //   4201
      case ERR_OBJECT_DOES_NOT_EXIST                        : return("ERR_OBJECT_DOES_NOT_EXIST"            );    //   4202
      case ERR_UNKNOWN_OBJECT_TYPE                          : return("ERR_UNKNOWN_OBJECT_TYPE"              );    //   4203
      case ERR_NO_OBJECT_NAME                               : return("ERR_NO_OBJECT_NAME"                   );    //   4204
      case ERR_OBJECT_COORDINATES_ERROR                     : return("ERR_OBJECT_COORDINATES_ERROR"         );    //   4205
      case ERR_NO_SPECIFIED_SUBWINDOW                       : return("ERR_NO_SPECIFIED_SUBWINDOW"           );    //   4206
      case ERR_OBJECT_ERROR                                 : return("ERR_OBJECT_ERROR"                     );    //   4207
      case ERR_CHART_PROP_INVALID                           : return("ERR_CHART_PROP_INVALID"               );    //   4210
      case ERR_CHART_NOT_FOUND                              : return("ERR_CHART_NOT_FOUND"                  );    //   4211
      case ERR_CHARTWINDOW_NOT_FOUND                        : return("ERR_CHARTWINDOW_NOT_FOUND"            );    //   4212
      case ERR_CHARTINDICATOR_NOT_FOUND                     : return("ERR_CHARTINDICATOR_NOT_FOUND"         );    //   4213
      case ERR_SYMBOL_SELECT                                : return("ERR_SYMBOL_SELECT"                    );    //   4220
      case ERR_NOTIFICATION_SEND_ERROR                      : return("ERR_NOTIFICATION_SEND_ERROR"          );    //   4250
      case ERR_NOTIFICATION_PARAMETER                       : return("ERR_NOTIFICATION_PARAMETER"           );    //   4251
      case ERR_NOTIFICATION_SETTINGS                        : return("ERR_NOTIFICATION_SETTINGS"            );    //   4252
      case ERR_NOTIFICATION_TOO_FREQUENT                    : return("ERR_NOTIFICATION_TOO_FREQUENT"        );    //   4253
      case ERR_FTP_NOSERVER                                 : return("ERR_FTP_NOSERVER"                     );    //   4260
      case ERR_FTP_NOLOGIN                                  : return("ERR_FTP_NOLOGIN"                      );    //   4261
      case ERR_FTP_CONNECT_FAILED                           : return("ERR_FTP_CONNECT_FAILED"               );    //   4262
      case ERR_FTP_CLOSED                                   : return("ERR_FTP_CLOSED"                       );    //   4263
      case ERR_FTP_CHANGEDIR                                : return("ERR_FTP_CHANGEDIR"                    );    //   4264
      case ERR_FTP_FILE_ERROR                               : return("ERR_FTP_FILE_ERROR"                   );    //   4265
      case ERR_FTP_ERROR                                    : return("ERR_FTP_ERROR"                        );    //   4266
      case ERR_FILE_TOO_MANY_OPENED                         : return("ERR_FILE_TOO_MANY_OPENED"             );    //   5001
      case ERR_FILE_WRONG_FILENAME                          : return("ERR_FILE_WRONG_FILENAME"              );    //   5002
      case ERR_FILE_TOO_LONG_FILENAME                       : return("ERR_FILE_TOO_LONG_FILENAME"           );    //   5003
      case ERR_FILE_CANNOT_OPEN                             : return("ERR_FILE_CANNOT_OPEN"                 );    //   5004
      case ERR_FILE_BUFFER_ALLOC_ERROR                      : return("ERR_FILE_BUFFER_ALLOC_ERROR"          );    //   5005
      case ERR_FILE_CANNOT_DELETE                           : return("ERR_FILE_CANNOT_DELETE"               );    //   5006
      case ERR_FILE_INVALID_HANDLE                          : return("ERR_FILE_INVALID_HANDLE"              );    //   5007
      case ERR_FILE_UNKNOWN_HANDLE                          : return("ERR_FILE_UNKNOWN_HANDLE"              );    //   5008
      case ERR_FILE_NOT_TOWRITE                             : return("ERR_FILE_NOT_TOWRITE"                 );    //   5009
      case ERR_FILE_NOT_TOREAD                              : return("ERR_FILE_NOT_TOREAD"                  );    //   5010
      case ERR_FILE_NOT_BIN                                 : return("ERR_FILE_NOT_BIN"                     );    //   5011
      case ERR_FILE_NOT_TXT                                 : return("ERR_FILE_NOT_TXT"                     );    //   5012
      case ERR_FILE_NOT_TXTORCSV                            : return("ERR_FILE_NOT_TXTORCSV"                );    //   5013
      case ERR_FILE_NOT_CSV                                 : return("ERR_FILE_NOT_CSV"                     );    //   5014
      case ERR_FILE_READ_ERROR                              : return("ERR_FILE_READ_ERROR"                  );    //   5015
      case ERR_FILE_WRITE_ERROR                             : return("ERR_FILE_WRITE_ERROR"                 );    //   5016
      case ERR_FILE_BIN_STRINGSIZE                          : return("ERR_FILE_BIN_STRINGSIZE"              );    //   5017
      case ERR_FILE_INCOMPATIBLE                            : return("ERR_FILE_INCOMPATIBLE"                );    //   5018
      case ERR_FILE_IS_DIRECTORY                            : return("ERR_FILE_IS_DIRECTORY"                );    //   5019
      case ERR_FILE_NOT_FOUND                               : return("ERR_FILE_NOT_FOUND"                   );    //   5020
      case ERR_FILE_CANNOT_REWRITE                          : return("ERR_FILE_CANNOT_REWRITE"              );    //   5021
      case ERR_FILE_WRONG_DIRECTORYNAME                     : return("ERR_FILE_WRONG_DIRECTORYNAME"         );    //   5022
      case ERR_FILE_DIRECTORY_NOT_EXIST                     : return("ERR_FILE_DIRECTORY_NOT_EXIST"         );    //   5023
      case ERR_FILE_NOT_DIRECTORY                           : return("ERR_FILE_NOT_DIRECTORY"               );    //   5024
      case ERR_FILE_CANT_DELETE_DIRECTORY                   : return("ERR_FILE_CANT_DELETE_DIRECTORY"       );    //   5025
      case ERR_FILE_CANT_CLEAN_DIRECTORY                    : return("ERR_FILE_CANT_CLEAN_DIRECTORY"        );    //   5026
      case ERR_FILE_ARRAYRESIZE_ERROR                       : return("ERR_FILE_ARRAYRESIZE_ERROR"           );    //   5027
      case ERR_FILE_STRINGRESIZE_ERROR                      : return("ERR_FILE_STRINGRESIZE_ERROR"          );    //   5028
      case ERR_FILE_STRUCT_WITH_OBJECTS                     : return("ERR_FILE_STRUCT_WITH_OBJECTS"         );    //   5029
      case ERR_WEBREQUEST_INVALID_ADDRESS                   : return("ERR_WEBREQUEST_INVALID_ADDRESS"       );    //   5200
      case ERR_WEBREQUEST_CONNECT_FAILED                    : return("ERR_WEBREQUEST_CONNECT_FAILED"        );    //   5201
      case ERR_WEBREQUEST_TIMEOUT                           : return("ERR_WEBREQUEST_TIMEOUT"               );    //   5202
      case ERR_WEBREQUEST_REQUEST_FAILED                    : return("ERR_WEBREQUEST_REQUEST_FAILED"        );    //   5203

      // user defined errors: 65536-99999 (0x10000-0x1869F)
      case ERR_RUNTIME_ERROR                                : return("ERR_RUNTIME_ERROR"                    );    //  65536
      case ERR_NOT_IMPLEMENTED                              : return("ERR_NOT_IMPLEMENTED"                  );    //  65537
      case ERR_INVALID_INPUT_PARAMETER                      : return("ERR_INVALID_INPUT_PARAMETER"          );    //  65538
      case ERR_INVALID_CONFIG_PARAMVALUE                    : return("ERR_INVALID_CONFIG_PARAMVALUE"        );    //  65539
      case ERS_TERMINAL_NOT_YET_READY                       : return("ERS_TERMINAL_NOT_YET_READY"           );    //  65540      status, no error
      case ERR_INVALID_TIMEZONE_CONFIG                      : return("ERR_INVALID_TIMEZONE_CONFIG"          );    //  65541
      case ERR_INVALID_MARKET_DATA                          : return("ERR_INVALID_MARKET_DATA"              );    //  65542
      case ERR_CANCELLED_BY_USER                            : return("ERR_CANCELLED_BY_USER"                );    //  65543
      case ERR_FUNC_NOT_ALLOWED                             : return("ERR_FUNC_NOT_ALLOWED"                 );    //  65544
      case ERR_INVALID_COMMAND                              : return("ERR_INVALID_COMMAND"                  );    //  65545
      case ERR_ILLEGAL_STATE                                : return("ERR_ILLEGAL_STATE"                    );    //  65546
      case ERS_EXECUTION_STOPPING                           : return("ERS_EXECUTION_STOPPING"               );    //  65547      status, no error
      case ERR_ORDER_CHANGED                                : return("ERR_ORDER_CHANGED"                    );    //  65548
      case ERR_HISTORY_INSUFFICIENT                         : return("ERR_HISTORY_INSUFFICIENT"             );    //  65549
      case ERR_CONCURRENT_MODIFICATION                      : return("ERR_CONCURRENT_MODIFICATION"          );    //  65550

      // Win32 error codes (for error descriptions see FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), ...))
      case ERR_WIN32_ERROR                                  : return("win32:NO_ERROR"                       );    // 100000 +    0
      case ERR_WIN32_ERROR + ERROR_INVALID_FUNCTION         : return("win32:ERROR_INVALID_FUNCTION"         );    // 100000 +    1
      case ERR_WIN32_ERROR + ERROR_FILE_NOT_FOUND           : return("win32:ERROR_FILE_NOT_FOUND"           );    // 100000 +    2
      case ERR_WIN32_ERROR + ERROR_PATH_NOT_FOUND           : return("win32:ERROR_PATH_NOT_FOUND"           );    // 100000 +    3
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_OPEN_FILES      : return("win32:ERROR_TOO_MANY_OPEN_FILES"      );    // 100000 +    4
      case ERR_WIN32_ERROR + ERROR_ACCESS_DENIED            : return("win32:ERROR_ACCESS_DENIED"            );    // 100000 +    5
      case ERR_WIN32_ERROR + ERROR_INVALID_HANDLE           : return("win32:ERROR_INVALID_HANDLE"           );    // 100000 +    6
      case ERR_WIN32_ERROR + ERROR_ARENA_TRASHED            : return("win32:ERROR_ARENA_TRASHED"            );    // 100000 +    7
      case ERR_WIN32_ERROR + ERROR_NOT_ENOUGH_MEMORY        : return("win32:ERROR_NOT_ENOUGH_MEMORY"        );    // 100000 +    8
      case ERR_WIN32_ERROR + ERROR_INVALID_BLOCK            : return("win32:ERROR_INVALID_BLOCK"            );    // 100000 +    9
      case ERR_WIN32_ERROR + ERROR_BAD_ENVIRONMENT          : return("win32:ERROR_BAD_ENVIRONMENT"          );    // 100000 +   10
      case ERR_WIN32_ERROR + ERROR_BAD_FORMAT               : return("win32:ERROR_BAD_FORMAT"               );    // 100000 +   11
      case ERR_WIN32_ERROR + ERROR_INVALID_ACCESS           : return("win32:ERROR_INVALID_ACCESS"           );    // 100000 +   12
      case ERR_WIN32_ERROR + ERROR_INVALID_DATA             : return("win32:ERROR_INVALID_DATA"             );    // 100000 +   13
      case ERR_WIN32_ERROR + ERROR_OUTOFMEMORY              : return("win32:ERROR_OUTOFMEMORY"              );    // 100000 +   14
      case ERR_WIN32_ERROR + ERROR_INVALID_DRIVE            : return("win32:ERROR_INVALID_DRIVE"            );    // 100000 +   15
      case ERR_WIN32_ERROR + ERROR_CURRENT_DIRECTORY        : return("win32:ERROR_CURRENT_DIRECTORY"        );    // 100000 +   16
      case ERR_WIN32_ERROR + ERROR_NOT_SAME_DEVICE          : return("win32:ERROR_NOT_SAME_DEVICE"          );    // 100000 +   17
      case ERR_WIN32_ERROR + ERROR_NO_MORE_FILES            : return("win32:ERROR_NO_MORE_FILES"            );    // 100000 +   18
      case ERR_WIN32_ERROR + ERROR_WRITE_PROTECT            : return("win32:ERROR_WRITE_PROTECT"            );    // 100000 +   19
      case ERR_WIN32_ERROR + ERROR_BAD_UNIT                 : return("win32:ERROR_BAD_UNIT"                 );    // 100000 +   20
      case ERR_WIN32_ERROR + ERROR_NOT_READY                : return("win32:ERROR_NOT_READY"                );    // 100000 +   21
      case ERR_WIN32_ERROR + ERROR_BAD_COMMAND              : return("win32:ERROR_BAD_COMMAND"              );    // 100000 +   22
      case ERR_WIN32_ERROR + ERROR_CRC                      : return("win32:ERROR_CRC"                      );    // 100000 +   23
      case ERR_WIN32_ERROR + ERROR_BAD_LENGTH               : return("win32:ERROR_BAD_LENGTH"               );    // 100000 +   24
      case ERR_WIN32_ERROR + ERROR_SEEK                     : return("win32:ERROR_SEEK"                     );    // 100000 +   25
      case ERR_WIN32_ERROR + ERROR_NOT_DOS_DISK             : return("win32:ERROR_NOT_DOS_DISK"             );    // 100000 +   26
      case ERR_WIN32_ERROR + ERROR_SECTOR_NOT_FOUND         : return("win32:ERROR_SECTOR_NOT_FOUND"         );    // 100000 +   27
      case ERR_WIN32_ERROR + ERROR_OUT_OF_PAPER             : return("win32:ERROR_OUT_OF_PAPER"             );    // 100000 +   28
      case ERR_WIN32_ERROR + ERROR_WRITE_FAULT              : return("win32:ERROR_WRITE_FAULT"              );    // 100000 +   29
      case ERR_WIN32_ERROR + ERROR_READ_FAULT               : return("win32:ERROR_READ_FAULT"               );    // 100000 +   30
      case ERR_WIN32_ERROR + ERROR_GEN_FAILURE              : return("win32:ERROR_GEN_FAILURE"              );    // 100000 +   31
      case ERR_WIN32_ERROR + ERROR_SHARING_VIOLATION        : return("win32:ERROR_SHARING_VIOLATION"        );    // 100000 +   32
      case ERR_WIN32_ERROR + ERROR_LOCK_VIOLATION           : return("win32:ERROR_LOCK_VIOLATION"           );    // 100000 +   33
      case ERR_WIN32_ERROR + ERROR_WRONG_DISK               : return("win32:ERROR_WRONG_DISK"               );    // 100000 +   34
      case ERR_WIN32_ERROR + ERROR_SHARING_BUFFER_EXCEEDED  : return("win32:ERROR_SHARING_BUFFER_EXCEEDED"  );    // 100000 +   36
      case ERR_WIN32_ERROR + ERROR_HANDLE_EOF               : return("win32:ERROR_HANDLE_EOF"               );    // 100000 +   38
      case ERR_WIN32_ERROR + ERROR_HANDLE_DISK_FULL         : return("win32:ERROR_HANDLE_DISK_FULL"         );    // 100000 +   39
      case ERR_WIN32_ERROR + ERROR_NOT_SUPPORTED            : return("win32:ERROR_NOT_SUPPORTED"            );    // 100000 +   50
      case ERR_WIN32_ERROR + ERROR_REM_NOT_LIST             : return("win32:ERROR_REM_NOT_LIST"             );    // 100000 +   51
      case ERR_WIN32_ERROR + ERROR_DUP_NAME                 : return("win32:ERROR_DUP_NAME"                 );    // 100000 +   52
      case ERR_WIN32_ERROR + ERROR_BAD_NETPATH              : return("win32:ERROR_BAD_NETPATH"              );    // 100000 +   53
      case ERR_WIN32_ERROR + ERROR_NETWORK_BUSY             : return("win32:ERROR_NETWORK_BUSY"             );    // 100000 +   54
      case ERR_WIN32_ERROR + ERROR_DEV_NOT_EXIST            : return("win32:ERROR_DEV_NOT_EXIST"            );    // 100000 +   55
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_CMDS            : return("win32:ERROR_TOO_MANY_CMDS"            );    // 100000 +   56
      case ERR_WIN32_ERROR + ERROR_ADAP_HDW_ERR             : return("win32:ERROR_ADAP_HDW_ERR"             );    // 100000 +   57
      case ERR_WIN32_ERROR + ERROR_BAD_NET_RESP             : return("win32:ERROR_BAD_NET_RESP"             );    // 100000 +   58
      case ERR_WIN32_ERROR + ERROR_UNEXP_NET_ERR            : return("win32:ERROR_UNEXP_NET_ERR"            );    // 100000 +   59
      case ERR_WIN32_ERROR + ERROR_BAD_REM_ADAP             : return("win32:ERROR_BAD_REM_ADAP"             );    // 100000 +   60
      case ERR_WIN32_ERROR + ERROR_PRINTQ_FULL              : return("win32:ERROR_PRINTQ_FULL"              );    // 100000 +   61
      case ERR_WIN32_ERROR + ERROR_NO_SPOOL_SPACE           : return("win32:ERROR_NO_SPOOL_SPACE"           );    // 100000 +   62
      case ERR_WIN32_ERROR + ERROR_PRINT_CANCELLED          : return("win32:ERROR_PRINT_CANCELLED"          );    // 100000 +   63
      case ERR_WIN32_ERROR + ERROR_NETNAME_DELETED          : return("win32:ERROR_NETNAME_DELETED"          );    // 100000 +   64
      case ERR_WIN32_ERROR + ERROR_NETWORK_ACCESS_DENIED    : return("win32:ERROR_NETWORK_ACCESS_DENIED"    );    // 100000 +   65
      case ERR_WIN32_ERROR + ERROR_BAD_DEV_TYPE             : return("win32:ERROR_BAD_DEV_TYPE"             );    // 100000 +   66
      case ERR_WIN32_ERROR + ERROR_BAD_NET_NAME             : return("win32:ERROR_BAD_NET_NAME"             );    // 100000 +   67
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_NAMES           : return("win32:ERROR_TOO_MANY_NAMES"           );    // 100000 +   68
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_SESS            : return("win32:ERROR_TOO_MANY_SESS"            );    // 100000 +   69
      case ERR_WIN32_ERROR + ERROR_SHARING_PAUSED           : return("win32:ERROR_SHARING_PAUSED"           );    // 100000 +   70
      case ERR_WIN32_ERROR + ERROR_REQ_NOT_ACCEP            : return("win32:ERROR_REQ_NOT_ACCEP"            );    // 100000 +   71
      case ERR_WIN32_ERROR + ERROR_REDIR_PAUSED             : return("win32:ERROR_REDIR_PAUSED"             );    // 100000 +   72
      case ERR_WIN32_ERROR + ERROR_FILE_EXISTS              : return("win32:ERROR_FILE_EXISTS"              );    // 100000 +   80
      case ERR_WIN32_ERROR + ERROR_CANNOT_MAKE              : return("win32:ERROR_CANNOT_MAKE"              );    // 100000 +   82
      case ERR_WIN32_ERROR + ERROR_FAIL_I24                 : return("win32:ERROR_FAIL_I24"                 );    // 100000 +   83
      case ERR_WIN32_ERROR + ERROR_OUT_OF_STRUCTURES        : return("win32:ERROR_OUT_OF_STRUCTURES"        );    // 100000 +   84
      case ERR_WIN32_ERROR + ERROR_ALREADY_ASSIGNED         : return("win32:ERROR_ALREADY_ASSIGNED"         );    // 100000 +   85
      case ERR_WIN32_ERROR + ERROR_INVALID_PASSWORD         : return("win32:ERROR_INVALID_PASSWORD"         );    // 100000 +   86
      case ERR_WIN32_ERROR + ERROR_INVALID_PARAMETER        : return("win32:ERROR_INVALID_PARAMETER"        );    // 100000 +   87
      case ERR_WIN32_ERROR + ERROR_NET_WRITE_FAULT          : return("win32:ERROR_NET_WRITE_FAULT"          );    // 100000 +   88
      case ERR_WIN32_ERROR + ERROR_NO_PROC_SLOTS            : return("win32:ERROR_NO_PROC_SLOTS"            );    // 100000 +   89
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_SEMAPHORES      : return("win32:ERROR_TOO_MANY_SEMAPHORES"      );    // 100000 +  100
      case ERR_WIN32_ERROR + ERROR_EXCL_SEM_ALREADY_OWNED   : return("win32:ERROR_EXCL_SEM_ALREADY_OWNED"   );    // 100000 +  101
      case ERR_WIN32_ERROR + ERROR_SEM_IS_SET               : return("win32:ERROR_SEM_IS_SET"               );    // 100000 +  102
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_SEM_REQUESTS    : return("win32:ERROR_TOO_MANY_SEM_REQUESTS"    );    // 100000 +  103
      case ERR_WIN32_ERROR + ERROR_INVALID_AT_INTERRUPT_TIME: return("win32:ERROR_INVALID_AT_INTERRUPT_TIME");    // 100000 +  104
      case ERR_WIN32_ERROR + ERROR_SEM_OWNER_DIED           : return("win32:ERROR_SEM_OWNER_DIED"           );    // 100000 +  105
      case ERR_WIN32_ERROR + ERROR_SEM_USER_LIMIT           : return("win32:ERROR_SEM_USER_LIMIT"           );    // 100000 +  106
      case ERR_WIN32_ERROR + ERROR_DISK_CHANGE              : return("win32:ERROR_DISK_CHANGE"              );    // 100000 +  107
      case ERR_WIN32_ERROR + ERROR_DRIVE_LOCKED             : return("win32:ERROR_DRIVE_LOCKED"             );    // 100000 +  108
      case ERR_WIN32_ERROR + ERROR_BROKEN_PIPE              : return("win32:ERROR_BROKEN_PIPE"              );    // 100000 +  109
      case ERR_WIN32_ERROR + ERROR_OPEN_FAILED              : return("win32:ERROR_OPEN_FAILED"              );    // 100000 +  110
      case ERR_WIN32_ERROR + ERROR_BUFFER_OVERFLOW          : return("win32:ERROR_BUFFER_OVERFLOW"          );    // 100000 +  111
      case ERR_WIN32_ERROR + ERROR_DISK_FULL                : return("win32:ERROR_DISK_FULL"                );    // 100000 +  112
      case ERR_WIN32_ERROR + ERROR_NO_MORE_SEARCH_HANDLES   : return("win32:ERROR_NO_MORE_SEARCH_HANDLES"   );    // 100000 +  113
      case ERR_WIN32_ERROR + ERROR_INVALID_TARGET_HANDLE    : return("win32:ERROR_INVALID_TARGET_HANDLE"    );    // 100000 +  114
      case ERR_WIN32_ERROR + ERROR_INVALID_CATEGORY         : return("win32:ERROR_INVALID_CATEGORY"         );    // 100000 +  117
      case ERR_WIN32_ERROR + ERROR_INVALID_VERIFY_SWITCH    : return("win32:ERROR_INVALID_VERIFY_SWITCH"    );    // 100000 +  118
      case ERR_WIN32_ERROR + ERROR_BAD_DRIVER_LEVEL         : return("win32:ERROR_BAD_DRIVER_LEVEL"         );    // 100000 +  119
      case ERR_WIN32_ERROR + ERROR_CALL_NOT_IMPLEMENTED     : return("win32:ERROR_CALL_NOT_IMPLEMENTED"     );    // 100000 +  120
      case ERR_WIN32_ERROR + ERROR_SEM_TIMEOUT              : return("win32:ERROR_SEM_TIMEOUT"              );    // 100000 +  121
      case ERR_WIN32_ERROR + ERROR_INSUFFICIENT_BUFFER      : return("win32:ERROR_INSUFFICIENT_BUFFER"      );    // 100000 +  122
      case ERR_WIN32_ERROR + ERROR_INVALID_NAME             : return("win32:ERROR_INVALID_NAME"             );    // 100000 +  123
      case ERR_WIN32_ERROR + ERROR_INVALID_LEVEL            : return("win32:ERROR_INVALID_LEVEL"            );    // 100000 +  124
      case ERR_WIN32_ERROR + ERROR_NO_VOLUME_LABEL          : return("win32:ERROR_NO_VOLUME_LABEL"          );    // 100000 +  125
      case ERR_WIN32_ERROR + ERROR_MOD_NOT_FOUND            : return("win32:ERROR_MOD_NOT_FOUND"            );    // 100000 +  126
      case ERR_WIN32_ERROR + ERROR_PROC_NOT_FOUND           : return("win32:ERROR_PROC_NOT_FOUND"           );    // 100000 +  127
      // ...
      case ERR_WIN32_ERROR + ERROR_NOT_A_REPARSE_POINT      : return("win32:ERROR_NOT_A_REPARSE_POINT"      );    // 100000 + 4390
   }

   char* format = "%d";
   if (error >= ERR_WIN32_ERROR) {
      error -= ERR_WIN32_ERROR;
      format = "win32:%d";
   }
   uint size = _scprintf(format, error) + 1;                // +1 for the terminating '\0'
   char* buffer = new char[size];                           // TODO: close memory leak
   sprintf_s(buffer, size, format, error);

   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Return the hexadecimale representation of an integer.
 *
 * @param  int value - 4 byte integer value
 *
 * @return char* - 8 characters string value
 *
 * @example IntToHexStr(13465610) => "00CD780A"
 */
const char* WINAPI IntToHexStr(int value) {
   uint size = 9;
   char* buffer = new char[size];                                    // TODO: close memory leak
   sprintf_s(buffer, size, "%p", value);

   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of one or more INIT_* flags.
 *
 * @param  DWORD flags
 *
 * @return char*
 */
const char* WINAPI InitFlagsToStr(DWORD flags) {
   string str("");

   if (flags & INIT_TIMEZONE           ) str.append("|INIT_TIMEZONE"           );
   if (flags & INIT_PIPVALUE           ) str.append("|INIT_PIPVALUE"           );
   if (flags & INIT_BARS_ON_HIST_UPDATE) str.append("|INIT_BARS_ON_HIST_UPDATE");
   if (flags & INIT_CUSTOMLOG          ) str.append("|INIT_CUSTOMLOG"          );
   if (flags & INIT_NO_BARS_REQUIRED   ) str.append("|INIT_NO_BARS_REQUIRED"   );
   if (!str.length())                    str.append("|"+ to_string(flags)      );

   return(strcpy(new char[str.length()], str.c_str()+1));            // skip the leading "|"
   #pragma EXPANDER_EXPORT                                           // TODO: close memory leak
}


/**
 * Return a readable version of one or more DEINIT_* flags.
 *
 * @param  DWORD flags
 *
 * @return char*
 */
const char* WINAPI DeinitFlagsToStr(DWORD flags) {
   string str("");

 //if (flags & DEINIT_*) str.append("|DEINIT_*"          );          // a.t.m. there are no DEINIT flags
   if (!str.length())    str.append("|"+ to_string(flags));

   return(strcpy(new char[str.length()], str.c_str()+1));            // skip the leading "|"
   #pragma EXPANDER_EXPORT                                           // TODO: close memory leak
}


/**
 * Return a readable version of an InitializeReason.
 *
 * @param  InitializeReason reason
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI InitReasonToStr(InitializeReason reason) {
   switch (reason) {
      case NULL                : return("NULL"                );
      case IR_USER             : return("IR_USER"             );
      case IR_TEMPLATE         : return("IR_TEMPLATE"         );
      case IR_PROGRAM          : return("IR_PROGRAM"          );
      case IR_PROGRAM_AFTERTEST: return("IR_PROGRAM_AFTERTEST");
      case IR_PARAMETERS       : return("IR_PARAMETERS"       );
      case IR_TIMEFRAMECHANGE  : return("IR_TIMEFRAMECHANGE"  );
      case IR_SYMBOLCHANGE     : return("IR_SYMBOLCHANGE"     );
      case IR_RECOMPILE        : return("IR_RECOMPILE"        );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an InitializeReason)", reason));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias for InitReasonToStr()
 */
const char* WINAPI InitializeReasonToStr(InitializeReason reason) {
   return(InitReasonToStr(reason));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the readable version of an UninitializeReason.
 *
 * @param  UninitializeReason reason
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI UninitReasonToStr(UninitializeReason reason) {
   switch (reason) {
      case UR_UNDEFINED  : return("UR_UNDEFINED"  );
      case UR_REMOVE     : return("UR_REMOVE"     );
      case UR_RECOMPILE  : return("UR_RECOMPILE"  );
      case UR_CHARTCHANGE: return("UR_CHARTCHANGE");
      case UR_CHARTCLOSE : return("UR_CHARTCLOSE" );
      case UR_PARAMETERS : return("UR_PARAMETERS" );
      case UR_ACCOUNT    : return("UR_ACCOUNT"    );
      // since build > 509
      case UR_TEMPLATE   : return("UR_TEMPLATE"   );
      case UR_INITFAILED : return("UR_INITFAILED" );
      case UR_CLOSE      : return("UR_CLOSE"      );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an UninitializeReason)", reason));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias for UninitReasonToStr()
 */
const char* WINAPI UninitializeReasonToStr(UninitializeReason reason) {
   return(UninitReasonToStr(reason));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a description of a ModuleType.
 *
 * @param  ModuleType type
 *
 * @return char* - description or NULL if the parameter is invalid
 */
const char* WINAPI ModuleTypeDescription(ModuleType type) {
   switch (type) {
      case NULL        : return("NULL"     );
      case MT_EXPERT   : return("Expert"   );
      case MT_SCRIPT   : return("Script"   );
      case MT_INDICATOR: return("Indicator");
      case MT_LIBRARY  : return("Library"  );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ModuleType)", type));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a ModuleType.
 *
 * @param  ModuleType type
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI ModuleTypeToStr(ModuleType type) {
   switch (type) {
      case NULL        : return("NULL"        );
      case MT_EXPERT   : return("MT_EXPERT"   );
      case MT_SCRIPT   : return("MT_SCRIPT"   );
      case MT_INDICATOR: return("MT_INDICATOR");
      case MT_LIBRARY  : return("MT_LIBRARY"  );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ModuleType)", type));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias for numberFormat()
 */
string WINAPI numberToStr(double value, const char* format) {
   return(numberFormat(value, format));
}


/**
 * Alias for NumberFormat()
 */
const char* WINAPI NumberToStr(double value, const char* format) {
   return(NumberFormat(value, format));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a description of a ProgramType.
 *
 * @param  ProgramType type
 *
 * @return char* - description or NULL if the parameter is invalid
 */
const char* WINAPI ProgramTypeDescription(ProgramType type) {
   switch (type) {
      case NULL        : return("NULL"     );
      case PT_EXPERT   : return("Expert"   );
      case PT_SCRIPT   : return("Script"   );
      case PT_INDICATOR: return("Indicator");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ProgramType)", type));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a ProgramType.
 *
 * @param  ProgramType type
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI ProgramTypeToStr(ProgramType type) {
   switch (type) {
      case NULL        : return("NULL"        );
      case PT_EXPERT   : return("PT_EXPERT"   );
      case PT_SCRIPT   : return("PT_SCRIPT"   );
      case PT_INDICATOR: return("PT_INDICATOR");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not a ProgramType)", type));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a description of a RootFunction (the name).
 *
 * @param  RootFunction fn
 *
 * @return char* - name or NULL if the parameter is invalid
 */
const char* WINAPI RootFunctionDescription(RootFunction fn) {
   switch (fn) {
      case NULL     : return("(null)");
      case RF_INIT  : return("init"  );
      case RF_START : return("start" );
      case RF_DEINIT: return("deinit");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter fn: %d (not a RootFunction)", fn));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a RootFunction.
 *
 * @param  RootFunction fn
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI RootFunctionToStr(RootFunction fn) {
   switch (fn) {
      case NULL     : return("NULL"     );
      case RF_INIT  : return("RF_INIT"  );
      case RF_START : return("RF_START" );
      case RF_DEINIT: return("RF_DEINIT");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter fn: %d (not a RootFunction)", fn));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a description of an operation type.
 *
 * @param  int type - operation type
 *
 * @return char* - description or NULL if the parameter is invalid
 */
const char* WINAPI OperationTypeDescription(int type) {
   switch (type) {
      case OP_BUY      : return("Buy"       );
      case OP_SELL     : return("Sell"      );
      case OP_BUYLIMIT : return("Buy Limit" );
      case OP_SELLLIMIT: return("Sell Limit");
      case OP_BUYSTOP  : return("Stop Buy"  );
      case OP_SELLSTOP : return("Stop Sell" );
      case OP_BALANCE  : return("Balance"   );
      case OP_CREDIT   : return("Credit"    );
      case OP_UNDEFINED: return("undefined" );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type=%d (not an operation type)", type));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of an operation type.
 *
 * @param  int type - operation type
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI OperationTypeToStr(int type) {
   switch (type) {
      case OP_BUY      : return("OP_BUY"      );
      case OP_SELL     : return("OP_SELL"     );
      case OP_BUYLIMIT : return("OP_BUYLIMIT" );
      case OP_SELLLIMIT: return("OP_SELLLIMIT");
      case OP_BUYSTOP  : return("OP_BUYSTOP"  );
      case OP_SELLSTOP : return("OP_SELLSTOP" );
      case OP_BALANCE  : return("OP_BALANCE"  );
      case OP_CREDIT   : return("OP_CREDIT"   );
      case OP_UNDEFINED: return("OP_UNDEFINED");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not an operation type)", type));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias for OperationTypeDescription()
 */
const char* WINAPI OrderTypeDescription(int type) {
   return(OperationTypeDescription(type));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias for OperationTypeToStr()
 */
const char* WINAPI OrderTypeToStr(int type) {
   return(OperationTypeToStr(type));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a description of a period id.
 *
 * @param  int period - period id or amount of minutes per period bar
 *
 * @return char* - description or NULL if the parameter is invalid
 */
const char* WINAPI PeriodDescription(int period) {
   if (period < 0) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter period: %d", period));

   switch (period) {
      case NULL      : return("NULL");
      case PERIOD_M1 : return("M1" );     // 1 minute
      case PERIOD_M5 : return("M5" );     // 5 minutes
      case PERIOD_M15: return("M15");     // 15 minutes
      case PERIOD_M30: return("M30");     // 30 minutes
      case PERIOD_H1 : return("H1" );     // 1 hour
      case PERIOD_H4 : return("H4" );     // 4 hour
      case PERIOD_D1 : return("D1" );     // 1 day
      case PERIOD_W1 : return("W1" );     // 1 week
      case PERIOD_MN1: return("MN1");     // 1 month
      case PERIOD_Q1 : return("Q1" );     // 1 quarter
   }

   uint size = _scprintf("%d", period) + 1;
   char* buffer = new char[size];                                    // TODO: close memory leak
   sprintf_s(buffer, size, "%d", period);

   return(buffer);
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a period id.
 *
 * @param  int period - period id
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI PeriodToStr(int period) {
   if (period < 0) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter period: %d", period));

   switch (period) {
      case NULL      : return("NULL"      );
      case PERIOD_M1 : return("PERIOD_M1" );     // 1 minute
      case PERIOD_M5 : return("PERIOD_M5" );     // 5 minutes
      case PERIOD_M15: return("PERIOD_M15");     // 15 minutes
      case PERIOD_M30: return("PERIOD_M30");     // 30 minutes
      case PERIOD_H1 : return("PERIOD_H1" );     // 1 hour
      case PERIOD_H4 : return("PERIOD_H4" );     // 4 hour
      case PERIOD_D1 : return("PERIOD_D1" );     // 1 day
      case PERIOD_W1 : return("PERIOD_W1" );     // 1 week
      case PERIOD_MN1: return("PERIOD_MN1");     // 1 month
      case PERIOD_Q1 : return("PERIOD_Q1" );     // 1 quarter
   }
   return((char*)error(ERR_INVALID_PARAMETER, "unknown parameter period: %d", period));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of ShowWindow() command.
 *
 * @param  int cmd
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI ShowWindowCmdToStr(int cmd) {
   switch (cmd) {
      case SW_HIDE           : return("SW_HIDE"           );
      case SW_SHOWNORMAL     : return("SW_SHOWNORMAL"     );
      case SW_SHOWMINIMIZED  : return("SW_SHOWMINIMIZED"  );
      case SW_SHOWMAXIMIZED  : return("SW_SHOWMAXIMIZED"  );
      case SW_SHOWNOACTIVATE : return("SW_SHOWNOACTIVATE" );
      case SW_SHOW           : return("SW_SHOW"           );
      case SW_MINIMIZE       : return("SW_MINIMIZE"       );
      case SW_SHOWMINNOACTIVE: return("SW_SHOWMINNOACTIVE");
      case SW_SHOWNA         : return("SW_SHOWNA"         );
      case SW_RESTORE        : return("SW_RESTORE"        );
      case SW_SHOWDEFAULT    : return("SW_SHOWDEFAULT"    );
      case SW_FORCEMINIMIZE  : return("SW_FORCEMINIMIZE"  );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter cmd: %d", cmd));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a C string.
 *
 * @param  char* value
 *
 * @return char* - content of the string or the string "NULL" if the parameter is a NULL pointer
 *
 * Note: If implemented in MQL MetaTrader raises for a NULL pointer ERR_NOT_INITIALIZED_STRING and logs
 *       "warn: not initialized string".
 */
const char* WINAPI StringToStr(const char* value) {
   return(value ? value : "NULL");
   #pragma EXPANDER_EXPORT
}


/**
 * Alias for PeriodDescription()
 */
const char* WINAPI TimeframeDescription(int timeframe) {
   return(PeriodDescription(timeframe));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias for PeriodToStr()
 */
const char* WINAPI TimeframeToStr(int timeframe) {
   return(PeriodToStr(timeframe));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a description of a trade direction id.
 *
 * @param  int direction - direction id
 *
 * @return char* - description or NULL if the parameter is invalid
 */
const char* WINAPI TradeDirectionDescription(int direction) {
   switch (direction) {
      case TRADE_DIRECTIONS_LONG:  return("Long" );
      case TRADE_DIRECTIONS_SHORT: return("Short");
      case TRADE_DIRECTIONS_BOTH:  return("Both" );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter direction: %d (not a trade direction)", direction));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a trade direction id.
 *
 * @param  int direction - direction id
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI TradeDirectionToStr(int direction) {
   switch (direction) {
      case TRADE_DIRECTIONS_LONG:  return("TRADE_DIRECTIONS_LONG" );
      case TRADE_DIRECTIONS_SHORT: return("TRADE_DIRECTIONS_SHORT");
      case TRADE_DIRECTIONS_BOTH:  return("TRADE_DIRECTIONS_BOTH" );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter direction: %d (not a trade direction)", direction));
   #pragma EXPANDER_EXPORT
}
