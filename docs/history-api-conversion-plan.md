# MT4 history API conversion plan

## Scope and goal

Convert the working legacy MQL4 history API in `mql40/libraries/rsfHistory1.mq4` into native MT4Expander C/C++ exports. The first C/C++ iteration should preserve the MQL API semantics, file formats, validation behavior, and metadata model while moving the heavy history-file operations into the DLL to remove MQL performance overhead and the MT4-imposed per-module open-file limit.

## Existing implementation overview

### Public API surface to preserve

The current MQL library exposes two handle-oriented layers:

| Layer | MQL entry point | Purpose |
| --- | --- | --- |
| History set | `HistorySet1.Create(symbol, descr, digits, format, directory="")` | Create/reset a 9-timeframe set for one symbol. |
| History set | `HistorySet1.Get(symbol, directory="")` | Locate an existing set and derive metadata from the first existing timeframe file. |
| History set | `HistorySet1.Close(hSet)` | Flush and close all open files in a set. |
| History set | `HistorySet1.AddTick(hSet, time, value, flags=NULL)` | Add one tick to all standard timeframes. |
| History file | `HistoryFile1.Open(symbol, timeframe, description, digits, format, mode, directory="")` | Open or create one `.hst` file. |
| History file | `HistoryFile1.Close(hFile)` | Flush buffered data and close one file. |
| History file | `HistoryFile1.FindBar(hFile, time, lpBarExists[])` | Locate the existing or would-be bar offset covering a timestamp. |
| History file | `HistoryFile1.ReadBar(hFile, offset, bar[])` | Read one bar into the common `double[6]` bar shape. |
| History file | `HistoryFile1.WriteBar(hFile, offset, bar[], flags=NULL)` | Write one bar, optionally treating `bar[BAR_T]` as an open time. |
| History file | `HistoryFile1.UpdateBar(hFile, offset, value)` | Update OHLCV of one existing bar with a tick value. |
| History file | `HistoryFile1.InsertBar(hFile, offset, bar[], flags=NULL)` | Insert a bar, shifting younger bars if needed. |
| History file | `HistoryFile1.WriteLastStoredBar(hFile, flags=NULL)` | Flush cached `lastStoredBar` metadata/data. |
| History file | `HistoryFile1.WriteBufferedBar(hFile, flags=NULL)` | Flush the tick-buffered current bar. |
| History file | `HistoryFile1.MoveBars(hFile, fromOffset, destOffset)` | Move a contiguous block of bars inside a file. |
| History file | `HistoryFile1.AddTick(hFile, time, value, flags=NULL)` | Add one tick to one timeframe. |

The native conversion should initially export the same logical operations with stable MQL-friendly signatures. The authoritative public contract is the MQL include `mql40/include/rsf/history.mqh`; use that file for the actual exported names, parameter order, default behavior, and wrapper shape. Treat `mql40/libraries/rsfHistory1.mq4` as the behavior reference and `history.mqh` as the API/signature reference. Do not invent suffix-free replacement names unless `history.mqh` explicitly defines such wrappers.

### Data formats already available in C/C++

The repository already contains packed C/C++ structs for the on-disk history header and both bar formats:

- `HISTORY_HEADER`: 148 bytes, including `barFormat`, 64-byte ANSI description, 12-byte symbol field, period, digits, and sync timestamps.
- `HISTORY_BAR_400`: 44 bytes, with 32-bit `time`, `open`, `low`, `high`, `close`, and `ticks` as a double.
- `HISTORY_BAR_401`: 60 bytes, with 64-bit-compatible time/tick-volume unions, `open`, `high`, `low`, `close`, spread, and real volume.

Important implementation detail: the MQL comments state that format 400 stores strings in ANSI and format 401 stores strings in UTF-16. The existing C/C++ `HISTORY_HEADER` currently models the header as ANSI `char` fields. Before implementing writes, confirm whether the UTF-16 distinction applies to the header strings, MT4 UI/API string handling, or another production-specific interpretation; this is the one place where the plan needs confirmation despite the MQL implementation being production-proven.

### Core state model to port

The MQL implementation maintains process-global arrays for two handle spaces:

- `hs.*`: history-set handles, symbol metadata, target directory, default format for created files, and up to nine child file handles.
- `hf.*`: history-file handles, file mode, header, file format, bar size, symbol metadata, period duration, directory, and several cached metadata blocks.

The C/C++ conversion should keep the same conceptual state but model it as explicit structs stored in vectors/maps:

```cpp
struct HistorySetState {
   int handle;
   std::string symbol;
   std::string symbolUpper;
   std::string description;
   int digits;
   std::string directory;
   int format;
   int files[9];
   bool open;
};

struct BarCache {
   int offset;
   time32 openTime;
   time32 closeTime;
   time32 nextCloseTime;
   double data[6];
   bool modified;
};

struct RangeMeta {
   int bars;
   int fromOffset;
   time32 fromOpenTime;
   time32 fromCloseTime;
   time32 fromNextCloseTime;
   int toOffset;
   time32 toOpenTime;
   time32 toCloseTime;
   time32 toNextCloseTime;
};

struct HistoryFileState {
   int handle;
   FILE* file;              // or Win32 HANDLE for explicit share/flush semantics
   bool readAccess;
   bool writeAccess;
   HISTORY_HEADER header;
   int format;
   int barSize;
   std::string symbol;
   std::string symbolUpper;
   int period;
   int periodSecs;
   int digits;
   std::string directory;
   RangeMeta stored;
   RangeMeta total;
   BarCache lastStoredBar;
   BarCache bufferedBar;
   bool open;
};
```

Use monotonically increasing integer handles, never `0`, and preserve the MQL behavior where positive handles are open, negative internal state means closed, and `0` means invalid. Native internals do not need array index == handle, but preserving it will simplify MQL wrapper parity and error messages.

### File and directory semantics

The MQL code distinguishes three directory modes:

1. Empty directory: current trade-server history directory, using MQL `FileOpenHistory()`.
2. Relative directory: MQL sandbox/files-relative path, using MQL `FileOpen()`.
3. Absolute directory: currently rejected as not implemented in `HistoryFile1.Open()`.

The DLL should implement all path resolution natively and should not depend on MQL file APIs:

- Empty directory maps to MT4Expander terminal helpers, preferably `GetHistoryRootPathA()` plus account-server directory discovery or the existing `FindHistoryDirectoryA()` behavior.
- Relative directory maps to `GetMqlSandboxPathA(inTester)` plus the supplied relative path.
- Absolute directory can be implemented in C/C++ as part of the conversion if desired, because the DLL is not constrained by the MQL sandbox. If compatibility with the current MQL behavior is required, keep rejecting it for the first compatibility pass and enable it behind a later explicit change.

Use binary mode, explicit little-endian struct writes, and validation that `fileSize >= sizeof(HISTORY_HEADER)` before reading an existing file. For read-only open of a missing file, preserve the `-1` return value; reserve `0` for errors.

## Conversion phases

### Phase 0: Lock down parity requirements

Deliverables:

- A short MQL-to-C export mapping table derived from the authoritative `mql40/include/rsf/history.mqh` include.
- A parity checklist covering return values, flags, error behavior, and path behavior.
- Golden `.hst` fixtures for format 400 and 401, ideally one sparse history, one contiguous history, and one file with month/week boundaries.

Open confirmation before coding:

1. Should absolute directories become supported in the native API immediately, or should first-pass behavior match MQL and reject them?
2. For format 401, are `description` and `symbol` expected to be UTF-16 in any on-disk header currently used in production, or are existing C/C++ header structs authoritative?
3. Do any names/signatures in `mql40/include/rsf/history.mqh` require compatibility aliases in addition to the primary DLL exports?

### Phase 1: Native module scaffolding

Deliverables:

- Add `header/lib/history.h` and `src/lib/history.cpp`.
- Register both files in `MT4Expander.vcproj`.
- Define exported functions matching the public API surface.
- Define native state containers, standard periods `{1,5,15,30,60,240,1440,10080,43200}`, constants for `HST_*`, and helper functions for validation.
- Update the authoritative `mql40/include/rsf/history.mqh` wrapper only as needed after the DLL exports exist; avoid introducing a second competing history include.

Implementation notes:

- Use the repository's existing export style: functions declared in headers and implemented with `#pragma EXPANDER_EXPORT` in `.cpp` files.
- Keep the first pass single-threaded or guard state with a `CRITICAL_SECTION`; MT4 usually calls MQL code on known terminal threads, but DLL state is process-global.
- Avoid exceptions crossing DLL boundaries. Return `0`, `false`, or `-1` as the MQL API does and log through existing `error()`, `warn()`, and related helpers.

### Phase 2: Struct conversion and primitive I/O

Deliverables:

- Bar conversion helpers:
  - common `double[6]` ⇄ `HISTORY_BAR_400`
  - common `double[6]` ⇄ `HISTORY_BAR_401`
- Header creation helper that truncates description to 63 chars and validates symbol/digits/format.
- Read/write helpers by offset:
  - `dataOffset = sizeof(HISTORY_HEADER) + offset * barSize`
  - seek/read exact bar size
  - seek/write exact bar size
  - truncate/extend files as needed

Implementation notes:

- Preserve the common bar order `BAR_T`, `BAR_O`, `BAR_H`, `BAR_L`, `BAR_C`, `BAR_V`.
- Preserve format differences: format 400 order is time/open/low/high/close/ticks; format 401 order is time/open/high/low/close/tickVolume/spread/realVolume.
- When writing format 401, set the high halves/reserved fields deterministically to zero unless production files require otherwise.

### Phase 3: `HistoryFile_Open`, metadata scan, and `Close`

Deliverables:

- Implement open/create/reset behavior for `FILE_READ`, `FILE_WRITE`, and read-write modes.
- Read existing header, detect `barFormat`, set `barSize`, and validate file size alignment.
- Initialize `stored`, `total`, `lastStoredBar`, and `bufferedBar` metadata from the file:
  - empty files have `total.bars == 0` and no stored range.
  - non-empty files read first and last bars and calculate their close/next-close metadata.
- Implement close flushing in the same order as MQL: buffered bar first, then last stored bar if modified, then close the OS handle.

Implementation notes:

- Month and week close-time calculations are critical for `PERIOD_W1` and `PERIOD_MN1`; centralize these in a `BarTimes calculateBarTimes(period, openTime)` helper.
- Keep all time values as MT4 `time32` until a specific need for `time64` is proven.

### Phase 4: Read/write/update/insert/move primitives

Deliverables:

- `HistoryFile_FindBar` ported with the same fast-path metadata checks and binary-search fallback.
- `HistoryFile_ReadBar` updates `lastStoredBar` exactly like MQL.
- `HistoryFile_WriteBar` handles open-time conversion when `HST_TIME_IS_OPENTIME` is absent, updates stored/total metadata, and invalidates or refreshes caches.
- `HistoryFile_UpdateBar` adjusts high/low/close/volume for existing bars.
- `HistoryFile_InsertBar` supports appending and insertion into the middle using `MoveBars`.
- `HistoryFile_MoveBars` moves bar blocks safely, preferably backward when ranges overlap.

Implementation notes:

- The native implementation can improve performance substantially by doing block moves with a fixed-size buffer instead of one-bar MQL loops.
- Preserve duplicate/out-of-order handling: if the requested offset already exists, update/write; if a bar must be inserted, shift younger bars and keep metadata consistent.
- Treat all file offsets as 64-bit internally even though MQL returns int handles and bar offsets.

### Phase 5: Tick aggregation and set management

Deliverables:

- `HistoryFile_AddTick` ported after primitives are proven.
- `HistorySet_Create`, `HistorySet_Get`, `HistorySet_Close`, and `HistorySet_AddTick` implemented on top of file functions.
- Tick buffering behavior preserved for these flags:
  - `HST_BUFFER_TICKS`
  - `HST_SKIP_DUPLICATE_TICKS`
  - `HST_FILL_GAPS`
  - `HST_TIME_IS_OPENTIME`

Implementation notes:

- Keep `HST_BUFFER_TICKS` as the default high-throughput path: update a current buffered bar in memory and flush only on period change or close.
- Preserve gap-fill behavior by inserting missing bars with the prior close as OHLC where MQL does so.
- Ensure `HistorySet_AddTick` lazily opens one file per standard timeframe and then forwards the tick to each file handle.

### Phase 6: Compatibility wrapper and migration

Deliverables:

- Replace or augment the MQL `HistorySet1.*` / `HistoryFile1.*` library layer with thin calls to the DLL.
- Keep the legacy MQL implementation available behind a compile-time switch until the native version is verified in production.
- Document the wrapper's path behavior and the fact that the DLL no longer has the MQL 64-open-file limit.

Recommended approach:

- Start from `mql40/include/rsf/history.mqh` and keep it as the single MQL-facing history include.
- Preserve the names and signatures already used by MQL where possible.
- If the DLL needs internal helper names, hide them behind the wrapper rather than changing MQL call sites.
- Keep the legacy MQL implementation available behind a compile-time switch until native parity is proven.

## Test strategy

### Native unit tests or standalone harness

Because this is a Visual Studio 2008-era DLL project, a simple console harness may be easier than introducing a test framework. The harness should link the same source or call the DLL exports and operate on temporary directories.

Minimum cases:

1. Create format 400 file, write/read one bar, verify byte size `148 + 44`.
2. Create format 401 file, write/read one bar, verify byte size `148 + 60`.
3. Append ticks to M1 and verify OHLCV aggregation.
4. Add duplicate ticks with `HST_SKIP_DUPLICATE_TICKS` and verify volume behavior.
5. Insert an older bar before existing data and verify `MoveBars` and metadata.
6. Insert bars across daily, weekly, and monthly boundaries.
7. Read existing golden files generated by the legacy MQL implementation.
8. Open more than 64 files in one process to verify the MQL handle limit is removed.

### MQL parity tests

Add a temporary MQL script or EA that:

- Generates the same tick stream through the legacy MQL implementation and the native implementation in separate directories.
- Compares resulting `.hst` files byte-for-byte where deterministic.
- For cases where metadata/reserved bytes differ intentionally, compares decoded bars and headers.

### Performance tests

Benchmark:

- legacy MQL `HistorySet1.AddTick` vs native `HistorySet_AddTick`
- buffered vs unbuffered mode
- one symbol and many symbols
- file counts above 64 open handles

Report throughput as ticks/second and total wall-clock time for a fixed tick stream.

## Risks and mitigations

| Risk | Mitigation |
| --- | --- |
| Format 401 string encoding ambiguity | Confirm before writes; add fixtures from production terminals. |
| MT4 terminal concurrently reading/writing `.hst` files | Use explicit share modes and flush strategy matching current behavior; document expected offline/custom-history usage. |
| Metadata cache parity bugs | Port metadata updates function-by-function and test after each primitive before implementing tick aggregation. |
| Week/month boundary mistakes | Centralize bar-time calculations and add fixtures for Mondays, month ends, leap years, and DST-neutral UTC timestamps. |
| Export signature mismatch with legacy MQL4.0 | Keep MQL-friendly primitive types only: `int`, `bool`, `double`, `datetime`, `string`, and arrays where currently used. |
| Windows XP / VS2008 constraints | Avoid modern C++ features if this project must remain VS2008-compatible; use C++03-compatible containers and Win32 APIs. |

## Suggested first coding milestone

Implement a small but complete vertical slice before porting all tick logic:

1. Add native file/header/bar helpers.
2. Export the four `mql40/include/rsf/history.mqh` functions corresponding to file open, close, read-bar, and write-bar operations.
3. Wire those four functions through `mql40/include/rsf/history.mqh` only after confirming their names and signatures match the existing MQL usage.
4. Verify format 400 and 401 round trips against generated files.
5. Only then port `FindBar`, `InsertBar`, and tick aggregation.

This milestone proves the DLL can safely own file handles, format conversion, and `.hst` bytes before carrying over the more complex buffering and gap-fill behavior.
