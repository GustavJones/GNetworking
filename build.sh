# Change directory to .sh file location
cd $(dirname $0)

# Configure project
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DGNETWORKING_BUILD_TESTS=ON

# Build project
cmake --build build -j
