gimp-xyz
========

This is a plugin that allows GIMP to read and write RPG Maker 2k/2k3 XYZ files.

The code is adapted from the [WebP Plugin by Nathan Osman]
(http://registry.gimp.org/node/25874), though only as a template rather than
anything substantial. Nevertheless it retains the GPLv3 as a license.

Binary Installation
-------------------

### All Platforms
Make sure to first extract the entire contents of the zip archive first.

### Windows
Run "install_windows.bat". If you'd like to install for all users, run it as an administrator (right click->Run As Administrator).

### Mac OS X
Run "install_mac_osx.command" and follow the instructions.

Source Installation
-------------------

The following commands should do the trick:

    make
    mkdir -p ~/.gimp-2.8/plug-ins
    mv file-xyz.* ~/.gimp-2.8/plug-ins

You may need to configure the makefile and/or source files to properly build it,
but the default configuration is most likely fine.
