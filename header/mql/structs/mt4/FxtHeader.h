#pragma once

/**
 * MT4 struct FXT_HEADER (Tickdatei-Header)
 *
 * Version 405
 *
 *  • Tickdateien ab Version 405 haben je nach MetaTrader-Version unterschiedliche Tickdatenformate.
 *  • Tickdateien enthalten keine Infos zu MODE_MARGINREQUIRED, Tests benötigen also existierende und gültige Serverinformationen.
 *  • Vor den modellierten Ticks einer Datei kann sich ein Prolog von History-Bars mit Pseudo-Ticks befinden, der dem Expert eine
 *    History vor dem Startzeitpunkt des Test zur Verfügung stellt. Der vom Terminal erzeugte Prolog ist 1000 Bars lang, der erste
 *    modellierte Tick befindet sich an der 1000-und-ersten Bar. Der Expert muß prüfen, ob diese History für seine Strategie
 *    ausreichend ist und ggf. auf das Eintreffen ausreichender Bars warten.
 */
struct FXT_HEADER {                                // -- offset ---- size --- description ----------------------------------------------------------------------------
   uint   version;                                 //         0         4     Header-Version                               405
   char   description[64];                         //         4        64     z.B. Copyright (szchar)
   char   serverName[128];                         //        68       128     Name des Accountservers (szchar)
   char   symbol[MAX_SYMBOL_LENGTH+1];             //       196        12     Symbol (szchar)
   uint   period;                                  //       208         4     Timeframe in Minuten
   uint   modelType;                               //       212         4     0=EveryTick|1=ControlPoints|2=BarOpen
   uint   modeledBars;                             //       216         4     Anzahl der modellierten Bars (ohne Prolog)
   uint   firstBarTime;                            //       220         4     OpenTime der Bar des ersten Ticks (ohne Prolog)
   uint   lastBarTime;                             //       224         4     OpenTime der Bar des letzten Ticks (ohne Prolog)
   BYTE   _alignment1[4];                          //       228         4     (alignment to the next double)
   double modelQuality;                            //       232         8     max. 99.9

   // common parameters                            // ----------------------------------------------------------------------------------------------------------------
   char   baseCurrency[MAX_SYMBOL_LENGTH+1];       //       240        12     Base currency (szchar)                     = StringLeft(symbol, 3)
   uint   spread;                                  //       252         4     Spread in Points: 0=current spread         = MarketInfo(MODE_SPREAD)
   uint   digits;                                  //       256         4     Digits                                     = MarketInfo(MODE_DIGITS)
   BYTE   _alignment2[4];                          //       260         4     (alignment to the next double)
   double pointSize;                               //       264         8     Auflösung, z.B. 0.0000'1                   = MarketInfo(MODE_POINT)
   uint   minLotsize;                              //       272         4     Mindest-Lotsize in Hundertsteln Lot        = MarketInfo(MODE_MINLOT)  * 100
   uint   maxLotsize;                              //       276         4     Höchst-Lotsize in Hundertsteln Lot         = MarketInfo(MODE_MAXLOT)  * 100
   uint   lotStepsize;                             //       280         4     Lot-Stepsize in Hundertsteln Lot           = MarketInfo(MODE_LOTSTEP) * 100
   uint   stopDistance;                            //       284         4     Stop level in Points                       = MarketInfo(MODE_STOPLEVEL)
   BOOL   pendingsGTC;                             //       288         4     close pending orders at end of day or GTC    @see struct SYMBOL
   BYTE   _alignment3[4];                          //       292         4     (alignment to the next double)

   // profit calculation parameters                // ----------------------------------------------------------------------------------------------------------------
   double contractSize;                            //       296         8     Größe eines Lots, z.B. 100000              = MarketInfo(MODE_LOTSIZE)
   double tickValue;                               //       304         8     Tick-Value in Quote-Currency ???           = MarketInfo(MODE_TICKVALUE)
   double tickSize;                                //       312         8     Tick-Größe, z.B. 0.0000'1                  = MarketInfo(MODE_TICKSIZE)
   uint   profitCalculationMode;                   //       320         4     0=Forex|1=CFD|2=Futures                    = MarketInfo(MODE_PROFITCALCMODE)

   // swap calculation parameters                  // ----------------------------------------------------------------------------------------------------------------
   BOOL   swapEnabled;                             //       324         4     ob Swaps berechnet werden
   uint   swapType;                                //       328         4     0=Points|1=BaseCurrency|2=Interest|3=MarginCurrency = MarketInfo(MODE_SWAPTYPE)
   BYTE   _alignment4[4];                          //       332         4     (alignment to the next double)
   double swapLongValue;                           //       336         8     long overnight swap value                  = MarketInfo(MODE_SWAPLONG)
   double swapShortValue;                          //       344         8     short overnight swap values                = MarketInfo(MODE_SWAPSHORT)
   uint   swapTripleRolloverDay;                   //       352         4     weekday of triple swaps                    = WEDNESDAY|FRIDAY

   // margin calculation parameters                // ----------------------------------------------------------------------------------------------------------------
   uint   accountLeverage;                         //       356         4     account leverage                           = AccountLeverage()
   uint   freeMarginCalculationType;               //       360         4     free margin calculation type               = AccountFreeMarginMode()
   uint   marginCalculationMode;                   //       364         4     margin calculation mode                    = MarketInfo(MODE_MARGINCALCMODE)
   uint   marginStopoutLevel;                      //       368         4     margin stopout level                       = AccountStopoutLevel()
   uint   marginStopoutType;                       //       372         4     margin stopout type                        = AccountStopoutMode()
   double marginInit;                              //       376         8     initial margin requirement (in units)      = MarketInfo(MODE_MARGININIT)
   double marginMaintenance;                       //       384         8     maintainance margin requirement (in units) = MarketInfo(MODE_MARGINMAINTENANCE)
   double marginHedged;                            //       392         8     hedged margin requirement (in units)       = MarketInfo(MODE_MARGINHEDGED)
   double marginDivider;                           //       400         8     Leverage: relativ=zum Account, absolut=CustomLeverage
   char   marginCurrency[MAX_SYMBOL_LENGTH+1];     //       408        12                                                = AccountCurrency()
   BYTE   _alignment5[4];                          //       420         4     (alignment to the next double)

   // commission calculation parameters            // ----------------------------------------------------------------------------------------------------------------
   double commissionValue;                         //       424         8     commission rate
   uint   commissionCalculationMode;               //       432         4     0=Money|1=Pips|2=Percent                     @see COMMISSION_MODE_*
   uint   commissionType;                          //       436         4     0=RoundTurn|1=PerDeal                        @see COMMISSION_TYPE_*

   // later additions                              // ----------------------------------------------------------------------------------------------------------------
   uint   firstBar;                                //       440         4     bar number/index??? of first bar (ohne Prolog) or 0 for first bar
   uint   lastBar;                                 //       444         4     bar number/index??? of last bar (ohne Prolog) or 0 for last bar
   uint   startPeriod[6];                          //       448        24     [0] = firstBar (bar where the smaller period modeling started)
   uint   testerSettingFrom;                       //       472         4     begin date from tester settings
   uint   testerSettingTo;                         //       476         4     end date from tester settings
   uint   freezeDistance;                          //       480         4     freeze level in points                     = MarketInfo(MODE_FREEZELEVEL)
   uint   modelErrors;                             //       484         4     number of errors during model generation
   BYTE   reserved[240];                           //       488       240     unused
};                                                 // ----------------------------------------------------------------------------------------------------------------
                                                   //               = 728     Warum bin ich nicht auf Ibiza?


// Tickdata, letztes Feld: // expert flag 0-bar is modified, but expert is not run
