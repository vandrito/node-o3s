#!/bin/sh
echo "Cleaning..."
rm -f node_o3s.tgz
echo "Building..."
node-gyp clean
node-gyp configure
node-gyp build
echo "Creating archive and uploading..."
mkdir -p package
cp -R ./binding.gyp ./src ./lib ./package.json ./package
tar czvf node_o3s.tgz package
sleep 1
wput node_o3s.tgz -Bu ftp://o3sinside:jghf333qq11039@ftp.p436.phpnet.org
rm node_o3s.tgz
rm -Rf package
echo "Done."
