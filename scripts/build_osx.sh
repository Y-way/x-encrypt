mkdir -p build_osx && cd build_osx
cmake -GXcode ../
cd ..
cmake --build build_osx --config Release
mkdir -p plugin_osx/Plugins/XEncrypt.bundle/Contents/MacOS/
cp build_osx/Release/XEncrypt.bundle/Contents/MacOS/XEncrypt plugin_osx/Plugins/XEncrypt.bundle/Contents/MacOS/XEncrypt
