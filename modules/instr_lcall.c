/**
 * @file instr_jmp.c
 * @author Eralp Çelebi (eralp.celebi.personal@gmail.com)
 * @brief "JMP @A+DPTR" instruction.
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

void LCALL(ADUC841_REGISTERS* R, ADUC841_STORAGE* S) {
        uint16_t Destination = aduc841_fetch_word(R, S);

        aduc841_push_word(R, S, R->PC + 1);

        printf("*(0x%04x) LCALL *(0x%04x) \n", R->PC - 2, Destination);

        R->PC = Destination - 1;

        return;
}

MODULE(LCALL) { aduc841_bind_dispatch(0x12, LCALL); }
