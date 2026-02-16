/**
 * @file aduc841.h
 * @author Eralp Çelebi (eralp.celebi.personal@gmail.com)
 * @brief An implementation for emulating the ADuC841.
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

#pragma once

#include <stdint.h>
// static ADUC841_DISPATCH aduc841_dispatch[256] = { 0 };

struct S_ADUC841_REGISTERS {
        uint16_t PC;
        uint16_t DPTR;
        uint8_t  SP, A, B, PSW;
        uint64_t C;
};
typedef struct S_ADUC841_REGISTERS ADUC841_REGISTERS;

struct S_ADUC841_STORAGE {
        uint8_t  IRAM[256];
        uint8_t  SFR[128];
        uint8_t* CODE;
        uint16_t CODE_SIZE;
        uint8_t* XDATA;
        uint16_t XDATA_SIZE;
};
typedef struct S_ADUC841_STORAGE ADUC841_STORAGE;

typedef void (*ADUC841_DISPATCH)(ADUC841_REGISTERS*, ADUC841_STORAGE*);
typedef void (*ADUC841_MODULE)(void);

enum E_ADUC841_ACCESS_PROCEDURE {
        MEMORY_INDIRECT,
        MEMORY_DIRECT,
};

enum E_ADUC841_FLAGS { PSW_P = 0, PSW_RS0 = 3, PSW_RS1 = 4, PSW_OV = 2, PSW_AC = 6, PSW_CY = 7 };

static const uint16_t CODE_SIZE  = UINT16_MAX;
static const uint16_t XDATA_SIZE = UINT16_MAX;

int aduc841_bind_module(ADUC841_MODULE Source);
int aduc841_bind_dispatch(uint8_t OP, ADUC841_DISPATCH Source);
int aduc841_init(void);
int aduc841_load(const char* Location);
int aduc841_exec(void);

static inline uint8_t aduc841_fetch_byte(ADUC841_REGISTERS* R, ADUC841_STORAGE* S) {
        return S->CODE[++R->PC];
}

static inline uint16_t aduc841_fetch_word(ADUC841_REGISTERS* R, ADUC841_STORAGE* S) {
        uint8_t MSB = S->CODE[++R->PC];
        uint8_t LSB = S->CODE[++R->PC];
        return (MSB << 8) | LSB;
}

static inline void aduc841_push_byte(ADUC841_REGISTERS* R, ADUC841_STORAGE* S, uint8_t X) {
        S->IRAM[++R->SP] = X;
}

static inline uint8_t aduc841_pop_byte(ADUC841_REGISTERS* R, ADUC841_STORAGE* S) {
        return S->IRAM[R->SP--];
}

static inline void aduc841_push_word(ADUC841_REGISTERS* R, ADUC841_STORAGE* S, uint16_t X) {
        S->IRAM[++R->SP] = X & 0xFF;
        S->IRAM[++R->SP] = (X & 0xFF00) >> 8;
}

static inline uint16_t aduc841_pop_word(ADUC841_REGISTERS* R, ADUC841_STORAGE* S) {
        uint16_t Destination = 0;
        Destination |= (S->IRAM[R->SP--] << 8) & 0xFF00;
        Destination |= (S->IRAM[R->SP--]) & 0xFF;

        return Destination;
}

#define MODULE_CAPACITY   32
#define MODULE(Name)      void Name##_init(void)
#define BIND_MODULE(Name) aduc841_bind_module(Name##_init)

#define PSW_GET(PSW, Bit) (((PSW) >> (Bit)) & 1u)
#define PSW_SET(PSW, Bit, X)                     \
        do {                                     \
                if (X)                           \
                        (PSW) |= (1u << (Bit));  \
                else                             \
                        (PSW) &= ~(1u << (Bit)); \
        } while (0)
