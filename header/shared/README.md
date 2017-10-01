
The files in this directory are shared with the project ```rosasurfer/mt4-mql```. All header files in
```{rosasurfer/mt4-mql}/mql4/include/shared``` should be symlinked to this directory.

If the build script cannot find symlinked files here it uses the backup files in
```{rosasurfer/mt4-expander}/header/shared/bak``` for compilation. However if it finds symlinked files here it will
update the files in the backup directory (only after a successful build).

- - -

A comfortable way to manage symbolic links on Windows is the [Link Shell Extension](http://schinagl.priv.at/nt/hardlinkshellext/linkshellextension.html)
by Hermann Schinagl.

To enable Git support of symbolic links on Windows you must add ```core.symlinks = true``` to your Git configuration.
