#pragma once
#include "expander.h"


/**
 * MT4 struct SYMBOL (file format of "symbols.raw")
 *
 * All symbols of a file are sorted alphabetically by "name".
 */
#pragma pack(push, 1)

struct SYMBOL {                                    // -- offset ---- size --- description -------------------------------------------------------------------
   char   name        [MAX_SYMBOL_LENGTH+1];       //         0        12     broker specific symbol (e.g. "AUDCAD.m")
   char   description [54];                        //        12        54     description, original size = 64
   char   origin      [10];                        //        66        10     custom: tracks the symbol a symbol template originated from
   char   altName     [MAX_SYMBOL_LENGTH+1];       //        76        12     standard symbol (e.g. "AUDCAD"), set only if different from "name"
   char   baseCurrency[MAX_SYMBOL_LENGTH+1];       //        88        12     base currency
   uint   group;                                   //       100         4     index of the symbol group in "symgroups.raw" (zero based)
   uint   digits;                                  //       104         4     digits

   uint   tradeMode;                               //       108         4     0=NoTrading | 1=CloseOnly | 2=FullTrading
   uint   backgroundColor;                         //       112         4     color in "MarketWatch" window
   uint   arrayKey;                                //       116         4     unique but varibale array/vector id >= 0 (not the array/vector index)
   uint   id;                                      //       120         4     unique and constant symbol id >= 0

   BYTE   unknown1[32];                            //       124        32

   BYTE   sunday   [208];                          //       156       208
   BYTE   monday   [208];                          //       364       208
   BYTE   tuesday  [208];                          //       572       208
   BYTE   wednesday[208];                          //       780       208
   BYTE   thursday [208];                          //       988       208
   BYTE   friday   [208];                          //      1196       208
   BYTE   saturday [208];                          //      1404       208

   BYTE   unknown2[16];                            //      1612        16
   DWORD  unknown3;                                //      1628         4     ?
   DWORD  unknown4;                                //      1632         4
   DWORD  _alignment1;                             //      1636         4     (alignment to the next double)
   double unknown5;                                //      1640         8     ?
   BYTE   unknown6[12];                            //      1648        12

   uint   spread;                                  //      1660         4     spread in points: 0=variable spread
   BYTE   unknown7[8];                             //      1664         8

   BOOL   swapEnabled;                             //      1672         4     whether or not swaps are calculated
   uint   swapType;                                //      1676         4     = MarketInfo(MODE_SWAPTYPE) => 0=Points|1=BaseCurrency|2=Interest|3=MarginCurrency
   double swapLongValue;                           //      1680         8     swap long
   double swapShortValue;                          //      1688         8     swap short
   uint   swapTripleRolloverDay;                   //      1696         4     weekday of triple swaps (Sun=0)

   DWORD  _alignment2;                             //      1700         4     (alignment to the next double)
   double contractSize;                            //      1704         8     lot unitsize
   BYTE   unknown8[16];                            //      1712        16

   uint   stopDistance;                            //      1728         4     stop level
   BYTE   unknown9[8];                             //      1732         8
   DWORD  _alignment3;                             //      1740         4     (alignment to the next double)

   double marginInit;                              //      1744         8     initial margin     (0=SYMBOL.contractSize)
   double marginMaintenance;                       //      1752         8     maintenance margin (0=SYMBOL.contractSize)
   double marginHedged;                            //      1760         8     hedged margin
   double marginDivider;                           //      1768         8     leverage: absolute (symbol specific) or relative to the account

   double pointSize;                               //      1776         8     point size
   double pointsPerUnit;                           //      1784         8     ooints per unit

   BYTE   unknown10[24];                           //      1792        24
   char   marginCurrency[MAX_SYMBOL_LENGTH+1];     //      1816        12     margin currency

   BYTE   unknown11[104];                          //      1828       104
   int    unknown12;                               //      1932         4     ?
};                                                 // -------------------------------------------------------------------------------------------------------
#pragma pack(pop)                                  //              = 1936


// getters (used by MQL4)
const char* WINAPI symbol_Name                 (const SYMBOL* symbol);
const char* WINAPI symbol_Description          (const SYMBOL* symbol);
const char* WINAPI symbol_Origin               (const SYMBOL* symbol);
const char* WINAPI symbol_AltName              (const SYMBOL* symbol);
const char* WINAPI symbol_BaseCurrency         (const SYMBOL* symbol);
uint        WINAPI symbol_Group                (const SYMBOL* symbol);
uint        WINAPI symbol_Digits               (const SYMBOL* symbol);
uint        WINAPI symbol_TradeMode            (const SYMBOL* symbol);
uint        WINAPI symbol_BackgroundColor      (const SYMBOL* symbol);
uint        WINAPI symbol_ArrayKey             (const SYMBOL* symbol);
uint        WINAPI symbol_Id                   (const SYMBOL* symbol);
uint        WINAPI symbol_Spread               (const SYMBOL* symbol);
BOOL        WINAPI symbol_SwapEnabled          (const SYMBOL* symbol);
uint        WINAPI symbol_SwapType             (const SYMBOL* symbol);
double      WINAPI symbol_SwapLongValue        (const SYMBOL* symbol);
double      WINAPI symbol_SwapShortValue       (const SYMBOL* symbol);
uint        WINAPI symbol_SwapTripleRolloverDay(const SYMBOL* symbol);
double      WINAPI symbol_ContractSize         (const SYMBOL* symbol);
uint        WINAPI symbol_StopDistance         (const SYMBOL* symbol);
double      WINAPI symbol_MarginInit           (const SYMBOL* symbol);
double      WINAPI symbol_MarginMaintenance    (const SYMBOL* symbol);
double      WINAPI symbol_MarginHedged         (const SYMBOL* symbol);
double      WINAPI symbol_MarginDivider        (const SYMBOL* symbol);
double      WINAPI symbol_PointSize            (const SYMBOL* symbol);
double      WINAPI symbol_PointsPerUnit        (const SYMBOL* symbol);
const char* WINAPI symbol_MarginCurrency       (const SYMBOL* symbol);

const char* WINAPI symbols_Name                (const SYMBOL symbols[], int index);
uint        WINAPI symbols_ArrayKey            (const SYMBOL symbols[], int index);
uint        WINAPI symbols_Id                  (const SYMBOL symbols[], int index);

// Setter
const char* WINAPI symbol_SetName           (SYMBOL* symbol, const char* name       );
const char* WINAPI symbol_SetDescription    (SYMBOL* symbol, const char* description);
const char* WINAPI symbol_SetBaseCurrency   (SYMBOL* symbol, const char* currency   );
int         WINAPI symbol_SetGroup          (SYMBOL* symbol, int         index      );
int         WINAPI symbol_SetDigits         (SYMBOL* symbol, int         digits     );
int         WINAPI symbol_SetBackgroundColor(SYMBOL* symbol, int         color      );
int         WINAPI symbol_SetId             (SYMBOL* symbol, int         id         );
const char* WINAPI symbol_SetMarginCurrency (SYMBOL* symbol, const char* currency   );

int         WINAPI symbols_SetId            (SYMBOL symbols[], int index, int id);

// helpers
int        __cdecl CompareSymbols(const void* a, const void* b);
BOOL        WINAPI SortSymbols(SYMBOL symbols[], int size);
