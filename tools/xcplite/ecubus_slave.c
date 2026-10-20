/*
 * Small XCPlite slave used by the EcuBus-Pro XCP tests.
 *
 * It is a normal XCP-on-UDP server (XCPlite has no CAN transport). The test
 * bridges its packets onto the simulate CAN bus. One line on stdout gives the
 * master the addresses of a measurement and a calibration parameter:
 *
 *   ECUBUS_SLAVE {"port":18001,"counter":{"addr":1,"ext":1},"gain":{"addr":2147549184,"ext":0,"seg":1},"event":0}
 *
 * Build: tools/xcplite/build.sh
 */

#define _DEFAULT_SOURCE
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xcplib.h"

static volatile sig_atomic_t running = 1;
/* File scope so absolute addressing can see them. A stack variable is outside the XCP base. */
static uint32_t counter = 0;
static uint32_t gain = 7;

static void on_signal(int sig) {
    (void)sig;
    running = 0;
}

int main(int argc, char **argv) {
    uint16_t port = 18001;
    uint8_t addr[4] = {127, 0, 0, 1};
    tXcpCalSegIndex seg;
    tXcpEventId event;
    uint32_t counter_addr;
    uint8_t counter_ext;
    uint32_t gain_addr;

    if (argc > 1) {
        int parsed = atoi(argv[1]);
        if (parsed <= 0 || parsed > 65535) {
            fprintf(stderr, "port must be 1..65535\n");
            return 2;
        }
        port = (uint16_t)parsed;
    }

    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);
    XcpSetLogLevel(1);

    if (!XcpInit("ecubus_slave", "V1", XCP_MODE_LOCAL)) {
        fprintf(stderr, "XcpInit failed\n");
        return 1;
    }
    if (!XcpEthServerInit(addr, port, false, 64 * 1024)) {
        fprintf(stderr, "XcpEthServerInit failed\n");
        return 1;
    }

    seg = XcpCreateCalSeg("gain", &gain, sizeof(gain));
    if (seg == 0xFFFF) {
        fprintf(stderr, "XcpCreateCalSeg failed\n");
        return 1;
    }
    event = XcpCreateEvent("tick", 1000000, 0);
    counter_addr = ApplXcpGetAddr((const uint8_t *)&counter);
    counter_ext = ApplXcpGetAddrExt((const uint8_t *)&counter);
    /* Default XCPlite segment address: 0x80000000 | (segment << 16) | offset. */
    gain_addr = 0x80000000u + ((uint32_t)seg << 16);

    printf(
        "ECUBUS_SLAVE {\"port\":%u,\"counter\":{\"addr\":%u,\"ext\":%u},"
        "\"gain\":{\"addr\":%u,\"ext\":0,\"seg\":%u},\"event\":%u}\n",
        (unsigned)port, counter_addr, (unsigned)counter_ext, gain_addr, (unsigned)seg, (unsigned)event);
    fflush(stdout);

    while (running) {
        counter++;
        XcpEvent(event);
        usleep(1000);
    }

    XcpEthServerShutdown();
    return 0;
}
