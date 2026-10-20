#!/usr/bin/env bash
# Fetch a pinned XCPlite commit and build the default-config examples, tests,
# the no_a2l examples, and the EcuBus CAN-bridge slave (ecubus_slave).
# Nothing here is packaged into the Electron app.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
PIN="da324776c9c2274d9a25cba890851ca1be4fc5ac"
SRC="$ROOT/upstream"
URL="https://github.com/vectorgrp/XCPlite.git"

if ! echo 'int main(){return 0;}' | c++ -x c++ -o /tmp/xcplite-cxx-check - >/dev/null 2>&1; then
  echo "XCPlite needs a C++ toolchain with libstdc++ (install g++ or libstdc++-dev)." >&2
  exit 1
fi
rm -f /tmp/xcplite-cxx-check

if [[ ! -d "$SRC/.git" ]]; then
  rm -rf "$SRC"
  mkdir -p "$SRC"
  git -C "$SRC" init
  git -C "$SRC" remote add origin "$URL"
fi

head="$(git -C "$SRC" rev-parse HEAD 2>/dev/null || true)"
if [[ "$head" != "$PIN" ]]; then
  git -C "$SRC" fetch --depth 1 origin "$PIN"
  git -C "$SRC" checkout --force FETCH_HEAD
fi

# c_demo checks XcpEthServerStatus() on the first loop iteration, before the
# transmit thread has set its running flag, and then exits 0. Keep the slave up.
PATCH="$ROOT/patches/c_demo-server-status.patch"
if ! grep -q 'transmit thread sets its running' "$SRC/examples/c_demo/src/main.c"; then
  git -C "$SRC" apply "$PATCH"
fi

build_tree() {
  local dir="$1"
  shift
  if [[ ! -x "$dir/hello_xcp" && ! -x "$dir/no_a2l_demo" ]]; then
    cmake -B "$dir" -S "$SRC" -DCMAKE_BUILD_TYPE=Release "$@"
    cmake --build "$dir" --parallel "$(nproc 2>/dev/null || echo 4)"
  fi
}

build_tree "$SRC/build" -DXCPLITE_BUILD_TESTS=ON -DXCPLITE_BUILD_EXAMPLES=ON
build_tree "$SRC/build-no_a2l" -DXCPLITE_CONFIGURATION=no_a2l -DXCPLITE_BUILD_EXAMPLES=ON
if [[ "$SRC/examples/c_demo/src/main.c" -nt "$SRC/build/c_demo" ]]; then
  cmake --build "$SRC/build" --target c_demo --parallel "$(nproc 2>/dev/null || echo 4)"
fi

cc -O2 -std=c11 -Wall -DXCPLITE_CONFIGURATION=\"default\" \
  -I"$SRC/inc" -I"$SRC/src" \
  "$ROOT/ecubus_slave.c" "$SRC/build/libxcplite.a" -pthread -lm -latomic \
  -o "$SRC/build/ecubus_slave"

# sr_a2l_test is in XCPlite's test/ tree but is not a CMake target.
# It is a hello_xcp-style UDP server, so build it the same way.
cc -O2 -std=c11 -DXCPLITE_CONFIGURATION=\"default\" \
  -I"$SRC/inc" -I"$SRC/src" \
  "$SRC/test/sr_a2l_test/src/main.c" "$SRC/build/libxcplite.a" -pthread -lm -latomic \
  -o "$SRC/build/sr_a2l_test"

echo "XCPlite $PIN ready:"
echo "  $SRC/build/ecubus_slave"
echo "  $SRC/build/hello_xcp"
echo "  $SRC/build-no_a2l/no_a2l_demo"
