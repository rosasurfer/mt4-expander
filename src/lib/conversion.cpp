#include "expander.h"
#include "lib/conversion.h"
#include "lib/format.h"
#include "lib/string.h"


/**
 * Return a description of a test's bar model id.
 *
 * @param  int id
 *
 * @return char* - description or NULL if the parameter is invalid
 */
const char* WINAPI BarModelDescription(int id) {
   switch (id) {
      case MODE_EVERYTICK:     return("EveryTick"    );
      case MODE_CONTROLPOINTS: return("ControlPoints");
      case MODE_BAROPEN:       return("OpenPrices"   );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a bar model)", id));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a test's bar model id.
 *
 * @param  int id
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI BarModelToStr(int id) {
   switch (id) {
      case MODE_EVERYTICK:     return("MODE_EVERYTICK"    );
      case MODE_CONTROLPOINTS: return("MODE_CONTROLPOINTS");
      case MODE_BAROPEN:       return("MODE_BAROPEN"      );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter id: %d (not a bar model)", id));
   #pragma EXPANDER_EXPORT
}


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
 * Return a description of a CoreFunction id (the name).
 *
 * @param  CoreFunction func
 *
 * @return char* - name or NULL if the parameter is invalid
 */
const char* WINAPI CoreFunctionDescription(CoreFunction func) {
   switch (func) {
      case NULL     : return("NULL"  );
      case CF_INIT  : return("init"  );
      case CF_START : return("start" );
      case CF_DEINIT: return("deinit");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter func: %d (not a CoreFunction)", func));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a CoreFunction id.
 *
 * @param  CoreFunction func
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI CoreFunctionToStr(CoreFunction func) {
   switch (func) {
      case NULL     : return("NULL"     );
      case CF_INIT  : return("CF_INIT"  );
      case CF_START : return("CF_START" );
      case CF_DEINIT: return("CF_DEINIT");
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter func: %d (not a CoreFunction)", func));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of an MQL error code.
 *
 * @param  int error
 *
 * @return char*
 */
const char* WINAPI ErrorToStrA(int error) {
   // for Win32 error codes see https://docs.microsoft.com/en-us/windows/desktop/debug/system-error-codes
   #ifndef ERROR_DEVICE_SUPPORT_IN_PROGRESS
   #define ERROR_DEVICE_SUPPORT_IN_PROGRESS                             171L
   #endif
   #ifndef ERROR_INCOMPATIBLE_WITH_GLOBAL_SHORT_NAME_REGISTRY_SETTING
   #define ERROR_INCOMPATIBLE_WITH_GLOBAL_SHORT_NAME_REGISTRY_SETTING   304L
   #endif
   #ifndef ERROR_SHORT_NAMES_NOT_ENABLED_ON_VOLUME
   #define ERROR_SHORT_NAMES_NOT_ENABLED_ON_VOLUME                      305L
   #endif
   #ifndef ERROR_SECURITY_STREAM_IS_INCONSISTENT
   #define ERROR_SECURITY_STREAM_IS_INCONSISTENT                        306L
   #endif
   #ifndef ERROR_INVALID_LOCK_RANGE
   #define ERROR_INVALID_LOCK_RANGE                                     307L
   #endif
   #ifndef ERROR_IMAGE_SUBSYSTEM_NOT_PRESENT
   #define ERROR_IMAGE_SUBSYSTEM_NOT_PRESENT                            308L
   #endif
   #ifndef ERROR_NOTIFICATION_GUID_ALREADY_DEFINED
   #define ERROR_NOTIFICATION_GUID_ALREADY_DEFINED                      309L
   #endif
   #ifndef ERROR_INVALID_EXCEPTION_HANDLER
   #define ERROR_INVALID_EXCEPTION_HANDLER                              310L
   #endif
   #ifndef ERROR_DUPLICATE_PRIVILEGES
   #define ERROR_DUPLICATE_PRIVILEGES                                   311L
   #endif
   #ifndef ERROR_NO_RANGES_PROCESSED
   #define ERROR_NO_RANGES_PROCESSED                                    312L
   #endif
   #ifndef ERROR_NOT_ALLOWED_ON_SYSTEM_FILE
   #define ERROR_NOT_ALLOWED_ON_SYSTEM_FILE                             313L
   #endif
   #ifndef ERROR_DISK_RESOURCES_EXHAUSTED
   #define ERROR_DISK_RESOURCES_EXHAUSTED                               314L
   #endif
   #ifndef ERROR_INVALID_TOKEN
   #define ERROR_INVALID_TOKEN                                          315L
   #endif
   #ifndef ERROR_DEVICE_FEATURE_NOT_SUPPORTED
   #define ERROR_DEVICE_FEATURE_NOT_SUPPORTED                           316L
   #endif
   #ifndef ERROR_UNDEFINED_SCOPE
   #define ERROR_UNDEFINED_SCOPE                                        319L
   #endif
   #ifndef ERROR_INVALID_CAP
   #define ERROR_INVALID_CAP                                            320L
   #endif
   #ifndef ERROR_DEVICE_UNREACHABLE
   #define ERROR_DEVICE_UNREACHABLE                                     321L
   #endif
   #ifndef ERROR_DEVICE_NO_RESOURCES
   #define ERROR_DEVICE_NO_RESOURCES                                    322L
   #endif
   #ifndef ERROR_DATA_CHECKSUM_ERROR
   #define ERROR_DATA_CHECKSUM_ERROR                                    323L
   #endif
   #ifndef ERROR_INTERMIXED_KERNEL_EA_OPERATION
   #define ERROR_INTERMIXED_KERNEL_EA_OPERATION                         324L
   #endif
   #ifndef ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED
   #define ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED                          326L
   #endif
   #ifndef ERROR_OFFSET_ALIGNMENT_VIOLATION
   #define ERROR_OFFSET_ALIGNMENT_VIOLATION                             327L
   #endif
   #ifndef ERROR_INVALID_FIELD_IN_PARAMETER_LIST
   #define ERROR_INVALID_FIELD_IN_PARAMETER_LIST                        328L
   #endif
   #ifndef ERROR_OPERATION_IN_PROGRESS
   #define ERROR_OPERATION_IN_PROGRESS                                  329L
   #endif
   #ifndef ERROR_BAD_DEVICE_PATH
   #define ERROR_BAD_DEVICE_PATH                                        330L
   #endif
   #ifndef ERROR_TOO_MANY_DESCRIPTORS
   #define ERROR_TOO_MANY_DESCRIPTORS                                   331L
   #endif
   #ifndef ERROR_SCRUB_DATA_DISABLED
   #define ERROR_SCRUB_DATA_DISABLED                                    332L
   #endif
   #ifndef ERROR_NOT_REDUNDANT_STORAGE
   #define ERROR_NOT_REDUNDANT_STORAGE                                  333L
   #endif
   #ifndef ERROR_RESIDENT_FILE_NOT_SUPPORTED
   #define ERROR_RESIDENT_FILE_NOT_SUPPORTED                            334L
   #endif
   #ifndef ERROR_COMPRESSED_FILE_NOT_SUPPORTED
   #define ERROR_COMPRESSED_FILE_NOT_SUPPORTED                          335L
   #endif
   #ifndef ERROR_DIRECTORY_NOT_SUPPORTED
   #define ERROR_DIRECTORY_NOT_SUPPORTED                                336L
   #endif
   #ifndef ERROR_NOT_READ_FROM_COPY
   #define ERROR_NOT_READ_FROM_COPY                                     337L
   #endif
   #ifndef ERROR_PWD_TOO_LONG
   #define ERROR_PWD_TOO_LONG                                           657L
   #endif
   #ifndef ERROR_DISK_REPAIR_REDIRECTED
   #define ERROR_DISK_REPAIR_REDIRECTED                                 792L
   #endif
   #ifndef ERROR_DISK_REPAIR_UNSUCCESSFUL
   #define ERROR_DISK_REPAIR_UNSUCCESSFUL                               793L
   #endif
   #ifndef ERROR_CORRUPT_LOG_OVERFULL
   #define ERROR_CORRUPT_LOG_OVERFULL                                   794L
   #endif
   #ifndef ERROR_CORRUPT_LOG_CORRUPTED
   #define ERROR_CORRUPT_LOG_CORRUPTED                                  795L
   #endif
   #ifndef ERROR_CORRUPT_LOG_UNAVAILABLE
   #define ERROR_CORRUPT_LOG_UNAVAILABLE                                796L
   #endif
   #ifndef ERROR_CORRUPT_LOG_DELETED_FULL
   #define ERROR_CORRUPT_LOG_DELETED_FULL                               797L
   #endif
   #ifndef ERROR_CORRUPT_LOG_CLEARED
   #define ERROR_CORRUPT_LOG_CLEARED                                    798L
   #endif
   #ifndef ERROR_ORPHAN_NAME_EXHAUSTED
   #define ERROR_ORPHAN_NAME_EXHAUSTED                                  799L
   #endif
   #ifndef ERROR_OPLOCK_SWITCHED_TO_NEW_HANDLE
   #define ERROR_OPLOCK_SWITCHED_TO_NEW_HANDLE                          800L
   #endif
   #ifndef ERROR_CANNOT_GRANT_REQUESTED_OPLOCK
   #define ERROR_CANNOT_GRANT_REQUESTED_OPLOCK                          801L
   #endif
   #ifndef ERROR_CANNOT_BREAK_OPLOCK
   #define ERROR_CANNOT_BREAK_OPLOCK                                    802L
   #endif
   #ifndef ERROR_OPLOCK_HANDLE_CLOSED
   #define ERROR_OPLOCK_HANDLE_CLOSED                                   803L
   #endif
   #ifndef ERROR_NO_ACE_CONDITION
   #define ERROR_NO_ACE_CONDITION                                       804L
   #endif
   #ifndef ERROR_INVALID_ACE_CONDITION
   #define ERROR_INVALID_ACE_CONDITION                                  805L
   #endif
   #ifndef ERROR_FILE_HANDLE_REVOKED
   #define ERROR_FILE_HANDLE_REVOKED                                    806L
   #endif
   #ifndef ERROR_IMAGE_AT_DIFFERENT_BASE
   #define ERROR_IMAGE_AT_DIFFERENT_BASE                                807L
   #endif

   switch (error) {
      case NO_ERROR                                                                       : return("NO_ERROR"                                                           );    //      0

      // trade server errors
      case ERR_NO_RESULT                                                                  : return("ERR_NO_RESULT"                                                      );    //      1
      case ERR_TRADESERVER_GONE                                                           : return("ERR_TRADESERVER_GONE"                                               );    //      2
      case ERR_INVALID_TRADE_PARAMETERS                                                   : return("ERR_INVALID_TRADE_PARAMETERS"                                       );    //      3
      case ERR_SERVER_BUSY                                                                : return("ERR_SERVER_BUSY"                                                    );    //      4
      case ERR_OLD_VERSION                                                                : return("ERR_OLD_VERSION"                                                    );    //      5
      case ERR_NO_CONNECTION                                                              : return("ERR_NO_CONNECTION"                                                  );    //      6
      case ERR_NOT_ENOUGH_RIGHTS                                                          : return("ERR_NOT_ENOUGH_RIGHTS"                                              );    //      7
      case ERR_TOO_FREQUENT_REQUESTS                                                      : return("ERR_TOO_FREQUENT_REQUESTS"                                          );    //      8
      case ERR_MALFUNCTIONAL_TRADE                                                        : return("ERR_MALFUNCTIONAL_TRADE"                                            );    //      9
      case ERR_ACCOUNT_DISABLED                                                           : return("ERR_ACCOUNT_DISABLED"                                               );    //     64
      case ERR_INVALID_ACCOUNT                                                            : return("ERR_INVALID_ACCOUNT"                                                );    //     65
      case ERR_TRADE_TIMEOUT                                                              : return("ERR_TRADE_TIMEOUT"                                                  );    //    128
      case ERR_INVALID_PRICE                                                              : return("ERR_INVALID_PRICE"                                                  );    //    129
      case ERR_INVALID_STOP                                                               : return("ERR_INVALID_STOP"                                                   );    //    130
      case ERR_INVALID_TRADE_VOLUME                                                       : return("ERR_INVALID_TRADE_VOLUME"                                           );    //    131
      case ERR_MARKET_CLOSED                                                              : return("ERR_MARKET_CLOSED"                                                  );    //    132
      case ERR_TRADE_DISABLED                                                             : return("ERR_TRADE_DISABLED"                                                 );    //    133
      case ERR_NOT_ENOUGH_MONEY                                                           : return("ERR_NOT_ENOUGH_MONEY"                                               );    //    134
      case ERR_PRICE_CHANGED                                                              : return("ERR_PRICE_CHANGED"                                                  );    //    135
      case ERR_OFF_QUOTES                                                                 : return("ERR_OFF_QUOTES"                                                     );    //    136
      case ERR_BROKER_BUSY                                                                : return("ERR_BROKER_BUSY"                                                    );    //    137
      case ERR_REQUOTE                                                                    : return("ERR_REQUOTE"                                                        );    //    138
      case ERR_ORDER_LOCKED                                                               : return("ERR_ORDER_LOCKED"                                                   );    //    139
      case ERR_LONG_POSITIONS_ONLY_ALLOWED                                                : return("ERR_LONG_POSITIONS_ONLY_ALLOWED"                                    );    //    140
      case ERR_TOO_MANY_REQUESTS                                                          : return("ERR_TOO_MANY_REQUESTS"                                              );    //    141
      case ERR_ORDER_QUEUED                                                               : return("ERR_ORDER_QUEUED"                                                   );    //    142
      case ERR_ORDER_ACCEPTED                                                             : return("ERR_ORDER_ACCEPTED"                                                 );    //    143
      case ERR_ORDER_DISCARDED                                                            : return("ERR_ORDER_DISCARDED"                                                );    //    144
      case ERR_TRADE_MODIFY_DENIED                                                        : return("ERR_TRADE_MODIFY_DENIED"                                            );    //    145
      case ERR_TRADE_CONTEXT_BUSY                                                         : return("ERR_TRADE_CONTEXT_BUSY"                                             );    //    146
      case ERR_TRADE_EXPIRATION_DENIED                                                    : return("ERR_TRADE_EXPIRATION_DENIED"                                        );    //    147
      case ERR_TRADE_TOO_MANY_ORDERS                                                      : return("ERR_TRADE_TOO_MANY_ORDERS"                                          );    //    148
      case ERR_TRADE_HEDGE_PROHIBITED                                                     : return("ERR_TRADE_HEDGE_PROHIBITED"                                         );    //    149
      case ERR_TRADE_PROHIBITED_BY_FIFO                                                   : return("ERR_TRADE_PROHIBITED_BY_FIFO"                                       );    //    150

      // runtime errors
      case ERR_NO_MQLERROR                                                                : return("ERR_NO_MQLERROR"                                                    );    //   4000
      case ERR_WRONG_FUNCTION_POINTER                                                     : return("ERR_WRONG_FUNCTION_POINTER"                                         );    //   4001
      case ERR_ARRAY_INDEX_OUT_OF_RANGE                                                   : return("ERR_ARRAY_INDEX_OUT_OF_RANGE"                                       );    //   4002
      case ERR_NO_MEMORY_FOR_CALL_STACK                                                   : return("ERR_NO_MEMORY_FOR_CALL_STACK"                                       );    //   4003
      case ERR_RECURSIVE_STACK_OVERFLOW                                                   : return("ERR_RECURSIVE_STACK_OVERFLOW"                                       );    //   4004
      case ERR_NOT_ENOUGH_STACK_FOR_PARAM                                                 : return("ERR_NOT_ENOUGH_STACK_FOR_PARAM"                                     );    //   4005
      case ERR_NO_MEMORY_FOR_PARAM_STRING                                                 : return("ERR_NO_MEMORY_FOR_PARAM_STRING"                                     );    //   4006
      case ERR_NO_MEMORY_FOR_TEMP_STRING                                                  : return("ERR_NO_MEMORY_FOR_TEMP_STRING"                                      );    //   4007
      case ERR_NOT_INITIALIZED_STRING                                                     : return("ERR_NOT_INITIALIZED_STRING"                                         );    //   4008
      case ERR_NOT_INITIALIZED_ARRAYSTRING                                                : return("ERR_NOT_INITIALIZED_ARRAYSTRING"                                    );    //   4009
      case ERR_NO_MEMORY_FOR_ARRAYSTRING                                                  : return("ERR_NO_MEMORY_FOR_ARRAYSTRING"                                      );    //   4010
      case ERR_TOO_LONG_STRING                                                            : return("ERR_TOO_LONG_STRING"                                                );    //   4011
      case ERR_REMAINDER_FROM_ZERO_DIVIDE                                                 : return("ERR_REMAINDER_FROM_ZERO_DIVIDE"                                     );    //   4012
      case ERR_ZERO_DIVIDE                                                                : return("ERR_ZERO_DIVIDE"                                                    );    //   4013
      case ERR_UNKNOWN_COMMAND                                                            : return("ERR_UNKNOWN_COMMAND"                                                );    //   4014
      case ERR_WRONG_JUMP                                                                 : return("ERR_WRONG_JUMP"                                                     );    //   4015
      case ERR_NOT_INITIALIZED_ARRAY                                                      : return("ERR_NOT_INITIALIZED_ARRAY"                                          );    //   4016
      case ERR_DLL_CALLS_NOT_ALLOWED                                                      : return("ERR_DLL_CALLS_NOT_ALLOWED"                                          );    //   4017
      case ERR_CANNOT_LOAD_LIBRARY                                                        : return("ERR_CANNOT_LOAD_LIBRARY"                                            );    //   4018
      case ERR_CANNOT_CALL_FUNCTION                                                       : return("ERR_CANNOT_CALL_FUNCTION"                                           );    //   4019
      case ERR_EX4_CALLS_NOT_ALLOWED                                                      : return("ERR_EX4_CALLS_NOT_ALLOWED"                                          );    //   4020
      case ERR_NO_MEMORY_FOR_RETURNED_STR                                                 : return("ERR_NO_MEMORY_FOR_RETURNED_STR"                                     );    //   4021
      case ERR_SYSTEM_BUSY                                                                : return("ERR_SYSTEM_BUSY"                                                    );    //   4022
      case ERR_DLL_EXCEPTION                                                              : return("ERR_DLL_EXCEPTION"                                                  );    //   4023
      case ERR_INTERNAL_ERROR                                                             : return("ERR_INTERNAL_ERROR"                                                 );    //   4024
      case ERR_OUT_OF_MEMORY                                                              : return("ERR_OUT_OF_MEMORY"                                                  );    //   4025
      case ERR_INVALID_POINTER                                                            : return("ERR_INVALID_POINTER"                                                );    //   4026
      case ERR_FORMAT_TOO_MANY_FORMATTERS                                                 : return("ERR_FORMAT_TOO_MANY_FORMATTERS"                                     );    //   4027
      case ERR_FORMAT_TOO_MANY_PARAMETERS                                                 : return("ERR_FORMAT_TOO_MANY_PARAMETERS"                                     );    //   4028
      case ERR_ARRAY_INVALID                                                              : return("ERR_ARRAY_INVALID"                                                  );    //   4029
      case ERR_CHART_NOREPLY                                                              : return("ERR_CHART_NOREPLY"                                                  );    //   4030
      case ERR_INVALID_FUNCTION_PARAMSCNT                                                 : return("ERR_INVALID_FUNCTION_PARAMSCNT"                                     );    //   4050
      case ERR_INVALID_PARAMETER                                                          : return("ERR_INVALID_PARAMETER"                                              );    //   4051
      case ERR_STRING_FUNCTION_INTERNAL                                                   : return("ERR_STRING_FUNCTION_INTERNAL"                                       );    //   4052
      case ERR_ARRAY_ERROR                                                                : return("ERR_ARRAY_ERROR"                                                    );    //   4053
      case ERR_SERIES_NOT_AVAILABLE                                                       : return("ERR_SERIES_NOT_AVAILABLE"                                           );    //   4054
      case ERR_CUSTOM_INDICATOR_ERROR                                                     : return("ERR_CUSTOM_INDICATOR_ERROR"                                         );    //   4055
      case ERR_INCOMPATIBLE_ARRAY                                                         : return("ERR_INCOMPATIBLE_ARRAY"                                             );    //   4056
      case ERR_GLOBAL_VARIABLES_PROCESSING                                                : return("ERR_GLOBAL_VARIABLES_PROCESSING"                                    );    //   4057
      case ERR_GLOBAL_VARIABLE_NOT_FOUND                                                  : return("ERR_GLOBAL_VARIABLE_NOT_FOUND"                                      );    //   4058
      case ERR_FUNC_NOT_ALLOWED_IN_TESTER                                                 : return("ERR_FUNC_NOT_ALLOWED_IN_TESTER"                                     );    //   4059
      case ERR_FUNCTION_NOT_CONFIRMED                                                     : return("ERR_FUNCTION_NOT_CONFIRMED"                                         );    //   4060
      case ERR_SEND_MAIL_ERROR                                                            : return("ERR_SEND_MAIL_ERROR"                                                );    //   4061
      case ERR_STRING_PARAMETER_EXPECTED                                                  : return("ERR_STRING_PARAMETER_EXPECTED"                                      );    //   4062
      case ERR_INTEGER_PARAMETER_EXPECTED                                                 : return("ERR_INTEGER_PARAMETER_EXPECTED"                                     );    //   4063
      case ERR_DOUBLE_PARAMETER_EXPECTED                                                  : return("ERR_DOUBLE_PARAMETER_EXPECTED"                                      );    //   4064
      case ERR_ARRAY_AS_PARAMETER_EXPECTED                                                : return("ERR_ARRAY_AS_PARAMETER_EXPECTED"                                    );    //   4065
      case ERS_HISTORY_UPDATE                                                             : return("ERS_HISTORY_UPDATE"                                                 );    //   4066   non-terminating status
      case ERR_TRADE_ERROR                                                                : return("ERR_TRADE_ERROR"                                                    );    //   4067
      case ERR_RESOURCE_NOT_FOUND                                                         : return("ERR_RESOURCE_NOT_FOUND"                                             );    //   4068
      case ERR_RESOURCE_NOT_SUPPORTED                                                     : return("ERR_RESOURCE_NOT_SUPPORTED"                                         );    //   4069
      case ERR_RESOURCE_DUPLICATED                                                        : return("ERR_RESOURCE_DUPLICATED"                                            );    //   4070
      case ERR_INDICATOR_CANNOT_INIT                                                      : return("ERR_INDICATOR_CANNOT_INIT"                                          );    //   4071
      case ERR_INDICATOR_CANNOT_LOAD                                                      : return("ERR_INDICATOR_CANNOT_LOAD"                                          );    //   4072
      case ERR_NO_HISTORY_DATA                                                            : return("ERR_NO_HISTORY_DATA"                                                );    //   4073
      case ERR_NO_MEMORY_FOR_HISTORY                                                      : return("ERR_NO_MEMORY_FOR_HISTORY"                                          );    //   4074
      case ERR_NO_MEMORY_FOR_INDICATOR                                                    : return("ERR_NO_MEMORY_FOR_INDICATOR"                                        );    //   4075
      case ERR_END_OF_FILE                                                                : return("ERR_END_OF_FILE"                                                    );    //   4099
      case ERR_FILE_ERROR                                                                 : return("ERR_FILE_ERROR"                                                     );    //   4100
      case ERR_WRONG_FILE_NAME                                                            : return("ERR_WRONG_FILE_NAME"                                                );    //   4101
      case ERR_TOO_MANY_OPENED_FILES                                                      : return("ERR_TOO_MANY_OPENED_FILES"                                          );    //   4102
      case ERR_CANNOT_OPEN_FILE                                                           : return("ERR_CANNOT_OPEN_FILE"                                               );    //   4103
      case ERR_INCOMPATIBLE_FILEACCESS                                                    : return("ERR_INCOMPATIBLE_FILEACCESS"                                        );    //   4104
      case ERR_NO_TICKET_SELECTED                                                         : return("ERR_NO_TICKET_SELECTED"                                             );    //   4105
      case ERR_SYMBOL_NOT_AVAILABLE                                                       : return("ERR_SYMBOL_NOT_AVAILABLE"                                           );    //   4106
      case ERR_INVALID_PRICE_PARAM                                                        : return("ERR_INVALID_PRICE_PARAM"                                            );    //   4107
      case ERR_INVALID_TICKET                                                             : return("ERR_INVALID_TICKET"                                                 );    //   4108
      case ERR_TERMINAL_AUTOTRADE_DISABLED                                                : return("ERR_TERMINAL_AUTOTRADE_DISABLED"                                    );    //   4109
      case ERR_PROGRAM_LONGS_DISABLED                                                     : return("ERR_PROGRAM_LONGS_DISABLED"                                         );    //   4110
      case ERR_PROGRAM_SHORTS_DISABLED                                                    : return("ERR_PROGRAM_LONGS_DISABLED"                                         );    //   4111
      case ERR_BROKER_AUTOTRADE_DISABLED                                                  : return("ERR_BROKER_AUTOTRADE_DISABLED"                                      );    //   4112
      case ERR_OBJECT_ALREADY_EXISTS                                                      : return("ERR_OBJECT_ALREADY_EXISTS"                                          );    //   4200
      case ERR_UNKNOWN_OBJECT_PROPERTY                                                    : return("ERR_UNKNOWN_OBJECT_PROPERTY"                                        );    //   4201
      case ERR_OBJECT_DOES_NOT_EXIST                                                      : return("ERR_OBJECT_DOES_NOT_EXIST"                                          );    //   4202
      case ERR_UNKNOWN_OBJECT_TYPE                                                        : return("ERR_UNKNOWN_OBJECT_TYPE"                                            );    //   4203
      case ERR_NO_OBJECT_NAME                                                             : return("ERR_NO_OBJECT_NAME"                                                 );    //   4204
      case ERR_OBJECT_COORDINATES_ERROR                                                   : return("ERR_OBJECT_COORDINATES_ERROR"                                       );    //   4205
      case ERR_NO_SPECIFIED_SUBWINDOW                                                     : return("ERR_NO_SPECIFIED_SUBWINDOW"                                         );    //   4206
      case ERR_OBJECT_ERROR                                                               : return("ERR_OBJECT_ERROR"                                                   );    //   4207
      case ERR_CHART_PROP_INVALID                                                         : return("ERR_CHART_PROP_INVALID"                                             );    //   4210
      case ERR_CHART_NOT_FOUND                                                            : return("ERR_CHART_NOT_FOUND"                                                );    //   4211
      case ERR_CHARTWINDOW_NOT_FOUND                                                      : return("ERR_CHARTWINDOW_NOT_FOUND"                                          );    //   4212
      case ERR_CHARTINDICATOR_NOT_FOUND                                                   : return("ERR_CHARTINDICATOR_NOT_FOUND"                                       );    //   4213
      case ERR_SYMBOL_SELECT                                                              : return("ERR_SYMBOL_SELECT"                                                  );    //   4220
      case ERR_NOTIFICATION_SEND_ERROR                                                    : return("ERR_NOTIFICATION_SEND_ERROR"                                        );    //   4250
      case ERR_NOTIFICATION_PARAMETER                                                     : return("ERR_NOTIFICATION_PARAMETER"                                         );    //   4251
      case ERR_NOTIFICATION_SETTINGS                                                      : return("ERR_NOTIFICATION_SETTINGS"                                          );    //   4252
      case ERR_NOTIFICATION_TOO_FREQUENT                                                  : return("ERR_NOTIFICATION_TOO_FREQUENT"                                      );    //   4253
      case ERR_FTP_NOSERVER                                                               : return("ERR_FTP_NOSERVER"                                                   );    //   4260
      case ERR_FTP_NOLOGIN                                                                : return("ERR_FTP_NOLOGIN"                                                    );    //   4261
      case ERR_FTP_CONNECT_FAILED                                                         : return("ERR_FTP_CONNECT_FAILED"                                             );    //   4262
      case ERR_FTP_CLOSED                                                                 : return("ERR_FTP_CLOSED"                                                     );    //   4263
      case ERR_FTP_CHANGEDIR                                                              : return("ERR_FTP_CHANGEDIR"                                                  );    //   4264
      case ERR_FTP_FILE_ERROR                                                             : return("ERR_FTP_FILE_ERROR"                                                 );    //   4265
      case ERR_FTP_ERROR                                                                  : return("ERR_FTP_ERROR"                                                      );    //   4266
      case ERR_FILE_TOO_MANY_OPENED                                                       : return("ERR_FILE_TOO_MANY_OPENED"                                           );    //   5001
      case ERR_FILE_WRONG_FILENAME                                                        : return("ERR_FILE_WRONG_FILENAME"                                            );    //   5002
      case ERR_FILE_TOO_LONG_FILENAME                                                     : return("ERR_FILE_TOO_LONG_FILENAME"                                         );    //   5003
      case ERR_FILE_CANNOT_OPEN                                                           : return("ERR_FILE_CANNOT_OPEN"                                               );    //   5004
      case ERR_FILE_BUFFER_ALLOC_ERROR                                                    : return("ERR_FILE_BUFFER_ALLOC_ERROR"                                        );    //   5005
      case ERR_FILE_CANNOT_DELETE                                                         : return("ERR_FILE_CANNOT_DELETE"                                             );    //   5006
      case ERR_FILE_INVALID_HANDLE                                                        : return("ERR_FILE_INVALID_HANDLE"                                            );    //   5007
      case ERR_FILE_UNKNOWN_HANDLE                                                        : return("ERR_FILE_UNKNOWN_HANDLE"                                            );    //   5008
      case ERR_FILE_NOT_TOWRITE                                                           : return("ERR_FILE_NOT_TOWRITE"                                               );    //   5009
      case ERR_FILE_NOT_TOREAD                                                            : return("ERR_FILE_NOT_TOREAD"                                                );    //   5010
      case ERR_FILE_NOT_BIN                                                               : return("ERR_FILE_NOT_BIN"                                                   );    //   5011
      case ERR_FILE_NOT_TXT                                                               : return("ERR_FILE_NOT_TXT"                                                   );    //   5012
      case ERR_FILE_NOT_TXTORCSV                                                          : return("ERR_FILE_NOT_TXTORCSV"                                              );    //   5013
      case ERR_FILE_NOT_CSV                                                               : return("ERR_FILE_NOT_CSV"                                                   );    //   5014
      case ERR_FILE_READ_ERROR                                                            : return("ERR_FILE_READ_ERROR"                                                );    //   5015
      case ERR_FILE_WRITE_ERROR                                                           : return("ERR_FILE_WRITE_ERROR"                                               );    //   5016
      case ERR_FILE_BIN_STRINGSIZE                                                        : return("ERR_FILE_BIN_STRINGSIZE"                                            );    //   5017
      case ERR_FILE_INCOMPATIBLE                                                          : return("ERR_FILE_INCOMPATIBLE"                                              );    //   5018
      case ERR_FILE_IS_DIRECTORY                                                          : return("ERR_FILE_IS_DIRECTORY"                                              );    //   5019
      case ERR_FILE_NOT_FOUND                                                             : return("ERR_FILE_NOT_FOUND"                                                 );    //   5020
      case ERR_FILE_CANNOT_REWRITE                                                        : return("ERR_FILE_CANNOT_REWRITE"                                            );    //   5021
      case ERR_FILE_WRONG_DIRECTORYNAME                                                   : return("ERR_FILE_WRONG_DIRECTORYNAME"                                       );    //   5022
      case ERR_FILE_DIRECTORY_NOT_EXIST                                                   : return("ERR_FILE_DIRECTORY_NOT_EXIST"                                       );    //   5023
      case ERR_FILE_NOT_DIRECTORY                                                         : return("ERR_FILE_NOT_DIRECTORY"                                             );    //   5024
      case ERR_FILE_CANT_DELETE_DIRECTORY                                                 : return("ERR_FILE_CANT_DELETE_DIRECTORY"                                     );    //   5025
      case ERR_FILE_CANT_CLEAN_DIRECTORY                                                  : return("ERR_FILE_CANT_CLEAN_DIRECTORY"                                      );    //   5026
      case ERR_FILE_ARRAYRESIZE_ERROR                                                     : return("ERR_FILE_ARRAYRESIZE_ERROR"                                         );    //   5027
      case ERR_FILE_STRINGRESIZE_ERROR                                                    : return("ERR_FILE_STRINGRESIZE_ERROR"                                        );    //   5028
      case ERR_FILE_STRUCT_WITH_OBJECTS                                                   : return("ERR_FILE_STRUCT_WITH_OBJECTS"                                       );    //   5029
      case ERR_WEBREQUEST_INVALID_ADDRESS                                                 : return("ERR_WEBREQUEST_INVALID_ADDRESS"                                     );    //   5200
      case ERR_WEBREQUEST_CONNECT_FAILED                                                  : return("ERR_WEBREQUEST_CONNECT_FAILED"                                      );    //   5201
      case ERR_WEBREQUEST_TIMEOUT                                                         : return("ERR_WEBREQUEST_TIMEOUT"                                             );    //   5202
      case ERR_WEBREQUEST_REQUEST_FAILED                                                  : return("ERR_WEBREQUEST_REQUEST_FAILED"                                      );    //   5203

      // user defined errors: 65536-99999 (0x10000-0x1869F)
      case ERR_USER_ERROR_FIRST                                                           : return("ERR_USER_ERROR_FIRST"                                               );    //  65536
      case ERR_CANCELLED_BY_USER                                                          : return("ERR_CANCELLED_BY_USER"                                              );    //  65537
      case ERR_CONCURRENT_MODIFICATION                                                    : return("ERR_CONCURRENT_MODIFICATION"                                        );    //  65538
      case ERS_EXECUTION_STOPPING                                                         : return("ERS_EXECUTION_STOPPING"                                             );    //  65539   non-terminating status
      case ERR_FUNC_NOT_ALLOWED                                                           : return("ERR_FUNC_NOT_ALLOWED"                                               );    //  65540
      case ERR_HISTORY_INSUFFICIENT                                                       : return("ERR_HISTORY_INSUFFICIENT"                                           );    //  65541
      case ERR_ILLEGAL_STATE                                                              : return("ERR_ILLEGAL_STATE"                                                  );    //  65542
      case ERR_ACCESS_DENIED                                                              : return("ERR_ACCESS_DENIED"                                                  );    //  65543
      case ERR_INVALID_COMMAND                                                            : return("ERR_INVALID_COMMAND"                                                );    //  65544
      case ERR_INVALID_CONFIG_VALUE                                                       : return("ERR_INVALID_CONFIG_VALUE"                                           );    //  65545
      case ERR_INVALID_FILE_FORMAT                                                        : return("ERR_INVALID_FILE_FORMAT"                                            );    //  65546
      case ERR_INVALID_INPUT_PARAMETER                                                    : return("ERR_INVALID_INPUT_PARAMETER"                                        );    //  65547
      case ERR_INVALID_MARKET_DATA                                                        : return("ERR_INVALID_MARKET_DATA"                                            );    //  65548
      case ERR_INVALID_TIMEZONE_CONFIG                                                    : return("ERR_INVALID_TIMEZONE_CONFIG"                                        );    //  65549
      case ERR_MIXED_SYMBOLS                                                              : return("ERR_MIXED_SYMBOLS"                                                  );    //  65550
      case ERR_NOT_IMPLEMENTED                                                            : return("ERR_NOT_IMPLEMENTED"                                                );    //  65551
      case ERR_ORDER_CHANGED                                                              : return("ERR_ORDER_CHANGED"                                                  );    //  65552
      case ERR_RUNTIME_ERROR                                                              : return("ERR_RUNTIME_ERROR"                                                  );    //  65553
      case ERR_TERMINAL_INIT_FAILURE                                                      : return("ERR_TERMINAL_INIT_FAILURE"                                          );    //  65554
      case ERS_TERMINAL_NOT_YET_READY                                                     : return("ERS_TERMINAL_NOT_YET_READY"                                         );    //  65555   non-terminating status
      case ERR_TOTAL_POSITION_NOT_FLAT                                                    : return("ERR_TOTAL_POSITION_NOT_FLAT"                                        );    //  65556
      case ERR_UNDEFINED_STATE                                                            : return("ERR_UNDEFINED_STATE"                                                );    //  65557
      case ERR_STOP_DISTANCE_VIOLATED                                                     : return("ERR_STOP_DISTANCE_VIOLATED"                                         );    //  65558
      case ERR_MARGIN_STOPOUT                                                             : return("ERR_MARGIN_STOPOUT"                                                 );    //  65559

      // Win32 error codes (for error descriptions see FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), ...))
      case ERR_WIN32_ERROR                                                                : return("win32:NO_ERROR"                                                     );    // 100000 +    0
      case ERR_WIN32_ERROR + ERROR_INVALID_FUNCTION                                       : return("win32:ERROR_INVALID_FUNCTION"                                       );    // 100000 +    1
      case ERR_WIN32_ERROR + ERROR_FILE_NOT_FOUND                                         : return("win32:ERROR_FILE_NOT_FOUND"                                         );    // 100000 +    2
      case ERR_WIN32_ERROR + ERROR_PATH_NOT_FOUND                                         : return("win32:ERROR_PATH_NOT_FOUND"                                         );    // 100000 +    3
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_OPEN_FILES                                    : return("win32:ERROR_TOO_MANY_OPEN_FILES"                                    );    // 100000 +    4
      case ERR_WIN32_ERROR + ERROR_ACCESS_DENIED                                          : return("win32:ERROR_ACCESS_DENIED"                                          );    // 100000 +    5
      case ERR_WIN32_ERROR + ERROR_INVALID_HANDLE                                         : return("win32:ERROR_INVALID_HANDLE"                                         );    // 100000 +    6
      case ERR_WIN32_ERROR + ERROR_ARENA_TRASHED                                          : return("win32:ERROR_ARENA_TRASHED"                                          );    // 100000 +    7
      case ERR_WIN32_ERROR + ERROR_NOT_ENOUGH_MEMORY                                      : return("win32:ERROR_NOT_ENOUGH_MEMORY"                                      );    // 100000 +    8
      case ERR_WIN32_ERROR + ERROR_INVALID_BLOCK                                          : return("win32:ERROR_INVALID_BLOCK"                                          );    // 100000 +    9
      case ERR_WIN32_ERROR + ERROR_BAD_ENVIRONMENT                                        : return("win32:ERROR_BAD_ENVIRONMENT"                                        );    // 100000 +   10
      case ERR_WIN32_ERROR + ERROR_BAD_FORMAT                                             : return("win32:ERROR_BAD_FORMAT"                                             );    // 100000 +   11
      case ERR_WIN32_ERROR + ERROR_INVALID_ACCESS                                         : return("win32:ERROR_INVALID_ACCESS"                                         );    // 100000 +   12
      case ERR_WIN32_ERROR + ERROR_INVALID_DATA                                           : return("win32:ERROR_INVALID_DATA"                                           );    // 100000 +   13
      case ERR_WIN32_ERROR + ERROR_OUTOFMEMORY                                            : return("win32:ERROR_OUTOFMEMORY"                                            );    // 100000 +   14
      case ERR_WIN32_ERROR + ERROR_INVALID_DRIVE                                          : return("win32:ERROR_INVALID_DRIVE"                                          );    // 100000 +   15
      case ERR_WIN32_ERROR + ERROR_CURRENT_DIRECTORY                                      : return("win32:ERROR_CURRENT_DIRECTORY"                                      );    // 100000 +   16
      case ERR_WIN32_ERROR + ERROR_NOT_SAME_DEVICE                                        : return("win32:ERROR_NOT_SAME_DEVICE"                                        );    // 100000 +   17
      case ERR_WIN32_ERROR + ERROR_NO_MORE_FILES                                          : return("win32:ERROR_NO_MORE_FILES"                                          );    // 100000 +   18
      case ERR_WIN32_ERROR + ERROR_WRITE_PROTECT                                          : return("win32:ERROR_WRITE_PROTECT"                                          );    // 100000 +   19
      case ERR_WIN32_ERROR + ERROR_BAD_UNIT                                               : return("win32:ERROR_BAD_UNIT"                                               );    // 100000 +   20
      case ERR_WIN32_ERROR + ERROR_NOT_READY                                              : return("win32:ERROR_NOT_READY"                                              );    // 100000 +   21
      case ERR_WIN32_ERROR + ERROR_BAD_COMMAND                                            : return("win32:ERROR_BAD_COMMAND"                                            );    // 100000 +   22
      case ERR_WIN32_ERROR + ERROR_CRC                                                    : return("win32:ERROR_CRC"                                                    );    // 100000 +   23
      case ERR_WIN32_ERROR + ERROR_BAD_LENGTH                                             : return("win32:ERROR_BAD_LENGTH"                                             );    // 100000 +   24
      case ERR_WIN32_ERROR + ERROR_SEEK                                                   : return("win32:ERROR_SEEK"                                                   );    // 100000 +   25
      case ERR_WIN32_ERROR + ERROR_NOT_DOS_DISK                                           : return("win32:ERROR_NOT_DOS_DISK"                                           );    // 100000 +   26
      case ERR_WIN32_ERROR + ERROR_SECTOR_NOT_FOUND                                       : return("win32:ERROR_SECTOR_NOT_FOUND"                                       );    // 100000 +   27
      case ERR_WIN32_ERROR + ERROR_OUT_OF_PAPER                                           : return("win32:ERROR_OUT_OF_PAPER"                                           );    // 100000 +   28
      case ERR_WIN32_ERROR + ERROR_WRITE_FAULT                                            : return("win32:ERROR_WRITE_FAULT"                                            );    // 100000 +   29
      case ERR_WIN32_ERROR + ERROR_READ_FAULT                                             : return("win32:ERROR_READ_FAULT"                                             );    // 100000 +   30
      case ERR_WIN32_ERROR + ERROR_GEN_FAILURE                                            : return("win32:ERROR_GEN_FAILURE"                                            );    // 100000 +   31
      case ERR_WIN32_ERROR + ERROR_SHARING_VIOLATION                                      : return("win32:ERROR_SHARING_VIOLATION"                                      );    // 100000 +   32
      case ERR_WIN32_ERROR + ERROR_LOCK_VIOLATION                                         : return("win32:ERROR_LOCK_VIOLATION"                                         );    // 100000 +   33
      case ERR_WIN32_ERROR + ERROR_WRONG_DISK                                             : return("win32:ERROR_WRONG_DISK"                                             );    // 100000 +   34
      // ...
      case ERR_WIN32_ERROR + ERROR_SHARING_BUFFER_EXCEEDED                                : return("win32:ERROR_SHARING_BUFFER_EXCEEDED"                                );    // 100000 +   36
      // ...
      case ERR_WIN32_ERROR + ERROR_HANDLE_EOF                                             : return("win32:ERROR_HANDLE_EOF"                                             );    // 100000 +   38
      case ERR_WIN32_ERROR + ERROR_HANDLE_DISK_FULL                                       : return("win32:ERROR_HANDLE_DISK_FULL"                                       );    // 100000 +   39
      // ...
      case ERR_WIN32_ERROR + ERROR_NOT_SUPPORTED                                          : return("win32:ERROR_NOT_SUPPORTED"                                          );    // 100000 +   50
      case ERR_WIN32_ERROR + ERROR_REM_NOT_LIST                                           : return("win32:ERROR_REM_NOT_LIST"                                           );    // 100000 +   51
      case ERR_WIN32_ERROR + ERROR_DUP_NAME                                               : return("win32:ERROR_DUP_NAME"                                               );    // 100000 +   52
      case ERR_WIN32_ERROR + ERROR_BAD_NETPATH                                            : return("win32:ERROR_BAD_NETPATH"                                            );    // 100000 +   53
      case ERR_WIN32_ERROR + ERROR_NETWORK_BUSY                                           : return("win32:ERROR_NETWORK_BUSY"                                           );    // 100000 +   54
      case ERR_WIN32_ERROR + ERROR_DEV_NOT_EXIST                                          : return("win32:ERROR_DEV_NOT_EXIST"                                          );    // 100000 +   55
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_CMDS                                          : return("win32:ERROR_TOO_MANY_CMDS"                                          );    // 100000 +   56
      case ERR_WIN32_ERROR + ERROR_ADAP_HDW_ERR                                           : return("win32:ERROR_ADAP_HDW_ERR"                                           );    // 100000 +   57
      case ERR_WIN32_ERROR + ERROR_BAD_NET_RESP                                           : return("win32:ERROR_BAD_NET_RESP"                                           );    // 100000 +   58
      case ERR_WIN32_ERROR + ERROR_UNEXP_NET_ERR                                          : return("win32:ERROR_UNEXP_NET_ERR"                                          );    // 100000 +   59
      case ERR_WIN32_ERROR + ERROR_BAD_REM_ADAP                                           : return("win32:ERROR_BAD_REM_ADAP"                                           );    // 100000 +   60
      case ERR_WIN32_ERROR + ERROR_PRINTQ_FULL                                            : return("win32:ERROR_PRINTQ_FULL"                                            );    // 100000 +   61
      case ERR_WIN32_ERROR + ERROR_NO_SPOOL_SPACE                                         : return("win32:ERROR_NO_SPOOL_SPACE"                                         );    // 100000 +   62
      case ERR_WIN32_ERROR + ERROR_PRINT_CANCELLED                                        : return("win32:ERROR_PRINT_CANCELLED"                                        );    // 100000 +   63
      case ERR_WIN32_ERROR + ERROR_NETNAME_DELETED                                        : return("win32:ERROR_NETNAME_DELETED"                                        );    // 100000 +   64
      case ERR_WIN32_ERROR + ERROR_NETWORK_ACCESS_DENIED                                  : return("win32:ERROR_NETWORK_ACCESS_DENIED"                                  );    // 100000 +   65
      case ERR_WIN32_ERROR + ERROR_BAD_DEV_TYPE                                           : return("win32:ERROR_BAD_DEV_TYPE"                                           );    // 100000 +   66
      case ERR_WIN32_ERROR + ERROR_BAD_NET_NAME                                           : return("win32:ERROR_BAD_NET_NAME"                                           );    // 100000 +   67
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_NAMES                                         : return("win32:ERROR_TOO_MANY_NAMES"                                         );    // 100000 +   68
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_SESS                                          : return("win32:ERROR_TOO_MANY_SESS"                                          );    // 100000 +   69
      case ERR_WIN32_ERROR + ERROR_SHARING_PAUSED                                         : return("win32:ERROR_SHARING_PAUSED"                                         );    // 100000 +   70
      case ERR_WIN32_ERROR + ERROR_REQ_NOT_ACCEP                                          : return("win32:ERROR_REQ_NOT_ACCEP"                                          );    // 100000 +   71
      case ERR_WIN32_ERROR + ERROR_REDIR_PAUSED                                           : return("win32:ERROR_REDIR_PAUSED"                                           );    // 100000 +   72
      // ...
      case ERR_WIN32_ERROR + ERROR_FILE_EXISTS                                            : return("win32:ERROR_FILE_EXISTS"                                            );    // 100000 +   80
      // ...
      case ERR_WIN32_ERROR + ERROR_CANNOT_MAKE                                            : return("win32:ERROR_CANNOT_MAKE"                                            );    // 100000 +   82
      case ERR_WIN32_ERROR + ERROR_FAIL_I24                                               : return("win32:ERROR_FAIL_I24"                                               );    // 100000 +   83
      case ERR_WIN32_ERROR + ERROR_OUT_OF_STRUCTURES                                      : return("win32:ERROR_OUT_OF_STRUCTURES"                                      );    // 100000 +   84
      case ERR_WIN32_ERROR + ERROR_ALREADY_ASSIGNED                                       : return("win32:ERROR_ALREADY_ASSIGNED"                                       );    // 100000 +   85
      case ERR_WIN32_ERROR + ERROR_INVALID_PASSWORD                                       : return("win32:ERROR_INVALID_PASSWORD"                                       );    // 100000 +   86
      case ERR_WIN32_ERROR + ERROR_INVALID_PARAMETER                                      : return("win32:ERROR_INVALID_PARAMETER"                                      );    // 100000 +   87
      case ERR_WIN32_ERROR + ERROR_NET_WRITE_FAULT                                        : return("win32:ERROR_NET_WRITE_FAULT"                                        );    // 100000 +   88
      case ERR_WIN32_ERROR + ERROR_NO_PROC_SLOTS                                          : return("win32:ERROR_NO_PROC_SLOTS"                                          );    // 100000 +   89
      // ...
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_SEMAPHORES                                    : return("win32:ERROR_TOO_MANY_SEMAPHORES"                                    );    // 100000 +  100
      case ERR_WIN32_ERROR + ERROR_EXCL_SEM_ALREADY_OWNED                                 : return("win32:ERROR_EXCL_SEM_ALREADY_OWNED"                                 );    // 100000 +  101
      case ERR_WIN32_ERROR + ERROR_SEM_IS_SET                                             : return("win32:ERROR_SEM_IS_SET"                                             );    // 100000 +  102
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_SEM_REQUESTS                                  : return("win32:ERROR_TOO_MANY_SEM_REQUESTS"                                  );    // 100000 +  103
      case ERR_WIN32_ERROR + ERROR_INVALID_AT_INTERRUPT_TIME                              : return("win32:ERROR_INVALID_AT_INTERRUPT_TIME"                              );    // 100000 +  104
      case ERR_WIN32_ERROR + ERROR_SEM_OWNER_DIED                                         : return("win32:ERROR_SEM_OWNER_DIED"                                         );    // 100000 +  105
      case ERR_WIN32_ERROR + ERROR_SEM_USER_LIMIT                                         : return("win32:ERROR_SEM_USER_LIMIT"                                         );    // 100000 +  106
      case ERR_WIN32_ERROR + ERROR_DISK_CHANGE                                            : return("win32:ERROR_DISK_CHANGE"                                            );    // 100000 +  107
      case ERR_WIN32_ERROR + ERROR_DRIVE_LOCKED                                           : return("win32:ERROR_DRIVE_LOCKED"                                           );    // 100000 +  108
      case ERR_WIN32_ERROR + ERROR_BROKEN_PIPE                                            : return("win32:ERROR_BROKEN_PIPE"                                            );    // 100000 +  109
      case ERR_WIN32_ERROR + ERROR_OPEN_FAILED                                            : return("win32:ERROR_OPEN_FAILED"                                            );    // 100000 +  110
      case ERR_WIN32_ERROR + ERROR_BUFFER_OVERFLOW                                        : return("win32:ERROR_BUFFER_OVERFLOW"                                        );    // 100000 +  111
      case ERR_WIN32_ERROR + ERROR_DISK_FULL                                              : return("win32:ERROR_DISK_FULL"                                              );    // 100000 +  112
      case ERR_WIN32_ERROR + ERROR_NO_MORE_SEARCH_HANDLES                                 : return("win32:ERROR_NO_MORE_SEARCH_HANDLES"                                 );    // 100000 +  113
      case ERR_WIN32_ERROR + ERROR_INVALID_TARGET_HANDLE                                  : return("win32:ERROR_INVALID_TARGET_HANDLE"                                  );    // 100000 +  114
      // ...
      case ERR_WIN32_ERROR + ERROR_INVALID_CATEGORY                                       : return("win32:ERROR_INVALID_CATEGORY"                                       );    // 100000 +  117
      case ERR_WIN32_ERROR + ERROR_INVALID_VERIFY_SWITCH                                  : return("win32:ERROR_INVALID_VERIFY_SWITCH"                                  );    // 100000 +  118
      case ERR_WIN32_ERROR + ERROR_BAD_DRIVER_LEVEL                                       : return("win32:ERROR_BAD_DRIVER_LEVEL"                                       );    // 100000 +  119
      case ERR_WIN32_ERROR + ERROR_CALL_NOT_IMPLEMENTED                                   : return("win32:ERROR_CALL_NOT_IMPLEMENTED"                                   );    // 100000 +  120
      case ERR_WIN32_ERROR + ERROR_SEM_TIMEOUT                                            : return("win32:ERROR_SEM_TIMEOUT"                                            );    // 100000 +  121
      case ERR_WIN32_ERROR + ERROR_INSUFFICIENT_BUFFER                                    : return("win32:ERROR_INSUFFICIENT_BUFFER"                                    );    // 100000 +  122
      case ERR_WIN32_ERROR + ERROR_INVALID_NAME                                           : return("win32:ERROR_INVALID_NAME"                                           );    // 100000 +  123
      case ERR_WIN32_ERROR + ERROR_INVALID_LEVEL                                          : return("win32:ERROR_INVALID_LEVEL"                                          );    // 100000 +  124
      case ERR_WIN32_ERROR + ERROR_NO_VOLUME_LABEL                                        : return("win32:ERROR_NO_VOLUME_LABEL"                                        );    // 100000 +  125
      case ERR_WIN32_ERROR + ERROR_MOD_NOT_FOUND                                          : return("win32:ERROR_MOD_NOT_FOUND"                                          );    // 100000 +  126
      case ERR_WIN32_ERROR + ERROR_PROC_NOT_FOUND                                         : return("win32:ERROR_PROC_NOT_FOUND"                                         );    // 100000 +  127
      case ERR_WIN32_ERROR + ERROR_WAIT_NO_CHILDREN                                       : return("win32:ERROR_WAIT_NO_CHILDREN"                                       );    // 100000 +  128
      case ERR_WIN32_ERROR + ERROR_CHILD_NOT_COMPLETE                                     : return("win32:ERROR_CHILD_NOT_COMPLETE"                                     );    // 100000 +  129
      case ERR_WIN32_ERROR + ERROR_DIRECT_ACCESS_HANDLE                                   : return("win32:ERROR_DIRECT_ACCESS_HANDLE"                                   );    // 100000 +  130
      case ERR_WIN32_ERROR + ERROR_NEGATIVE_SEEK                                          : return("win32:ERROR_NEGATIVE_SEEK"                                          );    // 100000 +  131
      case ERR_WIN32_ERROR + ERROR_SEEK_ON_DEVICE                                         : return("win32:ERROR_SEEK_ON_DEVICE"                                         );    // 100000 +  132
      case ERR_WIN32_ERROR + ERROR_IS_JOIN_TARGET                                         : return("win32:ERROR_IS_JOIN_TARGET"                                         );    // 100000 +  133
      case ERR_WIN32_ERROR + ERROR_IS_JOINED                                              : return("win32:ERROR_IS_JOINED"                                              );    // 100000 +  134
      case ERR_WIN32_ERROR + ERROR_IS_SUBSTED                                             : return("win32:ERROR_IS_SUBSTED"                                             );    // 100000 +  135
      case ERR_WIN32_ERROR + ERROR_NOT_JOINED                                             : return("win32:ERROR_NOT_JOINED"                                             );    // 100000 +  136
      case ERR_WIN32_ERROR + ERROR_NOT_SUBSTED                                            : return("win32:ERROR_NOT_SUBSTED"                                            );    // 100000 +  137
      case ERR_WIN32_ERROR + ERROR_JOIN_TO_JOIN                                           : return("win32:ERROR_JOIN_TO_JOIN"                                           );    // 100000 +  138
      case ERR_WIN32_ERROR + ERROR_SUBST_TO_SUBST                                         : return("win32:ERROR_SUBST_TO_SUBST"                                         );    // 100000 +  139
      case ERR_WIN32_ERROR + ERROR_JOIN_TO_SUBST                                          : return("win32:ERROR_JOIN_TO_SUBST"                                          );    // 100000 +  140
      case ERR_WIN32_ERROR + ERROR_SUBST_TO_JOIN                                          : return("win32:ERROR_SUBST_TO_JOIN"                                          );    // 100000 +  141
      case ERR_WIN32_ERROR + ERROR_BUSY_DRIVE                                             : return("win32:ERROR_BUSY_DRIVE"                                             );    // 100000 +  142
      case ERR_WIN32_ERROR + ERROR_SAME_DRIVE                                             : return("win32:ERROR_SAME_DRIVE"                                             );    // 100000 +  143
      case ERR_WIN32_ERROR + ERROR_DIR_NOT_ROOT                                           : return("win32:ERROR_DIR_NOT_ROOT"                                           );    // 100000 +  144
      case ERR_WIN32_ERROR + ERROR_DIR_NOT_EMPTY                                          : return("win32:ERROR_DIR_NOT_EMPTY"                                          );    // 100000 +  145
      case ERR_WIN32_ERROR + ERROR_IS_SUBST_PATH                                          : return("win32:ERROR_IS_SUBST_PATH"                                          );    // 100000 +  146
      case ERR_WIN32_ERROR + ERROR_IS_JOIN_PATH                                           : return("win32:ERROR_IS_JOIN_PATH"                                           );    // 100000 +  147
      case ERR_WIN32_ERROR + ERROR_PATH_BUSY                                              : return("win32:ERROR_PATH_BUSY"                                              );    // 100000 +  148
      case ERR_WIN32_ERROR + ERROR_IS_SUBST_TARGET                                        : return("win32:ERROR_IS_SUBST_TARGET"                                        );    // 100000 +  149
      case ERR_WIN32_ERROR + ERROR_SYSTEM_TRACE                                           : return("win32:ERROR_SYSTEM_TRACE"                                           );    // 100000 +  150
      case ERR_WIN32_ERROR + ERROR_INVALID_EVENT_COUNT                                    : return("win32:ERROR_INVALID_EVENT_COUNT"                                    );    // 100000 +  151
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_MUXWAITERS                                    : return("win32:ERROR_TOO_MANY_MUXWAITERS"                                    );    // 100000 +  152
      case ERR_WIN32_ERROR + ERROR_INVALID_LIST_FORMAT                                    : return("win32:ERROR_INVALID_LIST_FORMAT"                                    );    // 100000 +  153
      case ERR_WIN32_ERROR + ERROR_LABEL_TOO_LONG                                         : return("win32:ERROR_LABEL_TOO_LONG"                                         );    // 100000 +  154
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_TCBS                                          : return("win32:ERROR_TOO_MANY_TCBS"                                          );    // 100000 +  155
      case ERR_WIN32_ERROR + ERROR_SIGNAL_REFUSED                                         : return("win32:ERROR_SIGNAL_REFUSED"                                         );    // 100000 +  156
      case ERR_WIN32_ERROR + ERROR_DISCARDED                                              : return("win32:ERROR_DISCARDED"                                              );    // 100000 +  157
      case ERR_WIN32_ERROR + ERROR_NOT_LOCKED                                             : return("win32:ERROR_NOT_LOCKED"                                             );    // 100000 +  158
      case ERR_WIN32_ERROR + ERROR_BAD_THREADID_ADDR                                      : return("win32:ERROR_BAD_THREADID_ADDR"                                      );    // 100000 +  159
      case ERR_WIN32_ERROR + ERROR_BAD_ARGUMENTS                                          : return("win32:ERROR_BAD_ARGUMENTS"                                          );    // 100000 +  160
      case ERR_WIN32_ERROR + ERROR_BAD_PATHNAME                                           : return("win32:ERROR_BAD_PATHNAME"                                           );    // 100000 +  161
      case ERR_WIN32_ERROR + ERROR_SIGNAL_PENDING                                         : return("win32:ERROR_SIGNAL_PENDING"                                         );    // 100000 +  162
      // ...
      case ERR_WIN32_ERROR + ERROR_MAX_THRDS_REACHED                                      : return("win32:ERROR_MAX_THRDS_REACHED"                                      );    // 100000 +  164
      // ...
      case ERR_WIN32_ERROR + ERROR_LOCK_FAILED                                            : return("win32:ERROR_LOCK_FAILED"                                            );    // 100000 +  167
      // ...
      case ERR_WIN32_ERROR + ERROR_BUSY                                                   : return("win32:ERROR_BUSY"                                                   );    // 100000 +  170
      case ERR_WIN32_ERROR + ERROR_DEVICE_SUPPORT_IN_PROGRESS                             : return("win32:ERROR_DEVICE_SUPPORT_IN_PROGRESS"                             );    // 100000 +  171
      // ...
      case ERR_WIN32_ERROR + ERROR_CANCEL_VIOLATION                                       : return("win32:ERROR_CANCEL_VIOLATION"                                       );    // 100000 +  173
      case ERR_WIN32_ERROR + ERROR_ATOMIC_LOCKS_NOT_SUPPORTED                             : return("win32:ERROR_ATOMIC_LOCKS_NOT_SUPPORTED"                             );    // 100000 +  174
      // ...
      case ERR_WIN32_ERROR + ERROR_INVALID_SEGMENT_NUMBER                                 : return("win32:ERROR_INVALID_SEGMENT_NUMBER"                                 );    // 100000 +  180
      // ...
      case ERR_WIN32_ERROR + ERROR_INVALID_ORDINAL                                        : return("win32:ERROR_INVALID_ORDINAL"                                        );    // 100000 +  182
      case ERR_WIN32_ERROR + ERROR_ALREADY_EXISTS                                         : return("win32:ERROR_ALREADY_EXISTS"                                         );    // 100000 +  183
      // ...
      case ERR_WIN32_ERROR + ERROR_INVALID_FLAG_NUMBER                                    : return("win32:ERROR_INVALID_FLAG_NUMBER"                                    );    // 100000 +  186
      case ERR_WIN32_ERROR + ERROR_SEM_NOT_FOUND                                          : return("win32:ERROR_SEM_NOT_FOUND"                                          );    // 100000 +  187
      case ERR_WIN32_ERROR + ERROR_INVALID_STARTING_CODESEG                               : return("win32:ERROR_INVALID_STARTING_CODESEG"                               );    // 100000 +  188
      case ERR_WIN32_ERROR + ERROR_INVALID_STACKSEG                                       : return("win32:ERROR_INVALID_STACKSEG"                                       );    // 100000 +  189
      case ERR_WIN32_ERROR + ERROR_INVALID_MODULETYPE                                     : return("win32:ERROR_INVALID_MODULETYPE"                                     );    // 100000 +  190
      case ERR_WIN32_ERROR + ERROR_INVALID_EXE_SIGNATURE                                  : return("win32:ERROR_INVALID_EXE_SIGNATURE"                                  );    // 100000 +  191
      case ERR_WIN32_ERROR + ERROR_EXE_MARKED_INVALID                                     : return("win32:ERROR_EXE_MARKED_INVALID"                                     );    // 100000 +  192
      case ERR_WIN32_ERROR + ERROR_BAD_EXE_FORMAT                                         : return("win32:ERROR_BAD_EXE_FORMAT"                                         );    // 100000 +  193
      case ERR_WIN32_ERROR + ERROR_ITERATED_DATA_EXCEEDS_64k                              : return("win32:ERROR_ITERATED_DATA_EXCEEDS_64k"                              );    // 100000 +  194
      case ERR_WIN32_ERROR + ERROR_INVALID_MINALLOCSIZE                                   : return("win32:ERROR_INVALID_MINALLOCSIZE"                                   );    // 100000 +  195
      case ERR_WIN32_ERROR + ERROR_DYNLINK_FROM_INVALID_RING                              : return("win32:ERROR_DYNLINK_FROM_INVALID_RING"                              );    // 100000 +  196
      case ERR_WIN32_ERROR + ERROR_IOPL_NOT_ENABLED                                       : return("win32:ERROR_IOPL_NOT_ENABLED"                                       );    // 100000 +  197
      case ERR_WIN32_ERROR + ERROR_INVALID_SEGDPL                                         : return("win32:ERROR_INVALID_SEGDPL"                                         );    // 100000 +  198
      case ERR_WIN32_ERROR + ERROR_AUTODATASEG_EXCEEDS_64k                                : return("win32:ERROR_AUTODATASEG_EXCEEDS_64k"                                );    // 100000 +  199
      case ERR_WIN32_ERROR + ERROR_RING2SEG_MUST_BE_MOVABLE                               : return("win32:ERROR_RING2SEG_MUST_BE_MOVABLE"                               );    // 100000 +  200
      case ERR_WIN32_ERROR + ERROR_RELOC_CHAIN_XEEDS_SEGLIM                               : return("win32:ERROR_RELOC_CHAIN_XEEDS_SEGLIM"                               );    // 100000 +  201
      case ERR_WIN32_ERROR + ERROR_INFLOOP_IN_RELOC_CHAIN                                 : return("win32:ERROR_INFLOOP_IN_RELOC_CHAIN"                                 );    // 100000 +  202
      case ERR_WIN32_ERROR + ERROR_ENVVAR_NOT_FOUND                                       : return("win32:ERROR_ENVVAR_NOT_FOUND"                                       );    // 100000 +  203
      // ...
      case ERR_WIN32_ERROR + ERROR_NO_SIGNAL_SENT                                         : return("win32:ERROR_NO_SIGNAL_SENT"                                         );    // 100000 +  205
      case ERR_WIN32_ERROR + ERROR_FILENAME_EXCED_RANGE                                   : return("win32:ERROR_FILENAME_EXCED_RANGE"                                   );    // 100000 +  206
      case ERR_WIN32_ERROR + ERROR_RING2_STACK_IN_USE                                     : return("win32:ERROR_RING2_STACK_IN_USE"                                     );    // 100000 +  207
      case ERR_WIN32_ERROR + ERROR_META_EXPANSION_TOO_LONG                                : return("win32:ERROR_META_EXPANSION_TOO_LONG"                                );    // 100000 +  208
      case ERR_WIN32_ERROR + ERROR_INVALID_SIGNAL_NUMBER                                  : return("win32:ERROR_INVALID_SIGNAL_NUMBER"                                  );    // 100000 +  209
      case ERR_WIN32_ERROR + ERROR_THREAD_1_INACTIVE                                      : return("win32:ERROR_THREAD_1_INACTIVE"                                      );    // 100000 +  210
      // ...
      case ERR_WIN32_ERROR + ERROR_LOCKED                                                 : return("win32:ERROR_LOCKED"                                                 );    // 100000 +  212
      // ...
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_MODULES                                       : return("win32:ERROR_TOO_MANY_MODULES"                                       );    // 100000 +  214
      case ERR_WIN32_ERROR + ERROR_NESTING_NOT_ALLOWED                                    : return("win32:ERROR_NESTING_NOT_ALLOWED"                                    );    // 100000 +  215
      case ERR_WIN32_ERROR + ERROR_EXE_MACHINE_TYPE_MISMATCH                              : return("win32:ERROR_EXE_MACHINE_TYPE_MISMATCH"                              );    // 100000 +  216
      case ERR_WIN32_ERROR + ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY                        : return("win32:ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY"                        );    // 100000 +  217
      case ERR_WIN32_ERROR + ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY                 : return("win32:ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY"                 );    // 100000 +  218
      // ...
      case ERR_WIN32_ERROR + ERROR_FILE_CHECKED_OUT                                       : return("win32:ERROR_FILE_CHECKED_OUT"                                       );    // 100000 +  220
      case ERR_WIN32_ERROR + ERROR_CHECKOUT_REQUIRED                                      : return("win32:ERROR_CHECKOUT_REQUIRED"                                      );    // 100000 +  221
      case ERR_WIN32_ERROR + ERROR_BAD_FILE_TYPE                                          : return("win32:ERROR_BAD_FILE_TYPE"                                          );    // 100000 +  222
      case ERR_WIN32_ERROR + ERROR_FILE_TOO_LARGE                                         : return("win32:ERROR_FILE_TOO_LARGE"                                         );    // 100000 +  223
      case ERR_WIN32_ERROR + ERROR_FORMS_AUTH_REQUIRED                                    : return("win32:ERROR_FORMS_AUTH_REQUIRED"                                    );    // 100000 +  224
      case ERR_WIN32_ERROR + ERROR_VIRUS_INFECTED                                         : return("win32:ERROR_VIRUS_INFECTED"                                         );    // 100000 +  225
      case ERR_WIN32_ERROR + ERROR_VIRUS_DELETED                                          : return("win32:ERROR_VIRUS_DELETED"                                          );    // 100000 +  226
      // ...
      case ERR_WIN32_ERROR + ERROR_PIPE_LOCAL                                             : return("win32:ERROR_PIPE_LOCAL"                                             );    // 100000 +  229
      case ERR_WIN32_ERROR + ERROR_BAD_PIPE                                               : return("win32:ERROR_BAD_PIPE"                                               );    // 100000 +  230
      case ERR_WIN32_ERROR + ERROR_PIPE_BUSY                                              : return("win32:ERROR_PIPE_BUSY"                                              );    // 100000 +  231
      case ERR_WIN32_ERROR + ERROR_NO_DATA                                                : return("win32:ERROR_NO_DATA"                                                );    // 100000 +  232
      case ERR_WIN32_ERROR + ERROR_PIPE_NOT_CONNECTED                                     : return("win32:ERROR_PIPE_NOT_CONNECTED"                                     );    // 100000 +  233
      case ERR_WIN32_ERROR + ERROR_MORE_DATA                                              : return("win32:ERROR_MORE_DATA"                                              );    // 100000 +  234
      // ...
      case ERR_WIN32_ERROR + ERROR_VC_DISCONNECTED                                        : return("win32:ERROR_VC_DISCONNECTED"                                        );    // 100000 +  240
      // ...
      case ERR_WIN32_ERROR + ERROR_INVALID_EA_NAME                                        : return("win32:ERROR_INVALID_EA_NAME"                                        );    // 100000 +  254
      case ERR_WIN32_ERROR + ERROR_EA_LIST_INCONSISTENT                                   : return("win32:ERROR_EA_LIST_INCONSISTENT"                                   );    // 100000 +  255
      // ...
      case ERR_WIN32_ERROR + WAIT_TIMEOUT                                                 : return("win32:WAIT_TIMEOUT"                                                 );    // 100000 +  258
      case ERR_WIN32_ERROR + ERROR_NO_MORE_ITEMS                                          : return("win32:ERROR_NO_MORE_ITEMS"                                          );    // 100000 +  259
      // ...
      case ERR_WIN32_ERROR + ERROR_CANNOT_COPY                                            : return("win32:ERROR_CANNOT_COPY"                                            );    // 100000 +  266
      case ERR_WIN32_ERROR + ERROR_DIRECTORY                                              : return("win32:ERROR_DIRECTORY"                                              );    // 100000 +  267
      // ...
      case ERR_WIN32_ERROR + ERROR_EAS_DIDNT_FIT                                          : return("win32:ERROR_EAS_DIDNT_FIT"                                          );    // 100000 +  275
      case ERR_WIN32_ERROR + ERROR_EA_FILE_CORRUPT                                        : return("win32:ERROR_EA_FILE_CORRUPT"                                        );    // 100000 +  276
      case ERR_WIN32_ERROR + ERROR_EA_TABLE_FULL                                          : return("win32:ERROR_EA_TABLE_FULL"                                          );    // 100000 +  277
      case ERR_WIN32_ERROR + ERROR_INVALID_EA_HANDLE                                      : return("win32:ERROR_INVALID_EA_HANDLE"                                      );    // 100000 +  278
      // ...
      case ERR_WIN32_ERROR + ERROR_EAS_NOT_SUPPORTED                                      : return("win32:ERROR_EAS_NOT_SUPPORTED"                                      );    // 100000 +  282
      // ...
      case ERR_WIN32_ERROR + ERROR_NOT_OWNER                                              : return("win32:ERROR_NOT_OWNER"                                              );    // 100000 +  288
      // ...
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_POSTS                                         : return("win32:ERROR_TOO_MANY_POSTS"                                         );    // 100000 +  298
      case ERR_WIN32_ERROR + ERROR_PARTIAL_COPY                                           : return("win32:ERROR_PARTIAL_COPY"                                           );    // 100000 +  299
      case ERR_WIN32_ERROR + ERROR_OPLOCK_NOT_GRANTED                                     : return("win32:ERROR_OPLOCK_NOT_GRANTED"                                     );    // 100000 +  300
      case ERR_WIN32_ERROR + ERROR_INVALID_OPLOCK_PROTOCOL                                : return("win32:ERROR_INVALID_OPLOCK_PROTOCOL"                                );    // 100000 +  301
      case ERR_WIN32_ERROR + ERROR_DISK_TOO_FRAGMENTED                                    : return("win32:ERROR_DISK_TOO_FRAGMENTED"                                    );    // 100000 +  302
      case ERR_WIN32_ERROR + ERROR_DELETE_PENDING                                         : return("win32:ERROR_DELETE_PENDING"                                         );    // 100000 +  303
      case ERR_WIN32_ERROR + ERROR_INCOMPATIBLE_WITH_GLOBAL_SHORT_NAME_REGISTRY_SETTING   : return("win32:ERROR_INCOMPATIBLE_WITH_GLOBAL_SHORT_NAME_REGISTRY_SETTING"   );    // 100000 +  304
      case ERR_WIN32_ERROR + ERROR_SHORT_NAMES_NOT_ENABLED_ON_VOLUME                      : return("win32:ERROR_SHORT_NAMES_NOT_ENABLED_ON_VOLUME"                      );    // 100000 +  305
      case ERR_WIN32_ERROR + ERROR_SECURITY_STREAM_IS_INCONSISTENT                        : return("win32:ERROR_SECURITY_STREAM_IS_INCONSISTENT"                        );    // 100000 +  306
      case ERR_WIN32_ERROR + ERROR_INVALID_LOCK_RANGE                                     : return("win32:ERROR_INVALID_LOCK_RANGE"                                     );    // 100000 +  307
      case ERR_WIN32_ERROR + ERROR_IMAGE_SUBSYSTEM_NOT_PRESENT                            : return("win32:ERROR_IMAGE_SUBSYSTEM_NOT_PRESENT"                            );    // 100000 +  308
      case ERR_WIN32_ERROR + ERROR_NOTIFICATION_GUID_ALREADY_DEFINED                      : return("win32:ERROR_NOTIFICATION_GUID_ALREADY_DEFINED"                      );    // 100000 +  309
      case ERR_WIN32_ERROR + ERROR_INVALID_EXCEPTION_HANDLER                              : return("win32:ERROR_INVALID_EXCEPTION_HANDLER"                              );    // 100000 +  310
      case ERR_WIN32_ERROR + ERROR_DUPLICATE_PRIVILEGES                                   : return("win32:ERROR_DUPLICATE_PRIVILEGES"                                   );    // 100000 +  311
      case ERR_WIN32_ERROR + ERROR_NO_RANGES_PROCESSED                                    : return("win32:ERROR_NO_RANGES_PROCESSED"                                    );    // 100000 +  312
      case ERR_WIN32_ERROR + ERROR_NOT_ALLOWED_ON_SYSTEM_FILE                             : return("win32:ERROR_NOT_ALLOWED_ON_SYSTEM_FILE"                             );    // 100000 +  313
      case ERR_WIN32_ERROR + ERROR_DISK_RESOURCES_EXHAUSTED                               : return("win32:ERROR_DISK_RESOURCES_EXHAUSTED"                               );    // 100000 +  314
      case ERR_WIN32_ERROR + ERROR_INVALID_TOKEN                                          : return("win32:ERROR_INVALID_TOKEN"                                          );    // 100000 +  315
      case ERR_WIN32_ERROR + ERROR_DEVICE_FEATURE_NOT_SUPPORTED                           : return("win32:ERROR_DEVICE_FEATURE_NOT_SUPPORTED"                           );    // 100000 +  316
      case ERR_WIN32_ERROR + ERROR_MR_MID_NOT_FOUND                                       : return("win32:ERROR_MR_MID_NOT_FOUND"                                       );    // 100000 +  317
      case ERR_WIN32_ERROR + ERROR_SCOPE_NOT_FOUND                                        : return("win32:ERROR_SCOPE_NOT_FOUND"                                        );    // 100000 +  318
      case ERR_WIN32_ERROR + ERROR_UNDEFINED_SCOPE                                        : return("win32:ERROR_UNDEFINED_SCOPE"                                        );    // 100000 +  319
      case ERR_WIN32_ERROR + ERROR_INVALID_CAP                                            : return("win32:ERROR_INVALID_CAP"                                            );    // 100000 +  320
      case ERR_WIN32_ERROR + ERROR_DEVICE_UNREACHABLE                                     : return("win32:ERROR_DEVICE_UNREACHABLE"                                     );    // 100000 +  321
      case ERR_WIN32_ERROR + ERROR_DEVICE_NO_RESOURCES                                    : return("win32:ERROR_DEVICE_NO_RESOURCES"                                    );    // 100000 +  322
      case ERR_WIN32_ERROR + ERROR_DATA_CHECKSUM_ERROR                                    : return("win32:ERROR_DATA_CHECKSUM_ERROR"                                    );    // 100000 +  323
      case ERR_WIN32_ERROR + ERROR_INTERMIXED_KERNEL_EA_OPERATION                         : return("win32:ERROR_INTERMIXED_KERNEL_EA_OPERATION"                         );    // 100000 +  324
      // ...
      case ERR_WIN32_ERROR + ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED                          : return("win32:ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED"                          );    // 100000 +  326
      case ERR_WIN32_ERROR + ERROR_OFFSET_ALIGNMENT_VIOLATION                             : return("win32:ERROR_OFFSET_ALIGNMENT_VIOLATION"                             );    // 100000 +  327
      case ERR_WIN32_ERROR + ERROR_INVALID_FIELD_IN_PARAMETER_LIST                        : return("win32:ERROR_INVALID_FIELD_IN_PARAMETER_LIST"                        );    // 100000 +  328
      case ERR_WIN32_ERROR + ERROR_OPERATION_IN_PROGRESS                                  : return("win32:ERROR_OPERATION_IN_PROGRESS"                                  );    // 100000 +  329
      case ERR_WIN32_ERROR + ERROR_BAD_DEVICE_PATH                                        : return("win32:ERROR_BAD_DEVICE_PATH"                                        );    // 100000 +  330
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_DESCRIPTORS                                   : return("win32:ERROR_TOO_MANY_DESCRIPTORS"                                   );    // 100000 +  331
      case ERR_WIN32_ERROR + ERROR_SCRUB_DATA_DISABLED                                    : return("win32:ERROR_SCRUB_DATA_DISABLED"                                    );    // 100000 +  332
      case ERR_WIN32_ERROR + ERROR_NOT_REDUNDANT_STORAGE                                  : return("win32:ERROR_NOT_REDUNDANT_STORAGE"                                  );    // 100000 +  333
      case ERR_WIN32_ERROR + ERROR_RESIDENT_FILE_NOT_SUPPORTED                            : return("win32:ERROR_RESIDENT_FILE_NOT_SUPPORTED"                            );    // 100000 +  334
      case ERR_WIN32_ERROR + ERROR_COMPRESSED_FILE_NOT_SUPPORTED                          : return("win32:ERROR_COMPRESSED_FILE_NOT_SUPPORTED"                          );    // 100000 +  335
      case ERR_WIN32_ERROR + ERROR_DIRECTORY_NOT_SUPPORTED                                : return("win32:ERROR_DIRECTORY_NOT_SUPPORTED"                                );    // 100000 +  336
      case ERR_WIN32_ERROR + ERROR_NOT_READ_FROM_COPY                                     : return("win32:ERROR_NOT_READ_FROM_COPY"                                     );    // 100000 +  337
      // ...
      case ERR_WIN32_ERROR + ERROR_FAIL_NOACTION_REBOOT                                   : return("win32:ERROR_FAIL_NOACTION_REBOOT"                                   );    // 100000 +  350
      case ERR_WIN32_ERROR + ERROR_FAIL_SHUTDOWN                                          : return("win32:ERROR_FAIL_SHUTDOWN"                                          );    // 100000 +  351
      case ERR_WIN32_ERROR + ERROR_FAIL_RESTART                                           : return("win32:ERROR_FAIL_RESTART"                                           );    // 100000 +  352
      case ERR_WIN32_ERROR + ERROR_MAX_SESSIONS_REACHED                                   : return("win32:ERROR_MAX_SESSIONS_REACHED"                                   );    // 100000 +  353
      // ...
      case ERR_WIN32_ERROR + ERROR_THREAD_MODE_ALREADY_BACKGROUND                         : return("win32:ERROR_THREAD_MODE_ALREADY_BACKGROUND"                         );    // 100000 +  400
      case ERR_WIN32_ERROR + ERROR_THREAD_MODE_NOT_BACKGROUND                             : return("win32:ERROR_THREAD_MODE_NOT_BACKGROUND"                             );    // 100000 +  401
      case ERR_WIN32_ERROR + ERROR_PROCESS_MODE_ALREADY_BACKGROUND                        : return("win32:ERROR_PROCESS_MODE_ALREADY_BACKGROUND"                        );    // 100000 +  402
      case ERR_WIN32_ERROR + ERROR_PROCESS_MODE_NOT_BACKGROUND                            : return("win32:ERROR_PROCESS_MODE_NOT_BACKGROUND"                            );    // 100000 +  403
      // ...
      case ERR_WIN32_ERROR + ERROR_INVALID_ADDRESS                                        : return("win32:ERROR_INVALID_ADDRESS"                                        );    // 100000 +  487
      // ...
      case ERR_WIN32_ERROR + ERROR_USER_PROFILE_LOAD                                      : return("win32:ERROR_USER_PROFILE_LOAD"                                      );    // 100000 +  500
      case ERR_WIN32_ERROR + ERROR_ARITHMETIC_OVERFLOW                                    : return("win32:ERROR_ARITHMETIC_OVERFLOW"                                    );    // 100000 +  534
      case ERR_WIN32_ERROR + ERROR_PIPE_CONNECTED                                         : return("win32:ERROR_PIPE_CONNECTED"                                         );    // 100000 +  535
      case ERR_WIN32_ERROR + ERROR_PIPE_LISTENING                                         : return("win32:ERROR_PIPE_LISTENING"                                         );    // 100000 +  536
      case ERR_WIN32_ERROR + ERROR_VERIFIER_STOP                                          : return("win32:ERROR_VERIFIER_STOP"                                          );    // 100000 +  537
      case ERR_WIN32_ERROR + ERROR_ABIOS_ERROR                                            : return("win32:ERROR_ABIOS_ERROR"                                            );    // 100000 +  538
      case ERR_WIN32_ERROR + ERROR_WX86_WARNING                                           : return("win32:ERROR_WX86_WARNING"                                           );    // 100000 +  539
      case ERR_WIN32_ERROR + ERROR_WX86_ERROR                                             : return("win32:ERROR_WX86_ERROR"                                             );    // 100000 +  540
      case ERR_WIN32_ERROR + ERROR_TIMER_NOT_CANCELED                                     : return("win32:ERROR_TIMER_NOT_CANCELED"                                     );    // 100000 +  541
      case ERR_WIN32_ERROR + ERROR_UNWIND                                                 : return("win32:ERROR_UNWIND"                                                 );    // 100000 +  542
      case ERR_WIN32_ERROR + ERROR_BAD_STACK                                              : return("win32:ERROR_BAD_STACK"                                              );    // 100000 +  543
      case ERR_WIN32_ERROR + ERROR_INVALID_UNWIND_TARGET                                  : return("win32:ERROR_INVALID_UNWIND_TARGET"                                  );    // 100000 +  544
      case ERR_WIN32_ERROR + ERROR_INVALID_PORT_ATTRIBUTES                                : return("win32:ERROR_INVALID_PORT_ATTRIBUTES"                                );    // 100000 +  545
      case ERR_WIN32_ERROR + ERROR_PORT_MESSAGE_TOO_LONG                                  : return("win32:ERROR_PORT_MESSAGE_TOO_LONG"                                  );    // 100000 +  546
      case ERR_WIN32_ERROR + ERROR_INVALID_QUOTA_LOWER                                    : return("win32:ERROR_INVALID_QUOTA_LOWER"                                    );    // 100000 +  547
      case ERR_WIN32_ERROR + ERROR_DEVICE_ALREADY_ATTACHED                                : return("win32:ERROR_DEVICE_ALREADY_ATTACHED"                                );    // 100000 +  548
      case ERR_WIN32_ERROR + ERROR_INSTRUCTION_MISALIGNMENT                               : return("win32:ERROR_INSTRUCTION_MISALIGNMENT"                               );    // 100000 +  549
      case ERR_WIN32_ERROR + ERROR_PROFILING_NOT_STARTED                                  : return("win32:ERROR_PROFILING_NOT_STARTED"                                  );    // 100000 +  550
      case ERR_WIN32_ERROR + ERROR_PROFILING_NOT_STOPPED                                  : return("win32:ERROR_PROFILING_NOT_STOPPED"                                  );    // 100000 +  551
      case ERR_WIN32_ERROR + ERROR_COULD_NOT_INTERPRET                                    : return("win32:ERROR_COULD_NOT_INTERPRET"                                    );    // 100000 +  552
      case ERR_WIN32_ERROR + ERROR_PROFILING_AT_LIMIT                                     : return("win32:ERROR_PROFILING_AT_LIMIT"                                     );    // 100000 +  553
      case ERR_WIN32_ERROR + ERROR_CANT_WAIT                                              : return("win32:ERROR_CANT_WAIT"                                              );    // 100000 +  554
      case ERR_WIN32_ERROR + ERROR_CANT_TERMINATE_SELF                                    : return("win32:ERROR_CANT_TERMINATE_SELF"                                    );    // 100000 +  555
      case ERR_WIN32_ERROR + ERROR_UNEXPECTED_MM_CREATE_ERR                               : return("win32:ERROR_UNEXPECTED_MM_CREATE_ERR"                               );    // 100000 +  556
      case ERR_WIN32_ERROR + ERROR_UNEXPECTED_MM_MAP_ERROR                                : return("win32:ERROR_UNEXPECTED_MM_MAP_ERROR"                                );    // 100000 +  557
      case ERR_WIN32_ERROR + ERROR_UNEXPECTED_MM_EXTEND_ERR                               : return("win32:ERROR_UNEXPECTED_MM_EXTEND_ERR"                               );    // 100000 +  558
      case ERR_WIN32_ERROR + ERROR_BAD_FUNCTION_TABLE                                     : return("win32:ERROR_BAD_FUNCTION_TABLE"                                     );    // 100000 +  559
      case ERR_WIN32_ERROR + ERROR_NO_GUID_TRANSLATION                                    : return("win32:ERROR_NO_GUID_TRANSLATION"                                    );    // 100000 +  560
      case ERR_WIN32_ERROR + ERROR_INVALID_LDT_SIZE                                       : return("win32:ERROR_INVALID_LDT_SIZE"                                       );    // 100000 +  561
      case ERR_WIN32_ERROR + ERROR_INVALID_LDT_OFFSET                                     : return("win32:ERROR_INVALID_LDT_OFFSET"                                     );    // 100000 +  563
      case ERR_WIN32_ERROR + ERROR_INVALID_LDT_DESCRIPTOR                                 : return("win32:ERROR_INVALID_LDT_DESCRIPTOR"                                 );    // 100000 +  564
      case ERR_WIN32_ERROR + ERROR_TOO_MANY_THREADS                                       : return("win32:ERROR_TOO_MANY_THREADS"                                       );    // 100000 +  565
      case ERR_WIN32_ERROR + ERROR_THREAD_NOT_IN_PROCESS                                  : return("win32:ERROR_THREAD_NOT_IN_PROCESS"                                  );    // 100000 +  566
      case ERR_WIN32_ERROR + ERROR_PAGEFILE_QUOTA_EXCEEDED                                : return("win32:ERROR_PAGEFILE_QUOTA_EXCEEDED"                                );    // 100000 +  567
      case ERR_WIN32_ERROR + ERROR_LOGON_SERVER_CONFLICT                                  : return("win32:ERROR_LOGON_SERVER_CONFLICT"                                  );    // 100000 +  568
      case ERR_WIN32_ERROR + ERROR_SYNCHRONIZATION_REQUIRED                               : return("win32:ERROR_SYNCHRONIZATION_REQUIRED"                               );    // 100000 +  569
      case ERR_WIN32_ERROR + ERROR_NET_OPEN_FAILED                                        : return("win32:ERROR_NET_OPEN_FAILED"                                        );    // 100000 +  570
      case ERR_WIN32_ERROR + ERROR_IO_PRIVILEGE_FAILED                                    : return("win32:ERROR_IO_PRIVILEGE_FAILED"                                    );    // 100000 +  571
      case ERR_WIN32_ERROR + ERROR_CONTROL_C_EXIT                                         : return("win32:ERROR_CONTROL_C_EXIT"                                         );    // 100000 +  572
      case ERR_WIN32_ERROR + ERROR_MISSING_SYSTEMFILE                                     : return("win32:ERROR_MISSING_SYSTEMFILE"                                     );    // 100000 +  573
      case ERR_WIN32_ERROR + ERROR_UNHANDLED_EXCEPTION                                    : return("win32:ERROR_UNHANDLED_EXCEPTION"                                    );    // 100000 +  574
      case ERR_WIN32_ERROR + ERROR_APP_INIT_FAILURE                                       : return("win32:ERROR_APP_INIT_FAILURE"                                       );    // 100000 +  575
      case ERR_WIN32_ERROR + ERROR_PAGEFILE_CREATE_FAILED                                 : return("win32:ERROR_PAGEFILE_CREATE_FAILED"                                 );    // 100000 +  576
      case ERR_WIN32_ERROR + ERROR_INVALID_IMAGE_HASH                                     : return("win32:ERROR_INVALID_IMAGE_HASH"                                     );    // 100000 +  577
      case ERR_WIN32_ERROR + ERROR_NO_PAGEFILE                                            : return("win32:ERROR_NO_PAGEFILE"                                            );    // 100000 +  578
      case ERR_WIN32_ERROR + ERROR_ILLEGAL_FLOAT_CONTEXT                                  : return("win32:ERROR_ILLEGAL_FLOAT_CONTEXT"                                  );    // 100000 +  579
      case ERR_WIN32_ERROR + ERROR_NO_EVENT_PAIR                                          : return("win32:ERROR_NO_EVENT_PAIR"                                          );    // 100000 +  580
      case ERR_WIN32_ERROR + ERROR_DOMAIN_CTRLR_CONFIG_ERROR                              : return("win32:ERROR_DOMAIN_CTRLR_CONFIG_ERROR"                              );    // 100000 +  581
      case ERR_WIN32_ERROR + ERROR_ILLEGAL_CHARACTER                                      : return("win32:ERROR_ILLEGAL_CHARACTER"                                      );    // 100000 +  582
      case ERR_WIN32_ERROR + ERROR_UNDEFINED_CHARACTER                                    : return("win32:ERROR_UNDEFINED_CHARACTER"                                    );    // 100000 +  583
      case ERR_WIN32_ERROR + ERROR_FLOPPY_VOLUME                                          : return("win32:ERROR_FLOPPY_VOLUME"                                          );    // 100000 +  584
      case ERR_WIN32_ERROR + ERROR_BIOS_FAILED_TO_CONNECT_INTERRUPT                       : return("win32:ERROR_BIOS_FAILED_TO_CONNECT_INTERRUPT"                       );    // 100000 +  585
      case ERR_WIN32_ERROR + ERROR_BACKUP_CONTROLLER                                      : return("win32:ERROR_BACKUP_CONTROLLER"                                      );    // 100000 +  586
      case ERR_WIN32_ERROR + ERROR_MUTANT_LIMIT_EXCEEDED                                  : return("win32:ERROR_MUTANT_LIMIT_EXCEEDED"                                  );    // 100000 +  587
      case ERR_WIN32_ERROR + ERROR_FS_DRIVER_REQUIRED                                     : return("win32:ERROR_FS_DRIVER_REQUIRED"                                     );    // 100000 +  588
      case ERR_WIN32_ERROR + ERROR_CANNOT_LOAD_REGISTRY_FILE                              : return("win32:ERROR_CANNOT_LOAD_REGISTRY_FILE"                              );    // 100000 +  589
      case ERR_WIN32_ERROR + ERROR_DEBUG_ATTACH_FAILED                                    : return("win32:ERROR_DEBUG_ATTACH_FAILED"                                    );    // 100000 +  590
      case ERR_WIN32_ERROR + ERROR_SYSTEM_PROCESS_TERMINATED                              : return("win32:ERROR_SYSTEM_PROCESS_TERMINATED"                              );    // 100000 +  591
      case ERR_WIN32_ERROR + ERROR_DATA_NOT_ACCEPTED                                      : return("win32:ERROR_DATA_NOT_ACCEPTED"                                      );    // 100000 +  592
      case ERR_WIN32_ERROR + ERROR_VDM_HARD_ERROR                                         : return("win32:ERROR_VDM_HARD_ERROR"                                         );    // 100000 +  593
      case ERR_WIN32_ERROR + ERROR_DRIVER_CANCEL_TIMEOUT                                  : return("win32:ERROR_DRIVER_CANCEL_TIMEOUT"                                  );    // 100000 +  594
      case ERR_WIN32_ERROR + ERROR_REPLY_MESSAGE_MISMATCH                                 : return("win32:ERROR_REPLY_MESSAGE_MISMATCH"                                 );    // 100000 +  595
      case ERR_WIN32_ERROR + ERROR_LOST_WRITEBEHIND_DATA                                  : return("win32:ERROR_LOST_WRITEBEHIND_DATA"                                  );    // 100000 +  596
      case ERR_WIN32_ERROR + ERROR_CLIENT_SERVER_PARAMETERS_INVALID                       : return("win32:ERROR_CLIENT_SERVER_PARAMETERS_INVALID"                       );    // 100000 +  597
      case ERR_WIN32_ERROR + ERROR_NOT_TINY_STREAM                                        : return("win32:ERROR_NOT_TINY_STREAM"                                        );    // 100000 +  598
      case ERR_WIN32_ERROR + ERROR_STACK_OVERFLOW_READ                                    : return("win32:ERROR_STACK_OVERFLOW_READ"                                    );    // 100000 +  599
      case ERR_WIN32_ERROR + ERROR_CONVERT_TO_LARGE                                       : return("win32:ERROR_CONVERT_TO_LARGE"                                       );    // 100000 +  600
      case ERR_WIN32_ERROR + ERROR_FOUND_OUT_OF_SCOPE                                     : return("win32:ERROR_FOUND_OUT_OF_SCOPE"                                     );    // 100000 +  601
      case ERR_WIN32_ERROR + ERROR_ALLOCATE_BUCKET                                        : return("win32:ERROR_ALLOCATE_BUCKET"                                        );    // 100000 +  602
      case ERR_WIN32_ERROR + ERROR_MARSHALL_OVERFLOW                                      : return("win32:ERROR_MARSHALL_OVERFLOW"                                      );    // 100000 +  603
      case ERR_WIN32_ERROR + ERROR_INVALID_VARIANT                                        : return("win32:ERROR_INVALID_VARIANT"                                        );    // 100000 +  604
      case ERR_WIN32_ERROR + ERROR_BAD_COMPRESSION_BUFFER                                 : return("win32:ERROR_BAD_COMPRESSION_BUFFER"                                 );    // 100000 +  605
      case ERR_WIN32_ERROR + ERROR_AUDIT_FAILED                                           : return("win32:ERROR_AUDIT_FAILED"                                           );    // 100000 +  606
      case ERR_WIN32_ERROR + ERROR_TIMER_RESOLUTION_NOT_SET                               : return("win32:ERROR_TIMER_RESOLUTION_NOT_SET"                               );    // 100000 +  607
      case ERR_WIN32_ERROR + ERROR_INSUFFICIENT_LOGON_INFO                                : return("win32:ERROR_INSUFFICIENT_LOGON_INFO"                                );    // 100000 +  608
      case ERR_WIN32_ERROR + ERROR_BAD_DLL_ENTRYPOINT                                     : return("win32:ERROR_BAD_DLL_ENTRYPOINT"                                     );    // 100000 +  609
      case ERR_WIN32_ERROR + ERROR_BAD_SERVICE_ENTRYPOINT                                 : return("win32:ERROR_BAD_SERVICE_ENTRYPOINT"                                 );    // 100000 +  610
      case ERR_WIN32_ERROR + ERROR_IP_ADDRESS_CONFLICT1                                   : return("win32:ERROR_IP_ADDRESS_CONFLICT1"                                   );    // 100000 +  611
      case ERR_WIN32_ERROR + ERROR_IP_ADDRESS_CONFLICT2                                   : return("win32:ERROR_IP_ADDRESS_CONFLICT2"                                   );    // 100000 +  612
      case ERR_WIN32_ERROR + ERROR_REGISTRY_QUOTA_LIMIT                                   : return("win32:ERROR_REGISTRY_QUOTA_LIMIT"                                   );    // 100000 +  613
      case ERR_WIN32_ERROR + ERROR_NO_CALLBACK_ACTIVE                                     : return("win32:ERROR_NO_CALLBACK_ACTIVE"                                     );    // 100000 +  614
      case ERR_WIN32_ERROR + ERROR_PWD_TOO_SHORT                                          : return("win32:ERROR_PWD_TOO_SHORT"                                          );    // 100000 +  615
      case ERR_WIN32_ERROR + ERROR_PWD_TOO_RECENT                                         : return("win32:ERROR_PWD_TOO_RECENT"                                         );    // 100000 +  616
      case ERR_WIN32_ERROR + ERROR_PWD_HISTORY_CONFLICT                                   : return("win32:ERROR_PWD_HISTORY_CONFLICT"                                   );    // 100000 +  617
      case ERR_WIN32_ERROR + ERROR_UNSUPPORTED_COMPRESSION                                : return("win32:ERROR_UNSUPPORTED_COMPRESSION"                                );    // 100000 +  618
      case ERR_WIN32_ERROR + ERROR_INVALID_HW_PROFILE                                     : return("win32:ERROR_INVALID_HW_PROFILE"                                     );    // 100000 +  619
      case ERR_WIN32_ERROR + ERROR_INVALID_PLUGPLAY_DEVICE_PATH                           : return("win32:ERROR_INVALID_PLUGPLAY_DEVICE_PATH"                           );    // 100000 +  620
      case ERR_WIN32_ERROR + ERROR_QUOTA_LIST_INCONSISTENT                                : return("win32:ERROR_QUOTA_LIST_INCONSISTENT"                                );    // 100000 +  621
      case ERR_WIN32_ERROR + ERROR_EVALUATION_EXPIRATION                                  : return("win32:ERROR_EVALUATION_EXPIRATION"                                  );    // 100000 +  622
      case ERR_WIN32_ERROR + ERROR_ILLEGAL_DLL_RELOCATION                                 : return("win32:ERROR_ILLEGAL_DLL_RELOCATION"                                 );    // 100000 +  623
      case ERR_WIN32_ERROR + ERROR_DLL_INIT_FAILED_LOGOFF                                 : return("win32:ERROR_DLL_INIT_FAILED_LOGOFF"                                 );    // 100000 +  624
      case ERR_WIN32_ERROR + ERROR_VALIDATE_CONTINUE                                      : return("win32:ERROR_VALIDATE_CONTINUE"                                      );    // 100000 +  625
      case ERR_WIN32_ERROR + ERROR_NO_MORE_MATCHES                                        : return("win32:ERROR_NO_MORE_MATCHES"                                        );    // 100000 +  626
      case ERR_WIN32_ERROR + ERROR_RANGE_LIST_CONFLICT                                    : return("win32:ERROR_RANGE_LIST_CONFLICT"                                    );    // 100000 +  627
      case ERR_WIN32_ERROR + ERROR_SERVER_SID_MISMATCH                                    : return("win32:ERROR_SERVER_SID_MISMATCH"                                    );    // 100000 +  628
      case ERR_WIN32_ERROR + ERROR_CANT_ENABLE_DENY_ONLY                                  : return("win32:ERROR_CANT_ENABLE_DENY_ONLY"                                  );    // 100000 +  629
      case ERR_WIN32_ERROR + ERROR_FLOAT_MULTIPLE_FAULTS                                  : return("win32:ERROR_FLOAT_MULTIPLE_FAULTS"                                  );    // 100000 +  630
      case ERR_WIN32_ERROR + ERROR_FLOAT_MULTIPLE_TRAPS                                   : return("win32:ERROR_FLOAT_MULTIPLE_TRAPS"                                   );    // 100000 +  631
      case ERR_WIN32_ERROR + ERROR_NOINTERFACE                                            : return("win32:ERROR_NOINTERFACE"                                            );    // 100000 +  632
      case ERR_WIN32_ERROR + ERROR_DRIVER_FAILED_SLEEP                                    : return("win32:ERROR_DRIVER_FAILED_SLEEP"                                    );    // 100000 +  633
      case ERR_WIN32_ERROR + ERROR_CORRUPT_SYSTEM_FILE                                    : return("win32:ERROR_CORRUPT_SYSTEM_FILE"                                    );    // 100000 +  634
      case ERR_WIN32_ERROR + ERROR_COMMITMENT_MINIMUM                                     : return("win32:ERROR_COMMITMENT_MINIMUM"                                     );    // 100000 +  635
      case ERR_WIN32_ERROR + ERROR_PNP_RESTART_ENUMERATION                                : return("win32:ERROR_PNP_RESTART_ENUMERATION"                                );    // 100000 +  636
      case ERR_WIN32_ERROR + ERROR_SYSTEM_IMAGE_BAD_SIGNATURE                             : return("win32:ERROR_SYSTEM_IMAGE_BAD_SIGNATURE"                             );    // 100000 +  637
      case ERR_WIN32_ERROR + ERROR_PNP_REBOOT_REQUIRED                                    : return("win32:ERROR_PNP_REBOOT_REQUIRED"                                    );    // 100000 +  638
      case ERR_WIN32_ERROR + ERROR_INSUFFICIENT_POWER                                     : return("win32:ERROR_INSUFFICIENT_POWER"                                     );    // 100000 +  639
      case ERR_WIN32_ERROR + ERROR_MULTIPLE_FAULT_VIOLATION                               : return("win32:ERROR_MULTIPLE_FAULT_VIOLATION"                               );    // 100000 +  640
      case ERR_WIN32_ERROR + ERROR_SYSTEM_SHUTDOWN                                        : return("win32:ERROR_SYSTEM_SHUTDOWN"                                        );    // 100000 +  641
      case ERR_WIN32_ERROR + ERROR_PORT_NOT_SET                                           : return("win32:ERROR_PORT_NOT_SET"                                           );    // 100000 +  642
      case ERR_WIN32_ERROR + ERROR_DS_VERSION_CHECK_FAILURE                               : return("win32:ERROR_DS_VERSION_CHECK_FAILURE"                               );    // 100000 +  643
      case ERR_WIN32_ERROR + ERROR_RANGE_NOT_FOUND                                        : return("win32:ERROR_RANGE_NOT_FOUND"                                        );    // 100000 +  644
      case ERR_WIN32_ERROR + ERROR_NOT_SAFE_MODE_DRIVER                                   : return("win32:ERROR_NOT_SAFE_MODE_DRIVER"                                   );    // 100000 +  646
      case ERR_WIN32_ERROR + ERROR_FAILED_DRIVER_ENTRY                                    : return("win32:ERROR_FAILED_DRIVER_ENTRY"                                    );    // 100000 +  647
      case ERR_WIN32_ERROR + ERROR_DEVICE_ENUMERATION_ERROR                               : return("win32:ERROR_DEVICE_ENUMERATION_ERROR"                               );    // 100000 +  648
      case ERR_WIN32_ERROR + ERROR_MOUNT_POINT_NOT_RESOLVED                               : return("win32:ERROR_MOUNT_POINT_NOT_RESOLVED"                               );    // 100000 +  649
      case ERR_WIN32_ERROR + ERROR_INVALID_DEVICE_OBJECT_PARAMETER                        : return("win32:ERROR_INVALID_DEVICE_OBJECT_PARAMETER"                        );    // 100000 +  650
      case ERR_WIN32_ERROR + ERROR_MCA_OCCURED                                            : return("win32:ERROR_MCA_OCCURED"                                            );    // 100000 +  651
      case ERR_WIN32_ERROR + ERROR_DRIVER_DATABASE_ERROR                                  : return("win32:ERROR_DRIVER_DATABASE_ERROR"                                  );    // 100000 +  652
      case ERR_WIN32_ERROR + ERROR_SYSTEM_HIVE_TOO_LARGE                                  : return("win32:ERROR_SYSTEM_HIVE_TOO_LARGE"                                  );    // 100000 +  653
      case ERR_WIN32_ERROR + ERROR_DRIVER_FAILED_PRIOR_UNLOAD                             : return("win32:ERROR_DRIVER_FAILED_PRIOR_UNLOAD"                             );    // 100000 +  654
      case ERR_WIN32_ERROR + ERROR_VOLSNAP_PREPARE_HIBERNATE                              : return("win32:ERROR_VOLSNAP_PREPARE_HIBERNATE"                              );    // 100000 +  655
      case ERR_WIN32_ERROR + ERROR_HIBERNATION_FAILURE                                    : return("win32:ERROR_HIBERNATION_FAILURE"                                    );    // 100000 +  656
      case ERR_WIN32_ERROR + ERROR_PWD_TOO_LONG                                           : return("win32:ERROR_PWD_TOO_LONG"                                           );    // 100000 +  657
      case ERR_WIN32_ERROR + ERROR_FILE_SYSTEM_LIMITATION                                 : return("win32:ERROR_FILE_SYSTEM_LIMITATION"                                 );    // 100000 +  665
      case ERR_WIN32_ERROR + ERROR_ASSERTION_FAILURE                                      : return("win32:ERROR_ASSERTION_FAILURE"                                      );    // 100000 +  668
      case ERR_WIN32_ERROR + ERROR_ACPI_ERROR                                             : return("win32:ERROR_ACPI_ERROR"                                             );    // 100000 +  669
      case ERR_WIN32_ERROR + ERROR_WOW_ASSERTION                                          : return("win32:ERROR_WOW_ASSERTION"                                          );    // 100000 +  670
      case ERR_WIN32_ERROR + ERROR_PNP_BAD_MPS_TABLE                                      : return("win32:ERROR_PNP_BAD_MPS_TABLE"                                      );    // 100000 +  671
      case ERR_WIN32_ERROR + ERROR_PNP_TRANSLATION_FAILED                                 : return("win32:ERROR_PNP_TRANSLATION_FAILED"                                 );    // 100000 +  672
      case ERR_WIN32_ERROR + ERROR_PNP_IRQ_TRANSLATION_FAILED                             : return("win32:ERROR_PNP_IRQ_TRANSLATION_FAILED"                             );    // 100000 +  673
      case ERR_WIN32_ERROR + ERROR_PNP_INVALID_ID                                         : return("win32:ERROR_PNP_INVALID_ID"                                         );    // 100000 +  674
      case ERR_WIN32_ERROR + ERROR_WAKE_SYSTEM_DEBUGGER                                   : return("win32:ERROR_WAKE_SYSTEM_DEBUGGER"                                   );    // 100000 +  675
      case ERR_WIN32_ERROR + ERROR_HANDLES_CLOSED                                         : return("win32:ERROR_HANDLES_CLOSED"                                         );    // 100000 +  676
      case ERR_WIN32_ERROR + ERROR_EXTRANEOUS_INFORMATION                                 : return("win32:ERROR_EXTRANEOUS_INFORMATION"                                 );    // 100000 +  677
      case ERR_WIN32_ERROR + ERROR_RXACT_COMMIT_NECESSARY                                 : return("win32:ERROR_RXACT_COMMIT_NECESSARY"                                 );    // 100000 +  678
      case ERR_WIN32_ERROR + ERROR_MEDIA_CHECK                                            : return("win32:ERROR_MEDIA_CHECK"                                            );    // 100000 +  679
      case ERR_WIN32_ERROR + ERROR_GUID_SUBSTITUTION_MADE                                 : return("win32:ERROR_GUID_SUBSTITUTION_MADE"                                 );    // 100000 +  680
      case ERR_WIN32_ERROR + ERROR_STOPPED_ON_SYMLINK                                     : return("win32:ERROR_STOPPED_ON_SYMLINK"                                     );    // 100000 +  681
      case ERR_WIN32_ERROR + ERROR_LONGJUMP                                               : return("win32:ERROR_LONGJUMP"                                               );    // 100000 +  682
      case ERR_WIN32_ERROR + ERROR_PLUGPLAY_QUERY_VETOED                                  : return("win32:ERROR_PLUGPLAY_QUERY_VETOED"                                  );    // 100000 +  683
      case ERR_WIN32_ERROR + ERROR_UNWIND_CONSOLIDATE                                     : return("win32:ERROR_UNWIND_CONSOLIDATE"                                     );    // 100000 +  684
      case ERR_WIN32_ERROR + ERROR_REGISTRY_HIVE_RECOVERED                                : return("win32:ERROR_REGISTRY_HIVE_RECOVERED"                                );    // 100000 +  685
      case ERR_WIN32_ERROR + ERROR_DLL_MIGHT_BE_INSECURE                                  : return("win32:ERROR_DLL_MIGHT_BE_INSECURE"                                  );    // 100000 +  686
      case ERR_WIN32_ERROR + ERROR_DLL_MIGHT_BE_INCOMPATIBLE                              : return("win32:ERROR_DLL_MIGHT_BE_INCOMPATIBLE"                              );    // 100000 +  687
      case ERR_WIN32_ERROR + ERROR_DBG_EXCEPTION_NOT_HANDLED                              : return("win32:ERROR_DBG_EXCEPTION_NOT_HANDLED"                              );    // 100000 +  688
      case ERR_WIN32_ERROR + ERROR_DBG_REPLY_LATER                                        : return("win32:ERROR_DBG_REPLY_LATER"                                        );    // 100000 +  689
      case ERR_WIN32_ERROR + ERROR_DBG_UNABLE_TO_PROVIDE_HANDLE                           : return("win32:ERROR_DBG_UNABLE_TO_PROVIDE_HANDLE"                           );    // 100000 +  690
      case ERR_WIN32_ERROR + ERROR_DBG_TERMINATE_THREAD                                   : return("win32:ERROR_DBG_TERMINATE_THREAD"                                   );    // 100000 +  691
      case ERR_WIN32_ERROR + ERROR_DBG_TERMINATE_PROCESS                                  : return("win32:ERROR_DBG_TERMINATE_PROCESS"                                  );    // 100000 +  692
      case ERR_WIN32_ERROR + ERROR_DBG_CONTROL_C                                          : return("win32:ERROR_DBG_CONTROL_C"                                          );    // 100000 +  693
      case ERR_WIN32_ERROR + ERROR_DBG_PRINTEXCEPTION_C                                   : return("win32:ERROR_DBG_PRINTEXCEPTION_C"                                   );    // 100000 +  694
      case ERR_WIN32_ERROR + ERROR_DBG_RIPEXCEPTION                                       : return("win32:ERROR_DBG_RIPEXCEPTION"                                       );    // 100000 +  695
      case ERR_WIN32_ERROR + ERROR_DBG_CONTROL_BREAK                                      : return("win32:ERROR_DBG_CONTROL_BREAK"                                      );    // 100000 +  696
      case ERR_WIN32_ERROR + ERROR_DBG_COMMAND_EXCEPTION                                  : return("win32:ERROR_DBG_COMMAND_EXCEPTION"                                  );    // 100000 +  697
      case ERR_WIN32_ERROR + ERROR_OBJECT_NAME_EXISTS                                     : return("win32:ERROR_OBJECT_NAME_EXISTS"                                     );    // 100000 +  698
      case ERR_WIN32_ERROR + ERROR_THREAD_WAS_SUSPENDED                                   : return("win32:ERROR_THREAD_WAS_SUSPENDED"                                   );    // 100000 +  699
      case ERR_WIN32_ERROR + ERROR_IMAGE_NOT_AT_BASE                                      : return("win32:ERROR_IMAGE_NOT_AT_BASE"                                      );    // 100000 +  700
      case ERR_WIN32_ERROR + ERROR_RXACT_STATE_CREATED                                    : return("win32:ERROR_RXACT_STATE_CREATED"                                    );    // 100000 +  701
      case ERR_WIN32_ERROR + ERROR_SEGMENT_NOTIFICATION                                   : return("win32:ERROR_SEGMENT_NOTIFICATION"                                   );    // 100000 +  702
      case ERR_WIN32_ERROR + ERROR_BAD_CURRENT_DIRECTORY                                  : return("win32:ERROR_BAD_CURRENT_DIRECTORY"                                  );    // 100000 +  703
      case ERR_WIN32_ERROR + ERROR_FT_READ_RECOVERY_FROM_BACKUP                           : return("win32:ERROR_FT_READ_RECOVERY_FROM_BACKUP"                           );    // 100000 +  704
      case ERR_WIN32_ERROR + ERROR_FT_WRITE_RECOVERY                                      : return("win32:ERROR_FT_WRITE_RECOVERY"                                      );    // 100000 +  705
      case ERR_WIN32_ERROR + ERROR_IMAGE_MACHINE_TYPE_MISMATCH                            : return("win32:ERROR_IMAGE_MACHINE_TYPE_MISMATCH"                            );    // 100000 +  706
      case ERR_WIN32_ERROR + ERROR_RECEIVE_PARTIAL                                        : return("win32:ERROR_RECEIVE_PARTIAL"                                        );    // 100000 +  707
      case ERR_WIN32_ERROR + ERROR_RECEIVE_EXPEDITED                                      : return("win32:ERROR_RECEIVE_EXPEDITED"                                      );    // 100000 +  708
      case ERR_WIN32_ERROR + ERROR_RECEIVE_PARTIAL_EXPEDITED                              : return("win32:ERROR_RECEIVE_PARTIAL_EXPEDITED"                              );    // 100000 +  709
      case ERR_WIN32_ERROR + ERROR_EVENT_DONE                                             : return("win32:ERROR_EVENT_DONE"                                             );    // 100000 +  710
      case ERR_WIN32_ERROR + ERROR_EVENT_PENDING                                          : return("win32:ERROR_EVENT_PENDING"                                          );    // 100000 +  711
      case ERR_WIN32_ERROR + ERROR_CHECKING_FILE_SYSTEM                                   : return("win32:ERROR_CHECKING_FILE_SYSTEM"                                   );    // 100000 +  712
      case ERR_WIN32_ERROR + ERROR_FATAL_APP_EXIT                                         : return("win32:ERROR_FATAL_APP_EXIT"                                         );    // 100000 +  713
      case ERR_WIN32_ERROR + ERROR_PREDEFINED_HANDLE                                      : return("win32:ERROR_PREDEFINED_HANDLE"                                      );    // 100000 +  714
      case ERR_WIN32_ERROR + ERROR_WAS_UNLOCKED                                           : return("win32:ERROR_WAS_UNLOCKED"                                           );    // 100000 +  715
      case ERR_WIN32_ERROR + ERROR_SERVICE_NOTIFICATION                                   : return("win32:ERROR_SERVICE_NOTIFICATION"                                   );    // 100000 +  716
      case ERR_WIN32_ERROR + ERROR_WAS_LOCKED                                             : return("win32:ERROR_WAS_LOCKED"                                             );    // 100000 +  717
      case ERR_WIN32_ERROR + ERROR_LOG_HARD_ERROR                                         : return("win32:ERROR_LOG_HARD_ERROR"                                         );    // 100000 +  718
      case ERR_WIN32_ERROR + ERROR_ALREADY_WIN32                                          : return("win32:ERROR_ALREADY_WIN32"                                          );    // 100000 +  719
      case ERR_WIN32_ERROR + ERROR_IMAGE_MACHINE_TYPE_MISMATCH_EXE                        : return("win32:ERROR_IMAGE_MACHINE_TYPE_MISMATCH_EXE"                        );    // 100000 +  720
      case ERR_WIN32_ERROR + ERROR_NO_YIELD_PERFORMED                                     : return("win32:ERROR_NO_YIELD_PERFORMED"                                     );    // 100000 +  721
      case ERR_WIN32_ERROR + ERROR_TIMER_RESUME_IGNORED                                   : return("win32:ERROR_TIMER_RESUME_IGNORED"                                   );    // 100000 +  722
      case ERR_WIN32_ERROR + ERROR_ARBITRATION_UNHANDLED                                  : return("win32:ERROR_ARBITRATION_UNHANDLED"                                  );    // 100000 +  723
      case ERR_WIN32_ERROR + ERROR_CARDBUS_NOT_SUPPORTED                                  : return("win32:ERROR_CARDBUS_NOT_SUPPORTED"                                  );    // 100000 +  724
      case ERR_WIN32_ERROR + ERROR_MP_PROCESSOR_MISMATCH                                  : return("win32:ERROR_MP_PROCESSOR_MISMATCH"                                  );    // 100000 +  725
      case ERR_WIN32_ERROR + ERROR_HIBERNATED                                             : return("win32:ERROR_HIBERNATED"                                             );    // 100000 +  726
      case ERR_WIN32_ERROR + ERROR_RESUME_HIBERNATION                                     : return("win32:ERROR_RESUME_HIBERNATION"                                     );    // 100000 +  727
      case ERR_WIN32_ERROR + ERROR_FIRMWARE_UPDATED                                       : return("win32:ERROR_FIRMWARE_UPDATED"                                       );    // 100000 +  728
      case ERR_WIN32_ERROR + ERROR_DRIVERS_LEAKING_LOCKED_PAGES                           : return("win32:ERROR_DRIVERS_LEAKING_LOCKED_PAGES"                           );    // 100000 +  729
      case ERR_WIN32_ERROR + ERROR_WAKE_SYSTEM                                            : return("win32:ERROR_WAKE_SYSTEM"                                            );    // 100000 +  730
      case ERR_WIN32_ERROR + ERROR_WAIT_1                                                 : return("win32:ERROR_WAIT_1"                                                 );    // 100000 +  731
      case ERR_WIN32_ERROR + ERROR_WAIT_2                                                 : return("win32:ERROR_WAIT_2"                                                 );    // 100000 +  732
      case ERR_WIN32_ERROR + ERROR_WAIT_3                                                 : return("win32:ERROR_WAIT_3"                                                 );    // 100000 +  733
      case ERR_WIN32_ERROR + ERROR_WAIT_63                                                : return("win32:ERROR_WAIT_63"                                                );    // 100000 +  734
      case ERR_WIN32_ERROR + ERROR_ABANDONED_WAIT_0                                       : return("win32:ERROR_ABANDONED_WAIT_0"                                       );    // 100000 +  735
      case ERR_WIN32_ERROR + ERROR_ABANDONED_WAIT_63                                      : return("win32:ERROR_ABANDONED_WAIT_63"                                      );    // 100000 +  736
      case ERR_WIN32_ERROR + ERROR_USER_APC                                               : return("win32:ERROR_USER_APC"                                               );    // 100000 +  737
      case ERR_WIN32_ERROR + ERROR_KERNEL_APC                                             : return("win32:ERROR_KERNEL_APC"                                             );    // 100000 +  738
      case ERR_WIN32_ERROR + ERROR_ALERTED                                                : return("win32:ERROR_ALERTED"                                                );    // 100000 +  739
      case ERR_WIN32_ERROR + ERROR_ELEVATION_REQUIRED                                     : return("win32:ERROR_ELEVATION_REQUIRED"                                     );    // 100000 +  740
      case ERR_WIN32_ERROR + ERROR_REPARSE                                                : return("win32:ERROR_REPARSE"                                                );    // 100000 +  741
      case ERR_WIN32_ERROR + ERROR_OPLOCK_BREAK_IN_PROGRESS                               : return("win32:ERROR_OPLOCK_BREAK_IN_PROGRESS"                               );    // 100000 +  742
      case ERR_WIN32_ERROR + ERROR_VOLUME_MOUNTED                                         : return("win32:ERROR_VOLUME_MOUNTED"                                         );    // 100000 +  743
      case ERR_WIN32_ERROR + ERROR_RXACT_COMMITTED                                        : return("win32:ERROR_RXACT_COMMITTED"                                        );    // 100000 +  744
      case ERR_WIN32_ERROR + ERROR_NOTIFY_CLEANUP                                         : return("win32:ERROR_NOTIFY_CLEANUP"                                         );    // 100000 +  745
      case ERR_WIN32_ERROR + ERROR_PRIMARY_TRANSPORT_CONNECT_FAILED                       : return("win32:ERROR_PRIMARY_TRANSPORT_CONNECT_FAILED"                       );    // 100000 +  746
      case ERR_WIN32_ERROR + ERROR_PAGE_FAULT_TRANSITION                                  : return("win32:ERROR_PAGE_FAULT_TRANSITION"                                  );    // 100000 +  747
      case ERR_WIN32_ERROR + ERROR_PAGE_FAULT_DEMAND_ZERO                                 : return("win32:ERROR_PAGE_FAULT_DEMAND_ZERO"                                 );    // 100000 +  748
      case ERR_WIN32_ERROR + ERROR_PAGE_FAULT_COPY_ON_WRITE                               : return("win32:ERROR_PAGE_FAULT_COPY_ON_WRITE"                               );    // 100000 +  749
      case ERR_WIN32_ERROR + ERROR_PAGE_FAULT_GUARD_PAGE                                  : return("win32:ERROR_PAGE_FAULT_GUARD_PAGE"                                  );    // 100000 +  750
      case ERR_WIN32_ERROR + ERROR_PAGE_FAULT_PAGING_FILE                                 : return("win32:ERROR_PAGE_FAULT_PAGING_FILE"                                 );    // 100000 +  751
      case ERR_WIN32_ERROR + ERROR_CACHE_PAGE_LOCKED                                      : return("win32:ERROR_CACHE_PAGE_LOCKED"                                      );    // 100000 +  752
      case ERR_WIN32_ERROR + ERROR_CRASH_DUMP                                             : return("win32:ERROR_CRASH_DUMP"                                             );    // 100000 +  753
      case ERR_WIN32_ERROR + ERROR_BUFFER_ALL_ZEROS                                       : return("win32:ERROR_BUFFER_ALL_ZEROS"                                       );    // 100000 +  754
      case ERR_WIN32_ERROR + ERROR_REPARSE_OBJECT                                         : return("win32:ERROR_REPARSE_OBJECT"                                         );    // 100000 +  755
      case ERR_WIN32_ERROR + ERROR_RESOURCE_REQUIREMENTS_CHANGED                          : return("win32:ERROR_RESOURCE_REQUIREMENTS_CHANGED"                          );    // 100000 +  756
      case ERR_WIN32_ERROR + ERROR_TRANSLATION_COMPLETE                                   : return("win32:ERROR_TRANSLATION_COMPLETE"                                   );    // 100000 +  757
      case ERR_WIN32_ERROR + ERROR_NOTHING_TO_TERMINATE                                   : return("win32:ERROR_NOTHING_TO_TERMINATE"                                   );    // 100000 +  758
      case ERR_WIN32_ERROR + ERROR_PROCESS_NOT_IN_JOB                                     : return("win32:ERROR_PROCESS_NOT_IN_JOB"                                     );    // 100000 +  759
      case ERR_WIN32_ERROR + ERROR_PROCESS_IN_JOB                                         : return("win32:ERROR_PROCESS_IN_JOB"                                         );    // 100000 +  760
      case ERR_WIN32_ERROR + ERROR_VOLSNAP_HIBERNATE_READY                                : return("win32:ERROR_VOLSNAP_HIBERNATE_READY"                                );    // 100000 +  761
      case ERR_WIN32_ERROR + ERROR_FSFILTER_OP_COMPLETED_SUCCESSFULLY                     : return("win32:ERROR_FSFILTER_OP_COMPLETED_SUCCESSFULLY"                     );    // 100000 +  762
      case ERR_WIN32_ERROR + ERROR_INTERRUPT_VECTOR_ALREADY_CONNECTED                     : return("win32:ERROR_INTERRUPT_VECTOR_ALREADY_CONNECTED"                     );    // 100000 +  763
      case ERR_WIN32_ERROR + ERROR_INTERRUPT_STILL_CONNECTED                              : return("win32:ERROR_INTERRUPT_STILL_CONNECTED"                              );    // 100000 +  764
      case ERR_WIN32_ERROR + ERROR_WAIT_FOR_OPLOCK                                        : return("win32:ERROR_WAIT_FOR_OPLOCK"                                        );    // 100000 +  765
      case ERR_WIN32_ERROR + ERROR_DBG_EXCEPTION_HANDLED                                  : return("win32:ERROR_DBG_EXCEPTION_HANDLED"                                  );    // 100000 +  766
      case ERR_WIN32_ERROR + ERROR_DBG_CONTINUE                                           : return("win32:ERROR_DBG_CONTINUE"                                           );    // 100000 +  767
      case ERR_WIN32_ERROR + ERROR_CALLBACK_POP_STACK                                     : return("win32:ERROR_CALLBACK_POP_STACK"                                     );    // 100000 +  768
      case ERR_WIN32_ERROR + ERROR_COMPRESSION_DISABLED                                   : return("win32:ERROR_COMPRESSION_DISABLED"                                   );    // 100000 +  769
      case ERR_WIN32_ERROR + ERROR_CANTFETCHBACKWARDS                                     : return("win32:ERROR_CANTFETCHBACKWARDS"                                     );    // 100000 +  770
      case ERR_WIN32_ERROR + ERROR_CANTSCROLLBACKWARDS                                    : return("win32:ERROR_CANTSCROLLBACKWARDS"                                    );    // 100000 +  771
      case ERR_WIN32_ERROR + ERROR_ROWSNOTRELEASED                                        : return("win32:ERROR_ROWSNOTRELEASED"                                        );    // 100000 +  772
      case ERR_WIN32_ERROR + ERROR_BAD_ACCESSOR_FLAGS                                     : return("win32:ERROR_BAD_ACCESSOR_FLAGS"                                     );    // 100000 +  773
      case ERR_WIN32_ERROR + ERROR_ERRORS_ENCOUNTERED                                     : return("win32:ERROR_ERRORS_ENCOUNTERED"                                     );    // 100000 +  774
      case ERR_WIN32_ERROR + ERROR_NOT_CAPABLE                                            : return("win32:ERROR_NOT_CAPABLE"                                            );    // 100000 +  775
      case ERR_WIN32_ERROR + ERROR_REQUEST_OUT_OF_SEQUENCE                                : return("win32:ERROR_REQUEST_OUT_OF_SEQUENCE"                                );    // 100000 +  776
      case ERR_WIN32_ERROR + ERROR_VERSION_PARSE_ERROR                                    : return("win32:ERROR_VERSION_PARSE_ERROR"                                    );    // 100000 +  777
      case ERR_WIN32_ERROR + ERROR_BADSTARTPOSITION                                       : return("win32:ERROR_BADSTARTPOSITION"                                       );    // 100000 +  778
      case ERR_WIN32_ERROR + ERROR_MEMORY_HARDWARE                                        : return("win32:ERROR_MEMORY_HARDWARE"                                        );    // 100000 +  779
      case ERR_WIN32_ERROR + ERROR_DISK_REPAIR_DISABLED                                   : return("win32:ERROR_DISK_REPAIR_DISABLED"                                   );    // 100000 +  780
      case ERR_WIN32_ERROR + ERROR_INSUFFICIENT_RESOURCE_FOR_SPECIFIED_SHARED_SECTION_SIZE: return("win32:ERROR_INSUFFICIENT_RESOURCE_FOR_SPECIFIED_SHARED_SECTION_SIZE");    // 100000 +  781
      case ERR_WIN32_ERROR + ERROR_SYSTEM_POWERSTATE_TRANSITION                           : return("win32:ERROR_SYSTEM_POWERSTATE_TRANSITION"                           );    // 100000 +  782
      case ERR_WIN32_ERROR + ERROR_SYSTEM_POWERSTATE_COMPLEX_TRANSITION                   : return("win32:ERROR_SYSTEM_POWERSTATE_COMPLEX_TRANSITION"                   );    // 100000 +  783
      case ERR_WIN32_ERROR + ERROR_MCA_EXCEPTION                                          : return("win32:ERROR_MCA_EXCEPTION"                                          );    // 100000 +  784
      case ERR_WIN32_ERROR + ERROR_ACCESS_AUDIT_BY_POLICY                                 : return("win32:ERROR_ACCESS_AUDIT_BY_POLICY"                                 );    // 100000 +  785
      case ERR_WIN32_ERROR + ERROR_ACCESS_DISABLED_NO_SAFER_UI_BY_POLICY                  : return("win32:ERROR_ACCESS_DISABLED_NO_SAFER_UI_BY_POLICY"                  );    // 100000 +  786
      case ERR_WIN32_ERROR + ERROR_ABANDON_HIBERFILE                                      : return("win32:ERROR_ABANDON_HIBERFILE"                                      );    // 100000 +  787
      case ERR_WIN32_ERROR + ERROR_LOST_WRITEBEHIND_DATA_NETWORK_DISCONNECTED             : return("win32:ERROR_LOST_WRITEBEHIND_DATA_NETWORK_DISCONNECTED"             );    // 100000 +  788
      case ERR_WIN32_ERROR + ERROR_LOST_WRITEBEHIND_DATA_NETWORK_SERVER_ERROR             : return("win32:ERROR_LOST_WRITEBEHIND_DATA_NETWORK_SERVER_ERROR"             );    // 100000 +  789
      case ERR_WIN32_ERROR + ERROR_LOST_WRITEBEHIND_DATA_LOCAL_DISK_ERROR                 : return("win32:ERROR_LOST_WRITEBEHIND_DATA_LOCAL_DISK_ERROR"                 );    // 100000 +  790
      case ERR_WIN32_ERROR + ERROR_BAD_MCFG_TABLE                                         : return("win32:ERROR_BAD_MCFG_TABLE"                                         );    // 100000 +  791
      case ERR_WIN32_ERROR + ERROR_DISK_REPAIR_REDIRECTED                                 : return("win32:ERROR_DISK_REPAIR_REDIRECTED"                                 );    // 100000 +  792
      case ERR_WIN32_ERROR + ERROR_DISK_REPAIR_UNSUCCESSFUL                               : return("win32:ERROR_DISK_REPAIR_UNSUCCESSFUL"                               );    // 100000 +  793
      case ERR_WIN32_ERROR + ERROR_CORRUPT_LOG_OVERFULL                                   : return("win32:ERROR_CORRUPT_LOG_OVERFULL"                                   );    // 100000 +  794
      case ERR_WIN32_ERROR + ERROR_CORRUPT_LOG_CORRUPTED                                  : return("win32:ERROR_CORRUPT_LOG_CORRUPTED"                                  );    // 100000 +  795
      case ERR_WIN32_ERROR + ERROR_CORRUPT_LOG_UNAVAILABLE                                : return("win32:ERROR_CORRUPT_LOG_UNAVAILABLE"                                );    // 100000 +  796
      case ERR_WIN32_ERROR + ERROR_CORRUPT_LOG_DELETED_FULL                               : return("win32:ERROR_CORRUPT_LOG_DELETED_FULL"                               );    // 100000 +  797
      case ERR_WIN32_ERROR + ERROR_CORRUPT_LOG_CLEARED                                    : return("win32:ERROR_CORRUPT_LOG_CLEARED"                                    );    // 100000 +  798
      case ERR_WIN32_ERROR + ERROR_ORPHAN_NAME_EXHAUSTED                                  : return("win32:ERROR_ORPHAN_NAME_EXHAUSTED"                                  );    // 100000 +  799
      case ERR_WIN32_ERROR + ERROR_OPLOCK_SWITCHED_TO_NEW_HANDLE                          : return("win32:ERROR_OPLOCK_SWITCHED_TO_NEW_HANDLE"                          );    // 100000 +  800
      case ERR_WIN32_ERROR + ERROR_CANNOT_GRANT_REQUESTED_OPLOCK                          : return("win32:ERROR_CANNOT_GRANT_REQUESTED_OPLOCK"                          );    // 100000 +  801
      case ERR_WIN32_ERROR + ERROR_CANNOT_BREAK_OPLOCK                                    : return("win32:ERROR_CANNOT_BREAK_OPLOCK"                                    );    // 100000 +  802
      case ERR_WIN32_ERROR + ERROR_OPLOCK_HANDLE_CLOSED                                   : return("win32:ERROR_OPLOCK_HANDLE_CLOSED"                                   );    // 100000 +  803
      case ERR_WIN32_ERROR + ERROR_NO_ACE_CONDITION                                       : return("win32:ERROR_NO_ACE_CONDITION"                                       );    // 100000 +  804
      case ERR_WIN32_ERROR + ERROR_INVALID_ACE_CONDITION                                  : return("win32:ERROR_INVALID_ACE_CONDITION"                                  );    // 100000 +  805
      case ERR_WIN32_ERROR + ERROR_FILE_HANDLE_REVOKED                                    : return("win32:ERROR_FILE_HANDLE_REVOKED"                                    );    // 100000 +  806
      case ERR_WIN32_ERROR + ERROR_IMAGE_AT_DIFFERENT_BASE                                : return("win32:ERROR_IMAGE_AT_DIFFERENT_BASE"                                );    // 100000 +  807
      case ERR_WIN32_ERROR + ERROR_EA_ACCESS_DENIED                                       : return("win32:ERROR_EA_ACCESS_DENIED"                                       );    // 100000 +  994
      case ERR_WIN32_ERROR + ERROR_OPERATION_ABORTED                                      : return("win32:ERROR_OPERATION_ABORTED"                                      );    // 100000 +  995
      case ERR_WIN32_ERROR + ERROR_IO_INCOMPLETE                                          : return("win32:ERROR_IO_INCOMPLETE"                                          );    // 100000 +  996
      case ERR_WIN32_ERROR + ERROR_IO_PENDING                                             : return("win32:ERROR_IO_PENDING"                                             );    // 100000 +  997
      case ERR_WIN32_ERROR + ERROR_NOACCESS                                               : return("win32:ERROR_NOACCESS"                                               );    // 100000 +  998
      case ERR_WIN32_ERROR + ERROR_SWAPERROR                                              : return("win32:ERROR_SWAPERROR"                                              );    // 100000 +  999
      // ...
      case ERR_WIN32_ERROR + ERROR_CONTROL_ID_NOT_FOUND                                   : return("win32:ERROR_CONTROL_ID_NOT_FOUND"                                   );    // 100000 + 1421
      // ...
      case ERR_WIN32_ERROR + ERROR_NOT_A_REPARSE_POINT                                    : return("win32:ERROR_NOT_A_REPARSE_POINT"                                    );    // 100000 + 4390
   }

   char* format = "%d";
   if (error >= ERR_WIN32_ERROR) {
      error -= ERR_WIN32_ERROR;
      format = "win32:%d";
   }
   return(StrFormat(format, error));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of an MQL error code.
 *
 * @param  int error
 *
 * @return wchar*
 */
const wchar* WINAPI ErrorToStrW(int error) {
   wstring s = ansiToUnicode(string(ErrorToStrA(error)));
   return(wcsdup(s.c_str()));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the hexadecimale representation of an integer.
 *  e.g. IntToHexStr(13465610) => "00CD780A"
 *
 * @param  int value - 4 byte integer value
 *
 * @return char* - 8 character string
 */
char* WINAPI IntToHexStr(int value) {
   return(StrFormat("%p", value));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of one or more INIT_* flags.
 *
 * @param  DWORD flags
 *
 * @return char*
 */
char* WINAPI InitFlagsToStr(DWORD flags) {
   string str("");

   if (flags & INIT_TIMEZONE           ) str.append("|INIT_TIMEZONE"           );
   if (flags & INIT_PIPVALUE           ) str.append("|INIT_PIPVALUE"           );
   if (flags & INIT_BARS_ON_HIST_UPDATE) str.append("|INIT_BARS_ON_HIST_UPDATE");
   if (flags & INIT_NO_BARS_REQUIRED   ) str.append("|INIT_NO_BARS_REQUIRED"   );
   if (!str.length())                    str.append("|"+ to_string(flags)      );

   return(strcpy(new char[str.length()], str.c_str()+1));            // skip the leading "|"
   #pragma EXPANDER_EXPORT                                           // TODO: add to GC (close memory leak)
}


/**
 * Return a readable version of one or more DEINIT_* flags.
 *
 * @param  DWORD flags
 *
 * @return char*
 */
char* WINAPI DeinitFlagsToStr(DWORD flags) {
   string str("");

 //if (flags & DEINIT_*) str.append("|DEINIT_*"          );          // a.t.m. there are no DEINIT flags
   if (!str.length())    str.append("|"+ to_string(flags));

   return(strcpy(new char[str.length()], str.c_str()+1));            // skip the leading "|"
   #pragma EXPANDER_EXPORT                                           // TODO: add to GC (close memory leak)
}


/**
 * Alias of InitReasonToStr()
 */
const char* WINAPI InitializeReasonToStr(InitializeReason reason) {
   return(InitReasonToStr(reason));
   #pragma EXPANDER_EXPORT
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
      case IR_TERMINAL_FAILURE : return("IR_TERMINAL_FAILURE" );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter reason: %d (not an InitializeReason)", reason));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the description of a loglevel.
 *
 * @param  int level                 - loglevel
 * @param  BOOL upperCase [optional] - whether to return an uppercase string (default: yes)
 *
 * @return char* - description or NULL if the parameter is invalid
 */
const char* WINAPI LoglevelDescriptionA(int level, BOOL upperCase/*= TRUE*/) {
   if (upperCase) {
      switch (level) {
         case NULL:       return("NULL"  );
         case LOG_DEBUG:  return("DEBUG" );
         case LOG_INFO:   return("INFO"  );
         case LOG_NOTICE: return("NOTICE");
         case LOG_WARN:   return("WARN"  );
         case LOG_ERROR:  return("ERROR" );
         case LOG_FATAL:  return("FATAL" );
         case LOG_OFF:    return("OFF"   );        // not a regular loglevel
      }
   }
   else {
      switch (level) {
         case NULL:       return("null"  );
         case LOG_DEBUG:  return("debug" );
         case LOG_INFO:   return("info"  );
         case LOG_NOTICE: return("notice");
         case LOG_WARN:   return("warn"  );
         case LOG_ERROR:  return("error" );
         case LOG_FATAL:  return("fatal" );
         case LOG_OFF:    return("off"   );        // not a regular loglevel
      }
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter level: %d (not a loglevel)", level));
   #pragma EXPANDER_EXPORT
}


/**
 * Return a readable version of a loglevel constant.
 *
 * @param  int level - loglevel
 *
 * @return char* - readable version or NULL if the parameter is invalid
 */
const char* WINAPI LoglevelToStr(int level) {
   switch (level) {
      case NULL:       return("NULL"      );
      case LOG_DEBUG:  return("LOG_DEBUG" );
      case LOG_INFO:   return("LOG_INFO"  );
      case LOG_NOTICE: return("LOG_NOTICE");
      case LOG_WARN:   return("LOG_WARN"  );
      case LOG_ERROR:  return("LOG_ERROR" );
      case LOG_FATAL:  return("LOG_FATAL" );
      case LOG_OFF:    return("LOG_OFF"   );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter level: %d (not a loglevel)", level));
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
 * Alias of NumberFormat()
 *
 * Convert a numeric value to a formatted string.
 *
 * @param  doube value
 * @param  char* format - format control string as used for printf()
 *
 * @return char* - formatted string or a NULL pointer in case of errors
 *
 *       Format codes:
 * @see  https://alvinalexander.com/programming/printf-format-cheat-sheet
 * @see  http://www.cplusplus.com/reference/cstdio/printf/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm
 */
char* WINAPI NumberToStr(double value, const char* format) {
   return(NumberFormat(value, format));                        // TODO: support custom format codes
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
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter type: %d (not an operation type)", type));
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
 * Alias of OperationTypeDescription()
 */
const char* WINAPI OrderTypeDescription(int type) {
   return(OperationTypeDescription(type));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of OperationTypeToStr()
 */
const char* WINAPI OrderTypeToStr(int type) {
   return(OperationTypeToStr(type));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the description of a timeframe identifier. Supports custom timeframes.
 *
 * @param  int period - timeframe identifier or amount of minutes per bar period
 *
 * @return char* - description or a NULL pointer if the parameter is invalid
 *
 * Note: This implementation should match the one in MQL's stdfunctions.mqh.
 */
const char* WINAPI PeriodDescriptionA(int period) {
   if (period < 0) return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter period: %d", period));

   switch (period) {
      case NULL      : return("NULL");
      case PERIOD_M1 : return("M1" );     // 1 minute
      case PERIOD_M5 : return("M5" );     // 5 minutes
      case PERIOD_M15: return("M15");     // 15 minutes
      case PERIOD_M30: return("M30");     // 30 minutes
      case PERIOD_H1 : return("H1" );     // 1 hour
      case PERIOD_H2 : return("H2" );     // 2 hours (custom timeframe)
      case PERIOD_H3 : return("H3" );     // 3 hours (custom timeframe)
      case PERIOD_H4 : return("H4" );     // 4 hours
      case PERIOD_H6 : return("H6" );     // 6 hours (custom timeframe)
      case PERIOD_H8 : return("H8" );     // 8 hours (custom timeframe)
      case PERIOD_D1 : return("D1" );     // 1 day
      case PERIOD_W1 : return("W1" );     // 1 week
      case PERIOD_MN1: return("MN1");     // 1 month
      case PERIOD_Q1 : return("Q1" );     // 1 quarter (custom timeframe)
   }
   return(StrFormat("%d", period));
}


/**
 * Return the description of a timeframe identifier. Supports custom timeframes.
 *
 * @param  int period - timeframe identifier or amount of minutes per bar period
 *
 * @return wchar* - description or a NULL pointer if the parameter is invalid
 */
const wchar* WINAPI PeriodDescriptionW(int period) {
   const char* s = PeriodDescriptionA(period);
   if (!s) return(NULL);

   return(wcsdup(ansiToUnicode(string(s)).c_str()));
}


/**
 * Return a human-readable version of a timeframe identifier. Supports custom timeframes.
 *
 * @param  int period - timeframe identifier
 *
 * @return char* - string or NULL if the parameter is invalid
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
      case PERIOD_H2 : return("PERIOD_H2" );     // 2 hours (custom timeframe)
      case PERIOD_H3 : return("PERIOD_H3" );     // 3 hours (custom timeframe)
      case PERIOD_H4 : return("PERIOD_H4" );     // 4 hours
      case PERIOD_H6 : return("PERIOD_H6" );     // 6 hours (custom timeframe)
      case PERIOD_H8 : return("PERIOD_H8" );     // 8 hours (custom timeframe)
      case PERIOD_D1 : return("PERIOD_D1" );     // 1 day
      case PERIOD_W1 : return("PERIOD_W1" );     // 1 week
      case PERIOD_MN1: return("PERIOD_MN1");     // 1 month
      case PERIOD_Q1 : return("PERIOD_Q1" );     // 1 quarter (custom timeframe)
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
 * Return a readable version of an initialized or non-initialized string.
 *
 * @param  char* value
 *
 * @return char* - content of the string or "(null)" if the parameter is a NULL pointer
 *
 * Note: Purpose of this function is to output an initialized and a non-initialized string from the same function.
 *       In an MQL implementation the terminal raises ERR_NOT_INITIALIZED_STRING for a NULL pointer and logs the warning
 *       "warn: not initialized string".
 */
const char* WINAPI StringToStr(const char* value) {
   return(value ? value : "(null)");
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of PeriodDescriptionA()
 */
const char* WINAPI TimeframeDescription(int timeframe) {
   return(PeriodDescriptionA(timeframe));
}


/**
 * Alias of PeriodToStr()
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
      case TRADE_DIRECTION_LONG:  return("Long" );
      case TRADE_DIRECTION_SHORT: return("Short");
      case TRADE_DIRECTION_BOTH:  return("Both" );
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
      case TRADE_DIRECTION_LONG:  return("TRADE_DIRECTION_LONG" );
      case TRADE_DIRECTION_SHORT: return("TRADE_DIRECTION_SHORT");
      case TRADE_DIRECTION_BOTH:  return("TRADE_DIRECTION_BOTH" );
   }
   return((char*)error(ERR_INVALID_PARAMETER, "invalid parameter direction: %d (not a trade direction)", direction));
   #pragma EXPANDER_EXPORT
}


/**
 * Alias of UninitReasonToStr()
 */
const char* WINAPI UninitializeReasonToStr(UninitializeReason reason) {
   return(UninitReasonToStr(reason));
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
