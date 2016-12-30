#pragma once


struct TRADE {
   uint ticket;
};

typedef std::vector<TRADE*> ptrade_vector;


/**
 * MyFX struct TEST
 *
 * Structure holding metadata of a test: Strategy Tester settings, strategy settings, trade history
 */
struct TEST {                                            // - offset --- size --- description ---------------------------------------
   uint           id;                                    // ok              4     unique test id (positive, primary key)
   datetime       time;                                  // ok              4     time of the test
   char           symbol[MAX_SYMBOL_LENGTH+1];           // ok             12     tested symbol
   uint           timeframe;                             // ok              4     tested timeframe
   datetime       from;                                  // ok              4     time of the first tick of testing
   datetime       to;                                    // ok              4     time of the last tick of testing
   uint           tickModel;                             // ?               4     used tick model: 0=EveryTick|1=ControlPoints|2=BarOpen
   uint           spread;                                // ok              4     spread
   uint           bars;                                  // ok              4     number of tested bars
   uint           ticks;                                 // ok              4     number of tested ticks
   char           accountCurrency[MAX_SYMBOL_LENGTH+1];  // ok             12     account currency
   double         startingBalance;                       // ok              8     account starting balance
   uint           tradeDirections;                       // ok              4     allowed trade directions: Long|Short|Both
   char           strategyName    [MAX_PATH];            // ok            260     strategy name
   char           strategySettings[MAX_PATH];            // ok            260     strategy settings file name
   char           reportSymbol[MAX_SYMBOL_LENGTH+1];     // ok             12     generated unique symbol for chart reports
   ptrade_vector* trades;                                // ok              4     list of trades
};


// Getters


// Setters
