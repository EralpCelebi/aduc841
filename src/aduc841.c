/**
 * @file aduc841.c
 * @author Eralp Çelebi (eralp.celebi.personal@gmail.com)
 * @brief Procedures for emulating the ADuC841
 * @version 0.1
 * @date 2026-02-09
 *
 * @copyright Copyright (c) 2026 Eralp Çelebi
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version. This program is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should
 * have received a copy of the GNU General Public License along with this program. If not, see
 * <https://www.gnu.org/licenses/>.
 *
 */

#include <aduc841.h>
#include <modules.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

static ADUC841_STORAGE   S                  = { 0 };
static ADUC841_REGISTERS R                  = { 0 };
static ADUC841_DISPATCH  D[256]             = { 0 };
static ADUC841_MODULE    M[MODULE_CAPACITY] = { 0 };

int aduc841_bind_module(ADUC841_MODULE Source) {
        for (int I = 0; I < MODULE_CAPACITY; I++) {
                if (M[I] != NULL) continue;
                M[I] = Source;
                return 0;
        }

        return -1;
}

int aduc841_bind_dispatch(uint8_t OP, ADUC841_DISPATCH Source) {
        D[OP] = Source;
        return 0;
}

int aduc841_init(void) {
        S.CODE_SIZE = CODE_SIZE;
        S.CODE      = (uint8_t*)malloc(CODE_SIZE);

        S.XDATA_SIZE = XDATA_SIZE;
        S.XDATA      = (uint8_t*)malloc(XDATA_SIZE);

        memset(S.XDATA, 0, XDATA_SIZE);
        memset(S.CODE, 0, CODE_SIZE);

        memset(S.IRAM, 0, 256);
        memset(S.SFR, 0, 128);

        R.SP = 0x07;

        BIND_MODULE(NOP);
        BIND_MODULE(MUL);
        BIND_MODULE(DIV);
        BIND_MODULE(JMP);
        BIND_MODULE(LJMP);
        BIND_MODULE(LCALL);
        BIND_MODULE(RET);
        BIND_MODULE(PUSH);
        BIND_MODULE(POP);

        for (int I = 0; I < MODULE_CAPACITY; I++) {
                if (M[I] == NULL) continue;
                M[I]();
        }

        return 0;
}

int aduc841_load(const char* Location) {
        int   Status = 0;
        FILE* ROM    = fopen(Location, "r");

        if ((Status = fseek(ROM, 0, SEEK_SET))) return Status;

        fread(S.CODE, 1, UINT16_MAX, ROM);
        fclose(ROM);

        return Status;
}

int aduc841_exec(void) {
        uint8_t OP;

        for (;;) {
                OP = S.CODE[R.PC];
                D[OP](&R, &S);
                R.PC++;
        }

        return 0;
}
