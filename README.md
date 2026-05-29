
## MetaTrader 4 framework DLL extension


This project is part of the [MetaTrader 4 development framework](https://github.com/rosasurfer/mt4-mql).
- - -

### Build environment

The DDL is built with Visual Studio 2008 (an Express version was not tested) and requires at least Windows 7/Server 2008 R2.
<br>
<br>

### Shared header files

The header files in `{expander-root}/header/shared` should be shared with the [MetaTrader 4 MQL framework](https://github.com/rosasurfer/mt4-mql)
to make sure that definitions in both projects match. It can be accomplished by symlinking the files from `{rosasurfer/mt4-mql}/mql4/include/shared`
to `{expander-root}/header/shared`. The DLL can be build with and without shared headers. If you get the error
`Cannot open include file: 'shared/defines.h': No such file or directory` symlink the shared files or run the build tool at least once to
setup building without shared headers.
<br>
<br>

### Pre/Post-build events

If the Visual Studio build tool can't find the header files in `{expander-root}/header/shared` the pre-build event restores backups from
`{expander-root}/header/shared/bak`.

After a successfull build the post-build event creates new backups of the currently used files. If a (symlinked) directory
`{expander-root}/bin/mql4-libraries` is found the resulting DLL is copied into that directory.

```bash
:: Pre-build event
if not exist "$(ProjectDir)header\shared\defines.h" (
   echo Shared header "/header/shared/defines.h" missing, using backup "/header/shared/bak/defines.h"...
   copy "$(ProjectDir)header\shared\bak\defines.h" "$(ProjectDir)header\shared\" || exit 1
)
if not exist "$(ProjectDir)header\shared\errors.h" (
   echo Shared header "/header/shared/errors.h" missing, using backup "/header/shared/bak/errors.h"...
   copy "$(ProjectDir)header\shared\bak\errors.h" "$(ProjectDir)header\shared\" || exit 1
)
```

```bash
:: Post-build event
echo Backup shared header "/header/shared/defines.h"...
copy "$(ProjectDir)header\shared\defines.h" "$(ProjectDir)header\shared\bak\" || exit 1

echo Backup shared header "/header/shared/errors.h"...
copy "$(ProjectDir)header\shared\errors.h" "$(ProjectDir)header\shared\bak\" || exit 1

if exist "$(ProjectDir)bin\mql4-libraries\" (
    echo Copy DLL to MQL library folder...
    copy "$(TargetPath)" "$(ProjectDir)bin\mql4-libraries\rsfExpander.$(ConfigurationName)$(TargetExt)" || exit 1
)
```
<br>

### Managing symlinks and junctions on Windows

A comfortable way to manage Windows reparse points is the free [Link Shell Extension](http://schinagl.priv.at/nt/hardlinkshellext/linkshellextension.html)
by Hermann Schinagl. To activate Git support for symbolic links on Windows add the config option `core.symlinks = true` to your Git configuration:
```bash
$ git config --global core.symlinks true
```
