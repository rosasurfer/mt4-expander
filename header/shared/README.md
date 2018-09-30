
The files in this directory are shared with the project ```rosasurfer/mt4-mql```. This is accomplished by symlinking the files
in ```{rosasurfer/mt4-mql}/mql4/include/shared``` to this directory.

If the MT4Expander build script doesn't find symlinked files here it uses backups from ```{rosasurfer/mt4-expander}/header/shared/bak```
for compilation. If it finds symlinked files it will use them and update the backup files after a successful build.

- - -

A comfortable way to manage symbolic links on Windows is the free
[Link Shell Extension](http://schinagl.priv.at/nt/hardlinkshellext/linkshellextension.html) by Hermann Schinagl.

To activate Git support for symbolic links on Windows add the config option ```core.symlinks = true``` to your Git configuration:
```bash
$ git config --global core.symlinks true
```
