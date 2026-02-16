/**
 * @file instr_div.c
 * @author Eralp Çelebi (eralp.celebi.personal@gmail.com)
 * @brief "DIV AB" instruction.
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
#include <stdio.h>

void DIV(ADUC841_REGISTERS* R, ADUC841_STORAGE* __attribute__((unused)) S) {
        PSW_SET(R->PSW, PSW_CY, 0);
        PSW_SET(R->PSW, PSW_OV, 0);

        if (R->B == 0) {
                PSW_SET(R->PSW, PSW_OV, 1);
        } else {
                R->A /= R->B;
        }

        printf("*(0x%04x) DIV A,B \t #A = 0x%04x, #B = 0x%04x\n", R->PC, R->A, R->B);
        return;
}

MODULE(DIV) { aduc841_bind_dispatch(0x84, DIV); }
