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
 * Module Name: arith
 * Filename: float.c
 * Creator: Yaokai Liu
 * Create Date: 2026-11-18
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "arith.h"
#include "extbit.h"

#define max(a, b)   ((a) > (b) ? (a) : (b))
#define abs(a)      ((a) > (0) ? (a) : -(a))
#define sgn(a)      ((a) > (0) ?   1 :  -1)


// TODO