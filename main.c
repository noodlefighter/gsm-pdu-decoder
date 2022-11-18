#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "Pdu.h"

char pdu_str[] = "0891689102820005F0240D91688162098008F2000822118190813223085E7F897F5E7F7535";
int main(void)
{
    PDU_DESC pduDesc = {0};
    UINT8 pduError;
    if (!DecodePduData(pdu_str, &pduDesc, &pduError)) {
        printf("pdu failed\n");
        return 1;
    }
    printf("pdu done\n");

    printf("phoneNum: %s\n", pduDesc.phoneAddr);
    printf("usrDataLen: %d\n", (int)pduDesc.usrDataLen);
    const char *fmt;
    switch (pduDesc.usrDataFormat) {
    case GSM_7BIT:
        fmt = "GSM_7BIT";
        break;
    case ANSI_8BIT:
        fmt = "ANSI_8BIT";
        break;
    case UCS2_16BIT:
        fmt = "UCS2_16BIT";
        break;
    default:
        fmt = "????";
        break;
    }
    printf("usrDataFormat: %s\n", fmt);

    char usrDataAscii[100] = {0};
    extern UINT16 HexBuf2AsciiBuf(UINT8 *pHexBuf, UINT8 hexBufLen, char *pAsciiStrng);
    UINT16 n = HexBuf2AsciiBuf(pduDesc.usrData, pduDesc.usrDataLen, usrDataAscii);
    assert(n == pduDesc.usrDataLen*2);
    printf("usrData(hex): %s\n", usrDataAscii);
    printf("decode string:\n");

    char cmd[256];
    sprintf(cmd, "echo %s | xxd -r -p | iconv -f UCS-2BE -t UTF-8", usrDataAscii);
    system(cmd);
    return 0;
}
