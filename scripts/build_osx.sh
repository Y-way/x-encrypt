pushd .

cd ../XEncrypt/

mkdir -p build_osx && cd build_osx
cmake -GXcode ../ -DBUILD_SHARED_LIBS=ON -DARCHS="x86_64"
cd ..
cmake --build build_osx --config Release
mkdir -p plugin_osx/Plugins/

cp build_osx/Release/* plugin_osx/Plugins/

popd
