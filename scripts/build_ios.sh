pushd .

cd ../XEncrypt/
cmake -B build_ios -GXcode -DCMAKE_SYSTEM_NAME=iOS "-DCMAKE_OSX_ARCHITECTURES=arm64"
cmake --build build_ios --config Release
mkdir -p plugin_ios/Plugins/iOS/
cp build_ios/Release/*.a plugin_ios/Plugins/iOS/*.a 

popd
