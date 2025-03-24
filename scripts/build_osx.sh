echo 0
pushd .

cd ../XEncrypt/


# Building for shared library. Supporting x86_64 and arm64 architectures.
mkdir -p build_osx && cd build_osx
cmake -GXcode ../ -DBUILD_SHARED_LIBS=ON -DARCHS="\$(ARCHS_STANDARD)"

cd ..
cmake --build build_osx --config Release

mkdir -p plugin_osx/shared/Plugins/OSX
cp build_osx/Release/* plugin_osx/shared/Plugins/OSX

popd

# Building for static library. Supporting x86_64 and arm64 architectures.
pushd .

cd ../XEncrypt/

mkdir -p build_osx_a && cd build_osx_a
cmake -GXcode ../ -DBUILD_SHARED_LIBS=OFF -DARCHS="\$(ARCHS_STANDARD)"

cd ..
cmake --build build_osx_a --config Release

mkdir -p plugin_osx/static/Plugins/OSX
cp ./build_osx_a/Release/* plugin_osx/static/Plugins/OSX

popd
