#pragma once

/**
 * MT4 struct SYMBOL (Dateiformat "symbols.raw")
 *
 * Die Symbole einer Datei sind alphabetisch nach Namen sortiert.
 */
struct SYMBOL {                                    // -- offset ---- size --- description -------------------------------------------------------------------
   char   name        [MAX_SYMBOL_LENGTH+1];       //         0        12     Symbol         (szchar) z.B. "AUDCAD.mkt"
   char   description [54];                        //        12        54     Beschreibung   (szchar)
   char   origin      [10];                        //        66        10     pewa: ehemaliges Symbol, 10 Zeichen (szchar)
   char   altName     [MAX_SYMBOL_LENGTH+1];       //        76        12     Standardsymbol (szchar) z.B. "AUDCAD" (falls ungleich 'name')
   char   baseCurrency[MAX_SYMBOL_LENGTH+1];       //        88        12     Base Currency
   uint   group;                                   //       100         4     Index der Symbolgruppe in "symgroups.raw"
   uint   digits;                                  //       104         4     Digits

   uint   tradeMode;                               //       108         4     0=No|1=Close|2=Full
   uint   backgroundColor;                         //       112         4     Farbe im "MarketWatch" Window
   uint   arrayKey;                                //       116         4     eindeutige, aber variable Array-ID >= 0 (nicht der Array-Index)
   uint   id;                                      //       120         4     eindeutige und feste Symbol-ID >= 0

   BYTE   unknown1[32];                            //       124        32

   BYTE   monday   [208];                          //       156       208
   BYTE   tuesday  [208];                          //       364       208
   BYTE   wednesday[208];                          //       572       208
   BYTE   thursday [208];                          //       780       208
   BYTE   friday   [208];                          //       988       208
   BYTE   saturday [208];                          //      1196       208
   BYTE   sunday   [208];                          //      1404       208

   BYTE   unknown2[16];                            //      1612        16
   DWORD  unknown3;                                //      1628         4     ?
   DWORD  unknown4;                                //      1632         4
   DWORD  _alignment1;                             //      1636         4     (alignment to the next double)
   double unknown5;                                //      1640         8     ?
   BYTE   unknown6[12];                            //      1648        12

   uint   spread;                                  //      1660         4     Spread in Points: 0=current spread (variable)
   BYTE   unknown7[8];                             //      1664         8

   BOOL   swapEnabled;                             //      1672         4     ob Swaps berechnet werden
   uint   swapType;                                //      1676         4     0=Points|1=BaseCurrency|2=Interest|3=MarginCurrency = MarketInfo(MODE_SWAPTYPE)
   double swapLongValue;                           //      1680         8     Swap Long
   double swapShortValue;                          //      1688         8     Swap Short
   uint   swapTripleRolloverDay;                   //      1696         4     weekday of triple swaps = WEDNESDAY|FRIDAY

   DWORD  _alignment2;                             //      1700         4     (alignment to the next double)
   double contractSize;                            //      1704         8     Lot Size
   BYTE   unknown8[16];                            //      1712        16

   uint   stopDistance;                            //      1728         4     Stop Level
   BYTE   unknown9[8];                             //      1732         8
   DWORD  _alignment3;                             //      1740         4     (alignment to the next double)

   double marginInit;                              //      1744         8     Margin Init        (0=ContractSize)
   double marginMaintenance;                       //      1752         8     Margin Maintenance (0=ContractSize)
   double marginHedged;                            //      1760         8     Margin Hedged
   double marginDivider;                           //      1768         8     Leverage: relativ zum Account oder absolut (CustomLeverage)

   double pointSize;                               //      1776         8     Point Size
   double pointsPerUnit;                           //      1784         8     Points per Unit

   BYTE   unknown10[24];                           //      1792        24
   char   marginCurrency[MAX_SYMBOL_LENGTH+1];     //      1816        12     Margin Currency (szchar)

   BYTE   unknown11[104];                          //      1828       104
   int    unknown12;                               //      1932         4     ?
};                                                 // -------------------------------------------------------------------------------------------------------
                                                   //              = 1936

// Getter
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

// Helper
int                CompareSymbols(const void* a, const void* b);
BOOL        WINAPI SortSymbols(SYMBOL symbols[], int size);
