#!/bin/sh

#Fix working directory when run by Finder
cd `dirname $0`

clear
echo
echo "This script is designed to work with the GIMP release from gimp.org."
echo "If you've installed the GIMP from unofficial software repositories,"
echo "this script may not work."
echo
echo "If you have no idea what the above means, you can safely proceed."
echo

while :; do
read -n1 -rs -p "Would you like to install the XYZ file plugin for the user $USER? (y/n)"
echo
[ "$REPLY" == "y" ] && break
[ "$REPLY" == "n" ] && exit
done

#Determine processor. Uname is unreliable because the kernel might
#be running in 32 bit mode.
processor=`system_profiler SPHardwareDataType 2>/dev/null | grep 'Processor Name:' | sed 's/.*: //'`
if [ "$processor" == 'Intel Core Solo' ] || [ "$processor" == 'Intel Core Duo' ]; then
  arch="32"
else
  arch="64"
fi

echo
install_path="$HOME/Library/Application Support/GIMP/2.8/plug-ins"
mkdir -p "$install_path"
rm -f "$install_path/file-xyz"
cp bin/osx/$arch/file-xyz "$install_path"
chmod +x "$install_path/file-xyz"
if [ $? -eq 0 ]; then
  echo "Plugin successfully installed! You'll need to restart the GIMP to use it."
  rc=0
else
  echo "Something went wrong! Did you extract the entire contents of the ZIP archive?"
  rc=1
fi
echo
read -n1 -rs -p "Press any key to continue..."
echo
exit
