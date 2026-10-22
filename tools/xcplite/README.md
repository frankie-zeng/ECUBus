# XCPlite integration

[XCPlite](https://github.com/vectorgrp/XCPlite) (Vector, MIT) is an XCP slave used to test
EcuBus-Pro's XCP master. It speaks **XCP-on-Ethernet only** (UDP or TCP). It has no CAN
transport and no public hook a CAN adapter can register, so this tree does not patch it.

Two ways the same `XcpMaster` reaches it:

1. **UDP or TCP.** `XcpUdpTransport` / `XcpTcpTransport` talk to the stock examples and tests.
2. **Simulate CAN-FD over JSON-RPC.** The XCP master (and a GUI/worker script) stays on the
   project simulate device. `XcpliteCanBridge` (test-only) does not open `SIMULATE_CAN` itself.
   It connects to the TCP JSON-RPC listener that starts when that device is open
   (`setProjectSimulateCount`, default `127.0.0.1:17320`) and joins a free handle with
   `Can.Init` / `Can.SetControllerMode` / `Can.Write` / `Can.MainFunction_Read`. Each XCP
   packet is one CAN-FD frame. CONNECT's `MAX_CTO` / `MAX_DTO` are clamped to 64.

Addresses for the suite come from `test/xcp/a2lParse.ts`. That reader is test-only.
The application does not parse A2L files or load a seed & key DLL. `XcpGetSeed` and
`XcpUnlock` remain as plain protocol commands.

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
