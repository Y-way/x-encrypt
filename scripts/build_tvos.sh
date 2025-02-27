pushd .

cd ../XEncrypt/
cmake -B build_tvos -GXcode -DCMAKE_SYSTEM_NAME=tvOS -DCMAKE_OSX_ARCHITECTURES=arm64 -DPLAT=tvOS
cmake --build build_tvos --config Release
mkdir -p plugin_tvos/Plugins/tvOS/
cp build_tvos/Release/*.a plugin_tvos/Plugins/tvOS/

popd
