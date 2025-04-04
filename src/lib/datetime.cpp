/**
 * Comparison and ranges of time types supported by Windows:
 *
 * @link  https://www.codeproject.com/Articles/144159/Time-Format-Conversion-Made-Easy
 */
#include "expander.h"
#include "lib/datetime.h"
#include "lib/memory.h"
#include "lib/string.h"

#include <ctime>

#define UNIX_TIME_START    116444736000000000LL                // 01.01.1970 (start of Unix epoch) in Windows "ticks"
#define TICKS_PER_SECOND   10000000LL                          // a tick is 100ns


/**
 * Return the current GMT time as a 32-bit Unix timestamp (seconds since 01.01.1970 00:00 GMT). In tester this time is not
 * modelled. Use the MQL framework function TimeGmt() to get the modelled GMT time in tester.
 *
 * @return time32
 */
time32 WINAPI GetGmtTime32() {
   return(_time32(NULL));

   #pragma EXPANDER_EXPORT
}


/**
 * Return the current GMT time as a 64-bit Unix timestamp (seconds since 01.01.1970 00:00 GMT). In tester this time is not
 * modelled. Use the MQL framework function TimeGmt() to get the modelled GMT time in tester.
 *
 * @return time64
 */
time64 WINAPI GetGmtTime64() {
   return(_time64(NULL));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current local time as a 32-bit Unix timestamp (seconds since 01.01.1970 00:00 local time). In tester this time
 * is not modelled. Use the MQL framework function TimeLocal() to get the modelled local time in tester.
 *
 * @return time32
 */
time32 WINAPI GetLocalTime32() {
   return(FileTimeToUnixTime32(getLocalTimeAsFileTime()));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current local time as a 64-bit Unix timestamp (seconds since 01.01.1970 00:00 local time). In tester this time
 * is not modelled. Use the MQL framework function TimeLocal() to get the modelled local time in tester.
 *
 * @return time64
 */
time64 WINAPI GetLocalTime64() {
   return(FileTimeToUnixTime64(getLocalTimeAsFileTime()));
   #pragma EXPANDER_EXPORT
}


/**
 * Return the current local time as a SYSTEMTIME.
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI getLocalTime() {
   SYSTEMTIME st = {};
   GetLocalTime(&st);
   return(st);
}


/**
 * Return the current local time as a FILETIME.
 *
 * @return FILETIME
 */
FILETIME WINAPI getLocalTimeAsFileTime() {
   FILETIME st=getSystemTimeAsFileTime(), lt={};
   FileTimeToLocalFileTime(&st, &lt);
   return(lt);
}


/**
 * Return the current system time as a SYSTEMTIME.
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI getSystemTime() {
   SYSTEMTIME st = {};
   GetSystemTime(&st);
   return(st);
}


/**
 * Return the current system time as a FILETIME.
 *
 * @return FILETIME
 */
FILETIME WINAPI getSystemTimeAsFileTime() {
   FILETIME ft = {};
   GetSystemTimeAsFileTime(&ft);
   return(ft);
}


struct TimezoneMapping {
   int          id;
   const char*  windowsName;
   const char*  region;
   const char*  ianaName;
   const wchar* ianaNameLower;
};

enum { TIMEZONE_MAPPING_COUNT = 585 };


/**
 * Defines supported timezones and maps IANA to Windows names.
 *
 * @return TimezoneMapping[] - array of timezone mappings
 */
const TimezoneMapping* WINAPI GetTimeZoneMappings() {
   // following definitions match https://github.com/unicode-org/cldr/commits/main/common/supplemental/windowsZones.xml
   // version: git commit d07e48b (21.09.2024)

   static const TimezoneMapping mappings[TIMEZONE_MAPPING_COUNT] = {
      // additional trading-related non-standard timezones
      {  1, "?"                              , "001"  , "GMT"                         , L"gmt"                            },
      { 42, "?"                              , "001"  , "FXT"                         , L"fxt"                            },

      // (UTC-12:00) International Date Line West
      {  0, "Dateline Standard Time"         , "001", "Etc/GMT+12"                    , L"etc/gmt+12"                     },
      {  0, "Dateline Standard Time"         , "ZZ" , "Etc/GMT+12"                    , L"etc/gmt+12"                     },

      // (UTC-11:00) Coordinated Universal Time-11
      {  0, "UTC-11"                         , "001", "Etc/GMT+11"                    , L"etc/gmt+11"                     },
      {  0, "UTC-11"                         , "AS" , "Pacific/Pago_Pago"             , L"pacific/pago_pago"              },
      {  0, "UTC-11"                         , "NU" , "Pacific/Niue"                  , L"pacific/niue"                   },
      {  0, "UTC-11"                         , "UM" , "Pacific/Midway"                , L"pacific/midway"                 },
      {  0, "UTC-11"                         , "ZZ" , "Etc/GMT+11"                    , L"etc/gmt+11"                     },

      // (UTC-10:00) Aleutian Islands
      {  0, "Aleutian Standard Time"         , "001", "America/Adak"                  , L"america/adak"                   },
      {  0, "Aleutian Standard Time"         , "US" , "America/Adak"                  , L"america/adak"                   },

      // (UTC-10:00) Hawaii
      {  0, "Hawaiian Standard Time"         , "001", "Pacific/Honolulu"              , L"pacific/honolulu"               },
      {  0, "Hawaiian Standard Time"         , "CK" , "Pacific/Rarotonga"             , L"pacific/rarotonga"              },
      {  0, "Hawaiian Standard Time"         , "PF" , "Pacific/Tahiti"                , L"pacific/tahiti"                 },
      {  0, "Hawaiian Standard Time"         , "US" , "Pacific/Honolulu"              , L"pacific/honolulu"               },
      {  0, "Hawaiian Standard Time"         , "ZZ" , "Etc/GMT+10"                    , L"etc/gmt+10"                     },

      // (UTC-09:30) Marquesas Islands
      {  0, "Marquesas Standard Time"        , "001", "Pacific/Marquesas"             , L"pacific/marquesas"              },
      {  0, "Marquesas Standard Time"        , "PF" , "Pacific/Marquesas"             , L"pacific/marquesas"              },

      // (UTC-09:00) Alaska
      {  0, "Alaskan Standard Time"          , "001", "America/Anchorage"             , L"america/anchorage"              },
      {  0, "Alaskan Standard Time"          , "US" , "America/Anchorage"             , L"america/anchorage"              },
      {  0, "Alaskan Standard Time"          , "US" , "America/Juneau"                , L"america/juneau"                 },
      {  0, "Alaskan Standard Time"          , "US" , "America/Metlakatla"            , L"america/metlakatla"             },
      {  0, "Alaskan Standard Time"          , "US" , "America/Nome"                  , L"america/nome"                   },
      {  0, "Alaskan Standard Time"          , "US" , "America/Sitka"                 , L"america/sitka"                  },
      {  0, "Alaskan Standard Time"          , "US" , "America/Yakutat"               , L"america/yakutat"                },

      // (UTC-09:00) Coordinated Universal Time-09
      {  0, "UTC-09"                         , "001", "Etc/GMT+9"                     , L"etc/gmt+9"                      },
      {  0, "UTC-09"                         , "PF" , "Pacific/Gambier"               , L"pacific/gambier"                },
      {  0, "UTC-09"                         , "ZZ" , "Etc/GMT+9"                     , L"etc/gmt+9"                      },

      // (UTC-08:00) Baja California
      {  0, "Pacific Standard Time (Mexico)" , "001", "America/Tijuana"               , L"america/tijuana"                },
      {  0, "Pacific Standard Time (Mexico)" , "MX" , "America/Tijuana"               , L"america/tijuana"                },

      // (UTC-08:00) Coordinated Universal Time-08
      {  0, "UTC-08"                         , "001", "Etc/GMT+8"                     , L"etc/gmt+8"                      },
      {  0, "UTC-08"                         , "PN" , "Pacific/Pitcairn"              , L"pacific/pitcairn"               },
      {  0, "UTC-08"                         , "ZZ" , "Etc/GMT+8"                     , L"etc/gmt+8"                      },

      // (UTC-08:00) Pacific Time (US & Canada)
      {  0, "Pacific Standard Time"          , "001", "America/Los_Angeles"           , L"america/los_angeles"            },
      {  0, "Pacific Standard Time"          , "CA" , "America/Vancouver"             , L"america/vancouver"              },
      {  0, "Pacific Standard Time"          , "US" , "America/Los_Angeles"           , L"america/los_angeles"            },

      // (UTC-07:00) Arizona
      {  0, "US Mountain Standard Time"      , "001", "America/Phoenix"               , L"america/phoenix"                },
      {  0, "US Mountain Standard Time"      , "CA" , "America/Creston"               , L"america/creston"                },
      {  0, "US Mountain Standard Time"      , "CA" , "America/Dawson_Creek"          , L"america/dawson_creek"           },
      {  0, "US Mountain Standard Time"      , "CA" , "America/Fort_Nelson"           , L"america/fort_nelson"            },
      {  0, "US Mountain Standard Time"      , "MX" , "America/Hermosillo"            , L"america/hermosillo"             },
      {  0, "US Mountain Standard Time"      , "US" , "America/Phoenix"               , L"america/phoenix"                },
      {  0, "US Mountain Standard Time"      , "ZZ" , "Etc/GMT+7"                     , L"etc/gmt+7"                      },

      // (UTC-07:00) Chihuahua, La Paz, Mazatlan
      {  0, "Mountain Standard Time (Mexico)", "001", "America/Mazatlan"              , L"america/mazatlan"               },
      {  0, "Mountain Standard Time (Mexico)", "MX" , "America/Mazatlan"              , L"america/mazatlan"               },

      // (UTC-07:00) Mountain Time (US & Canada)
      {  0, "Mountain Standard Time"         , "001", "America/Denver"                , L"america/denver"                 },
      {  0, "Mountain Standard Time"         , "CA" , "America/Edmonton"              , L"america/edmonton"               },
      {  0, "Mountain Standard Time"         , "CA" , "America/Cambridge_Bay"         , L"america/cambridge_bay"          },
      {  0, "Mountain Standard Time"         , "CA" , "America/Inuvik"                , L"america/inuvik"                 },
      {  0, "Mountain Standard Time"         , "MX" , "America/Ciudad_Juarez"         , L"america/ciudad_juarez"          },
      {  0, "Mountain Standard Time"         , "US" , "America/Denver"                , L"america/denver"                 },
      {  0, "Mountain Standard Time"         , "US" , "America/Boise"                 , L"america/boise"                  },

      // (UTC-07:00) Yukon
      {  0, "Yukon Standard Time"            , "001", "America/Whitehorse"            , L"america/whitehorse"             },
      {  0, "Yukon Standard Time"            , "CA" , "America/Whitehorse"            , L"america/whitehorse"             },
      {  0, "Yukon Standard Time"            , "CA" , "America/Dawson"                , L"america/dawson"                 },

      // (UTC-06:00) Central America
      {  0, "Central America Standard Time"  , "001", "America/Guatemala"             , L"america/guatemala"              },
      {  0, "Central America Standard Time"  , "BZ" , "America/Belize"                , L"america/belize"                 },
      {  0, "Central America Standard Time"  , "CR" , "America/Costa_Rica"            , L"america/costa_rica"             },
      {  0, "Central America Standard Time"  , "EC" , "Pacific/Galapagos"             , L"pacific/galapagos"              },
      {  0, "Central America Standard Time"  , "GT" , "America/Guatemala"             , L"america/guatemala"              },
      {  0, "Central America Standard Time"  , "HN" , "America/Tegucigalpa"           , L"america/tegucigalpa"            },
      {  0, "Central America Standard Time"  , "NI" , "America/Managua"               , L"america/managua"                },
      {  0, "Central America Standard Time"  , "SV" , "America/El_Salvador"           , L"america/el_salvador"            },
      {  0, "Central America Standard Time"  , "ZZ" , "Etc/GMT+6"                     , L"etc/gmt+6"                      },

      // (UTC-06:00) Central Time (US & Canada)
      {  0, "Central Standard Time"          , "001", "America/Chicago"               , L"america/chicago"                },
      {  0, "Central Standard Time"          , "CA" , "America/Winnipeg"              , L"america/winnipeg"               },
      {  0, "Central Standard Time"          , "CA" , "America/Rankin_Inlet"          , L"america/rankin_inlet"           },
      {  0, "Central Standard Time"          , "CA" , "America/Resolute"              , L"america/resolute"               },
      {  0, "Central Standard Time"          , "MX" , "America/Matamoros"             , L"america/matamoros"              },
      {  0, "Central Standard Time"          , "MX" , "America/Ojinaga"               , L"america/ojinaga"                },
      {  0, "Central Standard Time"          , "US" , "America/Chicago"               , L"america/chicago"                },
      {  0, "Central Standard Time"          , "US" , "America/Indiana/Knox"          , L"america/indiana/knox"           },
      {  0, "Central Standard Time"          , "US" , "America/Indiana/Tell_City"     , L"america/indiana/tell_city"      },
      {  0, "Central Standard Time"          , "US" , "America/Menominee"             , L"america/menominee"              },
      {  0, "Central Standard Time"          , "US" , "America/North_Dakota/Beulah"   , L"america/north_dakota/beulah"    },
      {  0, "Central Standard Time"          , "US" , "America/North_Dakota/Center"   , L"america/north_dakota/center"    },
      {  0, "Central Standard Time"          , "US" , "America/North_Dakota/New_Salem", L"america/north_dakota/new_salem" },

      // (UTC-06:00) Easter Island
      {  0, "Easter Island Standard Time"    , "001", "Pacific/Easter"                , L"pacific/easter"                 },
      {  0, "Easter Island Standard Time"    , "CL" , "Pacific/Easter"                , L"pacific/easter"                 },

      // (UTC-06:00) Guadalajara, Mexico City, Monterrey
      {  0, "Central Standard Time (Mexico)" , "001", "America/Mexico_City"           , L"america/mexico_city"            },
      {  0, "Central Standard Time (Mexico)" , "MX" , "America/Mexico_City"           , L"america/mexico_city"            },
      {  0, "Central Standard Time (Mexico)" , "MX" , "America/Bahia_Banderas"        , L"america/bahia_banderas"         },
      {  0, "Central Standard Time (Mexico)" , "MX" , "America/Merida"                , L"america/merida"                 },
      {  0, "Central Standard Time (Mexico)" , "MX" , "America/Monterrey"             , L"america/monterrey"              },
      {  0, "Central Standard Time (Mexico)" , "MX" , "America/Chihuahua"             , L"america/chihuahua"              },

      // (UTC-06:00) Saskatchewan
      {  0, "Canada Central Standard Time"   , "001", "America/Regina"                , L"america/regina"                 },
      {  0, "Canada Central Standard Time"   , "CA" , "America/Regina"                , L"america/regina"                 },
      {  0, "Canada Central Standard Time"   , "CA" , "America/Swift_Current"         , L"america/swift_current"          },

      // (UTC-05:00) Bogota, Lima, Quito, Rio Branco
      {  0, "SA Pacific Standard Time"       , "001", "America/Bogota"                , L"america/bogota"                 },
      {  0, "SA Pacific Standard Time"       , "BR" , "America/Rio_Branco"            , L"america/rio_branco"             },
      {  0, "SA Pacific Standard Time"       , "BR" , "America/Eirunepe"              , L"america/eirunepe"               },
      {  0, "SA Pacific Standard Time"       , "CA" , "America/Coral_Harbour"         , L"america/coral_harbour"          },
      {  0, "SA Pacific Standard Time"       , "CO" , "America/Bogota"                , L"america/bogota"                 },
      {  0, "SA Pacific Standard Time"       , "EC" , "America/Guayaquil"             , L"america/guayaquil"              },
      {  0, "SA Pacific Standard Time"       , "JM" , "America/Jamaica"               , L"america/jamaica"                },
      {  0, "SA Pacific Standard Time"       , "KY" , "America/Cayman"                , L"america/cayman"                 },
      {  0, "SA Pacific Standard Time"       , "PA" , "America/Panama"                , L"america/panama"                 },
      {  0, "SA Pacific Standard Time"       , "PE" , "America/Lima"                  , L"america/lima"                   },
      {  0, "SA Pacific Standard Time"       , "ZZ" , "Etc/GMT+5"                     , L"etc/gmt+5"                      },

      // (UTC-05:00) Chetumal
      {  0, "Eastern Standard Time (Mexico)" , "001", "America/Cancun"                , L"america/cancun"                 },
      {  0, "Eastern Standard Time (Mexico)" , "MX" , "America/Cancun"                , L"america/cancun"                 },

      // (UTC-05:00) Eastern Time (US & Canada)
      {  0, "Eastern Standard Time"          , "001", "America/New_York"              , L"america/new_york"               },
      {  0, "Eastern Standard Time"          , "BS" , "America/Nassau"                , L"america/nassau"                 },
      {  0, "Eastern Standard Time"          , "CA" , "America/Toronto"               , L"america/toronto"                },
      {  0, "Eastern Standard Time"          , "CA" , "America/Iqaluit"               , L"america/iqaluit"                },
      {  0, "Eastern Standard Time"          , "US" , "America/New_York"              , L"america/new_york"               },
      {  0, "Eastern Standard Time"          , "US" , "America/Detroit"               , L"america/detroit"                },
      {  0, "Eastern Standard Time"          , "US" , "America/Indiana/Petersburg"    , L"america/indiana/petersburg"     },
      {  0, "Eastern Standard Time"          , "US" , "America/Indiana/Vincennes"     , L"america/indiana/vincennes"      },
      {  0, "Eastern Standard Time"          , "US" , "America/Indiana/Winamac"       , L"america/indiana/winamac"        },
      {  0, "Eastern Standard Time"          , "US" , "America/Kentucky/Monticello"   , L"america/kentucky/monticello"    },
      {  0, "Eastern Standard Time"          , "US" , "America/Louisville"            , L"america/louisville"             },

      // (UTC-05:00) Haiti
      {  0, "Haiti Standard Time"            , "001", "America/Port-au-Prince"        , L"america/port-au-prince"         },
      {  0, "Haiti Standard Time"            , "HT" , "America/Port-au-Prince"        , L"america/port-au-prince"         },

      // (UTC-05:00) Havana
      {  0, "Cuba Standard Time"             , "001", "America/Havana"                , L"america/havana"                 },
      {  0, "Cuba Standard Time"             , "CU" , "America/Havana"                , L"america/havana"                 },

      // (UTC-05:00) Indiana (East)
      {  0, "US Eastern Standard Time"       , "001", "America/Indianapolis"          , L"america/indianapolis"           },
      {  0, "US Eastern Standard Time"       , "US" , "America/Indianapolis"          , L"america/indianapolis"           },
      {  0, "US Eastern Standard Time"       , "US" , "America/Indiana/Marengo"       , L"america/indiana/marengo"        },
      {  0, "US Eastern Standard Time"       , "US" , "America/Indiana/Vevay"         , L"america/indiana/vevay"          },

      // (UTC-05:00) Turks and Caicos
      {  0, "Turks And Caicos Standard Time" , "001", "America/Grand_Turk"            , L"america/grand_turk"             },
      {  0, "Turks And Caicos Standard Time" , "TC" , "America/Grand_Turk"            , L"america/grand_turk"             },

      // (UTC-04:00) Asuncion
      {  0, "Paraguay Standard Time"         , "001", "America/Asuncion"              , L"america/asuncion"               },
      {  0, "Paraguay Standard Time"         , "PY" , "America/Asuncion"              , L"america/asuncion"               },

      // (UTC-04:00) Atlantic Time (Canada)
      {  0, "Atlantic Standard Time"         , "001", "America/Halifax"               , L"america/halifax"                },
      {  0, "Atlantic Standard Time"         , "BM" , "Atlantic/Bermuda"              , L"atlantic/bermuda"               },
      {  0, "Atlantic Standard Time"         , "CA" , "America/Halifax"               , L"america/halifax"                },
      {  0, "Atlantic Standard Time"         , "CA" , "America/Glace_Bay"             , L"america/glace_bay"              },
      {  0, "Atlantic Standard Time"         , "CA" , "America/Goose_Bay"             , L"america/goose_bay"              },
      {  0, "Atlantic Standard Time"         , "CA" , "America/Moncton"               , L"america/moncton"                },
      {  0, "Atlantic Standard Time"         , "GL" , "America/Thule"                 , L"america/thule"                  },

      // (UTC-04:00) Caracas
      {  0, "Venezuela Standard Time"        , "001", "America/Caracas"               , L"america/caracas"                },
      {  0, "Venezuela Standard Time"        , "VE" , "America/Caracas"               , L"america/caracas"                },

      // (UTC-04:00) Cuiaba
      {  0, "Central Brazilian Standard Time", "001", "America/Cuiaba"                , L"america/cuiaba"                 },
      {  0, "Central Brazilian Standard Time", "BR" , "America/Cuiaba"                , L"america/cuiaba"                 },
      {  0, "Central Brazilian Standard Time", "BR" , "America/Campo_Grande"          , L"america/campo_grande"           },

      // (UTC-04:00) Georgetown, La Paz, Manaus, San Juan
      {  0, "SA Western Standard Time"       , "001", "America/La_Paz"                , L"america/la_paz"                 },
      {  0, "SA Western Standard Time"       , "AG" , "America/Antigua"               , L"america/antigua"                },
      {  0, "SA Western Standard Time"       , "AI" , "America/Anguilla"              , L"america/anguilla"               },
      {  0, "SA Western Standard Time"       , "AW" , "America/Aruba"                 , L"america/aruba"                  },
      {  0, "SA Western Standard Time"       , "BB" , "America/Barbados"              , L"america/barbados"               },
      {  0, "SA Western Standard Time"       , "BL" , "America/St_Barthelemy"         , L"america/st_barthelemy"          },
      {  0, "SA Western Standard Time"       , "BO" , "America/La_Paz"                , L"america/la_paz"                 },
      {  0, "SA Western Standard Time"       , "BQ" , "America/Kralendijk"            , L"america/kralendijk"             },
      {  0, "SA Western Standard Time"       , "BR" , "America/Manaus"                , L"america/manaus"                 },
      {  0, "SA Western Standard Time"       , "BR" , "America/Boa_Vista"             , L"america/boa_vista"              },
      {  0, "SA Western Standard Time"       , "BR" , "America/Porto_Velho"           , L"america/porto_velho"            },
      {  0, "SA Western Standard Time"       , "CA" , "America/Blanc-Sablon"          , L"america/blanc-sablon"           },
      {  0, "SA Western Standard Time"       , "CW" , "America/Curacao"               , L"america/curacao"                },
      {  0, "SA Western Standard Time"       , "DM" , "America/Dominica"              , L"america/dominica"               },
      {  0, "SA Western Standard Time"       , "DO" , "America/Santo_Domingo"         , L"america/santo_domingo"          },
      {  0, "SA Western Standard Time"       , "GD" , "America/Grenada"               , L"america/grenada"                },
      {  0, "SA Western Standard Time"       , "GP" , "America/Guadeloupe"            , L"america/guadeloupe"             },
      {  0, "SA Western Standard Time"       , "GY" , "America/Guyana"                , L"america/guyana"                 },
      {  0, "SA Western Standard Time"       , "KN" , "America/St_Kitts"              , L"america/st_kitts"               },
      {  0, "SA Western Standard Time"       , "LC" , "America/St_Lucia"              , L"america/st_lucia"               },
      {  0, "SA Western Standard Time"       , "MF" , "America/Marigot"               , L"america/marigot"                },
      {  0, "SA Western Standard Time"       , "MQ" , "America/Martinique"            , L"america/martinique"             },
      {  0, "SA Western Standard Time"       , "MS" , "America/Montserrat"            , L"america/montserrat"             },
      {  0, "SA Western Standard Time"       , "PR" , "America/Puerto_Rico"           , L"america/puerto_rico"            },
      {  0, "SA Western Standard Time"       , "SX" , "America/Lower_Princes"         , L"america/lower_princes"          },
      {  0, "SA Western Standard Time"       , "TT" , "America/Port_of_Spain"         , L"america/port_of_spain"          },
      {  0, "SA Western Standard Time"       , "VC" , "America/St_Vincent"            , L"america/st_vincent"             },
      {  0, "SA Western Standard Time"       , "VG" , "America/Tortola"               , L"america/tortola"                },
      {  0, "SA Western Standard Time"       , "VI" , "America/St_Thomas"             , L"america/st_thomas"              },
      {  0, "SA Western Standard Time"       , "ZZ" , "Etc/GMT+4"                     , L"etc/gmt+4"                      },

      // (UTC-04:00) Santiago
      {  0, "Pacific SA Standard Time"       , "001", "America/Santiago"              , L"america/santiago"               },
      {  0, "Pacific SA Standard Time"       , "CL" , "America/Santiago"              , L"america/santiago"               },

      // (UTC-03:30) Newfoundland
      {  0, "Newfoundland Standard Time"     , "001", "America/St_Johns"              , L"america/st_johns"               },
      {  0, "Newfoundland Standard Time"     , "CA" , "America/St_Johns"              , L"america/st_johns"               },

      // (UTC-03:00) Araguaina
      {  0, "Tocantins Standard Time"        , "001", "America/Araguaina"             , L"america/araguaina"              },
      {  0, "Tocantins Standard Time"        , "BR" , "America/Araguaina"             , L"america/araguaina"              },

      // (UTC-03:00) Brasilia
      {  0, "E. South America Standard Time" , "001", "America/Sao_Paulo"             , L"america/sao_paulo"              },
      {  0, "E. South America Standard Time" , "BR" , "America/Sao_Paulo"             , L"america/sao_paulo"              },

      // (UTC-03:00) Cayenne, Fortaleza
      {  0, "SA Eastern Standard Time"       , "001", "America/Cayenne"               , L"america/cayenne"                },
      {  0, "SA Eastern Standard Time"       , "AQ" , "Antarctica/Rothera"            , L"antarctica/rothera"             },
      {  0, "SA Eastern Standard Time"       , "AQ" , "Antarctica/Palmer"             , L"antarctica/palmer"              },
      {  0, "SA Eastern Standard Time"       , "BR" , "America/Fortaleza"             , L"america/fortaleza"              },
      {  0, "SA Eastern Standard Time"       , "BR" , "America/Belem"                 , L"america/belem"                  },
      {  0, "SA Eastern Standard Time"       , "BR" , "America/Maceio"                , L"america/maceio"                 },
      {  0, "SA Eastern Standard Time"       , "BR" , "America/Recife"                , L"america/recife"                 },
      {  0, "SA Eastern Standard Time"       , "BR" , "America/Santarem"              , L"america/santarem"               },
      {  0, "SA Eastern Standard Time"       , "FK" , "Atlantic/Stanley"              , L"atlantic/stanley"               },
      {  0, "SA Eastern Standard Time"       , "GF" , "America/Cayenne"               , L"america/cayenne"                },
      {  0, "SA Eastern Standard Time"       , "SR" , "America/Paramaribo"            , L"america/paramaribo"             },
      {  0, "SA Eastern Standard Time"       , "ZZ" , "Etc/GMT+3"                     , L"etc/gmt+3"                      },

      // (UTC-03:00) City of Buenos Aires
      {  0, "Argentina Standard Time"        , "001", "America/Buenos_Aires"          , L"america/buenos_aires"           },
      {  0, "Argentina Standard Time"        , "AR" , "America/Buenos_Aires"          , L"america/buenos_aires"           },
      {  0, "Argentina Standard Time"        , "AR" , "America/Argentina/La_Rioja"    , L"america/argentina/la_rioja"     },
      {  0, "Argentina Standard Time"        , "AR" , "America/Argentina/Rio_Gallegos", L"america/argentina/rio_gallegos" },
      {  0, "Argentina Standard Time"        , "AR" , "America/Argentina/Salta"       , L"america/argentina/salta"        },
      {  0, "Argentina Standard Time"        , "AR" , "America/Argentina/San_Juan"    , L"america/argentina/san_juan"     },
      {  0, "Argentina Standard Time"        , "AR" , "America/Argentina/San_Luis"    , L"america/argentina/san_luis"     },
      {  0, "Argentina Standard Time"        , "AR" , "America/Argentina/Tucuman"     , L"america/argentina/tucuman"      },
      {  0, "Argentina Standard Time"        , "AR" , "America/Argentina/Ushuaia"     , L"america/argentina/ushuaia"      },
      {  0, "Argentina Standard Time"        , "AR" , "America/Catamarca"             , L"america/catamarca"              },
      {  0, "Argentina Standard Time"        , "AR" , "America/Cordoba"               , L"america/cordoba"                },
      {  0, "Argentina Standard Time"        , "AR" , "America/Jujuy"                 , L"america/jujuy"                  },
      {  0, "Argentina Standard Time"        , "AR" , "America/Mendoza"               , L"america/mendoza"                },

      // (UTC-03:00) Greenland
      {  0, "Greenland Standard Time"        , "001", "America/Godthab"               , L"america/godthab"                },
      {  0, "Greenland Standard Time"        , "GL" , "America/Godthab"               , L"america/godthab"                },

      // (UTC-03:00) Montevideo
      {  0, "Montevideo Standard Time"       , "001", "America/Montevideo"            , L"america/montevideo"             },
      {  0, "Montevideo Standard Time"       , "UY" , "America/Montevideo"            , L"america/montevideo"             },

      // (UTC-03:00) Punta Arenas
      {  0, "Magallanes Standard Time"       , "001", "America/Punta_Arenas"          , L"america/punta_arenas"           },
      {  0, "Magallanes Standard Time"       , "CL" , "America/Punta_Arenas"          , L"america/punta_arenas"           },

      // (UTC-03:00) Saint Pierre and Miquelon
      {  0, "Saint Pierre Standard Time"     , "001", "America/Miquelon"              , L"america/miquelon"               },
      {  0, "Saint Pierre Standard Time"     , "PM" , "America/Miquelon"              , L"america/miquelon"               },

      // (UTC-03:00) Salvador
      {  0, "Bahia Standard Time"            , "001", "America/Bahia"                 , L"america/bahia"                  },
      {  0, "Bahia Standard Time"            , "BR" , "America/Bahia"                 , L"america/bahia"                  },

      // (UTC-02:00) Coordinated Universal Time-02
      {  0, "UTC-02"                         , "001", "Etc/GMT+2"                     , L"etc/gmt+2"                      },
      {  0, "UTC-02"                         , "BR" , "America/Noronha"               , L"america/noronha"                },
      {  0, "UTC-02"                         , "GS" , "Atlantic/South_Georgia"        , L"atlantic/south_georgia"         },
      {  0, "UTC-02"                         , "ZZ" , "Etc/GMT+2"                     , L"etc/gmt+2"                      },

      // (UTC-01:00) Azores
      {  0, "Azores Standard Time"           , "001", "Atlantic/Azores"               , L"atlantic/azores"                },
      {  0, "Azores Standard Time"           , "GL" , "America/Scoresbysund"          , L"america/scoresbysund"           },
      {  0, "Azores Standard Time"           , "PT" , "Atlantic/Azores"               , L"atlantic/azores"                },

      // (UTC-01:00) Cabo Verde Is.
      {  0, "Cape Verde Standard Time"       , "001", "Atlantic/Cape_Verde"           , L"atlantic/cape_verde"            },
      {  0, "Cape Verde Standard Time"       , "CV" , "Atlantic/Cape_Verde"           , L"atlantic/cape_verde"            },
      {  0, "Cape Verde Standard Time"       , "ZZ" , "Etc/GMT+1"                     , L"etc/gmt+1"                      },

      // (UTC) Coordinated Universal Time
      {  0, "UTC"                            , "001", "Etc/UTC"                       , L"etc/utc"                        },
      {  0, "UTC"                            , "ZZ" , "Etc/UTC"                       , L"etc/utc"                        },
      {  0, "UTC"                            , "ZZ" , "Etc/GMT"                       , L"etc/gmt"                        },

      // (UTC+00:00) Dublin, Edinburgh, Lisbon, London
      {  0, "GMT Standard Time"              , "001", "Europe/London"                 , L"europe/london"                  },
      {  0, "GMT Standard Time"              , "ES" , "Atlantic/Canary"               , L"atlantic/canary"                },
      {  0, "GMT Standard Time"              , "FO" , "Atlantic/Faeroe"               , L"atlantic/faeroe"                },
      {  0, "GMT Standard Time"              , "GB" , "Europe/London"                 , L"europe/london"                  },
      {  0, "GMT Standard Time"              , "GG" , "Europe/Guernsey"               , L"europe/guernsey"                },
      {  0, "GMT Standard Time"              , "IE" , "Europe/Dublin"                 , L"europe/dublin"                  },
      {  0, "GMT Standard Time"              , "IM" , "Europe/Isle_of_Man"            , L"europe/isle_of_man"             },
      {  0, "GMT Standard Time"              , "JE" , "Europe/Jersey"                 , L"europe/jersey"                  },
      {  0, "GMT Standard Time"              , "PT" , "Europe/Lisbon"                 , L"europe/lisbon"                  },
      {  0, "GMT Standard Time"              , "PT" , "Atlantic/Madeira"              , L"atlantic/madeira"               },

      // (UTC+00:00) Monrovia, Reykjavik
      {  0, "Greenwich Standard Time"        , "001", "Atlantic/Reykjavik"            , L"atlantic/reykjavik"             },
      {  0, "Greenwich Standard Time"        , "BF" , "Africa/Ouagadougou"            , L"africa/ouagadougou"             },
      {  0, "Greenwich Standard Time"        , "CI" , "Africa/Abidjan"                , L"africa/abidjan"                 },
      {  0, "Greenwich Standard Time"        , "GH" , "Africa/Accra"                  , L"africa/accra"                   },
      {  0, "Greenwich Standard Time"        , "GL" , "America/Danmarkshavn"          , L"america/danmarkshavn"           },
      {  0, "Greenwich Standard Time"        , "GM" , "Africa/Banjul"                 , L"africa/banjul"                  },
      {  0, "Greenwich Standard Time"        , "GN" , "Africa/Conakry"                , L"africa/conakry"                 },
      {  0, "Greenwich Standard Time"        , "GW" , "Africa/Bissau"                 , L"africa/bissau"                  },
      {  0, "Greenwich Standard Time"        , "IS" , "Atlantic/Reykjavik"            , L"atlantic/reykjavik"             },
      {  0, "Greenwich Standard Time"        , "LR" , "Africa/Monrovia"               , L"africa/monrovia"                },
      {  0, "Greenwich Standard Time"        , "ML" , "Africa/Bamako"                 , L"africa/bamako"                  },
      {  0, "Greenwich Standard Time"        , "MR" , "Africa/Nouakchott"             , L"africa/nouakchott"              },
      {  0, "Greenwich Standard Time"        , "SH" , "Atlantic/St_Helena"            , L"atlantic/st_helena"             },
      {  0, "Greenwich Standard Time"        , "SL" , "Africa/Freetown"               , L"africa/freetown"                },
      {  0, "Greenwich Standard Time"        , "SN" , "Africa/Dakar"                  , L"africa/dakar"                   },
      {  0, "Greenwich Standard Time"        , "TG" , "Africa/Lome"                   , L"africa/lome"                    },

      // (UTC+00:00) Sao Tome
      {  0, "Sao Tome Standard Time"         , "001", "Africa/Sao_Tome"               , L"africa/sao_tome"                },
      {  0, "Sao Tome Standard Time"         , "ST" , "Africa/Sao_Tome"               , L"africa/sao_tome"                },

      // (UTC+01:00) Casablanca
      {  0, "Morocco Standard Time"          , "001", "Africa/Casablanca"             , L"africa/casablanca"              },
      {  0, "Morocco Standard Time"          , "EH" , "Africa/El_Aaiun"               , L"africa/el_aaiun"                },
      {  0, "Morocco Standard Time"          , "MA" , "Africa/Casablanca"             , L"africa/casablanca"              },

      // (UTC+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna
      {  0, "W. Europe Standard Time"        , "001", "Europe/Berlin"                 , L"europe/berlin"                  },
      {  0, "W. Europe Standard Time"        , "AD" , "Europe/Andorra"                , L"europe/andorra"                 },
      {  0, "W. Europe Standard Time"        , "AT" , "Europe/Vienna"                 , L"europe/vienna"                  },
      {  0, "W. Europe Standard Time"        , "CH" , "Europe/Zurich"                 , L"europe/zurich"                  },
      {  0, "W. Europe Standard Time"        , "DE" , "Europe/Berlin"                 , L"europe/berlin"                  },
      {  0, "W. Europe Standard Time"        , "DE" , "Europe/Busingen"               , L"europe/busingen"                },
      {  0, "W. Europe Standard Time"        , "GI" , "Europe/Gibraltar"              , L"europe/gibraltar"               },
      {  0, "W. Europe Standard Time"        , "IT" , "Europe/Rome"                   , L"europe/rome"                    },
      {  0, "W. Europe Standard Time"        , "LI" , "Europe/Vaduz"                  , L"europe/vaduz"                   },
      {  0, "W. Europe Standard Time"        , "LU" , "Europe/Luxembourg"             , L"europe/luxembourg"              },
      {  0, "W. Europe Standard Time"        , "MC" , "Europe/Monaco"                 , L"europe/monaco"                  },
      {  0, "W. Europe Standard Time"        , "MT" , "Europe/Malta"                  , L"europe/malta"                   },
      {  0, "W. Europe Standard Time"        , "NL" , "Europe/Amsterdam"              , L"europe/amsterdam"               },
      {  0, "W. Europe Standard Time"        , "NO" , "Europe/Oslo"                   , L"europe/oslo"                    },
      {  0, "W. Europe Standard Time"        , "SE" , "Europe/Stockholm"              , L"europe/stockholm"               },
      {  0, "W. Europe Standard Time"        , "SJ" , "Arctic/Longyearbyen"           , L"arctic/longyearbyen"            },
      {  0, "W. Europe Standard Time"        , "SM" , "Europe/San_Marino"             , L"europe/san_marino"              },
      {  0, "W. Europe Standard Time"        , "VA" , "Europe/Vatican"                , L"europe/vatican"                 },

      // (UTC+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague
      {  0, "Central Europe Standard Time"   , "001", "Europe/Budapest"               , L"europe/budapest"                },
      {  0, "Central Europe Standard Time"   , "AL" , "Europe/Tirane"                 , L"europe/tirane"                  },
      {  0, "Central Europe Standard Time"   , "CZ" , "Europe/Prague"                 , L"europe/prague"                  },
      {  0, "Central Europe Standard Time"   , "HU" , "Europe/Budapest"               , L"europe/budapest"                },
      {  0, "Central Europe Standard Time"   , "ME" , "Europe/Podgorica"              , L"europe/podgorica"               },
      {  0, "Central Europe Standard Time"   , "RS" , "Europe/Belgrade"               , L"europe/belgrade"                },
      {  0, "Central Europe Standard Time"   , "SI" , "Europe/Ljubljana"              , L"europe/ljubljana"               },
      {  0, "Central Europe Standard Time"   , "SK" , "Europe/Bratislava"             , L"europe/bratislava"              },

      // (UTC+01:00) Brussels, Copenhagen, Madrid, Paris
      {  0, "Romance Standard Time"          , "001", "Europe/Paris"                  , L"europe/paris"                   },
      {  0, "Romance Standard Time"          , "BE" , "Europe/Brussels"               , L"europe/brussels"                },
      {  0, "Romance Standard Time"          , "DK" , "Europe/Copenhagen"             , L"europe/copenhagen"              },
      {  0, "Romance Standard Time"          , "ES" , "Europe/Madrid"                 , L"europe/madrid"                  },
      {  0, "Romance Standard Time"          , "ES" , "Africa/Ceuta"                  , L"africa/ceuta"                   },
      {  0, "Romance Standard Time"          , "FR" , "Europe/Paris"                  , L"europe/paris"                   },

      // (UTC+01:00) Sarajevo, Skopje, Warsaw, Zagreb
      {  0, "Central European Standard Time" , "001", "Europe/Warsaw"                 , L"europe/warsaw"                  },
      {  0, "Central European Standard Time" , "BA" , "Europe/Sarajevo"               , L"europe/sarajevo"                },
      {  0, "Central European Standard Time" , "HR" , "Europe/Zagreb"                 , L"europe/zagreb"                  },
      {  0, "Central European Standard Time" , "MK" , "Europe/Skopje"                 , L"europe/skopje"                  },
      {  0, "Central European Standard Time" , "PL" , "Europe/Warsaw"                 , L"europe/warsaw"                  },

      // (UTC+01:00) West Central Africa
      {  0, "W. Central Africa Standard Time", "001", "Africa/Lagos"                  , L"africa/lagos"                   },
      {  0, "W. Central Africa Standard Time", "AO" , "Africa/Luanda"                 , L"africa/luanda"                  },
      {  0, "W. Central Africa Standard Time", "BJ" , "Africa/Porto-Novo"             , L"africa/porto-novo"              },
      {  0, "W. Central Africa Standard Time", "CD" , "Africa/Kinshasa"               , L"africa/kinshasa"                },
      {  0, "W. Central Africa Standard Time", "CF" , "Africa/Bangui"                 , L"africa/bangui"                  },
      {  0, "W. Central Africa Standard Time", "CG" , "Africa/Brazzaville"            , L"africa/brazzaville"             },
      {  0, "W. Central Africa Standard Time", "CM" , "Africa/Douala"                 , L"africa/douala"                  },
      {  0, "W. Central Africa Standard Time", "DZ" , "Africa/Algiers"                , L"africa/algiers"                 },
      {  0, "W. Central Africa Standard Time", "GA" , "Africa/Libreville"             , L"africa/libreville"              },
      {  0, "W. Central Africa Standard Time", "GQ" , "Africa/Malabo"                 , L"africa/malabo"                  },
      {  0, "W. Central Africa Standard Time", "NE" , "Africa/Niamey"                 , L"africa/niamey"                  },
      {  0, "W. Central Africa Standard Time", "NG" , "Africa/Lagos"                  , L"africa/lagos"                   },
      {  0, "W. Central Africa Standard Time", "TD" , "Africa/Ndjamena"               , L"africa/ndjamena"                },
      {  0, "W. Central Africa Standard Time", "TN" , "Africa/Tunis"                  , L"africa/tunis"                   },
      {  0, "W. Central Africa Standard Time", "ZZ" , "Etc/GMT-1"                     , L"etc/gmt-1"                      },

      // (UTC+02:00) Amman
      {  0, "Jordan Standard Time"           , "001", "Asia/Amman"                    , L"asia/amman"                     },
      {  0, "Jordan Standard Time"           , "JO" , "Asia/Amman"                    , L"asia/amman"                     },

      // (UTC+02:00) Athens, Bucharest
      {  0, "GTB Standard Time"              , "001", "Europe/Bucharest"              , L"europe/bucharest"               },
      {  0, "GTB Standard Time"              , "CY" , "Asia/Nicosia"                  , L"asia/nicosia"                   },
      {  0, "GTB Standard Time"              , "CY" , "Asia/Famagusta"                , L"asia/famagusta"                 },
      {  0, "GTB Standard Time"              , "GR" , "Europe/Athens"                 , L"europe/athens"                  },
      {  0, "GTB Standard Time"              , "RO" , "Europe/Bucharest"              , L"europe/bucharest"               },

      // (UTC+02:00) Beirut
      {  0, "Middle East Standard Time"      , "001", "Asia/Beirut"                   , L"asia/beirut"                    },
      {  0, "Middle East Standard Time"      , "LB" , "Asia/Beirut"                   , L"asia/beirut"                    },

      // (UTC+02:00) Cairo
      {  0, "Egypt Standard Time"            , "001", "Africa/Cairo"                  , L"africa/cairo"                   },
      {  0, "Egypt Standard Time"            , "EG" , "Africa/Cairo"                  , L"africa/cairo"                   },

      // (UTC+02:00) Chisinau
      {  0, "E. Europe Standard Time"        , "001", "Europe/Chisinau"               , L"europe/chisinau"                },
      {  0, "E. Europe Standard Time"        , "MD" , "Europe/Chisinau"               , L"europe/chisinau"                },

      // (UTC+02:00) Damascus
      {  0, "Syria Standard Time"            , "001", "Asia/Damascus"                 , L"asia/damascus"                  },
      {  0, "Syria Standard Time"            , "SY" , "Asia/Damascus"                 , L"asia/damascus"                  },

      // (UTC+02:00) Gaza, Hebron
      {  0, "West Bank Standard Time"        , "001", "Asia/Hebron"                   , L"asia/hebron"                    },
      {  0, "West Bank Standard Time"        , "PS" , "Asia/Hebron"                   , L"asia/hebron"                    },
      {  0, "West Bank Standard Time"        , "PS" , "Asia/Gaza"                     , L"asia/gaza"                      },

      // (UTC+02:00) Harare, Pretoria
      {  0, "South Africa Standard Time"     , "001", "Africa/Johannesburg"           , L"africa/johannesburg"            },
      {  0, "South Africa Standard Time"     , "BI" , "Africa/Bujumbura"              , L"africa/bujumbura"               },
      {  0, "South Africa Standard Time"     , "BW" , "Africa/Gaborone"               , L"africa/gaborone"                },
      {  0, "South Africa Standard Time"     , "CD" , "Africa/Lubumbashi"             , L"africa/lubumbashi"              },
      {  0, "South Africa Standard Time"     , "LS" , "Africa/Maseru"                 , L"africa/maseru"                  },
      {  0, "South Africa Standard Time"     , "MW" , "Africa/Blantyre"               , L"africa/blantyre"                },
      {  0, "South Africa Standard Time"     , "MZ" , "Africa/Maputo"                 , L"africa/maputo"                  },
      {  0, "South Africa Standard Time"     , "RW" , "Africa/Kigali"                 , L"africa/kigali"                  },
      {  0, "South Africa Standard Time"     , "SZ" , "Africa/Mbabane"                , L"africa/mbabane"                 },
      {  0, "South Africa Standard Time"     , "ZA" , "Africa/Johannesburg"           , L"africa/johannesburg"            },
      {  0, "South Africa Standard Time"     , "ZM" , "Africa/Lusaka"                 , L"africa/lusaka"                  },
      {  0, "South Africa Standard Time"     , "ZW" , "Africa/Harare"                 , L"africa/harare"                  },
      {  0, "South Africa Standard Time"     , "ZZ" , "Etc/GMT-2"                     , L"etc/gmt-2"                      },

      // (UTC+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius
      {  0, "FLE Standard Time"              , "001", "Europe/Kiev"                   , L"europe/kiev"                    },
      {  0, "FLE Standard Time"              , "AX" , "Europe/Mariehamn"              , L"europe/mariehamn"               },
      {  0, "FLE Standard Time"              , "BG" , "Europe/Sofia"                  , L"europe/sofia"                   },
      {  0, "FLE Standard Time"              , "EE" , "Europe/Tallinn"                , L"europe/tallinn"                 },
      {  0, "FLE Standard Time"              , "FI" , "Europe/Helsinki"               , L"europe/helsinki"                },
      {  0, "FLE Standard Time"              , "LT" , "Europe/Vilnius"                , L"europe/vilnius"                 },
      {  0, "FLE Standard Time"              , "LV" , "Europe/Riga"                   , L"europe/riga"                    },
      {  0, "FLE Standard Time"              , "UA" , "Europe/Kiev"                   , L"europe/kiev"                    },

      // (UTC+02:00) Jerusalem
      {  0, "Israel Standard Time"           , "001", "Asia/Jerusalem"                , L"asia/jerusalem"                 },
      {  0, "Israel Standard Time"           , "IL" , "Asia/Jerusalem"                , L"asia/jerusalem"                 },

      // (UTC+02:00) Juba
      {  0, "South Sudan Standard Time"      , "001", "Africa/Juba"                   , L"africa/juba"                    },
      {  0, "South Sudan Standard Time"      , "SS" , "Africa/Juba"                   , L"africa/juba"                    },

      // (UTC+02:00) Kaliningrad
      {  0, "Kaliningrad Standard Time"      , "001", "Europe/Kaliningrad"            , L"europe/kaliningrad"             },
      {  0, "Kaliningrad Standard Time"      , "RU" , "Europe/Kaliningrad"            , L"europe/kaliningrad"             },

      // (UTC+02:00) Khartoum
      {  0, "Sudan Standard Time"            , "001", "Africa/Khartoum"               , L"africa/khartoum"                },
      {  0, "Sudan Standard Time"            , "SD" , "Africa/Khartoum"               , L"africa/khartoum"                },

      // (UTC+02:00) Tripoli
      {  0, "Libya Standard Time"            , "001", "Africa/Tripoli"                , L"africa/tripoli"                 },
      {  0, "Libya Standard Time"            , "LY" , "Africa/Tripoli"                , L"africa/tripoli"                 },

      // (UTC+02:00) Windhoek
      {  0, "Namibia Standard Time"          , "001", "Africa/Windhoek"               , L"africa/windhoek"                },
      {  0, "Namibia Standard Time"          , "NA" , "Africa/Windhoek"               , L"africa/windhoek"                },

      // (UTC+03:00) Baghdad
      {  0, "Arabic Standard Time"           , "001", "Asia/Baghdad"                  , L"asia/baghdad"                   },
      {  0, "Arabic Standard Time"           , "IQ" , "Asia/Baghdad"                  , L"asia/baghdad"                   },

      // (UTC+03:00) Istanbul
      {  0, "Turkey Standard Time"           , "001", "Europe/Istanbul"               , L"europe/istanbul"                },
      {  0, "Turkey Standard Time"           , "TR" , "Europe/Istanbul"               , L"europe/istanbul"                },

      // (UTC+03:00) Kuwait, Riyadh
      {  0, "Arab Standard Time"             , "001", "Asia/Riyadh"                   , L"asia/riyadh"                    },
      {  0, "Arab Standard Time"             , "BH" , "Asia/Bahrain"                  , L"asia/bahrain"                   },
      {  0, "Arab Standard Time"             , "KW" , "Asia/Kuwait"                   , L"asia/kuwait"                    },
      {  0, "Arab Standard Time"             , "QA" , "Asia/Qatar"                    , L"asia/qatar"                     },
      {  0, "Arab Standard Time"             , "SA" , "Asia/Riyadh"                   , L"asia/riyadh"                    },
      {  0, "Arab Standard Time"             , "YE" , "Asia/Aden"                     , L"asia/aden"                      },

      // (UTC+03:00) Minsk
      {  0, "Belarus Standard Time"          , "001", "Europe/Minsk"                  , L"europe/minsk"                   },
      {  0, "Belarus Standard Time"          , "BY" , "Europe/Minsk"                  , L"europe/minsk"                   },

      // (UTC+03:00) Moscow, St. Petersburg
      {  0, "Russian Standard Time"          , "001", "Europe/Moscow"                 , L"europe/moscow"                  },
      {  0, "Russian Standard Time"          , "RU" , "Europe/Moscow"                 , L"europe/moscow"                  },
      {  0, "Russian Standard Time"          , "RU" , "Europe/Kirov"                  , L"europe/kirov"                   },
      {  0, "Russian Standard Time"          , "UA" , "Europe/Simferopol"             , L"europe/simferopol"              },

      // (UTC+03:00) Nairobi
      {  0, "E. Africa Standard Time"        , "001", "Africa/Nairobi"                , L"africa/nairobi"                 },
      {  0, "E. Africa Standard Time"        , "AQ" , "Antarctica/Syowa"              , L"antarctica/syowa"               },
      {  0, "E. Africa Standard Time"        , "DJ" , "Africa/Djibouti"               , L"africa/djibouti"                },
      {  0, "E. Africa Standard Time"        , "ER" , "Africa/Asmera"                 , L"africa/asmera"                  },
      {  0, "E. Africa Standard Time"        , "ET" , "Africa/Addis_Ababa"            , L"africa/addis_ababa"             },
      {  0, "E. Africa Standard Time"        , "KE" , "Africa/Nairobi"                , L"africa/nairobi"                 },
      {  0, "E. Africa Standard Time"        , "KM" , "Indian/Comoro"                 , L"indian/comoro"                  },
      {  0, "E. Africa Standard Time"        , "MG" , "Indian/Antananarivo"           , L"indian/antananarivo"            },
      {  0, "E. Africa Standard Time"        , "SO" , "Africa/Mogadishu"              , L"africa/mogadishu"               },
      {  0, "E. Africa Standard Time"        , "TZ" , "Africa/Dar_es_Salaam"          , L"africa/dar_es_salaam"           },
      {  0, "E. Africa Standard Time"        , "UG" , "Africa/Kampala"                , L"africa/kampala"                 },
      {  0, "E. Africa Standard Time"        , "YT" , "Indian/Mayotte"                , L"indian/mayotte"                 },
      {  0, "E. Africa Standard Time"        , "ZZ" , "Etc/GMT-3"                     , L"etc/gmt-3"                      },

      // (UTC+03:30) Tehran
      {  0, "Iran Standard Time"             , "001", "Asia/Tehran"                   , L"asia/tehran"                    },
      {  0, "Iran Standard Time"             , "IR" , "Asia/Tehran"                   , L"asia/tehran"                    },

      // (UTC+04:00) Abu Dhabi, Muscat
      {  0, "Arabian Standard Time"          , "001", "Asia/Dubai"                    , L"asia/dubai"                     },
      {  0, "Arabian Standard Time"          , "AE" , "Asia/Dubai"                    , L"asia/dubai"                     },
      {  0, "Arabian Standard Time"          , "OM" , "Asia/Muscat"                   , L"asia/muscat"                    },
      {  0, "Arabian Standard Time"          , "ZZ" , "Etc/GMT-4"                     , L"etc/gmt-4"                      },

      // (UTC+04:00) Astrakhan, Ulyanovsk
      {  0, "Astrakhan Standard Time"        , "001", "Europe/Astrakhan"              , L"europe/astrakhan"               },
      {  0, "Astrakhan Standard Time"        , "RU" , "Europe/Astrakhan"              , L"europe/astrakhan"               },
      {  0, "Astrakhan Standard Time"        , "RU" , "Europe/Ulyanovsk"              , L"europe/ulyanovsk"               },

      // (UTC+04:00) Baku
      {  0, "Azerbaijan Standard Time"       , "001", "Asia/Baku"                     , L"asia/baku"                      },
      {  0, "Azerbaijan Standard Time"       , "AZ" , "Asia/Baku"                     , L"asia/baku"                      },

      // (UTC+04:00) Izhevsk, Samara
      {  0, "Russia Time Zone 3"             , "001", "Europe/Samara"                 , L"europe/samara"                  },
      {  0, "Russia Time Zone 3"             , "RU" , "Europe/Samara"                 , L"europe/samara"                  },

      // (UTC+04:00) Port Louis
      {  0, "Mauritius Standard Time"        , "001", "Indian/Mauritius"              , L"indian/mauritius"               },
      {  0, "Mauritius Standard Time"        , "MU" , "Indian/Mauritius"              , L"indian/mauritius"               },
      {  0, "Mauritius Standard Time"        , "RE" , "Indian/Reunion"                , L"indian/reunion"                 },
      {  0, "Mauritius Standard Time"        , "SC" , "Indian/Mahe"                   , L"indian/mahe"                    },

      // (UTC+04:00) Saratov
      {  0, "Saratov Standard Time"          , "001", "Europe/Saratov"                , L"europe/saratov"                 },
      {  0, "Saratov Standard Time"          , "RU" , "Europe/Saratov"                , L"europe/saratov"                 },

      // (UTC+04:00) Tbilisi
      {  0, "Georgian Standard Time"         , "001", "Asia/Tbilisi"                  , L"asia/tbilisi"                   },
      {  0, "Georgian Standard Time"         , "GE" , "Asia/Tbilisi"                  , L"asia/tbilisi"                   },

      // (UTC+04:00) Volgograd
      {  0, "Volgograd Standard Time"        , "001", "Europe/Volgograd"              , L"europe/volgograd"               },
      {  0, "Volgograd Standard Time"        , "RU" , "Europe/Volgograd"              , L"europe/volgograd"               },

      // (UTC+04:00) Yerevan
      {  0, "Caucasus Standard Time"         , "001", "Asia/Yerevan"                  , L"asia/yerevan"                   },
      {  0, "Caucasus Standard Time"         , "AM" , "Asia/Yerevan"                  , L"asia/yerevan"                   },

      // (UTC+04:30) Kabul
      {  0, "Afghanistan Standard Time"      , "001", "Asia/Kabul"                    , L"asia/kabul"                     },
      {  0, "Afghanistan Standard Time"      , "AF" , "Asia/Kabul"                    , L"asia/kabul"                     },

      // (UTC+05:00) Ashgabat, Tashkent
      {  0, "West Asia Standard Time"        , "001", "Asia/Tashkent"                 , L"asia/tashkent"                  },
      {  0, "West Asia Standard Time"        , "AQ" , "Antarctica/Mawson"             , L"antarctica/mawson"              },
      // Microsoft may create a new zone dedicated for Almaty and Qostanay.
      {  0, "West Asia Standard Time"        , "KZ" , "Asia/Oral"                     , L"asia/oral"                      },
      {  0, "West Asia Standard Time"        , "KZ" , "Asia/Almaty"                   , L"asia/almaty"                    },
      {  0, "West Asia Standard Time"        , "KZ" , "Asia/Aqtau"                    , L"asia/aqtau"                     },
      {  0, "West Asia Standard Time"        , "KZ" , "Asia/Aqtobe"                   , L"asia/aqtobe"                    },
      {  0, "West Asia Standard Time"        , "KZ" , "Asia/Atyrau"                   , L"asia/atyrau"                    },
      {  0, "West Asia Standard Time"        , "KZ" , "Asia/Qostanay"                 , L"asia/qostanay"                  },
      {  0, "West Asia Standard Time"        , "MV" , "Indian/Maldives"               , L"indian/maldives"                },
      {  0, "West Asia Standard Time"        , "TF" , "Indian/Kerguelen"              , L"indian/kerguelen"               },
      {  0, "West Asia Standard Time"        , "TJ" , "Asia/Dushanbe"                 , L"asia/dushanbe"                  },
      {  0, "West Asia Standard Time"        , "TM" , "Asia/Ashgabat"                 , L"asia/ashgabat"                  },
      {  0, "West Asia Standard Time"        , "UZ" , "Asia/Tashkent"                 , L"asia/tashkent"                  },
      {  0, "West Asia Standard Time"        , "UZ" , "Asia/Samarkand"                , L"asia/samarkand"                 },
      {  0, "West Asia Standard Time"        , "ZZ" , "Etc/GMT-5"                     , L"etc/gmt-5"                      },

      // (UTC+05:00) Ekaterinburg
      {  0, "Ekaterinburg Standard Time"     , "001", "Asia/Yekaterinburg"            , L"asia/yekaterinburg"             },
      {  0, "Ekaterinburg Standard Time"     , "RU" , "Asia/Yekaterinburg"            , L"asia/yekaterinburg"             },

      // (UTC+05:00) Islamabad, Karachi
      {  0, "Pakistan Standard Time"         , "001", "Asia/Karachi"                  , L"asia/karachi"                   },
      {  0, "Pakistan Standard Time"         , "PK" , "Asia/Karachi"                  , L"asia/karachi"                   },

      // (UTC+05:00) Qyzylorda
      {  0, "Qyzylorda Standard Time"        , "001", "Asia/Qyzylorda"                , L"asia/qyzylorda"                 },
      {  0, "Qyzylorda Standard Time"        , "KZ" , "Asia/Qyzylorda"                , L"asia/qyzylorda"                 },

      // (UTC+05:30) Chennai, Kolkata, Mumbai, New Delhi
      {  0, "India Standard Time"            , "001", "Asia/Calcutta"                 , L"asia/calcutta"                  },
      {  0, "India Standard Time"            , "IN" , "Asia/Calcutta"                 , L"asia/calcutta"                  },

      // (UTC+05:30) Sri Jayawardenepura
      {  0, "Sri Lanka Standard Time"        , "001", "Asia/Colombo"                  , L"asia/colombo"                   },
      {  0, "Sri Lanka Standard Time"        , "LK" , "Asia/Colombo"                  , L"asia/colombo"                   },

      // (UTC+05:45) Kathmandu
      {  0, "Nepal Standard Time"            , "001", "Asia/Katmandu"                 , L"asia/katmandu"                  },
      {  0, "Nepal Standard Time"            , "NP" , "Asia/Katmandu"                 , L"asia/katmandu"                  },

      // (UTC+06:00) Astana  // Microsoft probably keeps Central Asia Standard Time, but change Astana to something else.
      {  0, "Central Asia Standard Time"     , "001", "Asia/Bishkek"                  , L"asia/bishkek"                   },
      {  0, "Central Asia Standard Time"     , "AQ" , "Antarctica/Vostok"             , L"antarctica/vostok"              },
      {  0, "Central Asia Standard Time"     , "CN" , "Asia/Urumqi"                   , L"asia/urumqi"                    },
      {  0, "Central Asia Standard Time"     , "IO" , "Indian/Chagos"                 , L"indian/chagos"                  },
      {  0, "Central Asia Standard Time"     , "KG" , "Asia/Bishkek"                  , L"asia/bishkek"                   },
      {  0, "Central Asia Standard Time"     , "ZZ" , "Etc/GMT-6"                     , L"etc/gmt-6"                      },

      // (UTC+06:00) Dhaka
      {  0, "Bangladesh Standard Time"       , "001", "Asia/Dhaka"                    , L"asia/dhaka"                     },
      {  0, "Bangladesh Standard Time"       , "BD" , "Asia/Dhaka"                    , L"asia/dhaka"                     },
      {  0, "Bangladesh Standard Time"       , "BT" , "Asia/Thimphu"                  , L"asia/thimphu"                   },

      // (UTC+06:00) Omsk
      {  0, "Omsk Standard Time"             , "001", "Asia/Omsk"                     , L"asia/omsk"                      },
      {  0, "Omsk Standard Time"             , "RU" , "Asia/Omsk"                     , L"asia/omsk"                      },

      // (UTC+06:30) Yangon (Rangoon)
      {  0, "Myanmar Standard Time"          , "001", "Asia/Rangoon"                  , L"asia/rangoon"                   },
      {  0, "Myanmar Standard Time"          , "CC" , "Indian/Cocos"                  , L"indian/cocos"                   },
      {  0, "Myanmar Standard Time"          , "MM" , "Asia/Rangoon"                  , L"asia/rangoon"                   },

      // (UTC+07:00) Bangkok, Hanoi, Jakarta
      {  0, "SE Asia Standard Time"          , "001", "Asia/Bangkok"                  , L"asia/bangkok"                   },
      {  0, "SE Asia Standard Time"          , "AQ" , "Antarctica/Davis"              , L"antarctica/davis"               },
      {  0, "SE Asia Standard Time"          , "CX" , "Indian/Christmas"              , L"indian/christmas"               },
      {  0, "SE Asia Standard Time"          , "ID" , "Asia/Jakarta"                  , L"asia/jakarta"                   },
      {  0, "SE Asia Standard Time"          , "ID" , "Asia/Pontianak"                , L"asia/pontianak"                 },
      {  0, "SE Asia Standard Time"          , "KH" , "Asia/Phnom_Penh"               , L"asia/phnom_penh"                },
      {  0, "SE Asia Standard Time"          , "LA" , "Asia/Vientiane"                , L"asia/vientiane"                 },
      {  0, "SE Asia Standard Time"          , "TH" , "Asia/Bangkok"                  , L"asia/bangkok"                   },
      {  0, "SE Asia Standard Time"          , "VN" , "Asia/Saigon"                   , L"asia/saigon"                    },
      {  0, "SE Asia Standard Time"          , "ZZ" , "Etc/GMT-7"                     , L"etc/gmt-7"                      },

      // (UTC+07:00) Barnaul, Gorno-Altaysk
      {  0, "Altai Standard Time"            , "001", "Asia/Barnaul"                  , L"asia/barnaul"                   },
      {  0, "Altai Standard Time"            , "RU" , "Asia/Barnaul"                  , L"asia/barnaul"                   },

      // (UTC+07:00) Hovd
      {  0, "W. Mongolia Standard Time"      , "001", "Asia/Hovd"                     , L"asia/hovd"                      },
      {  0, "W. Mongolia Standard Time"      , "MN" , "Asia/Hovd"                     , L"asia/hovd"                      },

      // (UTC+07:00) Krasnoyarsk
      {  0, "North Asia Standard Time"       , "001", "Asia/Krasnoyarsk"              , L"asia/krasnoyarsk"               },
      {  0, "North Asia Standard Time"       , "RU" , "Asia/Krasnoyarsk"              , L"asia/krasnoyarsk"               },
      {  0, "North Asia Standard Time"       , "RU" , "Asia/Novokuznetsk"             , L"asia/novokuznetsk"              },

      // (UTC+07:00) Novosibirsk
      {  0, "N. Central Asia Standard Time"  , "001", "Asia/Novosibirsk"              , L"asia/novosibirsk"               },
      {  0, "N. Central Asia Standard Time"  , "RU" , "Asia/Novosibirsk"              , L"asia/novosibirsk"               },

      // (UTC+07:00) Tomsk
      {  0, "Tomsk Standard Time"            , "001", "Asia/Tomsk"                    , L"asia/tomsk"                     },
      {  0, "Tomsk Standard Time"            , "RU" , "Asia/Tomsk"                    , L"asia/tomsk"                     },

      // (UTC+08:00) Beijing, Chongqing, Hong Kong, Urumqi
      {  0, "China Standard Time"            , "001", "Asia/Shanghai"                 , L"asia/shanghai"                  },
      {  0, "China Standard Time"            , "CN" , "Asia/Shanghai"                 , L"asia/shanghai"                  },
      {  0, "China Standard Time"            , "HK" , "Asia/Hong_Kong"                , L"asia/hong_kong"                 },
      {  0, "China Standard Time"            , "MO" , "Asia/Macau"                    , L"asia/macau"                     },

      // (UTC+08:00) Irkutsk
      {  0, "North Asia East Standard Time"  , "001", "Asia/Irkutsk"                  , L"asia/irkutsk"                   },
      {  0, "North Asia East Standard Time"  , "RU" , "Asia/Irkutsk"                  , L"asia/irkutsk"                   },

      // (UTC+08:00) Kuala Lumpur, Singapore
      {  0, "Singapore Standard Time"        , "001", "Asia/Singapore"                , L"asia/singapore"                 },
      {  0, "Singapore Standard Time"        , "BN" , "Asia/Brunei"                   , L"asia/brunei"                    },
      {  0, "Singapore Standard Time"        , "ID" , "Asia/Makassar"                 , L"asia/makassar"                  },
      {  0, "Singapore Standard Time"        , "MY" , "Asia/Kuala_Lumpur"             , L"asia/kuala_lumpur"              },
      {  0, "Singapore Standard Time"        , "MY" , "Asia/Kuching"                  , L"asia/kuching"                   },
      {  0, "Singapore Standard Time"        , "PH" , "Asia/Manila"                   , L"asia/manila"                    },
      {  0, "Singapore Standard Time"        , "SG" , "Asia/Singapore"                , L"asia/singapore"                 },
      {  0, "Singapore Standard Time"        , "ZZ" , "Etc/GMT-8"                     , L"etc/gmt-8"                      },

      // (UTC+08:00) Perth
      {  0, "W. Australia Standard Time"     , "001", "Australia/Perth"               , L"australia/perth"                },
      {  0, "W. Australia Standard Time"     , "AU" , "Australia/Perth"               , L"australia/perth"                },

      // (UTC+08:00) Taipei
      {  0, "Taipei Standard Time"           , "001", "Asia/Taipei"                   , L"asia/taipei"                    },
      {  0, "Taipei Standard Time"           , "TW" , "Asia/Taipei"                   , L"asia/taipei"                    },

      // (UTC+08:00) Ulaanbaatar
      {  0, "Ulaanbaatar Standard Time"      , "001", "Asia/Ulaanbaatar"              , L"asia/ulaanbaatar"               },
      {  0, "Ulaanbaatar Standard Time"      , "MN" , "Asia/Ulaanbaatar"              , L"asia/ulaanbaatar"               },

      // (UTC+08:45) Eucla
      {  0, "Aus Central W. Standard Time"   , "001", "Australia/Eucla"               , L"australia/eucla"                },
      {  0, "Aus Central W. Standard Time"   , "AU" , "Australia/Eucla"               , L"australia/eucla"                },

      // (UTC+09:00) Chita
      {  0, "Transbaikal Standard Time"      , "001", "Asia/Chita"                    , L"asia/chita"                     },
      {  0, "Transbaikal Standard Time"      , "RU" , "Asia/Chita"                    , L"asia/chita"                     },

      // (UTC+09:00) Osaka, Sapporo, Tokyo
      {  0, "Tokyo Standard Time"            , "001", "Asia/Tokyo"                    , L"asia/tokyo"                     },
      {  0, "Tokyo Standard Time"            , "ID" , "Asia/Jayapura"                 , L"asia/jayapura"                  },
      {  0, "Tokyo Standard Time"            , "JP" , "Asia/Tokyo"                    , L"asia/tokyo"                     },
      {  0, "Tokyo Standard Time"            , "PW" , "Pacific/Palau"                 , L"pacific/palau"                  },
      {  0, "Tokyo Standard Time"            , "TL" , "Asia/Dili"                     , L"asia/dili"                      },
      {  0, "Tokyo Standard Time"            , "ZZ" , "Etc/GMT-9"                     , L"etc/gmt-9"                      },

      // (UTC+09:00) Pyongyang
      {  0, "North Korea Standard Time"      , "001", "Asia/Pyongyang"                , L"asia/pyongyang"                 },
      {  0, "North Korea Standard Time"      , "KP" , "Asia/Pyongyang"                , L"asia/pyongyang"                 },

      // (UTC+09:00) Seoul
      {  0, "Korea Standard Time"            , "001", "Asia/Seoul"                    , L"asia/seoul"                     },
      {  0, "Korea Standard Time"            , "KR" , "Asia/Seoul"                    , L"asia/seoul"                     },

      // (UTC+09:00) Yakutsk
      {  0, "Yakutsk Standard Time"          , "001", "Asia/Yakutsk"                  , L"asia/yakutsk"                   },
      {  0, "Yakutsk Standard Time"          , "RU" , "Asia/Yakutsk"                  , L"asia/yakutsk"                   },
      {  0, "Yakutsk Standard Time"          , "RU" , "Asia/Khandyga"                 , L"asia/khandyga"                  },

      // (UTC+09:30) Adelaide
      {  0, "Cen. Australia Standard Time"   , "001", "Australia/Adelaide"            , L"australia/adelaide"             },
      {  0, "Cen. Australia Standard Time"   , "AU" , "Australia/Adelaide"            , L"australia/adelaide"             },
      {  0, "Cen. Australia Standard Time"   , "AU" , "Australia/Broken_Hill"         , L"australia/broken_hill"          },

      // (UTC+09:30) Darwin
      {  0, "AUS Central Standard Time"      , "001", "Australia/Darwin"              , L"australia/darwin"               },
      {  0, "AUS Central Standard Time"      , "AU" , "Australia/Darwin"              , L"australia/darwin"               },

      // (UTC+10:00) Brisbane
      {  0, "E. Australia Standard Time"     , "001", "Australia/Brisbane"            , L"australia/brisbane"             },
      {  0, "E. Australia Standard Time"     , "AU" , "Australia/Brisbane"            , L"australia/brisbane"             },
      {  0, "E. Australia Standard Time"     , "AU" , "Australia/Lindeman"            , L"australia/lindeman"             },

      // (UTC+10:00) Canberra, Melbourne, Sydney
      {  0, "AUS Eastern Standard Time"      , "001", "Australia/Sydney"              , L"australia/sydney"               },
      {  0, "AUS Eastern Standard Time"      , "AU" , "Australia/Sydney"              , L"australia/sydney"               },
      {  0, "AUS Eastern Standard Time"      , "AU" , "Australia/Melbourne"           , L"australia/melbourne"            },

      // (UTC+10:00) Guam, Port Moresby
      {  0, "West Pacific Standard Time"     , "001", "Pacific/Port_Moresby"          , L"pacific/port_moresby"           },
      {  0, "West Pacific Standard Time"     , "AQ" , "Antarctica/DumontDUrville"     , L"antarctica/dumontdurville"      },
      {  0, "West Pacific Standard Time"     , "FM" , "Pacific/Truk"                  , L"pacific/truk"                   },
      {  0, "West Pacific Standard Time"     , "GU" , "Pacific/Guam"                  , L"pacific/guam"                   },
      {  0, "West Pacific Standard Time"     , "MP" , "Pacific/Saipan"                , L"pacific/saipan"                 },
      {  0, "West Pacific Standard Time"     , "PG" , "Pacific/Port_Moresby"          , L"pacific/port_moresby"           },
      {  0, "West Pacific Standard Time"     , "ZZ" , "Etc/GMT-10"                    , L"etc/gmt-10"                     },

      // (UTC+10:00) Hobart
      {  0, "Tasmania Standard Time"         , "001", "Australia/Hobart"              , L"australia/hobart"               },
      {  0, "Tasmania Standard Time"         , "AU" , "Australia/Hobart"              , L"australia/hobart"               },
      {  0, "Tasmania Standard Time"         , "AU" , "Antarctica/Macquarie"          , L"antarctica/macquarie"           },

      // (UTC+10:00) Vladivostok
      {  0, "Vladivostok Standard Time"      , "001", "Asia/Vladivostok"              , L"asia/vladivostok"               },
      {  0, "Vladivostok Standard Time"      , "RU" , "Asia/Vladivostok"              , L"asia/vladivostok"               },
      {  0, "Vladivostok Standard Time"      , "RU" , "Asia/Ust-Nera"                 , L"asia/ust-nera"                  },

      // (UTC+10:30) Lord Howe Island
      {  0, "Lord Howe Standard Time"        , "001", "Australia/Lord_Howe"           , L"australia/lord_howe"            },
      {  0, "Lord Howe Standard Time"        , "AU" , "Australia/Lord_Howe"           , L"australia/lord_howe"            },

      // (UTC+11:00) Bougainville Island
      {  0, "Bougainville Standard Time"     , "001", "Pacific/Bougainville"          , L"pacific/bougainville"           },
      {  0, "Bougainville Standard Time"     , "PG" , "Pacific/Bougainville"          , L"pacific/bougainville"           },

      // (UTC+11:00) Chokurdakh
      {  0, "Russia Time Zone 10"            , "001", "Asia/Srednekolymsk"            , L"asia/srednekolymsk"             },
      {  0, "Russia Time Zone 10"            , "RU" , "Asia/Srednekolymsk"            , L"asia/srednekolymsk"             },

      // (UTC+11:00) Magadan
      {  0, "Magadan Standard Time"          , "001", "Asia/Magadan"                  , L"asia/magadan"                   },
      {  0, "Magadan Standard Time"          , "RU" , "Asia/Magadan"                  , L"asia/magadan"                   },

      // (UTC+11:00) Norfolk Island
      {  0, "Norfolk Standard Time"          , "001", "Pacific/Norfolk"               , L"pacific/norfolk"                },
      {  0, "Norfolk Standard Time"          , "NF" , "Pacific/Norfolk"               , L"pacific/norfolk"                },

      // (UTC+11:00) Sakhalin
      {  0, "Sakhalin Standard Time"         , "001", "Asia/Sakhalin"                 , L"asia/sakhalin"                  },
      {  0, "Sakhalin Standard Time"         , "RU" , "Asia/Sakhalin"                 , L"asia/sakhalin"                  },

      // (UTC+11:00) Solomon Is., New Caledonia
      {  0, "Central Pacific Standard Time"  , "001", "Pacific/Guadalcanal"           , L"pacific/guadalcanal"            },
      {  0, "Central Pacific Standard Time"  , "AQ" , "Antarctica/Casey"              , L"antarctica/casey"               },
      {  0, "Central Pacific Standard Time"  , "FM" , "Pacific/Ponape"                , L"pacific/ponape"                 },
      {  0, "Central Pacific Standard Time"  , "FM" , "Pacific/Kosrae"                , L"pacific/kosrae"                 },
      {  0, "Central Pacific Standard Time"  , "NC" , "Pacific/Noumea"                , L"pacific/noumea"                 },
      {  0, "Central Pacific Standard Time"  , "SB" , "Pacific/Guadalcanal"           , L"pacific/guadalcanal"            },
      {  0, "Central Pacific Standard Time"  , "VU" , "Pacific/Efate"                 , L"pacific/efate"                  },
      {  0, "Central Pacific Standard Time"  , "ZZ" , "Etc/GMT-11"                    , L"etc/gmt-11"                     },

      // (UTC+12:00) Anadyr, Petropavlovsk-Kamchatsky
      {  0, "Russia Time Zone 11"            , "001", "Asia/Kamchatka"                , L"asia/kamchatka"                 },
      {  0, "Russia Time Zone 11"            , "RU" , "Asia/Kamchatka"                , L"asia/kamchatka"                 },
      {  0, "Russia Time Zone 11"            , "RU" , "Asia/Anadyr"                   , L"asia/anadyr"                    },

      // (UTC+12:00) Auckland, Wellington
      {  0, "New Zealand Standard Time"      , "001", "Pacific/Auckland"              , L"pacific/auckland"               },
      {  0, "New Zealand Standard Time"      , "AQ" , "Antarctica/McMurdo"            , L"antarctica/mcmurdo"             },
      {  0, "New Zealand Standard Time"      , "NZ" , "Pacific/Auckland"              , L"pacific/auckland"               },

      // (UTC+12:00) Coordinated Universal Time+12
      {  0, "UTC+12"                         , "001", "Etc/GMT-12"                    , L"etc/gmt-12"                     },
      {  0, "UTC+12"                         , "KI" , "Pacific/Tarawa"                , L"pacific/tarawa"                 },
      {  0, "UTC+12"                         , "MH" , "Pacific/Majuro"                , L"pacific/majuro"                 },
      {  0, "UTC+12"                         , "MH" , "Pacific/Kwajalein"             , L"pacific/kwajalein"              },
      {  0, "UTC+12"                         , "NR" , "Pacific/Nauru"                 , L"pacific/nauru"                  },
      {  0, "UTC+12"                         , "TV" , "Pacific/Funafuti"              , L"pacific/funafuti"               },
      {  0, "UTC+12"                         , "UM" , "Pacific/Wake"                  , L"pacific/wake"                   },
      {  0, "UTC+12"                         , "WF" , "Pacific/Wallis"                , L"pacific/wallis"                 },
      {  0, "UTC+12"                         , "ZZ" , "Etc/GMT-12"                    , L"etc/gmt-12"                     },

      // (UTC+12:00) Fiji
      {  0, "Fiji Standard Time"             , "001", "Pacific/Fiji"                  , L"pacific/fiji"                   },
      {  0, "Fiji Standard Time"             , "FJ" , "Pacific/Fiji"                  , L"pacific/fiji"                   },

      // (UTC+12:45) Chatham Islands
      {  0, "Chatham Islands Standard Time"  , "001", "Pacific/Chatham"               , L"pacific/chatham"                },
      {  0, "Chatham Islands Standard Time"  , "NZ" , "Pacific/Chatham"               , L"pacific/chatham"                },

      // (UTC+13:00) Coordinated Universal Time+13
      {  0, "UTC+13"                         , "001", "Etc/GMT-13"                    , L"etc/gmt-13"                     },
      {  0, "UTC+13"                         , "KI" , "Pacific/Enderbury"             , L"pacific/enderbury"              },
      {  0, "UTC+13"                         , "TK" , "Pacific/Fakaofo"               , L"pacific/fakaofo"                },
      {  0, "UTC+13"                         , "ZZ" , "Etc/GMT-13"                    , L"etc/gmt-13"                     },

      // (UTC+13:00) Nuku'alofa
      {  0, "Tonga Standard Time"            , "001", "Pacific/Tongatapu"             , L"pacific/tongatapu"              },
      {  0, "Tonga Standard Time"            , "TO" , "Pacific/Tongatapu"             , L"pacific/tongatapu"              },

      // (UTC+13:00) Samoa
      {  0, "Samoa Standard Time"            , "001", "Pacific/Apia"                  , L"pacific/apia"                   },
      {  0, "Samoa Standard Time"            , "WS" , "Pacific/Apia"                  , L"pacific/apia"                   },

      // (UTC+14:00) Kiritimati Island
      {  0, "Line Islands Standard Time"     , "001", "Pacific/Kiritimati"            , L"pacific/kiritimati"             },
      {  0, "Line Islands Standard Time"     , "KI" , "Pacific/Kiritimati"            , L"pacific/kiritimati"             },
      {  0, "Line Islands Standard Time"     , "ZZ" , "Etc/GMT-14"                    , L"etc/gmt-14"                     },
   };
   return mappings;
}


/**
 * Get a timezone id for an IANA timezone name.
 *
 * @param  char* name - case-insensitive IANA timezone name
 *
 * @return int - timezone id or NULL if no timezone with that name was found
 */
int WINAPI GetTimeZoneIdByIanaNameA(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);

   wchar* str = ansiToUtf16(name);
   if (!str) return NULL;

   int id = GetTimeZoneIdByIanaNameW(str);
   free(str);
   return id;
   #pragma EXPANDER_EXPORT
}


/**
 * Get a timezone id for an IANA timezone name.
 *
 * @param  wchar* name - case-insensitive IANA timezone name
 *
 * @return int - timezone id or NULL if no timezone with that name was found
 */
int WINAPI GetTimeZoneIdByIanaNameW(const wchar* name) {
   if ((uint)name < MIN_VALID_POINTER) return !error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name);
   if (!*name)                         return !error(ERR_INVALID_PARAMETER, "invalid parameter name: \"\" (empty)");

   const TimezoneMapping* mappings = GetTimeZoneMappings();
   wchar* lName = strToLower(wstrim(wsdupa(name)));

   for (uint i=0; i < TIMEZONE_MAPPING_COUNT; i++) {
      if (StrCompare(lName, mappings[i].ianaNameLower)) {
         return mappings[i].id;
      }
   }
   return NULL;
   #pragma EXPANDER_EXPORT
}


/**
 * Get timezone infos for a Windows timezone.
 *
 * @param  _Out_ TIME_ZONE_INFORMATION* tzi  - struct receiving the timezone infos
 * @param  _In_  char*                  name - timezone name
 *
 * @return BOOL - success status; FALSE if no timezone with the specified name was found
 */
BOOL WINAPI GetTimeZoneInfoByWindowsNameA(TIME_ZONE_INFORMATION* tzi, const char* name) {
   // open the registry key
   string key = string("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones\\").append(name);
   HKEY hKey;
   if (int error = RegOpenKey(HKEY_LOCAL_MACHINE, key.c_str(), &hKey)) {
      return(!error(ERR_WIN32_ERROR+error, "failed to open key: \"HKEY_LOCAL_MACHINE\\%s\"", key.c_str()));
   }

   // timezone info format in the registry
   struct REG_TIME_ZONE_INFORMATION {
      long       Bias;
      long       StandardBias;
      long       DaylightBias;
      SYSTEMTIME StandardDate;
      SYSTEMTIME DaylightDate;
   };

   // a local function as C++98 substitute for a lambda function
   struct local {
      static BOOL ReadRegistryValue(HKEY hKey, string &key, wchar* value, DWORD type, void* buffer, DWORD size) {
         int error = RegGetValueW(hKey, NULL, value, type, NULL, buffer, &size);
         if (error) error(ERR_WIN32_ERROR+error, "failed to read value: \"HKEY_LOCAL_MACHINE\\%s\\%S\"", key.c_str(), value);
         return(!error);
      }
   };
   REG_TIME_ZONE_INFORMATION regtzi = {};

   // read timezone settings from the registry
   BOOL success = 1;
   success = success && local::ReadRegistryValue(hKey, key, L"TZI", RRF_RT_REG_BINARY, &regtzi,            sizeof(regtzi));
   success = success && local::ReadRegistryValue(hKey, key, L"Std", RRF_RT_REG_SZ,     &tzi->StandardName, sizeof(tzi->StandardName));
   success = success && local::ReadRegistryValue(hKey, key, L"Dlt", RRF_RT_REG_SZ,     &tzi->DaylightName, sizeof(tzi->DaylightName));

   if (success) {
      tzi->Bias         = regtzi.Bias;
      tzi->DaylightBias = regtzi.DaylightBias;
      tzi->DaylightDate = regtzi.DaylightDate;
      tzi->StandardBias = regtzi.StandardBias;
      tzi->StandardDate = regtzi.StandardDate;
   }

   RegCloseKey(hKey);
   return(success);
}


/**
 * Format a Unix timestamp as a string representing GMT time.
 *
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string as supported by strftime()
 *
 * @return string - formatted string or an empty string in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI gmtTimeFormat(time32 time, const char* format) {
   TM tm = UnixTimeToTm(time);
   char* buffer = NULL;
   size_t bufSize = strlen(format) << 1;                    // times 2 (final initial size is 4 times strlen(format)

   for (;;) {
      bufSize <<= 1;                                        // times 2
      buffer = (char*) alloca(bufSize);                     // on the stack
      if (strftime(buffer, bufSize, format, &tm)) break;
   }
   return(string(buffer));
}


/**
 * Format a 32-bit Unix timestamp as a string representing GMT time.
 *
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string supported by strftime()
 *
 * @return char* - time string or NULL in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
char* WINAPI GmtTimeFormatA(time32 time, const char* format) {
   if (time == NaT)                      return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string s = gmtTimeFormat(time, format);
   return sdup(s.c_str());                                  // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a 64-bit Unix timestamp as a string representing GMT time.
 *
 * @param  time64 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  wchar* format - format control string supported by strftime()
 *
 * @return wchar* - time string or NULL in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI GmtTimeFormatW(time64 time, const wchar* format) {
   if (time == NaT)                      return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   TM tm = UnixTimeToTm(time);
   wchar* buffer = NULL;
   uint bufSize = 32;                                       // initial buffer size is 64 (32 << 1)

   for (;;) {
      bufSize <<= 1;
      buffer = (wchar*) alloca(bufSize * sizeof(wchar));
      if (wcsftime(buffer, bufSize, format, &tm)) break;
   }

   return wsdup(buffer);                                    // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a Unix timestamp as a string representing local time.
 *
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string as supported by strftime()
 *
 * @return string - formatted string or an empty string in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
string WINAPI localTimeFormat(time32 time, const char* format) {
   TM tm = UnixTimeToTm(time, TRUE);
   char* buffer = NULL;
   size_t bufSize = strlen(format) << 1;                    // times 2 (final initial size is 4 times strlen(format)

   for (;;) {
      bufSize <<= 1;                                        // times 2
      buffer = (char*) alloca(bufSize);                     // on the stack
      if (strftime(buffer, bufSize, format, &tm)) break;
   }
   return string(buffer);
}


/**
 * Format a 32-bit Unix timestamp as a string representing local time.
 *
 * @param  time32 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  char*  format - format control string supported by strftime()
 *
 * @return char* - time string or NULL in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
char* WINAPI LocalTimeFormatA(time32 time, const char* format) {
   if (time == NaT)                      return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((char*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   string s = localTimeFormat(time, format);
   return sdup(s.c_str());                                  // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Format a 64-bit Unix timestamp as a string representing local time.
 *
 * @param  time64 time   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  wchar* format - format control string supported by strftime()
 *
 * @return wchar* - time string or NULL in case of errors
 *
 * @see  http://www.cplusplus.com/reference/ctime/strftime/
 * @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/6330ff20-4729-4c4a-82af-932915d893ea.htm
 */
wchar* WINAPI LocalTimeFormatW(time64 time, const wchar* format) {
   if (time == NaT)                      return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time"));
   if (time < 0)                         return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time));
   if ((uint)format < MIN_VALID_POINTER) return((wchar*)!error(ERR_INVALID_PARAMETER, "invalid parameter format: 0x%p (not a valid pointer)", format));

   TM tm = UnixTimeToTm(time, TRUE);
   wchar* buffer = NULL;
   uint bufSize = 32;                                                // initial buffer size is 64 (32<<1)

   for (;;) {
      bufSize <<= 1;
      buffer = (wchar*) alloca(bufSize * sizeof(wchar));
      if (wcsftime(buffer, bufSize, format, &tm)) break;
   }

   return wsdup(buffer);                                             // TODO: add to GC (close memory leak)
   #pragma EXPANDER_EXPORT
}


/**
 * Convert a FILETIME to a SYSTEMTIME.
 *
 * @param  FILETIME &ft
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI FileTimeToSystemTime(const FILETIME &ft) {
   SYSTEMTIME st = {};
   FileTimeToSystemTime(&ft, &st);
   return(st);
}


/**
 * Convert a FILETIME to a 32-bit Unix timestamp.
 *
 * @param  FILETIME &ft
 *
 * @return time32
 */
time32 WINAPI FileTimeToUnixTime32(const FILETIME &ft) {
   // @see  https://stackoverflow.com/questions/20370920/convert-current-time-from-windows-to-unix-timestamp-in-c-or-c

   LARGE_INTEGER li = {};
   li.LowPart  = ft.dwLowDateTime;
   li.HighPart = ft.dwHighDateTime;

   int64 seconds = (li.QuadPart-UNIX_TIME_START) / TICKS_PER_SECOND;
   return((time32)seconds);
}


/**
 * Convert a FILETIME to a 64-bit Unix timestamp.
 *
 * @param  FILETIME &ft
 *
 * @return time64
 */
time64 WINAPI FileTimeToUnixTime64(const FILETIME &ft) {
   // @see  https://stackoverflow.com/questions/20370920/convert-current-time-from-windows-to-unix-timestamp-in-c-or-c

   LARGE_INTEGER li = {};
   li.LowPart  = ft.dwLowDateTime;
   li.HighPart = ft.dwHighDateTime;

   int64 seconds = (li.QuadPart-UNIX_TIME_START) / TICKS_PER_SECOND;
   return(seconds);
}


/**
 * Convert a SYSTEMTIME to a FILETIME.
 *
 * @param  SYSTEMTIME &st
 *
 * @return FILETIME
 */
FILETIME WINAPI SystemTimeToFileTime(const SYSTEMTIME &st) {
   FILETIME ft = {};
   SystemTimeToFileTime(&st, &ft);
   return(ft);
}


/**
 * Return a readable representation of a SYSTEMTIME.
 *
 * @param  SYSTEMTIME &st
 *
 * @return string
 */
string WINAPI SystemTimeToStr(const SYSTEMTIME &st) {
   std::ostringstream ss;
   SYSTEMTIME empty = {};

   if (MemCompare(&st, &empty, sizeof(SYSTEMTIME))) {
      ss << "{}";
   }
   else {
      ss <<  "{wYear="         << st.wYear            // year                          - [1601-30827]
         << ", wMonth="        << st.wMonth           // month of the year             - [1..12]
         << ", wDay="          << st.wDay             // day of the month              - [1..31]
         << ", wDayOfWeek="    << st.wDayOfWeek       // days since Sunday             - [0..6]
         << ", wHour="         << st.wHour            // hours since midnight          - [0..23]
         << ", wMinute="       << st.wMinute          // minutes after the hour        - [0..59]
         << ", wSecond="       << st.wSecond          // seconds after the minute      - [0..59]
         << ", wMilliseconds=" << st.wMilliseconds    // milliseconds after the second - [0..999]
         << "}";
   }
   return(ss.str());
}


/**
 * Convert a SYSTEMTIME to a C time.
 *
 * @param  SYSTEMTIME &st
 *
 * @return TM - C time (field TM.tm_yday will be incomplete)
 */
TM WINAPI SystemTimeToTm(const SYSTEMTIME &st) {
   TM time = {};
   time.tm_year  = st.wYear - 1900;
   time.tm_mon   = st.wMonth - 1;
   time.tm_mday  = st.wDay;
   time.tm_wday  = st.wDayOfWeek;
   time.tm_hour  = st.wHour;
   time.tm_min   = st.wMinute;
   time.tm_sec   = st.wSecond;
   time.tm_isdst = -1;                    // let the CRT compute whether DST is in effect
   return(time);
}


/**
 * Convert a SYSTEMTIME to a 32-bit Unix timestamp.
 *
 * @param  SYSTEMTIME &st
 *
 * @return time32
 */
time32 WINAPI SystemTimeToUnixTime32(const SYSTEMTIME &st) {
   return(FileTimeToUnixTime32(SystemTimeToFileTime(st)));
}


/**
 * Convert a SYSTEMTIME to a 64-bit Unix timestamp.
 *
 * @param  SYSTEMTIME &st
 *
 * @return time64
 */
time64 WINAPI SystemTimeToUnixTime64(const SYSTEMTIME &st) {
   return(FileTimeToUnixTime64(SystemTimeToFileTime(st)));
}


/**
 * Return a readable representation of a C time.
 *
 * @param  TM &time
 *
 * @return string
 */
string WINAPI TmToStr(const TM &time) {
   std::ostringstream ss;
   TM empty = {};

   if (MemCompare(&time, &empty, sizeof(TM))) {
      ss << "{}";
   }
   else {
      ss <<  "{year="  << time.tm_year                // years since 1900
         << ", mon="   << time.tm_mon                 // months since January       - [0..11]
         << ", mday="  << time.tm_mday                // day of the month           - [1..31]
         << ", yday="  << time.tm_yday                // days since January 1st     - [0..365]
         << ", wday="  << time.tm_wday                // days since Sunday          - [0..6]
         << ", hour="  << time.tm_hour                // hours since midnight       - [0..23]
         << ", min="   << time.tm_min                 // minutes after the hour     - [0..59]
         << ", sec="   << time.tm_sec                 // seconds after the minute   - [0..59]
         << ", isdst=" << time.tm_isdst               // daylight savings time flag - [1,0,-1] on/off/evaluate
         << "}";
   }
   return(ss.str());
}


/**
 * Convert a C time to a 32-bit Unix timestamp.
 *
 * @param  TM   &time
 * @param  BOOL isLocalTime [optional] - whether the C time holds local or GMT time (default: GMT)
 *
 * @return time32
 */
time32 WINAPI TmToUnixTime32(const TM &time, BOOL isLocalTime/*=FALSE*/) {
   TM tm = time;
   if (isLocalTime) return(_mktime32(&tm));           // convert a local time to a GMT Unix timestamp
   else             return(_mkgmtime32(&tm));         // convert a GMT time to a GMT Unix timestamp
}


/**
 * Convert a C time to a 64-bit Unix timestamp.
 *
 * @param  TM   &time
 * @param  BOOL isLocalTime [optional] - whether the C time holds local or GMT time (default: GMT)
 *
 * @return time64
 */
time64 WINAPI TmToUnixTime64(const TM &time, BOOL isLocalTime/*=FALSE*/) {
   TM tm = time;
   if (isLocalTime) return(_mktime64(&tm));           // convert a local time to a UTC timestamp
   else             return(_mkgmtime64(&tm));         // convert a UTC time to a UTC timestamp
}


/**
 * Convert a Unix timestamp to a FILETIME.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00)
 *
 * @return FILETIME
 */
FILETIME WINAPI UnixTimeToFileTime(time32 time) {
   // @see  https://docs.microsoft.com/en-us/windows/win32/sysinfo/converting-a-time-t-value-to-a-file-time

   ULARGE_INTEGER li = {};
   li.QuadPart = (time * TICKS_PER_SECOND) + UNIX_TIME_START;

   FILETIME ft = {};
   ft.dwLowDateTime  = li.LowPart;
   ft.dwHighDateTime = li.HighPart;
   return(ft);
}


/**
 * Convert a Unix timestamp to a SYSTEMTIME.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00)
 *
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI UnixTimeToSystemTime(time32 time) {
   return(FileTimeToSystemTime(UnixTimeToFileTime(time)));
}


/**
 * Convert a 32-bit Unix timestamp to a C time.
 *
 * @param  time32 time                   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  BOOL   toLocalTime [optional] - whether the resulting C time holds local or GMT time (default: GMT)
 *
 * @return TM
 */
TM WINAPI UnixTimeToTm(time32 time, BOOL toLocalTime/*=FALSE*/) {
   if (toLocalTime) return(*_localtime32(&time));
   else             return(*_gmtime32(&time));
}


/**
 * Convert a 64-bit Unix timestamp to a C time.
 *
 * @param  time64 time                   - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 * @param  BOOL   toLocalTime [optional] - whether the resulting C struct holds local or GMT time (default: GMT)
 *
 * @return TM
 */
TM WINAPI UnixTimeToTm(time64 time, BOOL toLocalTime/*=FALSE*/) {
   if (toLocalTime) return(*_localtime64(&time));
   else             return(*_gmtime64(&time));
}


// ---------------------------------------------------------------------------------------------------------------------------


/**
 * @todo: finish unit tests
 *
 * Convert a GMT time to local time.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00 GMT)
 *
 * @return time32 - Unix timestamp (seconds since 01.01.1970 00:00 local time) or NaT (Not-a-Time) in case of errors
 */
time32 WINAPI GmtToLocalTime(time32 time) {
   if (time == NaT) return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time")));
   if (time < 0)    return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time)));

   SYSTEMTIME st = UnixTimeToSystemTime(time), lt = {};

   if (!SystemTimeToTzSpecificLocalTime(NULL, &st, &lt)) {
      return(_NaT32(error(ERR_WIN32_ERROR+GetLastError(), "SystemTimeToTzSpecificLocalTime()")));
   }
   return(SystemTimeToUnixTime32(lt));
   #pragma EXPANDER_EXPORT
}


/**
 * @todo: finish unit tests
 *
 * Convert a local time to GMT.
 *
 * @param  time32 time - Unix timestamp (seconds since 01.01.1970 00:00 local time)
 *
 * @return time32 - Unix timestamp (seconds since 01.01.1970 00:00 GMT) or NaT (Not-a-Time) in case of errors
 */
time32 WINAPI LocalToGmtTime(time32 time) {
   if (time == NaT) return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: Not-a-Time")));
   if (time < 0)    return(_NaT32(error(ERR_INVALID_PARAMETER, "invalid parameter time: %d (must be non-negative)", time)));

   SYSTEMTIME lt = UnixTimeToSystemTime(time), st = {};

   if (!TzSpecificLocalTimeToSystemTime(NULL, &lt, &st)) {
      return(_NaT32(error(ERR_WIN32_ERROR+GetLastError(), "TzSpecificLocalTimeToSystemTime()")));
   }
   return(SystemTimeToUnixTime32(st));
   #pragma EXPANDER_EXPORT
}


/**
 * @return SYSTEMTIME
 */
SYSTEMTIME WINAPI D(const char* datetime) {
   SYSTEMTIME st = {};

   if (!datetime || strlen(datetime) < 19) {
      error(ERR_INVALID_PARAMETER, "invalid parameter datetime: %s", datetime);
      return st;
   }

   char* s = sdupa(datetime);
   s[4] = s[7] = s[10] = s[13] = s[16] = s[19] = '\0';   // format: 1981.09.30 20:00:00

   st.wYear   = (WORD) atoi(&s[ 0]);
   st.wMonth  = (WORD) atoi(&s[ 5]);
   st.wDay    = (WORD) atoi(&s[ 8]);
   st.wHour   = (WORD) atoi(&s[11]);
   st.wMinute = (WORD) atoi(&s[14]);
   st.wSecond = (WORD) atoi(&s[17]);

   return st;
}


/**
 * @return int
 */
int WINAPI test_Time(const char* name) {
   if ((uint)name < MIN_VALID_POINTER) return(!error(ERR_INVALID_PARAMETER, "invalid parameter name: 0x%p (not a valid pointer)", name));

   string s(name);
   debug("before trimming: %p = \"%s\"", s.c_str(), s.c_str());

   strim(s);
   debug("after trimming:  %p = \"%s\"", s.c_str(), s.c_str());
   return(NULL);

   TIME_ZONE_INFORMATION tzi = {};
   BOOL result = GetTimeZoneInfoByWindowsNameA(&tzi, name);
   debug("timezone \"%s\": %d", name, result);

   SYSTEMTIME st = D("1981.09.30 20:12:13");
   debug("SYSTEMTIME = %s", SystemTimeToStr(st).c_str());
   debug("time: %s", GmtTimeFormatA(SystemTimeToUnixTime32(st), "%a, %d.%m.%Y %H:%M:%S"));

   return(NULL);
   #pragma EXPANDER_EXPORT
}
