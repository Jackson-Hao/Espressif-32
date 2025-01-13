#include "version.h"
#include <stdio.h>

void vPrintTag(void) {
    printf("\n\n\n");
    printf("EEEEEEEEEE  SSSSSSSS  PPPPPPPPP  RRRRRRRR  EEEEEEEE  SSSSSSSS  SSSSSSSS  II  FFFFFFFffF\n");
    printf("EEEEEEEEEE  SSSSSSSS  PPPPPPPPP  RRRRRRRR  EEEEEEEE  SSSSSSSS  SSSSSSSS  II  FFFFFFffFF\n");
    printf("EEEE        SS        PP    PPP  RR    RR  EEEE      SS        SS        II  FF\n");
    printf("EEEEEEEE    SSSSSSSS  PPPPPPPPP  RRRRRRRR  EEEEEEEE  SSSSSSSS  SSSSSSSS  II  FFFFffFF\n");
    printf("EEEEEEEE    SSSSSSSS  PPPPPPPPP  RRRRRRRR  EEEEEEEE  SSSSSSSS  SSSSSSSS  II  FFffFFFF\n");
    printf("EEEE              SS  PP         RR    RR  EEEE            SS        SS  II  FF\n");
    printf("EEEEEEEEEE  SSSSSSSS  PP         RR     RR EEEEEEEE  SSSSSSSS  SSSSSSSS  II  FF\n");
    printf("EEEEEEEEEE  SSSSSSSS  PP         RR     RR EEEEEEEE  SSSSSSSS  SSSSSSSS  II  FF\n\n");
    
    printf("Version: %s\t\t", APP_VERSION);
    printf("Internal Code: %s\n", INTERNAL_CODE);
    printf("\n");
    printf("-----------------------------------------------------------------------------------------\n");
}