#ifndef F24_TRANSFER_DIAGNOSTICS_H
#define F24_TRANSFER_DIAGNOSTICS_H

#include <stdint.h>

/* Transfer-only observations; no change to the Custom Transfer wire format.
 * Stages identify the last completed/failed initialization check. Result is
 * the Renesas CAN result, including WAITING for an active-state timeout.
 */
#define TRANSFER_CAN_STAGE_CONFIG  (1U)
#define TRANSFER_CAN_STAGE_STATE   (2U)
#define TRANSFER_CAN_STAGE_START   (3U)
#define TRANSFER_CAN_STAGE_ACTIVE  (4U)
#define TRANSFER_CAN_STAGE_READY   (5U)

typedef struct {
    uint16_t result;
    uint16_t global_status;
    uint16_t channel_status;
    uint8_t stage;
    uint8_t standby;
} transfer_can_diagnostics_t;

void Device_CanGetDiagnostics(transfer_can_diagnostics_t *diagnostics);
/* Main-loop capture of C0ERFL bits0..14, followed by write-zero clearing of
 * only the observed bits. Multiple events can share one latched indication.
 * Bit8 is the raw SERR bit here, not the driver's callback-only TDCV bit.
 */
uint16_t Device_CanPollErrors(void);

#endif
