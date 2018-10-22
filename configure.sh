reset

rm CMakeCache.txt

cmake .. \
-DCMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS --std=gnu++11" \
-DTSPLINE_DIR="/opt/T-SPLINE" \

make

