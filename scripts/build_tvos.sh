pushd .

cd ../XEncrypt/
cmake -B build_tvos -GXcode -DCMAKE_SYSTEM_NAME=tvOS -DCMAKE_OSX_ARCHITECTURES=arm64 -DPLAT=tvOS
cmake --build build_tvos --config Release

mkdir -p plugin_tvos/static/Plugins/tvOS/
cp build_tvos/Release/*.a plugin_tvos/static/Plugins/tvOS/

mkdir -p plugin_tvos/shared/Plugins/tvOS/
cp build_tvos/Release/*.a plugin_tvos/shared/Plugins/tvOS/

popd
