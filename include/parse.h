/**
 * License
 *
 * LMP - A Lite Multiple Precision Arithmetic Library
 * Copyright (C) 2026 Yaokai Liu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Project Name: LMP
 * Module Name: include
 * Filename: parse.h
 * Creator: Yaokai Liu
 * Create Date: 2026-10-19
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#ifndef LMP_PARSE_H
#define LMP_PARSE_H
#include "lmp.h"

#ifndef MP_PARSE_SCALE
#define MP_PARSE_SCALE 4
#endif


uint32_t LMP_parse(const char * input, uint32_t *type, MP_NUMBER *number, const Allocator *allocator);

#endif //LMP_PARSE_H
