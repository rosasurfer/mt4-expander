#include "expander.h"
#include "lib/conversion.h"
#include "lib/datetime.h"
#include "lib/executioncontext.h"
#include "lib/helper.h"
#include "lib/math.h"
#include "lib/string.h"
#include "lib/terminal.h"
#include "lib/tester.h"
#include "lib/timeseries.h"
#include "struct/rsf/ExecutionContext.h"
#include "struct/rsf/Order.h"
#include "struct/rsf/Test.h"

#include <fstream>
#include <math.h>
#include <time.h>
#include <vector>


MqlInstanceList    g_mqlInstances(1);              // all MQL program instances: index 0 is not a valid pid and is always empty
std::vector<DWORD> g_threads;                      // all known threads executing MQL programs
std::vector<uint>  g_threadsPrograms;              // pid of the last MQL program executed by a thread
uint               g_lastUIThreadProgram;          // pid of the last MQL program executed by the UI thread
CRITICAL_SECTION   g_terminalMutex;                // mutex for application-wide locking


struct RECOMPILED_MODULE {                         // A struct holding the last MQL module with UninitReason UR_RECOMPILE.
   uint       pid;                                 // Only one module is tracked (the last one) and the variable is accessed
   ModuleType type;                                // from the UI thread only.
   char       name[MAX_FNAME];
} g_recompiledModule;


/**
 * Core function call order of multiple tests with VisualMode=on
 * =============================================================
 *
 * Indicators loaded by iCustom() are reloaded into the existing tester chart with IR_PROGRAM_AFTERTEST and unloaded when the tester chart closes.
 *
 * --- start of test (chart window opens) -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:07:44.038  SyncMainContext_init(152)    084F44E0  TestExpert  UR_UNDEFINED   ec={} (0x084F44E0)
 * 21:07:44.053  SyncMainContext_init(298)    084F44E0  TestExpert  UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_INIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F44E0)
 * 21:07:44.116  SyncLibContext_init(534)     084F48C8  rsfLib1     UR_UNDEFINED   ec={} (0x084F48C8)
 * 21:07:44.116  SyncLibContext_init(764)     084F48C8  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_INIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F48C8)
 * 21:07:44.131  SyncMainContext_start(368)   084F44E0  TestExpert                 ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=25766, changedBars=-1, validBars=-1, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35067, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F44E0)
 * --- expert loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:07:44.194  SyncMainContext_init(152)    084F4CB0  Trix        UR_UNDEFINED   ec={} (0x084F4CB0)
 * 21:07:44.194  SyncMainContext_init(298)    084F4CB0  Trix        UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * 21:07:44.241  SyncLibContext_init(534)     084F5098  rsfLib1     UR_UNDEFINED   ec={} (0x084F5098)
 * 21:07:44.241  SyncLibContext_init(764)     084F5098  rsfLib1     UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5098)
 * 21:07:44.241  SyncMainContext_start(368)   084F4CB0  Trix                       ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=25766, changedBars=25766, validBars=0, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35067, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * --- indicator loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- test finished --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:07:46.893  SyncMainContext_deinit(389)  084F4CB0  Trix        UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * 21:07:46.893  SyncMainContext_deinit(410)  084F4CB0  Trix        UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * 21:07:46.893  LeaveContext(836)            084F4CB0  Trix                       ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * 21:07:46.893  SyncLibContext_deinit(785)   084F5098  rsfLib1     UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5098)
 * 21:07:46.893  SyncLibContext_deinit(806)   084F5098  rsfLib1     UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5098)
 * 21:07:46.893  LeaveContext(836)            084F5098  rsfLib1                    ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5098)
 * --- indicator unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:07:46.893  SyncMainContext_deinit(389)  084F44E0  TestExpert  UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F44E0)
 * 21:07:46.893  SyncMainContext_deinit(410)  084F44E0  TestExpert  UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F44E0)
 * 21:07:46.893  LeaveContext(836)            084F44E0  TestExpert                 ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F44E0)
 * 21:07:46.893  SyncLibContext_deinit(785)   084F48C8  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * 21:07:46.893  SyncLibContext_deinit(806)   084F48C8  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * 21:07:46.893  LeaveContext(836)            084F48C8  rsfLib1                    ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * --- end of test: expert unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *
 * --- chart window becomes visible (if it was minimized) -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- indicator reloads with stateful libraries in UI thread ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:07:57.189  SyncLibContext_init(534)     084F5098  rsfLib1     UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=NULL, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F44E0, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5098)
 * 21:07:57.189  SyncLibContext_init(764)     084F5098  rsfLib1     UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5098)
 * 21:07:57.189  SyncMainContext_init(152)    084F44E0  Trix        UR_CHARTCLOSE  ec={} (0x084F44E0)
 * 21:07:57.189  SyncMainContext_init(298)    084F44E0  Trix        UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F44E0)
 *
 *
 * --- start of next test (new chart window opens) ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- expert reloads with stateful libraries ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:08:05.347  SyncLibContext_init(534)     084F48C8  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=NULL, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x08F10020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3880 (non-UI), hChart=0x00090DC2, hChartWindow=0x000B0DDE, test=0x099583C0, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * 21:08:05.347  SyncLibContext_init(764)     084F48C8  rsfLib1     UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=NULL, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=3184 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * 21:08:05.347  SyncMainContext_init(152)    084F4CB0  TestExpert  UR_UNDEFINED   ec={} (0x084F4CB0)
 * 21:08:05.347  SyncMainContext_init(298)    084F4CB0  TestExpert  UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_INIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F4CB0)
 * 21:08:05.347  SyncMainContext_start(368)   084F4CB0  TestExpert                 ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=25766, changedBars=-1, validBars=-1, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35067, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * --- expert loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:08:05.410  SyncMainContext_init(152)    084F5480  Trix        UR_UNDEFINED   ec={} (0x084F5480)
 * 21:08:05.410  SyncMainContext_init(298)    084F5480  Trix        UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5480)
 * 21:08:05.472  SyncLibContext_init(534)     084F5868  rsfLib1     UR_UNDEFINED   ec={} (0x084F5868)
 * 21:08:05.472  SyncLibContext_init(764)     084F5868  rsfLib1     UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5868)
 * 21:08:05.472  SyncMainContext_start(368)   084F5480  Trix                       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=25766, changedBars=25766, validBars=0, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35067, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5480)
 * --- indicator loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- test finished --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:08:08.545  SyncMainContext_deinit(389)  084F5480  Trix        UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5480)
 * 21:08:08.545  SyncMainContext_deinit(410)  084F5480  Trix        UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5480)
 * 21:08:08.545  LeaveContext(836)            084F5480  Trix                       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5480)
 * 21:08:08.545  SyncLibContext_deinit(785)   084F5868  rsfLib1     UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5868)
 * 21:08:08.545  SyncLibContext_deinit(806)   084F5868  rsfLib1     UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5868)
 * 21:08:08.545  LeaveContext(836)            084F5868  rsfLib1                    ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5868)
 * --- indicator unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:08:08.545  SyncMainContext_deinit(389)  084F4CB0  TestExpert  UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * 21:08:08.545  SyncMainContext_deinit(410)  084F4CB0  TestExpert  UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * 21:08:08.545  LeaveContext(836)            084F4CB0  TestExpert                 ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F4CB0)
 * 21:08:08.545  SyncLibContext_deinit(785)   084F48C8  rsfLib1     UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * 21:08:08.545  SyncLibContext_deinit(806)   084F48C8  rsfLib1     UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * 21:08:08.545  LeaveContext(836)            084F48C8  rsfLib1                    ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=0x0995F938, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F48C8)
 * --- end of test: expert unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *
 * --- chart window becomes visible (if it was minimized) -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- indicator reloads with stateful libraries in UI thread ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:08:08.561  SyncLibContext_init(534)     084F5868  rsfLib1     UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=NULL, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x09FE0020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=0x084F4CB0, threadId=3184 (non-UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5868)
 * 21:08:08.561  SyncLibContext_init(764)     084F5868  rsfLib1     UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x084F5868)
 * 21:08:08.561  SyncMainContext_init(152)    084F4CB0  Trix        UR_CHARTCLOSE  ec={} (0x084F4CB0)
 * 21:08:08.561  SyncMainContext_init(298)    084F4CB0  Trix        UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F4CB0)
 * --- chart window is closed -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 21:08:39.999  SyncMainContext_deinit(389)  084F4CB0  Trix        UR_CLOSE       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F4CB0)
 * 21:08:39.999  SyncMainContext_deinit(410)  084F4CB0  Trix        UR_CLOSE       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F4CB0)
 * 21:08:39.999  LeaveContext(836)            084F4CB0  Trix                       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F4CB0)
 * 21:08:39.999  SyncLibContext_deinit(785)   084F5868  rsfLib1     UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F5868)
 * 21:08:39.999  SyncLibContext_deinit(806)   084F5868  rsfLib1     UR_CLOSE       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F5868)
 * 21:08:39.999  LeaveContext(836)            084F5868  rsfLib1                    ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41532, superContext=NULL, threadId=3492 (UI), hChart=0x000D0DE2, hChartWindow=0x00080D8C, test=NULL, testing=TRUE, visualMode=TRUE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x084F5868)
 * --- indicator unloaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 */


/**
 * Core function call order of multiple tests with VisualMode=off
 * ==============================================================
 *
 * Between tests indicators loaded by iCustom() are reloaded with IR_PROGRAM_AFTERTEST and immediately unloaded.
 *
 * --- start of test -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:03.888  SyncMainContext_init(67)     0733A448  TestExpert  UR_UNDEFINED   ec={} (0x0733A448)
 * 07:30:03.888  SyncMainContext_init(213)    0733A448  TestExpert  UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_INIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0733A448)
 * 07:30:03.951  SyncLibContext_init(446)     07210248  rsfLib1     UR_UNDEFINED   ec={} (0x07210248)
 * 07:30:03.951  SyncLibContext_init(673)     07210248  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_INIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x07210248)
 * 07:30:03.951  SyncMainContext_start(283)   0733A448  TestExpert                 ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=25766, changedBars=-1, validBars=-1, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35076, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0733A448)
 * --- expert loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:04.013  SyncMainContext_init(67)     0747D9F0  Trix        UR_UNDEFINED   ec={} (0x0747D9F0)
 * 07:30:04.013  SyncMainContext_init(213)    0747D9F0  Trix        UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D9F0)
 * 07:30:04.091  SyncLibContext_init(446)     0742F0D8  rsfLib1     UR_UNDEFINED   ec={} (0x0742F0D8)
 * 07:30:04.091  SyncLibContext_init(673)     0742F0D8  rsfLib1     UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0742F0D8)
 * 07:30:04.091  SyncMainContext_start(283)   0747D9F0  Trix                       ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=25766, changedBars=25766, validBars=0, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35076, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D9F0)
 * --- indicator loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- test finished --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:04.528  SyncMainContext_deinit(304)  0747D9F0  Trix        UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D9F0)
 * 07:30:04.528  SyncMainContext_deinit(325)  0747D9F0  Trix        UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D9F0)
 * 07:30:04.528  LeaveContext(743)            0747D9F0  Trix                       ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D9F0)
 * 07:30:04.528  SyncLibContext_deinit(692)   0742F0D8  rsfLib1     UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0742F0D8)
 * 07:30:04.528  SyncLibContext_deinit(713)   0742F0D8  rsfLib1     UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0742F0D8)
 * 07:30:04.528  LeaveContext(743)            0742F0D8  rsfLib1                    ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0742F0D8)
 * --- indicator unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:04.528  SyncMainContext_deinit(304)  0733A448  TestExpert  UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0733A448)
 * 07:30:04.528  SyncMainContext_deinit(325)  0733A448  TestExpert  UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0733A448)
 * 07:30:04.528  LeaveContext(743)            0733A448  TestExpert                 ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0733A448)
 * 07:30:04.528  SyncLibContext_deinit(692)   07210248  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * 07:30:04.528  SyncLibContext_deinit(713)   07210248  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * 07:30:04.528  LeaveContext(743)            07210248  rsfLib1                    ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * --- end of test: expert unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *
 *
 * --- start of next test ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- indicator reloads with stateful libraries in UI thread ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:06.296  SyncLibContext_init(446)     0742F0D8  rsfLib1     UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=NULL, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x0733A448, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0742F0D8)
 * 07:30:06.296  SyncLibContext_init(673)     0742F0D8  rsfLib1     UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0742F0D8)
 * 07:30:06.296  SyncMainContext_init(67)     0D02D490  Trix        UR_CHARTCLOSE  ec={} (0x0D02D490)
 * 07:30:06.296  SyncMainContext_init(213)    0D02D490  Trix        UR_CHARTCLOSE  ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0D02D490)
 * --- indicator unloads in UI thread ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:06.296  SyncMainContext_deinit(304)  0D02D490  Trix        UR_REMOVE      ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0D02D490)
 * 07:30:06.296  SyncMainContext_deinit(325)  0D02D490  Trix        UR_REMOVE      ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_REMOVE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_REMOVE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0D02D490)
 * 07:30:06.296  LeaveContext(743)            0D02D490  Trix                       ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_REMOVE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_REMOVE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0D02D490)
 * 07:30:06.296  SyncLibContext_deinit(692)   0742F0D8  rsfLib1     UR_UNDEFINED   ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_REMOVE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0742F0D8)
 * 07:30:06.296  SyncLibContext_deinit(713)   0742F0D8  rsfLib1     UR_REMOVE      ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_REMOVE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_REMOVE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0742F0D8)
 * 07:30:06.296  LeaveContext(743)            0742F0D8  rsfLib1                    ec={pid=2, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM_AFTERTEST, programUninitReason=UR_REMOVE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_REMOVE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=0, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2896 (UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x0742F0D8)
 * --- indicator unloaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 *
 * --- expert reloads with stateful libraries ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:06.328  SyncLibContext_init(446)     07210248  rsfLib1     UR_UNDEFINED   ec={pid=1, previousPid=0, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=NULL, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=1564 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08E6E940, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * 07:30:06.328  SyncLibContext_init(673)     07210248  rsfLib1     UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=NULL, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * 07:30:06.328  SyncMainContext_init(67)     07210C88  TestExpert  UR_UNDEFINED   ec={} (0x07210C88)
 * 07:30:06.328  SyncMainContext_init(213)    07210C88  TestExpert  UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_INIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=TRUE, logFilename=""} (0x07210C88)
 * 07:30:06.328  SyncMainContext_start(283)   07210C88  TestExpert                 ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=25766, changedBars=-1, validBars=-1, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35076, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210C88)
 * --- expert loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:06.390  SyncMainContext_init(67)     0747D5C0  Trix        UR_UNDEFINED   ec={} (0x0747D5C0)
 * 07:30:06.390  SyncMainContext_init(213)    0747D5C0  Trix        UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D5C0)
 * 07:30:06.452  SyncLibContext_init(446)     07330998  rsfLib1     UR_UNDEFINED   ec={} (0x07330998)
 * 07:30:06.452  SyncLibContext_init(673)     07330998  rsfLib1     UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_INIT, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=NULL, bars=0, changedBars=-1, validBars=-1, ticks=0, cycleTicks=0, currTickTime=0, prevTickTime=0, bid=0, ask=0, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07330998)
 * 07:30:06.452  SyncMainContext_start(283)   0747D5C0  Trix                       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=25766, changedBars=25766, validBars=0, ticks=1, cycleTicks=1, currTickTime="2018.01.02 00:00:00", prevTickTime=0, bid=1.35066, ask=1.35076, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D5C0)
 * --- indicator loaded ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * --- test finished --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:06.905  SyncMainContext_deinit(304)  0747D5C0  Trix        UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_START, programInitReason=IR_PROGRAM, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D5C0)
 * 07:30:06.905  SyncMainContext_deinit(325)  0747D5C0  Trix        UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D5C0)
 * 07:30:06.905  LeaveContext(743)            0747D5C0  Trix                       ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=CF_DEINIT, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_INDICATOR, moduleName="Trix", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x0747D5C0)
 * 07:30:06.905  SyncLibContext_deinit(692)   07330998  rsfLib1     UR_UNDEFINED   ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07330998)
 * 07:30:06.905  SyncLibContext_deinit(713)   07330998  rsfLib1     UR_CHARTCLOSE  ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07330998)
 * 07:30:06.905  LeaveContext(743)            07330998  rsfLib1                    ec={pid=4, previousPid=0, programType=PT_INDICATOR, programName="Trix", programCoreFunction=NULL, programInitReason=IR_PROGRAM, programUninitReason=UR_CHARTCLOSE, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_CHARTCLOSE, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=2, validBars=27587, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=0x07210C88, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=NULL, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07330998)
 * --- indicator unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * 07:30:06.905  SyncMainContext_deinit(304)  07210C88  TestExpert  UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_START, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_START, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210C88)
 * 07:30:06.905  SyncMainContext_deinit(325)  07210C88  TestExpert  UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210C88)
 * 07:30:06.905  LeaveContext(743)            07210C88  TestExpert                 ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=CF_DEINIT, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_EXPERT, moduleName="TestExpert", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210C88)
 * 07:30:06.905  SyncLibContext_deinit(692)   07210248  rsfLib1     UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_INIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * 07:30:06.905  SyncLibContext_deinit(713)   07210248  rsfLib1     UR_UNDEFINED   ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * 07:30:06.905  LeaveContext(743)            07210248  rsfLib1                    ec={pid=3, previousPid=1, programType=PT_EXPERT, programName="TestExpert", programCoreFunction=NULL, programInitReason=IR_USER, programUninitReason=UR_UNDEFINED, programInitFlags=0, programDeinitFlags=0, moduleType=MT_LIBRARY, moduleName="rsfLib1", moduleCoreFunction=CF_DEINIT, moduleUninitReason=UR_UNDEFINED, moduleInitFlags=0, moduleDeinitFlags=0, symbol="GBPUSD", timeframe=PERIOD_M15, digits=5, point=1e-005, rates=0x0C530020, bars=27589, changedBars=-1, validBars=-1, ticks=1824, cycleTicks=1824, currTickTime="2018.01.26 23:45:00", prevTickTime="2018.01.26 23:30:00", bid=1.41531, ask=1.41541, superContext=NULL, threadId=2352 (non-UI), hChart=NULL, hChartWindow=NULL, test=0x08EDD9D8, testing=TRUE, visualMode=FALSE, optimization=FALSE, extReporting=FALSE, recorder=FALSE, mqlError=0, dllError=0, dllWarning=0, logEnabled=FALSE, logFilename=""} (0x07210248)
 * --- end of test: expert unloaded -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 */


/**
 * Core function call order on loading/unloading of MQL libraries
 * ==============================================================
 *
 * When already loaded libraries are reloaded they may or may not keep state depending on the reason for reloading. States
 * and core function call order during reloading are as follows:
 *
 * (1) Libraries loaded by indicators are reloaded during the indicator's regular init cycle (UR_CHARTCHANGE) and keep state.
 *
 *     Single indicator with nested library calls:
 *     --- first load -------------------------------------------------------------------------------------------------------
 *     Indicator::init()              UR_UNDEFINED    pid=0  create new context chain           set pid=1
 *     Indicator::LibraryA::init()    UR_UNDEFINED    pid=0  loaded by indicator                set pid=1
 *     Indicator::LibraryB::init()    UR_UNDEFINED    pid=0  loaded by indicator                set pid=1
 *     Indicator::LibraryC::init()    UR_UNDEFINED    pid=0  loaded by libraryA                 set pid=1
 *     --- deinit() ---------------------------------------------------------------------------------------------------------
 *     Indicator::deinit()            UR_CHARTCHANGE  pid=1  indicator first
 *     Indicator::LibraryA::deinit()  UR_UNDEFINED    pid=1  bug: global strings are already destroyed
 *     Indicator::LibraryC::deinit()  UR_UNDEFINED    pid=1  hierarchical (not in original loading order)
 *     Indicator::LibraryB::deinit()  UR_UNDEFINED    pid=1
 *     --- init() ----------------------------------- libraries keep state, indicators don't --------------------------------
 *     Indicator::LibraryA::init()    UR_UNDEFINED    pid=1
 *     Indicator::LibraryC::init()    UR_UNDEFINED    pid=1  hierarchical (not in original loading order)
 *     Indicator::LibraryB::init()    UR_UNDEFINED    pid=1
 *     Indicator::init()              UR_CHARTCHANGE  pid=0  indicator last (no state)          restore pid=1
 *     ----------------------------------------------------------------------------------------------------------------------
 *
 *     Multiple indicators with simple library calls:
 *     --- first load -------------------------------------------------------------------------------------------------------
 *     IndicatorA::init()             UR_UNDEFINED    pid=0  create new context chain           set pid=1
 *     IndicatorA::Library::init()    UR_UNDEFINED    pid=0                                     set pid=1
 *     IndicatorB::init()             UR_UNDEFINED    pid=0  create new context chain           set pid=2
 *     IndicatorB::Library::init()    UR_UNDEFINED    pid=0                                     set pid=2
 *     --- deinit() ---------------------------------------------------------------------------------------------------------
 *     IndicatorA::deinit()           UR_CHARTCHANGE  pid=1
 *     IndicatorA::Library::deinit()  UR_UNDEFINED    pid=1  bug: global strings are already destroyed
 *     IndicatorB::deinit()           UR_CHARTCHANGE  pid=2
 *     IndicatorB::Library::deinit()  UR_UNDEFINED    pid=2
 *     --- init() ----------------------------------- libraries keep state, indicators don't --------------------------------
 *     IndicatorA::Library::init()    UR_UNDEFINED    pid=1
 *     IndicatorA::init()             UR_CHARTCHANGE  pid=0  first indicator (no state)         restore pid=1
 *     IndicatorB::Library::init()    UR_UNDEFINED    pid=2
 *     IndicatorB::init()             UR_CHARTCHANGE  pid=0  second indicator (no state)        restore pid=2
 *     ----------------------------------------------------------------------------------------------------------------------
 *
 *
 * (2) Libraries loaded by experts are not reloaded during the expert's regular init cycle (UR_CHARTCHANGE).
 *
 *
 * (3) Libraries loaded by experts in tester are reloaded between multiple tests of the same strategy and keep state. In newer
 *     terminals (since when exactly?) this happens only if the test was not explicitly stopped by using the "Stop" button.
 *     In older terminals (e.g. build 500) this happens for all such tests.
 *
 *     Expert in tester with simple library calls:
 *     --- Tester Start -----------------------------------------------------------------------------------------------------
 *     Expert::init()                 UR_UNDEFINED    pid=0  create new context chain           set pid=1
 *     Expert::Library::init()        UR_UNDEFINED    pid=0                                     set pid=1
 *     --- Tester Stop ------------------------------------------------------------------------------------------------------
 *     Expert::deinit()               UR_UNDEFINED    pid=1
 *     Expert::Library::deinit()      UR_UNDEFINED    pid=1  bug: global strings are already destroyed
 *     --- Tester Start ----------------------------- libraries keep state --------------------------------------------------
 *     Expert::Library::init()        UR_UNDEFINED    pid=1  state of the finished test         set pid=2   set previousPid=1
 *     Expert::init()                 UR_UNDEFINED    pid=0                                     set pid=2   set previousPid=1
 *     ----------------------------------------------------------------------------------------------------------------------
 *
 *     The terminal implementation is considered broken by design. On start of a test libraries should always be in a clean
 *     state. Instead reloaded libraries keep state of the previously finished test, specifically:
 *      - Global variables are not reset and contain old values (except strings).
 *      - The last selected order context is not reset and order functions return wrong results.
 *      - The flag IsVisualMode() is not reset and may show wrong values, even if symbol or timeframe of the new test differ.
 *
 *     Workaround: On start of a test reused libraries need to be reset manually:
 *      - SyncLibContext_init() removes a library from the previously finished test's context chain and attaches it to the
 *        context chain of the new test.
 *      - MQL::core/library::init() resets a previously selected order context.
 *      - Global array variables must be reset by implementing Library.ResetGlobalVars().
 *      - The MQL function IsVisualMode() must not be used, instead use the corresponding flag in the execution context.
 *
 *
 * (4) After recompilation libraries are reloaded and don't keep state. In tester reloading can happen at the end of the
 *     current or on start of the next test. Reloading is always executed by the UI thread.
 *     Terminal bug: In online charts without a server connection reloading after unloading may not happen. An indicator or
 *                   expert will crash the next time it tries to call a function of a still unloaded library. In this case
 *                   the terminal log will show the message "Indicator/Expert stopped."
 *
 */// (prevent Visual Assist from merging above block in the hover tooltip of below function)


/**
 * Initialize and synchronize an MQL program's execution context with the master context stored in this DLL. Called by the
 * init() functions of the MQL main modules. For a general overview see type EXECUTION_CONTEXT.
 *
 * @param  EXECUTION_CONTEXT* ec                  - an MQL program's main module execution context
 * @param  ProgramType        programType         - program type
 * @param  char*              programName         - program name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason        - value of UninitializeReason() as returned by the terminal
 * @param  DWORD              initFlags           - program init configuration
 * @param  DWORD              deinitFlags         - program deinit configuration
 * @param  char*              symbol              - current chart symbol
 * @param  uint               timeframe           - current chart timeframe
 * @param  uint               digits              - the current symbol's "Digits" value (possibly incorrect)
 * @param  double             point               - the current symbol's "Point" value (possibly incorrect)
 * @param  int                recordMode          - an expert's "EA.Recorder" mode
 * @param  BOOL               isTesting           - value of IsTesting() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode        - value of IsVisualMode() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isOptimization      - value of IsOptimzation() as returned by the terminal
 * @param  BOOL               isExternalReporting - an expert's input parameter "Test.ExternalReporting"
 * @param  EXECUTION_CONTEXT* sec                 - super context as managed by the terminal (memory possibly already released)
 * @param  HWND               hChart              - value of WindowHandle() as returned by the terminal (possibly not yet set)
 * @param  int                droppedOnChart      - value of WindowOnDropped() as returned by the terminal (possibly incorrect)
 * @param  int                droppedOnPosX       - value of WindowXOnDropped() as returned by the terminal (possibly incorrect)
 * @param  int                droppedOnPosY       - value of WindowYOnDropped() as returned by the terminal (possibly incorrect)
 *
 * @return int - error status
 */
int WINAPI SyncMainContext_init(EXECUTION_CONTEXT* ec, ProgramType programType, const char* programName, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* symbol, uint timeframe, uint digits, double point, int recordMode, BOOL isTesting, BOOL isVisualMode, BOOL isOptimization, BOOL isExternalReporting, EXECUTION_CONTEXT* sec, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY) {
   if ((uint)ec          < MIN_VALID_POINTER)          return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if ((uint)programName < MIN_VALID_POINTER)          return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter programName: 0x%p (not a valid pointer)", programName)));
   if (strlen(programName) >= sizeof(ec->programName)) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "illegal length of parameter programName: \"%s\" (max %d characters)", programName, sizeof(ec->programName)-1)));
   if ((uint)symbol      < MIN_VALID_POINTER)          return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol)));
   if (strlen(symbol)    > MAX_SYMBOL_LENGTH)          return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (max %d characters)", symbol, MAX_SYMBOL_LENGTH)));
   if ((int)timeframe <= 0)                            return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", (int)timeframe)));
   if ((int)digits    <  0)                            return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d", (int)digits)));
   if (sec && (uint)sec  < MIN_VALID_POINTER)          return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter sec: 0x%p (not a valid pointer)", sec)));
   //debug("  %p  %-13s  %-14s  ec=%s", ec, programName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));
   if (ec->pid) SetLastThreadProgram(ec->pid);                             // set the thread's currently executed program asap (error handling)

   uint currentPid  = ec->pid;
   BOOL isPid       = (currentPid);
   uint previousPid = NULL;                                                // pid of a previous program instance (if any)
   EXECUTION_CONTEXT* master;

   // fix an unset chart handle (older terminals)
   if (!hChart) hChart = FindWindowHandle(hChart, sec, (ModuleType)programType, symbol, timeframe, isTesting, isVisualMode);
   if (hChart == INVALID_HWND) return(ERR_RUNTIME_ERROR);

   // resolve the real InitReason
   InitializeReason initReason = GetInitReason(ec, sec, programType, programName, uninitReason, symbol, isTesting, isVisualMode, hChart, droppedOnChart, droppedOnPosX, droppedOnPosY, previousPid);
   if (!initReason)                       return(ERR_RUNTIME_ERROR);
   if (initReason == IR_TERMINAL_FAILURE) return(_int(ERR_TERMINAL_INIT_FAILURE, debug("%s  ProgramInitReason=IR_TERMINAL_FAILURE", programName)));

   // (1) if ec.pid is not set: check if an indicator to be reused or something else
   //     - indicator in init cycle           (UI thread) => reuse the previous program and keep instance data
   //     - indicator in IR_PROGRAM_AFTERTEST (UI thread) => reuse the previous program and keep instance data
   //     - indicator after recompilation     (UI thread) => reuse the previous program and keep instance data
   //     - something else: new indicator|expert|script
   // (2) update main and master context
   // (3) synchronize already loaded libraries

   if (!isPid) {
      if (programType==PT_INDICATOR && previousPid) {                      // reuse the previous program chain and keep instance data
         currentPid = previousPid;
         SetLastThreadProgram(currentPid);                                 // set the thread's currently executed program asap (error handling)

         master = (*g_mqlInstances[currentPid])[0];
         if (initReason == IR_PROGRAM_AFTERTEST)
            master->superContext = sec = NULL;                             // reset the super context (the tested expert has already been released)
         *ec = *master;                                                    // restore main from master context (also restores the pid)
         (*g_mqlInstances[currentPid])[1] = ec;                             // store main context at original position (an empty slot)
      }
      else {
         // new indicator, new expert or new script
         uint lastPid = GetLastThreadProgram();                            // pid of the last program executed by the current thread

         // if an expert in tester check for a partially initialized context chain (master!=NULL, main=NULL, lib1!=NULL)
         if (programType==PT_EXPERT && isTesting && lastPid && Program_IsPartialTest(lastPid, programName)) {
            currentPid = lastPid;
            SetLastThreadProgram(currentPid);                              // set the thread's currently executed program asap (error handling)

            master = (*g_mqlInstances[currentPid])[0];
            *ec = *master;                                                 // copy master to main context
            (*g_mqlInstances[currentPid])[1] = ec;                          // store main context at old (empty) position
         }
         else {
            // create a new context chain                                  // TODO: on IR_PROGRAM_AFTERTEST somewhere exists a used context
            master  = new EXECUTION_CONTEXT();                             // create new master context
            *master = *ec;                                                 // copy main to master context
            ContextChain* chain = new ContextChain();
            chain->reserve(8);
            chain->push_back(master);                                      // store master and main context in a new context chain
            chain->push_back(ec);

            currentPid      = PushProgram(chain);                          // store the chain and update master and main context
            master->pid     = ec->pid     = currentPid;
            master->started = ec->started = time(NULL);
            SetLastThreadProgram(currentPid);
         }
      }
   }
   else {
      // ec.pid is set: an expert in init cycle or any other program after a repeated init() call
      master = (*g_mqlInstances[currentPid])[0];
      (*g_mqlInstances[currentPid])[1] = ec;                                // store main context at old (possibly empty) position
   }


   // (2) update main and master context
   ec_SetProgramType         (ec, programType );
   ec_SetProgramName         (ec, programName );
   ec_SetProgramCoreFunction (ec, CF_INIT     );                           // TODO: wrong for init() calls from start()
   ec_SetProgramInitReason   (ec, initReason  );
   ec_SetProgramUninitReason (ec, uninitReason);
   ec_SetProgramInitFlags    (ec, initFlags   );
   ec_SetProgramDeinitFlags  (ec, deinitFlags );

   ec_SetModuleType          (ec, (ModuleType)ec->programType);            // for main modules program and module values are the same
   ec_SetModuleName          (ec, ec->programName            );
   ec_SetModuleCoreFunction  (ec, ec->programCoreFunction    );
   ec_SetModuleUninitReason  (ec, ec->programUninitReason    );
   ec_SetModuleInitFlags     (ec, ec->programInitFlags       );
   ec_SetModuleDeinitFlags   (ec, ec->programDeinitFlags     );

   ec_SetSymbol              (ec, symbol   );
   ec_SetTimeframe           (ec, timeframe);
   master->rates = ec->rates = NULL;                                       // re-initialized on the next tick        // TODO: may be wrong for multiple
   ec_SetBars                (ec,  0);                                     // ...                                    //       init() calls from start()
   ec_SetValidBars           (ec, -1);                                     // ...                                    //       reset only on UR_CHARTCHANGE
   ec_SetChangedBars         (ec, -1);                                     // ...                                    //

   if (initReason == IR_SYMBOLCHANGE) {
      master->ticks        = ec->ticks        = 0;
      master->currTickTime = ec->currTickTime = 0;
      master->prevTickTime = ec->prevTickTime = 0;
      master->bid          = ec->bid          = 0;
      master->ask          = ec->ask          = 0;
   }
   else {}                                                                 // all values NULL or kept from the previous tick
   master->cycleTicks = ec->cycleTicks = 0;

   ec_SetDigits              (ec, digits);                                 // TODO: fix terminal bug
   ec_SetPipDigits           (ec, digits & (~1));
   ec_SetPip                 (ec, round(1./pow(10., (int)ec->pipDigits), ec->pipDigits));
   ec_SetPoint               (ec, point);
   ec_SetPipPoints           (ec, (uint)round(pow(10., (int)(digits & 1))));

   master->pipPriceFormat = ec->pipPriceFormat = strformat(".%d", ec->pipDigits);
   master->priceFormat    = ec->priceFormat    = (ec->digits==ec->pipDigits) ? ec->pipPriceFormat : strformat("%s'", ec->pipPriceFormat);

   ec_SetSuperContext        (ec, sec);
   ec_SetThreadId            (ec, GetCurrentThreadId());
   ec_SetHChart              (ec, hChart);                                 // chart handles must be set before test values
   ec_SetHChartWindow        (ec, hChart ? GetParent(hChart) : NULL);

   ec_SetRecordMode          (ec, recordMode);

   master->test = ec->test = Expert_InitTest(ec, isTesting);
   ec_SetTesting             (ec, isTesting     =Program_IsTesting     (ec, isTesting));
   ec_SetVisualMode          (ec, isVisualMode  =Program_IsVisualMode  (ec, isVisualMode));
   ec_SetOptimization        (ec, isOptimization=Program_IsOptimization(ec, isOptimization));
   ec_SetExternalReporting   (ec, isExternalReporting);

   EXECUTION_CONTEXT* ecRef = (master->superContext ? master->superContext : master);
   ec->logger =                   ecRef->logger;                           // logger instance first to catch further messages (TODO: move more up)
   ec_SetLoglevel            (ec, ecRef->loglevel        );
   ec_SetLoglevelTerminal    (ec, ecRef->loglevelTerminal);
   ec_SetLoglevelAlert       (ec, ecRef->loglevelAlert   );
   ec_SetLoglevelDebugger    (ec, ecRef->loglevelDebugger);
   ec_SetLoglevelFile        (ec, ecRef->loglevelFile    );
   ec_SetLoglevelMail        (ec, ecRef->loglevelMail    );
   ec_SetLoglevelSMS         (ec, ecRef->loglevelSMS     );
   ec_SetLogFilename         (ec, ecRef->logFilename     );

   // TODO: reset errors if not in an init() call from start()
   //ec->mqlError      = NULL;
   //ec->dllError      = NULL;
   //ec->dllWarning    = NULL;
   //ec->dllErrorMsg   = NULL;
   //ec->dllWarningMsg = NULL;

   // (3) synchronize already loaded libraries
   ContextChain &chain = *g_mqlInstances[currentPid];
   uint chainSize = chain.size();
   EXECUTION_CONTEXT bak, *lib;

   for (uint i=2; i < chainSize; ++i) {                                    // skip master and main context
      if (lib = chain[i]) {
          bak = *lib;                                                      // backup the library context
         *lib = *master;                                                   // overwrite library with master context
         lib->moduleType         = bak.moduleType;                         // restore library-specific values from the backup
         strcpy(lib->moduleName,   bak.moduleName);
         lib->moduleCoreFunction = bak.moduleCoreFunction;
         lib->moduleUninitReason = bak.moduleUninitReason;
         lib->moduleInitFlags    = bak.moduleInitFlags;
         lib->moduleDeinitFlags  = bak.moduleDeinitFlags;
      }
      else warn(ERR_ILLEGAL_STATE, "no module context found at chain[%d]: NULL  main=%s", i, EXECUTION_CONTEXT_toStr(ec));
   }

   //debug("  %p  %-13s  %-14s  ec=%s", ec, programName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * @param  EXECUTION_CONTEXT* ec          - main module context of a program
 * @param  void*              rates       - price history of the chart
 * @param  int                bars        - current amount of price bars (chart history)
 * @param  int                changedBars - current amount of changed indicator values
 * @param  uint               ticks       - number of calls of the MQL start() function
 * @param  datetime           time        - server time of the currently processed tick
 * @param  double             bid         - bid price of the currently processed tick
 * @param  double             ask         - ask price of the currently processed tick
 *
 * @return int - error status
 */
int WINAPI SyncMainContext_start(EXECUTION_CONTEXT* ec, const void* rates, int bars, int changedBars, uint ticks, datetime tickTime, double bid, double ask) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (!ec->pid)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  thread=%d  %s  ec=%s", GetCurrentThreadId(), (IsUIThread() ? "(UI)":"(non-UI)"), EXECUTION_CONTEXT_toStr(ec))));
   SetLastThreadProgram(ec->pid);                                    // set the thread's currently executed program asap (error handling)

   int      validBars    = (changedBars==-1) ? -1 : bars-changedBars;
   uint     cycleTicks   = ec->cycleTicks + 1;
   datetime prevTickTime = ec->currTickTime;
   DWORD    threadId     = GetCurrentThreadId();

   if (validBars && tickTime < prevTickTime) {                       // don't trigger a ticktime error if all bars have changed
      return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "ticktime is running backwards:  tick=%d  tickTime=%s  prevTickTime=%s  ec=%s", ticks, GmtTimeFormatA(tickTime, "%Y.%m.%d %H:%M:%S"), GmtTimeFormatA(prevTickTime, "%Y.%m.%d %H:%M:%S"), EXECUTION_CONTEXT_toStr(ec))));
   }

   ContextChain &chain = *g_mqlInstances[ec->pid];
   uint chainSize = chain.size();
   EXECUTION_CONTEXT* ctx;

   // update variable values in all loaded modules
   for (uint i=0; i < chainSize; ++i) {
      if (ctx = chain[i]) {
         ctx->programCoreFunction = CF_START;                        // in all contexts
         ctx->rates               = rates;
         ctx->bars                = bars;
         ctx->validBars           = validBars;
         ctx->changedBars         = changedBars;
         ctx->ticks               = ticks;
         ctx->cycleTicks          = cycleTicks;
         ctx->currTickTime        = tickTime;
         ctx->prevTickTime        = prevTickTime;
         ctx->bid                 = bid;
         ctx->ask                 = ask;
         ctx->threadId            = threadId;

         ctx->loglevel            = ec->loglevel;                    // configurable at runtime
         ctx->loglevelTerminal    = ec->loglevelTerminal;
         ctx->loglevelAlert       = ec->loglevelAlert;
         ctx->loglevelDebugger    = ec->loglevelDebugger;
         ctx->loglevelFile        = ec->loglevelFile;
         ctx->loglevelMail        = ec->loglevelMail;
         ctx->loglevelSMS         = ec->loglevelSMS;
         ctx->logger              = ec->logger;
         strcpy(ctx->logFilename, ec->logFilename);

         if (i < 2) {
            ctx->moduleCoreFunction = ctx->programCoreFunction;      // in master and main context only
         }
      }
      else warn(ERR_ILLEGAL_STATE, "no module context found at chain[%d]: NULL  main=%s", i, EXECUTION_CONTEXT_toStr(ec));
   }

   if (ec->test) {
      // update statistics for maxRunup/maxDrawdown calculations
      if ((uint)rates < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter rates: 0x%p (not a valid pointer)", rates)));
      if (bars <= 0)                       return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter bars: %d", bars)));

      TEST* test = ec->test;
      OrderList* positions = test->openPositions;
      double high=INT_MIN, low=INT_MAX;

      if (positions->size()) {
         switch (test->barModel) {
            case MODE_BAROPEN: {
               datetime barTime = iTime(rates, bars, 0);
               uint bar = (tickTime == barTime);                     // use the closed [1] or the current bar [0] for stats
               high = iHigh(rates, bars, bar);                       // (the last tick of a BarOpen test can be a BarClose tick)
               low  = iLow (rates, bars, bar);
               break;
            }
            case MODE_CONTROLPOINTS:
            case MODE_EVERYTICK:
               high = bid;                                           // inner prices to prevent stats distortion by spread widening
               low  = ask;
               break;
         }
         for (OrderList::iterator it=positions->begin(), end=positions->end(); it!=end; ++it) {
            ORDER* order = *it;
            if (high > order->highPrice) order->highPrice = high;    // explicite check for max. performance
            if (low  < order->lowPrice)  order->lowPrice  = low;
         }
      }
   }

   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Update a main module's execution context before the module is unloaded. Called only from the core function Module::deinit().
 * After deinit() was left the module is unloaded and it's memory must not be accessed until the module re-enters the core
 * function Module::init(). If the module is an expert and the expert is reloaded (UR_CHARTCHANGE in an online chart) the
 * module keeps state.
 *
 * @param  EXECUTION_CONTEXT* ec           - main module execution context
 * @param  UninitializeReason uninitReason - uninitialize reason as passed by the terminal
 *
 * @return int - error status
 */
int WINAPI SyncMainContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (!ec->pid)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  uninitReason=%s  thread=%d %s  ec=%s", UninitializeReasonToStr(uninitReason), GetCurrentThreadId(), (IsUIThread() ? "(UI)":"(non-UI)"), EXECUTION_CONTEXT_toStr(ec))));
   //debug("%p  %-13s  %-14s  ec=%s", ec, ec->programName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));
   SetLastThreadProgram(ec->pid);                                    // set the thread's currently executed program asap (error handling)

   ContextChain &chain = *g_mqlInstances[ec->pid];
   uint chainSize = chain.size();
   DWORD threadId = GetCurrentThreadId();
   EXECUTION_CONTEXT* ctx;

   // update variable values of all modules
   for (uint i=0; i < chainSize; ++i) {
      if (ctx = chain[i]) {
         ctx->programCoreFunction = CF_DEINIT;                       // in all contexts
         ctx->programUninitReason = uninitReason;
         ctx->threadId            = threadId;

         ctx->loglevel            = ec->loglevel;                    // configurable at runtime
         ctx->loglevelTerminal    = ec->loglevelTerminal;
         ctx->loglevelAlert       = ec->loglevelAlert;
         ctx->loglevelDebugger    = ec->loglevelDebugger;
         ctx->loglevelFile        = ec->loglevelFile;
         ctx->loglevelMail        = ec->loglevelMail;
         ctx->loglevelSMS         = ec->loglevelSMS;
         ctx->logger              = ec->logger;
         strcpy(ctx->logFilename, ec->logFilename);

         if (i < 2) {                                                // in master and main context only
            ctx->moduleCoreFunction = ctx->programCoreFunction;
            ctx->moduleUninitReason = ctx->programUninitReason;
         }
      }
      else warn(ERR_ILLEGAL_STATE, "no module context found at chain[%d]: %p  main=%s", i, chain[i], EXECUTION_CONTEXT_toStr(ec));
   }

   //debug("%p  %-13s  %-14s  ec=%s", ec, ec->programName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Synchronize a library's EXECUTION_CONTEXT with the context of the program's main module. Called only from the core
 * function Library::init(). Initializes the library context and adds it to the program's context chain.
 *
 * @param  EXECUTION_CONTEXT* ec             - the libray's execution context
 * @param  UninitializeReason uninitReason   - UninitializeReason as passed by the terminal (possibly incorrect)
 * @param  DWORD              initFlags      - init configuration
 * @param  DWORD              deinitFlags    - deinit configuration
 * @param  char*              moduleName     - the library's name (may contain a path depending on the terminal version)
 * @param  char*              symbol         - current chart symbol
 * @param  uint               timeframe      - current chart timeframe
 * @param  uint               digits         - the symbol's "Digits" value (possibly incorrect)
 * @param  double             point          - the symbol's "Point" value (possibly incorrect)
 * @param  BOOL               isTesting      - MQL::IsTesting()
 * @param  BOOL               isOptimization - MQL::IsOptimization()
 *
 * @return int - error status
 */
int WINAPI SyncLibContext_init(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason, DWORD initFlags, DWORD deinitFlags, const char* moduleName, const char* symbol, uint timeframe, uint digits, double point, BOOL isTesting, BOOL isOptimization) {
   if ((uint)ec         < MIN_VALID_POINTER)         return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if ((uint)moduleName < MIN_VALID_POINTER)         return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter moduleName: 0x%p (not a valid pointer)", moduleName)));
   if (strlen(moduleName) >= sizeof(ec->moduleName)) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "illegal length of parameter moduleName: \"%s\" (max %d characters)", moduleName, sizeof(ec->moduleName)-1)));
   if ((uint)symbol     < MIN_VALID_POINTER)         return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter symbol: 0x%p (not a valid pointer)", symbol)));
   if (strlen(symbol)   > MAX_SYMBOL_LENGTH)         return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "illegal length of parameter symbol: \"%s\" (max %d characters)", symbol, MAX_SYMBOL_LENGTH)));
   if ((int)timeframe <= 0)                          return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter timeframe: %d", (int)timeframe)));
   if ((int)digits < 0)                              return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter digits: %d", (int)digits)));
   if (point <= 0)                                   return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter point: %f", point)));
   //debug("   %p  %-13s  %-14s  ec=%s", ec, moduleName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));

   // fix the UninitializeReason
   uninitReason = FixUninitReason(ec, MT_LIBRARY, CF_INIT, uninitReason);
   if ((int)uninitReason < 0) return(ERR_RUNTIME_ERROR);


   // (1) if ec.pid is not set: the context is empty, check if recompilation or first-time load
   //     - UR_RECOMPILE: immediate reload in regular charts with connection        (UI thread)
   //                     reload on next usage in regular charts without connection (UI thread)
   //                     immediate reload or reload after a test finished in tester
   //
   //     - UR_UNDEFINED: first time load
   //
   // (2) if ec.pid is set: check if indicator in init cycle, indicator in IR_PROGRAM_AFTERTEST or reloaded expert between tests
   //     (2.1) indicator in init cycle or indicator in IR_PROGRAM_AFTERTEST (UI thread)
   //     (2.2) reloaded expert between tests                            (non-UI thread)

   if (!ec->pid) {
      // (1) recompilation or first-time load, the context is empty
      if (uninitReason == UR_RECOMPILE) {
         if (IsUIThread()) {
            // immediate reload in regular charts with connection or between tests, otherwise on next usage
            uint pid = FindModuleInLimbo(MT_LIBRARY, moduleName, UR_RECOMPILE, NULL, NULL);
            if (!pid) error(ERR_RUNTIME_ERROR, "UR_RECOMPILE - no %s library found in g_recompiledModule (pid=%d, type=%s, name=%s):  thread=%d %s  isTesting=%s", moduleName, g_recompiledModule.pid, ModuleTypeToStr(g_recompiledModule.type), g_recompiledModule.name, GetCurrentThreadId(), IsUIThread() ? "(UI)":"(non-UI)", BoolToStr(isTesting));
            else {
               SetLastThreadProgram(pid);
               g_recompiledModule = RECOMPILED_MODULE();             // reset recompilation tracker

               *ec = *(*g_mqlInstances[pid])[0];                      // initialize library context with master context
               ec->moduleType         = MT_LIBRARY;                  // update library specific values
               strcpy(ec->moduleName,   moduleName);
               ec->moduleCoreFunction = CF_INIT;
               ec->moduleUninitReason = uninitReason;
               ec->moduleInitFlags    = initFlags;
               ec->moduleDeinitFlags  = deinitFlags;
               ec->mqlError           = NULL;                        // reset errors
               ec->dllError           = NULL;
               ec->dllWarning         = NULL;
               ec->dllErrorMsg        = NULL;
               ec->dllWarningMsg      = NULL;

               g_mqlInstances[pid]->push_back(ec);                    // add context to the program's context chain
            }
         }
         else {
            // Library::init() of a formerly recompiled library at test start (non-UI thread), Expert::init() is called afterwards
            // check if a partially initialized context chain exists (master->coreFunction=CF_INIT, main=NULL)
            EXECUTION_CONTEXT* master;                               // master of current test
            uint currentPid  = GetLastThreadProgram();               // pid of the current test
            BOOL isPartialChain;

            if (!currentPid) {                                       // first library in init cyle: the thread never executed a program
               isPartialChain = FALSE;
            }
            else {                                                   // get the last executed program: it's myself or something else
               ContextChain &chain = *g_mqlInstances[currentPid];     // if partial chain found it's myself and another library with UR_RECOMPILE (which never gets reset)
               isPartialChain = (chain.size()>2 && (master=chain[0]) && chain[0]->programCoreFunction==CF_INIT && !chain[1]);
               if (!isPartialChain) warn(ERR_UNDEFINED_STATE, "unexpected library with UR_RECOMPILE in tester: a former library (pid=%d) seems to not have created a partial context chain");
            }

            if (!isPartialChain) {
               // create a new partially initialized chain
               master = new EXECUTION_CONTEXT();                     // create new master context
               ContextChain* chain = new ContextChain();
               chain->reserve(8);
               chain->push_back(master);                             // add master to a new chain
               chain->push_back(NULL);                               // add empty entry for the yet to come main context
               currentPid = PushProgram(chain);                      // store the chain
               uint threadIndex = SetLastThreadProgram(currentPid);

               master->pid            = currentPid;                  // update master context with the known values
               master->programType    = PT_EXPERT;
               master->moduleType     = MT_EXPERT;

               strcpy(master->newSymbol, symbol);                    // first moment a new symbol/timeframe show up
               master->newTimeframe   = timeframe;

               master->digits         = digits;                      // TODO: fix terminal bug
               master->pipDigits      = digits & (~1);
               master->pip            = round(1./pow((double)10., (int)master->pipDigits), master->pipDigits);
               master->point          = point;
               master->pipPoints      = (uint)round(pow((double)10., (int)(digits & 1)));

               master->pipPriceFormat = strformat(".%d", master->pipDigits);
               master->priceFormat    = (master->digits==master->pipDigits) ? master->pipPriceFormat : strformat("%s'", master->pipPriceFormat);

               master->superContext   = FALSE;
               master->threadId       = g_threads[threadIndex];

               master->testing        = TRUE;                        // TODO: so wrong, we can be online and not in tester
               master->optimization   = isOptimization;
            }

            // re-initialize the empty library context with the partial master context
            *ec = *master;
            ec->moduleType         = MT_LIBRARY;                     // update library specific values
            strcpy(ec->moduleName,   moduleName);
            ec->moduleCoreFunction = CF_INIT;
            ec->moduleUninitReason = uninitReason;
            ec->moduleInitFlags    = initFlags;
            ec->moduleDeinitFlags  = deinitFlags;
            strcpy(ec->symbol,       symbol);
            ec->timeframe          = timeframe;

            g_mqlInstances[currentPid]->push_back(ec);                // add library to the expert's context chain
         }
      }
      else {
         // (1.2) first time load of library, Library::init() is called after MainModule::init() in the current thread
         // Initialize the library with the current program's master context.
         uint pid = GetLastThreadProgram();                          // the program is currently executed
         if (!pid) return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "unknown program loading library \"%s\":  pid=0  UninitializeReason=%s  threadId=%d (%s)  ec=%s", moduleName, UninitializeReasonToStr(uninitReason), GetCurrentThreadId(), IsUIThread() ? "UI":"non-UI", EXECUTION_CONTEXT_toStr(ec))));

         *ec = *(*g_mqlInstances[pid])[0];                            // initialize library context with master context
         ec->moduleType         = MT_LIBRARY;                        // update library specific values
         strcpy(ec->moduleName,   moduleName);
         ec->moduleCoreFunction = CF_INIT;
         ec->moduleUninitReason = uninitReason;
         ec->moduleInitFlags    = initFlags;
         ec->moduleDeinitFlags  = deinitFlags;
         ec->mqlError           = NULL;                              // reset errors
         ec->dllError           = NULL;
         ec->dllWarning         = NULL;
         ec->dllErrorMsg        = NULL;
         ec->dllWarningMsg      = NULL;

         g_mqlInstances[pid]->push_back(ec);                          // add context to the program's context chain
      }
   }

   else if (IsUIThread()) {
      // (2.1) ec.pid is set: indicator in init cycle or in IR_PROGRAM_AFTERTEST (both UI thread)
      //       ec.pid points to the original indicator (still in limbo), Library::init() is called before Indicator::init()
      SetLastThreadProgram(ec->pid);                                 // set the thread's currently executed program asap (error handling)

      EXECUTION_CONTEXT* master = (*g_mqlInstances[ec->pid])[0];
      if (isTesting)                                                 // indicator in IR_PROGRAM_AFTERTEST
         master->programInitReason = IR_PROGRAM_AFTERTEST;
      else {}                                                        // indicator in init cycle

      // update known master values
      strcpy(master->newSymbol, symbol);                             // first moment a new symbol/timeframe show up
      master->newTimeframe   = timeframe;
      master->rates          = NULL;
      master->bars           =  0;
      master->validBars      = -1;
      master->changedBars    = -1;

      master->digits         = digits;                               // TODO: fix terminal bug
      master->pipDigits      = digits & (~1);
      master->pip            = round(1./pow((double)10., (int)master->pipDigits), master->pipDigits);
      master->point          = point;
      master->pipPoints      = (uint)round(pow((double)10., (int)(digits & 1)));

      master->pipPriceFormat = strformat(".%d", master->pipDigits);
      master->priceFormat    = (master->digits==master->pipDigits) ? master->pipPriceFormat : strformat("%s'", master->pipPriceFormat);

      master->superContext   = NULL;                                 // no super context at all or already released
      master->threadId       = GetCurrentThreadId();

      master->mqlError       = NO_ERROR;
      master->dllError       = NO_ERROR;
      master->dllWarning     = NO_ERROR;
      master->dllErrorMsg    = NULL;                                 // TODO: release memory of existing messages
      master->dllWarningMsg  = NULL;

      // re-initialize the library context with the updated master context
      EXECUTION_CONTEXT bak = *ec;                                   // create backup
      *ec = *master;                                                 // copy master over library context
      ec->moduleType         = bak.moduleType;                       // restore library specific values
      strcpy(ec->moduleName,   bak.moduleName);
      ec->moduleCoreFunction = CF_INIT;
      ec->moduleUninitReason = uninitReason;
      ec->moduleInitFlags    = initFlags;
      ec->moduleDeinitFlags  = deinitFlags;
      strcpy(ec->symbol,       symbol);
      ec->timeframe          = timeframe;

      g_mqlInstances[ec->pid]->push_back(ec);                         // add library context to the previous indicator's chain
   }

   else {
      // (2.2) ec.pid is set: reloaded expert between tests (non-UI thread), Library::init() is called before Expert::init()
      // ec.pid points to the previously finished test
      if (ec->programType!=PT_EXPERT || !ec->testing) return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "unexpected library init cycle:  thread=%d (%s)  ec=%s", GetCurrentThreadId(), IsUIThread()?"UI":"non-UI", EXECUTION_CONTEXT_toStr(ec))));

      EXECUTION_CONTEXT* master=NULL, *oldMaster=NULL;               // master of current and old test
      uint currentPid = GetLastThreadProgram();                      // pid of the new test
      BOOL isPartialChain;

      // check if a partially initialized context chain exists (master->programCoreFunction=NULL, main=NULL, lib1!=NULL)
      if (!currentPid || currentPid==ec->pid) {                      // first library in init cyle: the thread never executed a program
         isPartialChain = FALSE;                                     // or the program is the finished test (probably in an optimization)
      }
      else {                                                         // get the last executed program: it's myself or something else
         ContextChain &chain = *g_mqlInstances[currentPid];           // if partial chain found, it's myself with one more re-used library
         isPartialChain = (chain.size()>2 && (master=chain[0]) && !master->programCoreFunction && !chain[1]);
         if (!isPartialChain) debug("unseen library init cycle in tester (the former program seems not to be the former test):  ec=%s", EXECUTION_CONTEXT_toStr(ec));
      }

      if (!isPartialChain) {
         // create a new partially initialized chain
         master = new EXECUTION_CONTEXT();                           // create new master context

         ContextChain* chain = new ContextChain();
         chain->reserve(8);
         chain->push_back(master);                                   // add master to a new chain
         chain->push_back(NULL);                                     // add empty entry for the yet to come main context
         currentPid = PushProgram(chain);                            // store the chain
         uint threadIndex = SetLastThreadProgram(currentPid);

         master->pid               = currentPid;                     // update master context with the known values
         master->previousPid       = ec->pid;
         master->programType       = ec->programType;
         strcpy(master->programName, ec->programName);
         master->moduleType        = (ModuleType)master->programType;
         strcpy(master->moduleName,  master->programName);

         strcpy(master->newSymbol, symbol);                          // first moment symbol/timeframe show up
         master->newTimeframe   = timeframe;
         master->bars           =  0;
         master->validBars      = -1;
         master->changedBars    = -1;

         master->digits         = digits;                            // TODO: fix terminal bug
         master->pipDigits      = digits & (~1);
         master->pip            = round(1./pow((double)10., (int)master->pipDigits), master->pipDigits);
         master->point          = point;
         master->pipPoints      = (uint)round(pow((double)10., (int)(digits & 1)));

         master->pipPriceFormat = strformat(".%d", master->pipDigits);
         master->priceFormat    = (master->digits==master->pipDigits) ? master->pipPriceFormat : strformat("%s'", master->pipPriceFormat);

         master->threadId       = g_threads[threadIndex];
         master->testing        = TRUE;
         master->optimization   = isOptimization;
      }

      // re-initialize the library context with the master context
      EXECUTION_CONTEXT bak = *ec;                                   // create backup
      *ec = *master;                                                 // overwrite library with new master context
      ec->moduleType         = bak.moduleType;                       // restore library specific values from backup
      strcpy(ec->moduleName,   bak.moduleName);
      ec->moduleCoreFunction = CF_INIT;
      ec->moduleUninitReason = uninitReason;
      ec->moduleInitFlags    = initFlags;
      ec->moduleDeinitFlags  = deinitFlags;
      strcpy(ec->symbol,       symbol);
      ec->timeframe          = timeframe;

      g_mqlInstances[currentPid]->push_back(ec);                      // add library to the new test's context chain
   }

   //debug("   %p  %-13s  %-14s  ec=%s", ec, moduleName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Update a library's execution context before the library gets unloaded. Called only from the core function Library::deinit().
 * Depending on the current runtime situation the library may or may not keep state.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  UninitializeReason uninitReason - UninitializeReason as passed by the terminal (possibly incorrect)
 *
 * @return int - error status
 */
int WINAPI SyncLibContext_deinit(EXECUTION_CONTEXT* ec, UninitializeReason uninitReason) {
   if ((uint)ec < MIN_VALID_POINTER) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (!ec->pid)                     return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  uninitReason=%s  thread=%d (%s)  ec=%s", UninitializeReasonToStr(uninitReason), GetCurrentThreadId(), IsUIThread() ? "UI":"non-UI", EXECUTION_CONTEXT_toStr(ec))));
   //debug(" %p  %-13s  %-14s  ec=%s", ec, ec->moduleName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));
   SetLastThreadProgram(ec->pid);                        // set the thread's currently executed program asap (error handling)

   // try to fix the UninitializeReason
   uninitReason = FixUninitReason(ec, MT_LIBRARY, CF_DEINIT, uninitReason);
   if ((int)uninitReason < 0) return(ERR_RUNTIME_ERROR);

   ec->moduleCoreFunction = CF_DEINIT;                   // update library specific values
   ec->moduleUninitReason = uninitReason;

   ContextChain &chain = *g_mqlInstances[ec->pid];
   uint chainSize = chain.size();
   DWORD threadId = GetCurrentThreadId();

   for (uint i=0; i < chainSize; ++i) {                  // update variable values of all modules
      if (EXECUTION_CONTEXT* ctx = chain[i]) {
         ctx->threadId = threadId;
      }
   }

   //debug(" %p  %-13s  %-14s  ec=%s", ec, ec->moduleName, UninitializeReasonToStr(uninitReason), EXECUTION_CONTEXT_toStr(ec));
   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Process leaving of an MQL module's core function deinit(). Called only from MQL::deinit() and must be the very last
 * statement. After MQL::deinit() was left the module is unloaded and it's memory must not be accessed anymore.
 *
 * @param  EXECUTION_CONTEXT* ec - execution context of the module
 *
 * @return int - error status
 *
 *
 * Notes: (1) If the module is an MQL main module (an indicator, an expert or a script) the index of the main context in the
 *            program's context chain is set to NULL (the chain size doesn't change).
 *
 *        (2) If the module is an MQL library module the context is removed from the context chain and the chain size decreased.
 *
 *        (3) If an expert leaves MQL::deinit() and goes through an init cycle (UR_CHARTCHANGE or UR_PARAMETERS) its main module
 *            keeps state. If an indicator leaves MQL::deinit() and goes through an init cycle its main module doesn't keep
 *            state. Modules of any type going through an init cycle due to UR_RECOMPILE don't keep state.
 *
 *        (4) An unloaded module's memory must not be accessed (read/write) until the module re-enters the function MQL::init().
 *            Use the master context at chain index 0 to access data stored in the execution context of an unloaded module.
 */
int WINAPI LeaveContext(EXECUTION_CONTEXT* ec) {
   if ((uint)ec < MIN_VALID_POINTER)        return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid parameter ec: 0x%p (not a valid pointer)", ec)));
   if (!ec->pid)                            return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context (ec.pid=0):  thread=%d (%s)  ec=%s", GetCurrentThreadId(), IsUIThread() ? "UI":"non-UI", EXECUTION_CONTEXT_toStr(ec))));
   if (ec->moduleCoreFunction != CF_DEINIT) return(_int(ERR_INVALID_PARAMETER, error(ERR_INVALID_PARAMETER, "invalid execution context (ec.moduleCoreFunction not CF_DEINIT):  thread=%d (%s)  ec=%s", GetCurrentThreadId(), IsUIThread() ? "UI":"non-UI", EXECUTION_CONTEXT_toStr(ec))));
   if (g_mqlInstances.size() <= ec->pid)     return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "illegal list of ContextChains (size=%d) for pid=%d:  ec=%s", g_mqlInstances.size(), ec->pid, EXECUTION_CONTEXT_toStr(ec))));

   ContextChain &chain = *g_mqlInstances[ec->pid];
   uint chainSize = chain.size();
   if (chainSize < 2) return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "illegal context chain (size=%d):  ec=%s", chainSize, EXECUTION_CONTEXT_toStr(ec))));

   switch (ec->moduleType) {
      // --- main module -----------------------------------------------------------------------------------------------------
      case MT_INDICATOR:
      case MT_SCRIPT:
      case MT_EXPERT:
         EXECUTION_CONTEXT* ctx;
         for (uint i=0; i < chainSize; ++i) {
            if (ctx = chain[i]) {
               ctx->programCoreFunction   = (CoreFunction)NULL;            // mark MainModule::deinit() as left
               if (i < 2)
                  ctx->moduleCoreFunction = (CoreFunction)NULL;
               *ctx->newSymbol   = '\0';
               ctx->newTimeframe = NULL;
            }
            else warn(ERR_ILLEGAL_STATE, "no module context found at chain[%d]: %p  main=%s", i, chain[i], EXECUTION_CONTEXT_toStr(ec));
         }
         chain[1] = NULL;                                                  // unset the main execution context but keep the slot in the chain
         break;

      // --- library module --------------------------------------------------------------------------------------------------
      case MT_LIBRARY:
         ec->moduleCoreFunction = (CoreFunction)NULL;                      // mark Module::deinit() as left
         int i;
         for (i=chainSize-1; i >= 0; --i) {                                // iterate backwards (faster match)
            if (chain[i] == ec) {
               chain.erase(chain.begin() + i);                             // remove library context and slot from the chain
               break;
            }
         }
         if (i < 0) return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "library context not found in context chain (size=%d):  ec=%s", chainSize, EXECUTION_CONTEXT_toStr(ec))));

         // on recompilation store the library identifiers for look-up after recompilation
         if (ec->moduleUninitReason == UR_RECOMPILE) {
            if (!IsUIThread()) warn(ERR_UNDEFINED_STATE, "access to global var g_recompiledModule from non-UI thread: %d  ec=%s", GetCurrentThreadId(), EXECUTION_CONTEXT_toStr(ec));
            if (g_recompiledModule.pid != ec->pid) {                       // there can be only one recompilation at any time
               g_recompiledModule.pid        = ec->pid;
               g_recompiledModule.type       = ec->moduleType;
               strcpy(g_recompiledModule.name, ec->moduleName);
            }
         }
         break;

      default:
         return(_int(ERR_ILLEGAL_STATE, error(ERR_ILLEGAL_STATE, "illegal execution context (unknown ec.moduleType):  ec=%s", EXECUTION_CONTEXT_toStr(ec))));
   }

   // close an open logfile
   if (ec->logger && ec->logger->is_open())
      ec->logger->close();                                                 // re-opened automatically on next use

   return(NO_ERROR);
   #pragma EXPANDER_EXPORT
}


/**
 * Create and initialize a new TEST instance if the passed program is an expert in tester. If the context already holds
 * a pointer to a test, the existing test is returned.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isTesting - IsTesting() flag as passed by the terminal
 *
 * @return TEST* - test instance or NULL if the program is not an expert under test or in case of errors
 */
TEST* WINAPI Expert_InitTest(EXECUTION_CONTEXT* ec, BOOL isTesting) {
   if (ec->test)                                return(ec->test);
   if (ec->moduleType!=MT_EXPERT || !isTesting) return(NULL);

   int barModel = Tester_GetBarModel(); if (barModel == EMPTY) return(NULL);

   FXT_HEADER* fxtHeader = new FXT_HEADER();
   if (!Tester_ReadFxtHeader(ec->symbol, ec->timeframe, barModel, fxtHeader)) {
      delete fxtHeader;
      return(NULL);
   }

   TEST* test = new TEST();
   test->ec        = ec;
   test->created   = time(NULL);
   test->barModel  = barModel;
   test->fxtHeader = fxtHeader;

   test->openPositions        = new OrderList(); test->openPositions     ->reserve(32);
   test->openLongPositions    = new OrderList(); test->openLongPositions ->reserve(32);
   test->openShortPositions   = new OrderList(); test->openShortPositions->reserve(32);

   test->closedPositions      = new OrderList(); test->closedPositions     ->reserve(1024);
   test->closedLongPositions  = new OrderList(); test->closedLongPositions ->reserve(1024);
   test->closedShortPositions = new OrderList(); test->closedShortPositions->reserve(1024);

   return(test);
}


/**
 * Find the first unloaded module suitable for reloading matching the specified arguments.
 *
 * @param  ModuleType         type
 * @param  const char*        name
 * @param  UninitializeReason uninitReason
 * @param  BOOL               testing
 * @param  HWND               hChart
 *
 * @return uint - the found module's program id (pid) or NULL if no such module was found
 */
uint WINAPI FindModuleInLimbo(ModuleType type, const char* name, UninitializeReason uninitReason, BOOL testing, HWND hChart) {
   switch (type) {
      case MT_INDICATOR: {
         // If the indicator was not used in a test (testing=FALSE) master.threadId must be the UI thread.
         // If the indicator was used in a test (testing=TRUE) master.threadId depends on whether one of the indicator's
         // libraries has been reloaded before.
         uint chainsSize = g_mqlInstances.size();
         EXECUTION_CONTEXT* master;

         // TODO: In a test the hChart window is ignored - atm.
         if (testing) {
            for (uint i=1; i < chainsSize; ++i) {                                   // index[0] is always empty
               ContextChain &chain = *g_mqlInstances[i];
               uint size = chain.size();
               if (size) {
                  if (master = chain[0]) {
                     if (master->programType == MT_INDICATOR) {
                        if (!master->programCoreFunction) {                         // main module is unloaded
                           if (master->programUninitReason == uninitReason) {
                              if (StrCompare(master->programName, name)) {          // name check at the end
                                 if (size > 2) {                                    // with libraries master->threadId must be the UI thread
                                    if (IsUIThread(master->threadId)) {
                                       return(i);
                                    }
                                 }
                                 else if (!IsUIThread(master->threadId)) {          // without libraries master->threadId must not be the UI thread
                                    return(i);
                                 }
                              }
                           }
                        }
                     }
                  } else warn(ERR_ILLEGAL_STATE, "illegal master context found in ContextChain of program %d:  master=0x%p", i, master);
               } else warn(ERR_ILLEGAL_STATE, "illegal ContextChain found at g_mqlPrograms[%d]:  size=%d", size);
            }
         }

         // If not in a test a chart must exist. Possible use cases:
         // - a regular init cycle in the UI thread
         // - a recompilation (again in the UI thread)
         else {
            if (hChart) {
               for (uint i=1; i < chainsSize; ++i) {                                // index[0] is always empty
                  ContextChain &chain = *g_mqlInstances[i];
                  if (chain.size()) {
                     if (master = chain[0]) {
                        if (master->programType == MT_INDICATOR) {
                           if (!master->programCoreFunction) {                      // main module is unloaded
                              if (master->programUninitReason == uninitReason) {
                                 if (master->hChart == hChart) {
                                    if (IsUIThread(master->threadId)) {             // master->threadId must be the UI thread
                                       if (StrCompare(master->programName, name)) { // name check last
                                          return(i);
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     } else warn(ERR_ILLEGAL_STATE, "illegal master context found in ContextChain of program %d:  master=0x%p", i, master);
                  } else warn(ERR_ILLEGAL_STATE, "illegal ContextChain found at g_mqlPrograms[%d]:  size=%d", chain.size());
               }
            }
         }
         break;
      }

      case MT_LIBRARY:
         if (uninitReason == UR_RECOMPILE) {
            if (g_recompiledModule.type == MT_LIBRARY) {
               if (StrCompare(g_recompiledModule.name, name)) {
                  return(g_recompiledModule.pid);
               }
            }
         }
         break;

      default:
         return(error(ERR_INVALID_PARAMETER, "invalid parameter module type: %s (not supported)", ModuleTypeToStr(type)));
   }

   //debug("no matching %s indicator found:  %s  testing=%s  hChart=%d", name, UninitializeReasonToStr(reason), BoolToStr(testing), hChart);
   return(NULL);
}


/**
 * Find the chart of the current program and return its window handle. Replacement for the broken MQL function WindowHandle().
 * Also returns the correct window handle when the MQL function fails.
 *
 * Must be called only in SyncMainContext_init(), after that use the window handle stored in the program's EXECUTION_CONTEXT.
 *
 * @param  HWND               hChart       - value of WindowHandle() as returned by the terminal (possibly not yet set)
 * @param  EXECUTION_CONTEXT* sec          - super context as managed by the terminal (memory possibly already released)
 * @param  ModuleType         moduleType   - module type
 * @param  char*              symbol       - current symbol
 * @param  uint               timeframe    - current timeframe
 * @param  BOOL               isTesting    - value of IsTesting() as returned by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode - value of IsVisualMode() as returned by the terminal (possibly incorrect)
 *
 * @return HWND - Window handle or NULL if the program runs in the tester with VisualMode=Off;
 *                INVALID_HWND (-1) in case of errors.
 */
HWND WINAPI FindWindowHandle(HWND hChart, const EXECUTION_CONTEXT* sec, ModuleType moduleType, const char* symbol, uint timeframe, BOOL isTesting, BOOL isVisualMode) {
   if (hChart) return(hChart);                                       // if already defined return WindowHandle() as passed
   if (sec) return(sec->hChart);                                     // if a super context exists return the inherited chart handle
                                                                     // (if hChart is not set the super context is always valid)
   // Wir sind im Hauptmodul
   // - kein SuperContext
   // - WindowHandle() ist NULL

   if (isTesting && !isVisualMode)                                   // Im Tester bei VisualMode=Off gibt es keinen Chart: Rückgabewert NULL
      return(NULL);

   // Wir sind entweder: im Tester bei VisualMode=On              aber: kein Hauptmodul hat VisualMode=On und WindowHandle=NULL
   // oder               außerhalb des Testers

   HWND hWndMain = GetTerminalMainWindow();
   if (!hWndMain) return(INVALID_HWND);

   HWND hWndMdi  = GetDlgItem(hWndMain, IDC_MDI_CLIENT);
   if (!hWndMdi) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MDIClient window not found (hWndMain=%p)", hWndMain)));

   HWND hChartWindow = NULL;                                         // chart system window holding the chart AfxFrameOrView


   // (1) Indikator
   if (moduleType == MT_INDICATOR) {
      //
      // Wir sind entweder: normaler Template-Indikator bei Terminalstart oder LoadProfile und WindowHandle() ist noch NULL
      // oder:              Tester-Template-Indikator im Tester bei VisualMode=Off => es gibt keinen Chart
      // Wir sind immer:    im UIThread in init()
      //
      // Wir sind nicht:    in iCustom()
      // und auch nicht:    manuell geladener Indikator im Tester-Chart => WindowHandle() wäre gesetzt
      // und auch nicht:    getesteter Indikator eines neueren Builds   => dito

      // Bis Build 509+ ??? kann WindowHandle() bei Terminalstart oder LoadProfile in init() und in start() 0 zurückgeben,
      // solange das Terminal/der Chart nicht endgültig initialisiert sind. Hat das letzte Chartfenster in Z order noch keinen
      // Titel (es wird gerade erzeugt), ist dies das aktuelle Chartfenster. Existiert kein solches Fenster, wird der Indikator
      // über das Tester-Template in einem Test mit VisualMode=Off geladen und wird keinen Chart haben. Die start()-Funktion
      // wird in diesem Fall nie ausgeführt.
      if (!IsUIThread()) return(_INVALID_HWND(error(ERR_ILLEGAL_STATE, "unknown state, non-ui thread=%d  hChart=%d  sec=%d", GetCurrentThreadId(), hChart, sec)));

      HWND hWndChild = GetWindow(hWndMdi, GW_CHILD);                 // first child window in Z order (top most chart window)
      if (!hWndChild)                                                // MDIClient has no children
         return(NULL);                                               // there is no no chart: Tester with VisualMode=Off

      HWND hWndLast = GetWindow(hWndChild, GW_HWNDLAST);             // last child window in Z order (lowest chart window)
      if (GetWindowTextLength(hWndLast))                             // last child window already has a title
         return(NULL);                                               // there is no chart: Tester with VisualMode=Off

      hChartWindow = hWndLast;                                       // keep chart window (holding the chart AfxFrameOrView)
   }


   // (2) Script
   else if (moduleType == MT_SCRIPT) {
      // Bis Build 509+ ??? kann WindowHandle() bei Terminalstart oder LoadProfile in init() und in start() 0 zurückgeben,
      // solange das Terminal/der Chart nicht endgültig initialisiert sind. Ein laufendes Script wurde in diesem Fall über
      // die Konfiguration in "terminal-start.ini" gestartet und läuft im ersten passenden Chart in absoluter Reihenfolge
      // (CtrlID, nicht Z order).
      HWND hWndChild = GetWindow(hWndMdi, GW_CHILD);                 // first child window in Z order (top most chart window)
      if (!hWndChild) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MDIClient window has no children in Script::init()  hWndMain=%p", hWndMain)));

      uint size = MAX_CHARTDESCRIPTION_LENGTH + 1;
      char* chartDescription = (char*)alloca(size);                  // on the stack
      uint chars = GetChartDescription(symbol, timeframe, chartDescription, size);
      if (!chars) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "GetChartDescription()")));

      char* title = NULL;
      int id = INT_MAX;

      while (hWndChild) {                                            // iterate over all child windows
         title = GetWindowTextA(hWndChild);                          // Here we can't use GetInternalWindowText() as the window
                                                                     // creation needs to finish before we get a valid response.
         if (StrEndsWith(title, " (offline)"))
            title[strlen(title)-10] = '\0';
         if (StrCompare(title, chartDescription)) {                  // find all matching windows
            id = std::min(id, GetDlgCtrlID(hWndChild));              // track the smallest in absolute order
            if (!id) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MDIClient child window %p has no control id", hWndChild)));
         }
         hWndChild = GetWindow(hWndChild, GW_HWNDNEXT);              // next child in Z order
      }
      delete[] title;
      if (id == INT_MAX) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "no matching MDIClient child window found for \"%s\"", chartDescription)));

      hChartWindow = GetDlgItem(hWndMdi, id);                        // keep chart window (holding the chart AfxFrameOrView)
   }


   // (3) Expert
   else if (moduleType == MT_EXPERT) {
      return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "MQL::WindowHandle() => 0 in Expert::init()")));
   }
   else {
      return(_INVALID_HWND(error(ERR_INVALID_PARAMETER, "invalid parameter moduleType: %d", moduleType)));
   }


   // (4) Das gefundene Chartfenster hat genau ein Child (AfxFrameOrView), welches das gesuchte MetaTrader-Handle ist.
   hChart = GetWindow(hChartWindow, GW_CHILD);
   if (!hChart) return(_INVALID_HWND(error(ERR_RUNTIME_ERROR, "no MetaTrader chart window inside of last MDIClient child window %p found", hChartWindow)));

   return(hChart);
}


/**
 * Try to fix a module's UninitializeReason reason.
 *
 * @param  EXECUTION_CONTEXT* ec           - the module's execution context (possibly still empty)
 * @param  ModuleType         moduleType   - module type
 * @param  CoreFunction       coreFunction - the core function the module is currently in
 * @param  UninitializeReason uninitReason - UninitializeReason() as passed by the terminal (possibly incorrect)
 *
 * @return UninitializeReason - uninitialize reason or EMPTY (-1) in case of errors
 */
UninitializeReason WINAPI FixUninitReason(EXECUTION_CONTEXT* ec, ModuleType moduleType, CoreFunction coreFunction, UninitializeReason uninitReason) {
   UninitializeReason bak = uninitReason;

   if (moduleType == MT_LIBRARY) {
      if (coreFunction == CF_INIT) {
         if (uninitReason == UR_RECOMPILE) {
            if (ec->pid) {                                     // after a recompilation the context cannot have state
               uninitReason = (ec->programUninitReason!=UR_RECOMPILE ? ec->programUninitReason : UR_UNDEFINED);
            }
         }
         else if (!ec->programCoreFunction && uninitReason==UR_UNDEFINED ){
            if (ec->programUninitReason != UR_RECOMPILE) {     // if the main module is in limbo apply the main UninitReason
               uninitReason = ec->programUninitReason;
            }
         }
      }

      else if (coreFunction == CF_DEINIT) {
         if (uninitReason == UR_RECOMPILE) {
            if (!ec->programCoreFunction) {                    // if the main module is in limbo there can't be a recompilation
               uninitReason = (ec->programUninitReason!=UR_RECOMPILE ? ec->programUninitReason : UR_UNDEFINED);
            }
            else if (g_recompiledModule.pid == ec->pid) {
               uninitReason = UR_UNDEFINED;                    // there can only be one recompilation per program
            }
         }
         else if (!ec->programCoreFunction) {                  // if the main module is in limbo apply the main UninitReason
            if (uninitReason==UR_UNDEFINED && ec->programUninitReason!=UR_RECOMPILE) {
               uninitReason = ec->programUninitReason;
            }
         }
      }
      //if (uninitReason != bak) debug("overriding %s with %s", UninitializeReasonToStr(bak), UninitializeReasonToStr(uninitReason));
      return(uninitReason);
   }
   return((UninitializeReason)_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter moduleType: %s (not supported)", ModuleTypeToStr(moduleType))));
}


/**
 * Find the index of the current thread in the list of known threads. If the current thread is not found it is added to
 * the list.
 *
 * @return uint - thread index
 */
uint WINAPI GetCurrentThreadIndex() {
   DWORD currentThread = GetCurrentThreadId();

   // look-up the current thread
   uint size = g_threads.size();
   for (uint i=0; i < size; i++) {
      if (g_threads[i] == currentThread)                          // thread found
         return(i);
   }

   // thread not found
   if (!TryEnterCriticalSection(&g_terminalMutex)) {
      debug("waiting to aquire lock on: g_terminalMutex");
      EnterCriticalSection(&g_terminalMutex);
   }
   g_threads        .push_back(currentThread);                    // add current thread to the list
   g_threadsPrograms.push_back(0);                                // add empty program index of 0 (zero) to the list
   uint index = g_threads.size() - 1;
   LeaveCriticalSection(&g_terminalMutex);

   if (index > 768 && !(index % 100)) debug("added thread %d to thread list (size=%d)", currentThread, index+1);
   return(index);
}


/**
 * Get the id of the last MQL program executed by the current thread.
 *
 * @return uint - program id or NULL (0) if the current thread didn't yet execute a MQL program
 */
uint WINAPI GetLastThreadProgram() {
   uint index = GetCurrentThreadIndex();
   if (g_threadsPrograms.size() > index)
      return(g_threadsPrograms[index]);
   return(NULL);
}


/**
 * Link the specified MQL program to the current thread.
 *
 * @param  uint pid - MQL program id
 *
 * @return int - index of the current thread in the list of known threads or EMPTY (-1) in case of errors
 */
int WINAPI SetLastThreadProgram(uint pid) {
   if ((int)pid < 1) return(_EMPTY(error(ERR_INVALID_PARAMETER, "invalid parameter pid: %d", pid)));

   uint index = GetCurrentThreadIndex();
   g_threadsPrograms[index] = pid;                       // update the thread's last executed program

   if (IsUIThread())
      g_lastUIThreadProgram = pid;                       // update lastUIThreadProgram if the thread is the UI thread

   return(index);
}


/**
 * Resolve a program's real init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec             - an MQL program's execution context (possibly still empty)
 * @param  EXECUTION_CONTEXT* sec            - an MQL program's super context (memory possibly already released)
 * @param  ModuleType         programType    - program type
 * @param  char*              programName    - program name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason   - UninitializeReason() as passed by the terminal (possibly incorrect)
 * @param  char*              symbol         - current symbol
 * @param  BOOL               isTesting      - IsTesting() as passed by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode   - IsVisualMode() as passed by the terminal (possibly incorrect)
 * @param  HWND               hChart         - correct WindowHandle() value
 * @param  int                droppedOnChart - WindowOnDropped() as passed by the terminal (possibly incorrect)
 * @param  int                droppedOnPosX  - WindowXOnDropped() as passed by the terminal (possibly incorrect)
 * @param  int                droppedOnPosY  - WindowYOnDropped() as passed by the terminal (possibly incorrect)
 * @param  uint               &previousPid   - variable receiving the previous pid of a program instance (if any)
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason(EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, ProgramType programType, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL isTesting, BOOL isVisualMode, HWND hChart, int droppedOnChart, int droppedOnPosX, int droppedOnPosY, uint &previousPid) {

   if (programType == PT_INDICATOR) return(GetInitReason_indicator(ec, sec, programName, uninitReason, symbol, isTesting, isVisualMode, hChart, droppedOnChart, previousPid));
   if (programType == PT_EXPERT)    return(GetInitReason_expert   (ec,      programName, uninitReason, symbol, isTesting, droppedOnPosX, droppedOnPosY));
   if (programType == PT_SCRIPT)    return(GetInitReason_script   (ec,      programName,                                  droppedOnPosX, droppedOnPosY));

   return((InitializeReason)error(ERR_INVALID_PARAMETER, "invalid parameter programType: %d (unknown)  ec=%s", programType, ec));
}


/**
 * Resolve an indicator's real init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec             - the indicator's execution context (possibly still empty)
 * @param  EXECUTION_CONTEXT* sec            - the indicator's super context (memory possibly already released)
 * @param  char*              programName    - indicator name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason   - UninitializeReason() as passed by the terminal
 * @param  char*              symbol         - current symbol
 * @param  BOOL               isTesting      - IsTesting() as passed by the terminal (possibly incorrect)
 * @param  BOOL               isVisualMode   - IsVisualMode() as passed by the terminal (possibly incorrect)
 * @param  HWND               hChart         - correct WindowHandle() value
 * @param  int                droppedOnChart - WindowOnDropped() as passed by the terminal (possibly incorrect)
 * @param  uint               &previousPid   - variable receiving the previous pid of the indicator instance (if any)
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason_indicator(EXECUTION_CONTEXT* ec, const EXECUTION_CONTEXT* sec, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL isTesting, BOOL isVisualMode, HWND hChart, int droppedOnChart, uint &previousPid) {
   /*
   History:
   ------------------------------------------------------------------------------------------------------------------------------------
   onInitUser()
     build  <  654:  - UninitializeReason() = UR_PARAMETER
     build  >= 654:  - UninitializeReason() = UR_UNDEFINED
     build 547-551:  - Broken: init() is called two times, the 2nd time global module memory is reset (empty EXECUTION_CONTEXT)
   ------------------------------------------------------------------------------------------------------------------------------------
   - Build 577-583: onInitTemplate()         - Broken: Kein Aufruf bei Terminal-Start, der Indikator wird aber geladen.
   -----------------------------------------------------------------------------------------------------------------------------------
   - Build 556-569: onInitProgram()          - Broken: Wird in- und außerhalb des Testers bei jedem Tick aufgerufen.
   -----------------------------------------------------------------------------------------------------------------------------------
   - Build  <= 229: onInitProgramAfterTest() - UninitializeReason() = UR_UNDEFINED
   - Build     387: onInitProgramAfterTest() - Broken: Wird nie aufgerufen.
   - Build 388-628: onInitProgramAfterTest() - UninitializeReason() = UR_REMOVE
   - Build  <= 577: onInitProgramAfterTest() - Wird nur nach einem automatisiertem Test aufgerufen (VisualMode=Off), der Aufruf
                                               erfolgt vorm Start des nächsten Tests.
   - Build  >= 578: onInitProgramAfterTest() - Wird auch nach einem manuellen Test aufgerufen (VisualMode=On), nur in diesem Fall
                                               erfolgt der Aufruf sofort nach Testende.
   - Build  >= 633: onInitProgramAfterTest() - UninitializeReason() ist UR_CHARTCLOSE.
   -----------------------------------------------------------------------------------------------------------------------------------
   - Build 577:     onInitTimeframeChange()  - Broken: Bricht mit Logmessage "WARN: expert stopped" ab.
   -----------------------------------------------------------------------------------------------------------------------------------
   */
   static uint terminalBuild = GetTerminalBuild();
   if (!terminalBuild) return((InitializeReason)NULL);

   BOOL isUIThread = IsUIThread();

   // (1) UR_PARAMETERS
   if (uninitReason == UR_PARAMETERS) {
      // innerhalb iCustom(): nie
      if (sec) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_PARAMETERS:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s)", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      // außerhalb iCustom(): bei erster Parameter-Eingabe eines neuen Indikators oder Parameter-Wechsel eines vorhandenen Indikators (auch im Tester bei VisualMode=On), Input-Dialog
      BOOL isProgramNew;
      uint pid = ec->pid;
      if (pid) {
         isProgramNew = !(*g_mqlInstances[pid])[0]->ticks;            // im Master-Context nachschauen
      }
      else {
         pid = FindModuleInLimbo(MT_INDICATOR, programName, uninitReason, isTesting, hChart);
         if (!pid && terminalBuild >= 654) return((InitializeReason)error(ERR_RUNTIME_ERROR, "no %s indicator found in limbo:  UR_PARAMETERS  isTesting=%s  hChart=%p  ec=%s", programName, BoolToStr(isTesting), hChart, EXECUTION_CONTEXT_toStr(ec)));
         previousPid  = pid;
         isProgramNew = !pid;
      }
      if (isProgramNew) return(IR_USER      );                       // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators
      else              return(IR_PARAMETERS);                       // Parameter-Wechsel eines vorhandenen Indikators
   }


   // (2) UR_CHARTCHANGE
   if (uninitReason == UR_CHARTCHANGE) {
      // innerhalb iCustom(): nie
      if (sec) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_CHARTCHANGE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s)", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      // außerhalb iCustom(): bei Symbol- oder Timeframe-Wechsel eines vorhandenen Indikators, kein Input-Dialog
      uint pid = ec->pid;
      if (!pid) {
         pid = FindModuleInLimbo(MT_INDICATOR, programName, uninitReason, isTesting, hChart);
         if (!pid) return((InitializeReason)error(ERR_RUNTIME_ERROR, "no %s indicator found in limbo:  UR_CHARTCHANGE  isTesting=%s  hChart=%p  ec=%s", programName, BoolToStr(isTesting), hChart, EXECUTION_CONTEXT_toStr(ec)));
         previousPid = pid;
      }
      char* masterSymbol = (*g_mqlInstances[pid])[0]->symbol;
      if (StrCompare(masterSymbol, symbol)) return(IR_TIMEFRAMECHANGE);
      else                                  return(IR_SYMBOLCHANGE   );
   }


   // (3) UR_UNDEFINED
   if (uninitReason == UR_UNDEFINED) {
      // außerhalb iCustom(): je nach Umgebung
      if (!sec) {
         if (terminalBuild < 654) return(IR_TEMPLATE);               // wenn Template mit Indikator geladen wird (auch bei Start und im Tester bei VisualMode=On|Off), kein Input-Dialog
         if (droppedOnChart >= 0) return(IR_TEMPLATE);
         else                     return(IR_USER    );               // erste Parameter-Eingabe eines manuell neu hinzugefügten Indikators, Input-Dialog
      }
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (isTesting && !isVisualMode/*Fix*/ && isUIThread) {         // versionsunabhängig
         if (terminalBuild <= 229) {
            uint pid = FindModuleInLimbo(MT_INDICATOR, programName, uninitReason, isTesting, hChart);
            if (!pid) return((InitializeReason)error(ERR_RUNTIME_ERROR, "no %s indicator found in limbo:  UR_UNDEFINED  isTesting=%s  hChart=%p  ec=%s", programName, BoolToStr(isTesting), hChart, EXECUTION_CONTEXT_toStr(ec)));
            previousPid = pid;
            return(IR_PROGRAM_AFTERTEST);
         }
         return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_UNDEFINED:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      }
      return(IR_PROGRAM);
   }


   // (4) UR_REMOVE
   if (uninitReason == UR_REMOVE) {
      // außerhalb iCustom(): nie
      if (!sec)                      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_REMOVE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (!isTesting || !isUIThread) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_REMOVE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      uint pid = FindModuleInLimbo(MT_INDICATOR, programName, uninitReason, isTesting, hChart);
      if (!pid)                      return((InitializeReason)error(ERR_RUNTIME_ERROR, "no %s indicator found in limbo:  UR_REMOVE  isTesting=%s  hChart=%p  ec=%s", programName, BoolToStr(isTesting), hChart, EXECUTION_CONTEXT_toStr(ec)));

      if (!isVisualMode/*Fix*/ && 388<=terminalBuild && terminalBuild<=628) { previousPid = pid; return(IR_PROGRAM_AFTERTEST); }
      if ( isVisualMode/*Fix*/ && 578<=terminalBuild && terminalBuild<=628) { previousPid = pid; return(IR_PROGRAM_AFTERTEST); }
      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_REMOVE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
   }


   // (5) UR_RECOMPILE
   if (uninitReason == UR_RECOMPILE) {
      // innerhalb iCustom(): nie
      if (sec) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_RECOMPILE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      // außerhalb iCustom(): bei Reload nach Recompilation, vorhandener Indikator, kein Input-Dialog

      uint pid = FindModuleInLimbo(MT_INDICATOR, programName, uninitReason, isTesting, hChart);
      if (!pid) return((InitializeReason)error(ERR_RUNTIME_ERROR, "no %s indicator found in limbo:  UR_RECOMPILE  isTesting=%s  hChart=%p  ec=%s", programName, BoolToStr(isTesting), hChart, EXECUTION_CONTEXT_toStr(ec)));
      previousPid = pid;
      g_recompiledModule = RECOMPILED_MODULE();             // reset global recompile marker
      return(IR_RECOMPILE);
   }


   // (6) UR_CHARTCLOSE
   if (uninitReason == UR_CHARTCLOSE) {
      // außerhalb iCustom(): nie
      if (!sec)                      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_CHARTCLOSE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      // innerhalb iCustom(): je nach Umgebung, kein Input-Dialog
      if (!isTesting || !isUIThread) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_CHARTCLOSE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
      if (terminalBuild >= 633) {
         uint pid = FindModuleInLimbo(MT_INDICATOR, programName, uninitReason, isTesting, hChart);
         if (!pid) return((InitializeReason)error(ERR_RUNTIME_ERROR, "no %s indicator found in limbo:  UR_CHARTCLOSE  isTesting=%s  hChart=%p  ec=%s", programName, BoolToStr(isTesting), hChart, EXECUTION_CONTEXT_toStr(ec)));
         previousPid = pid;
         return(IR_PROGRAM_AFTERTEST);
      }
      return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UR_CHARTCLOSE:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s)", sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
   }


   switch (uninitReason) {
      case UR_ACCOUNT:       // nie
      case UR_TEMPLATE:      // build > 509
      case UR_INITFAILED:    // ...
      case UR_CLOSE:         // ...
         return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected %s:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", UninitializeReasonToStr(uninitReason), sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
   }

   return((InitializeReason)error(ERR_ILLEGAL_STATE, "unknown UninitializeReason %d:  sec=%p  isTesting=%s  isVisualMode=%s  thread=%d %s  build=%d  ec=%s", uninitReason, sec, BoolToStr(isTesting), BoolToStr(isVisualMode), GetCurrentThreadId(), isUIThread ? "(UI)":"(non-UI)", terminalBuild, EXECUTION_CONTEXT_toStr(ec)));
}


/**
 * Resolve an expert's real init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec            - the expert's execution context (possibly still empty)
 * @param  char*              programName   - expert name (with or without filepath depending on the terminal version)
 * @param  UninitializeReason uninitReason  - UninitializeReason() as passed by the terminal
 * @param  char*              symbol        - current symbol
 * @param  BOOL               isTesting     - IsTesting() as passed by the terminal
 * @param  int                droppedOnPosX - WindowXOnDropped() as passed by the terminal
 * @param  int                droppedOnPosY - WindowYOnDropped() as passed by the terminal
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason_expert(EXECUTION_CONTEXT* ec, const char* programName, UninitializeReason uninitReason, const char* symbol, BOOL isTesting, int droppedOnPosX, int droppedOnPosY) {
   //debug("uninitReason=%s  testing=%d  droppedX=%d  droppedY=%d  build=%d", UninitReasonToStr(uninitReason), isTesting, droppedOnPosX, droppedOnPosY, build);

   static uint terminalBuild = GetTerminalBuild();
   if (!terminalBuild) return((InitializeReason)NULL);

   // UR_PARAMETERS                                      // input parameters changed
   if (uninitReason == UR_PARAMETERS) {
      return(IR_PARAMETERS);
   }

   // UR_CHARTCHANGE                                     // chart symbol or period changed
   if (uninitReason == UR_CHARTCHANGE) {
      int pid = ec->pid;
      if (!pid) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s (ec.pid=0  Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, terminalBuild));
      char* masterSymbol = (*g_mqlInstances[pid])[0]->symbol;
      if (StrCompare(masterSymbol, symbol)) return(IR_TIMEFRAMECHANGE);
      else                                  return(IR_SYMBOLCHANGE);
   }

   // UR_RECOMPILE                                       // reloaded after recompilation
   if (uninitReason == UR_RECOMPILE) {
      return(IR_RECOMPILE);
   }

   // UR_CHARTCLOSE                                      // loaded into an existing chart after new template was loaded
   if (uninitReason == UR_CHARTCLOSE) {                  // (old builds only, corresponds to UR_TEMPLATE of new builds)
      if (terminalBuild > 509) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, terminalBuild));
      return(IR_USER);
   }

   // UR_UNDEFINED                                       // loaded into a new chart (also at terminal start and in tester)
   if (uninitReason == UR_UNDEFINED) {
      if (isTesting)          return(IR_USER);
      if (droppedOnPosX >= 0) return(IR_USER);           // TODO: It is rare but possible to manually load an expert with droppedOnPosX = -1.
      HWND hWndDlg = FindInputDialogA(PT_EXPERT, programName);
      if (hWndDlg)            return(IR_TERMINAL_FAILURE);
      else                    return(IR_TEMPLATE);
   }

   // UR_REMOVE                                          // loaded into an existing chart after a previously loaded one was removed manually
   if (uninitReason == UR_REMOVE) {
      if (droppedOnPosX >= 0) return(IR_USER);           // TODO: It is rare but possible to manually load an expert with droppedOnPosX = -1.
      HWND hWndDlg = FindInputDialogA(PT_EXPERT, programName);
      if (hWndDlg)            return(IR_TERMINAL_FAILURE);
      else                    return(IR_TEMPLATE);
   }

   // UR_TEMPLATE                                        // loaded into an existing chart after a previously loaded one was removed by LoadTemplate()
   if (uninitReason == UR_TEMPLATE) {
      if (terminalBuild <= 509) return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s  (Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, terminalBuild));
      if (droppedOnPosX >= 0)     return(IR_USER);       // TODO: It is rare but possible to manually load an expert with droppedOnPosX = -1.
      HWND hWndDlg = FindInputDialogA(PT_EXPERT, programName);
      if (hWndDlg)                return(IR_TERMINAL_FAILURE);
      else                        return(IR_TEMPLATE);
   }

   switch (uninitReason) {
      case UR_ACCOUNT:
      case UR_CLOSE:
      case UR_INITFAILED:
         return((InitializeReason)error(ERR_ILLEGAL_STATE, "unexpected UninitializeReason %s (Testing=%d  build=%d)", UninitializeReasonToStr(uninitReason), isTesting, terminalBuild));
   }
   return((InitializeReason)error(ERR_ILLEGAL_STATE, "unknown UninitializeReason %d (Testing=%d  build=%d)", uninitReason, isTesting, terminalBuild));
}


/**
 * Resolve a script's real init() reason.
 *
 * @param  EXECUTION_CONTEXT* ec            - the script's execution context (possibly still empty)
 * @param  char*              programName   - script name (with or without filepath depending on the terminal version)
 * @param  int                droppedOnPosX - WindowXOnDropped() as passed by the terminal
 * @param  int                droppedOnPosY - WindowYOnDropped() as passed by the terminal
 *
 * @return InitializeReason - init reason or NULL in case of errors
 */
InitializeReason WINAPI GetInitReason_script(EXECUTION_CONTEXT* ec, const char* programName, int droppedOnPosX, int droppedOnPosY) {
   return(IR_USER);
}


/**
 * Whether the program is executed in the tester with Optimization=On.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isOptimization - MQL::IsOptimization() status as passed by the terminal
 *
 * @return BOOL - real IsOptimization() status
 */
BOOL WINAPI Program_IsOptimization(const EXECUTION_CONTEXT* ec, BOOL isOptimization) {
   if (ec->superContext)
      return(ec->superContext->optimization);
   return(isOptimization);
}


/**
 * Whether the program with the specified pid is a partially initialized expert in tester, having an unset name or
 * matching the passed name.
 *
 * @param  uint  pid  - program id
 * @param  char* name - program name
 *
 * @return BOOL
 */
BOOL WINAPI Program_IsPartialTest(uint pid, const char* name) {
   if (!pid) return(FALSE);

   if (g_mqlInstances.size() > pid) {
      ContextChain &chain = *g_mqlInstances[pid];

      if (chain.size() > 2) {                                  // needs to have at least one registered library
         EXECUTION_CONTEXT* master = chain[0];
         EXECUTION_CONTEXT* main   = chain[1];

         if (master->programType==PT_EXPERT && master->testing) {
            if (main)                  return(FALSE);
            if (!*master->programName) return(TRUE);
            return(StrCompare(master->programName, name));     // name comparison last
         }
      }
   }
   return(FALSE);
}


/**
 * Whether the program is executed in the tester or on a test chart.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isTesting - MQL::IsTesting() status as passed by the terminal (possibly incorrect)
 *
 * @return BOOL - real IsTesting() status
 */
BOOL WINAPI Program_IsTesting(const EXECUTION_CONTEXT* ec, BOOL isTesting) {
   switch (ec->programType) {
      case PT_INDICATOR: {
         if (ec->superContext) return(ec->superContext->testing);    // prefer an inherited status
         if (isTesting)        return(TRUE);                         // indicator runs in iCustom() in Tester

         // (1) indicator was loaded manually                        // we have no super context
         //     (1.1) not in Tester:                     chart exists, title is set and doesn't end with "(visual)"
         //     (1.2) in Tester:                         chart exists, title is set and ends with        "(visual)"

         // (2) indicator was loaded by template
         //     (2.1) not in Tester:                     chart exists, title is empty or doesn't end with "(visual)"
         //     (2.2) in Tester:                         chart exists, title is set and ends with         "(visual)"
         //     (2.3) in Tester                       or chart doesn't exist with VisualMode=Off
         HWND hWnd = ec->hChartWindow;
         if (!hWnd) return(TRUE);                                    // (2.3) no chart => in Tester with VisualMode=Off

         wchar* title = GetInternalWindowTextW(hWnd);
         BOOL result = StrEndsWith(title, L"(visual)");
         delete[] title;
         return(result);                                             // all remaining cases according to "(visual)" in title
      }

      case PT_EXPERT:
         return(isTesting);

      case PT_SCRIPT: {
         HWND hWnd = ec->hChartWindow;
         if (hWnd) {
            wchar* title = GetInternalWindowTextW(hWnd);
            BOOL result = StrEndsWith(title, L"(visual)");
            delete[] title;
            return(result);
         }
         return(error(ERR_ILLEGAL_STATE, "script without a chart:  ec=%s", EXECUTION_CONTEXT_toStr(ec)));
      }
   }

   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType: %d", ec->programType));
}


/**
 * Whether the current program is executed in the tester or on a test chart with VisualMode=On.
 *
 * @param  EXECUTION_CONTEXT* ec
 * @param  BOOL               isVisualMode - MQL::IsVisualMode() status as passed by the terminal (possibly wrong)
 *
 * @return BOOL - real IsVisualMode() status
 */
BOOL WINAPI Program_IsVisualMode(const EXECUTION_CONTEXT* ec, BOOL isVisualMode) {
   if (ec->superContext)
      return(ec->superContext->visualMode);                          // prefer an inherited status

   switch (ec->programType) {
      case PT_INDICATOR: return(ec->testing && ec->hChart);
      case PT_EXPERT:    return(isVisualMode);
      case PT_SCRIPT:    return(ec->testing);                        // scripts can only run on visible charts
   }
   return(error(ERR_INVALID_PARAMETER, "invalid value ec.programType: %d", ec->programType));
}


/**
 * Push the specififed ContextChain (an MQL program) onto the end of the program list.
 *
 * @param  ContextChain* chain
 *
 * @return uint - index where the program is stored
 */
uint WINAPI PushProgram(ContextChain* chain) {
   if (!TryEnterCriticalSection(&g_terminalMutex)) {
      debug("waiting to aquire lock on g_terminalMutex...");
      EnterCriticalSection(&g_terminalMutex);
   }
   g_mqlInstances.push_back(chain);
   uint index = g_mqlInstances.size()-1;
   LeaveCriticalSection(&g_terminalMutex);

   //if (index > 31) debug("registered programs: %d", index);   // index[0] is always empty
   return(index);
}
