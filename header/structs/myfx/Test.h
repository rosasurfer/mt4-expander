#pragma once


struct ORDER {
   uint ticket;
};


typedef std::vector<ORDER> OrderVector;
typedef OrderVector        OrderHistory;


/**
 * MyFX struct TEST
 *
 * Structure holding metadata of a test: Strategy Tester settings, strategy settings, trade history
 */
struct TEST {                                            // - size --- description --------------------------------------------
   uint          id;                                     //      4     unique test id (positive, primary key)
   datetime      time;                                   //      4     time of the test
   char          symbol[MAX_SYMBOL_LENGTH+1];            //     12     tested symbol
   uint          timeframe;                              //      4     tested timeframe
   datetime      from;                                   //      4     time of the first tick of testing
   datetime      to;                                     //      4     time of the last tick of testing
   uint          tickModel;                              // ???  4     used tick model: 0=EveryTick|1=ControlPoints|2=BarOpen
   uint          spread;                                 //      4     spread
   uint          bars;                                   //      4     number of tested bars
   uint          ticks;                                  //      4     number of tested ticks
   char          accountCurrency[MAX_SYMBOL_LENGTH+1];   //     12     account currency
   double        startingBalance;                        //      8     account starting balance
   uint          tradeDirections;                        //      4     allowed trade directions: Long|Short|Both
   char          strategy[MAX_PATH];                     //    260     strategy name
   char          reportSymbol[MAX_SYMBOL_LENGTH+1];      //     12     generated unique symbol for charted reports
   OrderHistory* orders;                                 //      4     list of orders
};                                                       // -------------------------------------------------------------------
                                                         //  = 348                                warum bin ich nicht auf Ibiza


// Getters


// Setters
