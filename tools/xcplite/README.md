# XCPlite integration

[XCPlite](https://github.com/vectorgrp/XCPlite) (Vector, MIT) is an XCP slave used to test
EcuBus-Pro's XCP master. It speaks **XCP-on-Ethernet only** (UDP or TCP). It has no CAN
transport and no public hook a CAN adapter can register, so this tree does not patch it.

Two ways the same `XcpMaster` reaches it:

1. **UDP or TCP.** `XcpUdpTransport` / `XcpTcpTransport` talk to the stock examples and tests.
2. **Simulate CAN-FD.** `XcpliteCanBridge` (test-only) is a second node on the in-process
   simulate bus. It strips the 4-byte Ethernet header and puts each XCP packet in one CAN-FD
   frame. CONNECT's `MAX_CTO` / `MAX_DTO` are clamped to 64 so the master never builds a
   packet that cannot fit. This is the bus user scripts use through `xcpApi`. It is not the
   TCP JSON-RPC simulate API from later branches.

Sources are fetched by `tools/xcplite/build.sh` (pinned commit, see the script) into
`tools/xcplite/upstream/`, which is gitignored. They are not part of the Electron bundle.
The MIT notice is `tools/xcplite/UPSTREAM_LICENSE`.

## Build and test

```bash
npm run xcplite:build          # or: bash tools/xcplite/build.sh
npm run test -- --run test/xcp
```

Needs a C++ toolchain with libstdc++ (`g++` or `libstdc++-dev`) and `libatomic`.
The vitest file `test/xcp/xcpXcplite.test.ts` builds XCPlite if it is missing, then drives
every XCPlite program that is an XCP slave. `test/xcp/scripts/xcplite-can.ts` is a user
script compiled against `resources/lib/js` (run `npm run worker:js` after worker API changes).

`PROGRAM_START` is rejected by XCPlite (programming and seed/key are compiled out). The
suite records that as unsupported.

`examples/c_demo` checks `XcpEthServerStatus()` before its transmit thread has marked
itself running, then exits. `patches/c_demo-server-status.patch` waits briefly for that
flag. `build.sh` applies the patch after fetching the pinned commit.
