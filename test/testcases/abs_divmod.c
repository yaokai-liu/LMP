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
 * Module Name: test/testcases
 * Filename: abs_divmod.c
 * Creator: Yaokai Liu
 * Create Date: 2026-10-18
 * Copyright (c) 2026 Yaokai Liu. All rights reserved.
 **/

#include "test.h"
#include "allocator.h"


typedef struct {
  const char       *name;
  const lmp_limb_t *a;
  const lmp_limb_t *b;
  const lmp_limb_t *eq;
  const lmp_limb_t *er;
  int32_t           a_len;
  int32_t           b_len;
  int32_t           eq_len;
  int32_t           er_len;
} DivmodCase;

static void run_abs_divmod_case(const DivmodCase *c)
{
  lmp_limb_t a_limbs[64] = {};
  lmp_limb_t b_limbs[64] = {};
  lmp_limb_t q_limbs[64] = {};
  STDAllocator.memcpy(a_limbs, c->a, LMP_LIMB_SIZE * c->a_len);
  STDAllocator.memcpy(b_limbs, c->b, LMP_LIMB_SIZE * c->b_len);

  MP_INT A = { .alloc = 0, .size = c->a_len, .limbs = a_limbs };
  MP_INT B = { .alloc = 0, .size = c->b_len, .limbs = b_limbs };
  MP_INT Q = { .alloc = 0, .size = 0,        .limbs = q_limbs };

  MP_INT_abs_divmod(&A, &B, &Q, &STDAllocator);
  MP_INT_clamp(&A);
  MP_INT_clamp(&Q);

  ck_assert_msg(abs(Q.size) == c->eq_len,
                "%s: quotient size %d != %d", c->name, Q.size, c->eq_len);
  for (int32_t i = 0; i < c->eq_len; i++) {
    ck_assert_msg(Q.limbs[i] == c->eq[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
                  (unsigned long long)Q.limbs[i],
                  (unsigned long long)c->eq[i]);
  }
  ck_assert_msg(abs(A.size) == c->er_len,
                "%s: remainder size %d != %d", c->name, A.size, c->er_len);
  for (int32_t i = 0; i < c->er_len; i++) {
    ck_assert_msg(A.limbs[i] == c->er[i],
                  "%s: limb[%d] %llu != %llu", c->name, i,
                  (unsigned long long)A.limbs[i],
                  (unsigned long long)c->er[i]);
  }
}

static const lmp_limb_t a1[] = { 7 };
static const lmp_limb_t b1[] = { 3 };
static const lmp_limb_t q1[] = { 2 };
static const lmp_limb_t r1[] = { 1 };

static const lmp_limb_t a2[] = { 5 };
static const lmp_limb_t b2[] = { 9 };
static const lmp_limb_t q2[] = { 0 };
static const lmp_limb_t r2[] = { 5 };

static const lmp_limb_t a3[] = { 0xFFFFFFFFFFFFFFFFULL, 0x1 };
static const lmp_limb_t b3[] = { 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t q3[] = { 2 };
static const lmp_limb_t r3[] = { 1 };

static const lmp_limb_t a4[] = { 0x1111111111111111ULL,
                                 0x2222222222222222ULL,
                                 0x3333333333333333ULL,
                                 0x4444444444444444ULL };
static const lmp_limb_t b4[] = { 0x5555555555555555ULL,
                                 0x6666666666666666ULL,
                                 0x7777777777777777ULL,
                                 0x8888888888888888ULL };
static const lmp_limb_t q4[] = { 0 };

static const lmp_limb_t r4[] = { 0x1111111111111111ULL,
                                 0x2222222222222222ULL,
                                 0x3333333333333333ULL,
                                 0x4444444444444444ULL };


static const lmp_limb_t a5[] = { 0xFFFFFFFFFFFFFFFFULL,
                                 0xFFFFFFFFFFFFFFFFULL,
                                 0xFFFFFFFFFFFFFFFFULL,
                                 0xFFFFFFFFFFFFFFFFULL };
static const lmp_limb_t b5[] = { 2 };
static const lmp_limb_t q5[] = { 0xFFFFFFFFFFFFFFFFULL,
                                 0xFFFFFFFFFFFFFFFFULL,
                                 0xFFFFFFFFFFFFFFFFULL,
                                 0x7FFFFFFFFFFFFFFFULL };
static const lmp_limb_t r5[] = { 1 };

static const lmp_limb_t a6[] = { 0x0000000000000001ULL,
                                 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x8000000000000000ULL };
static const lmp_limb_t b6[] = { 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x8000000000000000ULL };
static const lmp_limb_t q6[] = { 1 };
static const lmp_limb_t r6[] = { 1 };

static const lmp_limb_t a7[] = { 0 };
static const lmp_limb_t b7[] = { 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x8000000000000000ULL };
static const lmp_limb_t q7[] = { 0 };
static const lmp_limb_t r7[] = { 0 };

static const lmp_limb_t a8[] = { 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x8000000000000000ULL };
static const lmp_limb_t b8[] = { 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x8000000000000000ULL };
static const lmp_limb_t q8[] = { 1 };
static const lmp_limb_t r8[] = { 0 };

static const lmp_limb_t a9[] = { 0x0000000000000002ULL,
                                 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x3000000000000000ULL };
static const lmp_limb_t b9[] = { 3 };
static const lmp_limb_t q9[] = { 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x0000000000000000ULL,
                                 0x1000000000000000ULL,};
static const lmp_limb_t r9[] = { 2 };


static const lmp_limb_t a10[] = { 0x70b6bf93f268294ULL, 0x5c381d8e7adaab5aULL, 0x9044ded70f26dbdbULL, 0x3b991fe96b6f3f8aULL };
static const lmp_limb_t b10[] = { 0x2e97b47026736677ULL, 0x3e48e5b91218669cULL, 0x81237e1b5786a8c9ULL, 0x195afad736d37232ULL, 0xb6d2b1c91032e1b4ULL, 0x504f16760c3fbe71ULL, 0x26c1fa95a178b4ceULL, 0xd303bd8ac4e40062ULL };
static const lmp_limb_t q10[] = { 0 };
static const lmp_limb_t r10[] = { 0x70b6bf93f268294ULL, 0x5c381d8e7adaab5aULL, 0x9044ded70f26dbdbULL, 0x3b991fe96b6f3f8aULL };

static const lmp_limb_t a11[] = { 0x4e0169efb5a69c0ULL, 0xb7dcfc3ea4845746ULL, 0xf35cb355e6f4b199ULL, 0xbef6c7bf95ecbb94ULL, 0xb53a08cdb0ff598bULL, 0x78649c7786e0b20cULL, 0x567ad3c4ab04decdULL, 0x8e3129516d94d279ULL };
static const lmp_limb_t b11[] = { 0x2152029125713208ULL, 0x905c99d06aef9524ULL, 0x4baf52ae82ab6e5fULL, 0xd573bbeee0cb8fc8ULL };
static const lmp_limb_t q11[] = { 0xcf3eb6fd707c4f3aULL, 0xed17df246570a292ULL, 0x603a594e722f5fa7ULL, 0xaa89177c85c0c326ULL };
static const lmp_limb_t r11[] = { 0x5a70ec8ed5649bf0ULL, 0x584b62892f1d7676ULL, 0xdff9d626d57a8951ULL, 0x4cf200fffc65348aULL };


static const lmp_limb_t a12[] = { 0x35e20cc15eb7df67ULL, 0x8883f7f96445e666ULL, 0x96f7670fef921573ULL, 0x7ed411be471ef933ULL, 0x66728d11ee44a29aULL, 0x950bc53b7cdee787ULL, 0x199fc7c9b4d8a056ULL, 0xba0f4a311726315dULL };
static const lmp_limb_t b12[] = { 0xe372321cdf3070c7ULL, 0xc5f57be42e378965ULL, 0x2abc9569477060d2ULL, 0x275fc2f94a1fa32bULL };
static const lmp_limb_t q12[] = { 0xaa013b1f2f2314f3ULL, 0xa56c47c24c4fcc83ULL, 0xf7c9e5a854436249ULL, 0xb9b6501513fe14b4ULL, 0x4ULL };
static const lmp_limb_t r12[] = { 0x76034c7cc6b84682ULL, 0x30d49f9bc55d6841ULL, 0x2bdd46ca83e6a1b7ULL, 0x14021b8841ba570bULL };


static const lmp_limb_t a13[] = { 0x5d04ed4e23a57f06ULL, 0x37da4714f493577aULL, 0x3846dc5b83123775ULL, 0xecbd5ed0107648fULL, 0xc9a691fa3945b034ULL, 0x2af4db09d1bf52b7ULL, 0x152648940b09d9d5ULL, 0x29e8779ba19daffeULL };
static const lmp_limb_t b13[] = { 0x5ebb56e88612b41bULL, 0x1da9adf5c78e31d6ULL, 0xf0eb11a62097c1d9ULL, 0x4bf5a4141a5c65b2ULL };
static const lmp_limb_t q13[] = { 0x40e8dca6f2a0ad03ULL, 0xabbe674100de174fULL, 0x7272da87052ca29fULL, 0x8d3d3f0a263187e0ULL };
static const lmp_limb_t r13[] = { 0x8ba64e85d6d723b5ULL, 0x148c35912cec3966ULL, 0xeced4ecb9d40821fULL, 0x47d83dc3d3eb5a0fULL };


static const lmp_limb_t a14[] = { 0x8508ab04267caa6eULL, 0xe7be2096e3fb17caULL, 0xefec53b9341c4810ULL, 0x49f221ee8a722c1fULL, 0xaabb9ecdf0b222a2ULL, 0x8395d10c600d6b11ULL, 0x7d2270e3b9492440ULL, 0x15e999216178c19cULL };
static const lmp_limb_t b14[] = { 0x9648af99cbfbd887ULL, 0x6d5143d210863554ULL, 0x2616800398c63a54ULL, 0xabdc6325c954e04cULL };
static const lmp_limb_t q14[] = { 0xa5d90749037ba2ebULL, 0xf920699dac75fc01ULL, 0x50fe50718f5198a1ULL, 0x20a3ed74edba38c3ULL };
static const lmp_limb_t r14[] = { 0xc9bbc0b6e96a7881ULL, 0x7b30fd65fd6f4e0fULL, 0xc21f6f6f9618af9dULL, 0x5d6a4350738c5b8eULL };


static const lmp_limb_t a15[] = { 0xcb776ebd100b6c22ULL, 0x40fbaa8824e64029ULL, 0x91de59a7c487f7c8ULL, 0xe473a3071f6c0d29ULL, 0x73a4195639ceabe4ULL, 0xc0c07d93a176956fULL, 0xb9d94c7b61401aadULL, 0x3b7883fb9286b0adULL };
static const lmp_limb_t b15[] = { 0x448592a308e78ed3ULL, 0x68a498a6f0156ebaULL, 0x3c945fedf24ec9c8ULL, 0xa0867b992be0d062ULL };
static const lmp_limb_t q15[] = { 0xdd00cfc1a96da5d8ULL, 0xe458e0b5662ffbebULL, 0x64d1c2fffc38b123ULL, 0x5ed7823830c958fdULL };
static const lmp_limb_t r15[] = { 0xb8e5ae7c32c5eb1aULL, 0xde35884bbc9c260eULL, 0x68488ebf14c7a4faULL, 0x1088d778a023df99ULL };

static const lmp_limb_t a16[] = { 0xb81efbaa0b36d5a9ULL, 0xa5003694dcec519aULL, 0x1372eaec79424f63ULL, 0x77a116b813ce0750ULL, 0x9379ae3c7a62c2caULL, 0xa848a5c68a0c623ULL, 0x11cbe13404ff8fd2ULL, 0x1c452f8cb9241c90ULL };
static const lmp_limb_t b16[] = { 0x411974800e360629ULL, 0xe501f983a43c0b88ULL, 0xcef0c0f2892dea6ULL, 0x3d5a1a15847b8c22ULL, 0xb1806761f9a81c24ULL, 0xb99ff9137c02c9d2ULL, 0x5905d0b1f33f6b82ULL, 0x57dfa49c1be89ULL };
static const lmp_limb_t q16[] = { 0x525ULL };
static const lmp_limb_t r16[] = { 0xd02aa4e0ef4924bcULL, 0x81d79458f404fda3ULL, 0x89a9e0f0bdaedecbULL, 0xd718e60584361c23ULL, 0x69e5d3331c89fc5aULL, 0x1688291f6e487d38ULL, 0x16e13dbb9fbb784dULL, 0x416ef476ae3f9ULL };


static const lmp_limb_t a17[] = { 0x6e9a51cd3e55021eULL, 0xbd760e4d7731ece4ULL, 0xce271a4211d4ef1ULL, 0x4246c23af5062a7bULL, 0xae197eccfc4471afULL, 0x6fbb781ab8f950c1ULL, 0x1e085f02c18565c6ULL, 0xa0cbcd9a05df1b7cULL };
static const lmp_limb_t b17[] = { 0x793181a5dbea7bd0ULL, 0xe00b08b540c75ba1ULL, 0x1c15e38ead0e41eeULL, 0x3aafbab18ff11a4ULL, 0x8fe5ef2f380cade6ULL, 0x773f42ab02c341efULL, 0x1674f284e2eaf5ebULL, 0x2d07a9042f8deULL };
static const lmp_limb_t q17[] = { 0x3922ULL };
static const lmp_limb_t r17[] = { 0x50292bced39f407eULL, 0x870e8ace7950cd76ULL, 0x724f7428f88a5d55ULL, 0xb3883cf0d838486eULL, 0x5f4e370cb7dd2251ULL, 0x837c89c6e15c37e6ULL, 0x188092d64d8f4ef3ULL, 0xd333f3909afdULL };


static const lmp_limb_t a18[] = { 0x21fbc24d77c63046ULL, 0xe1caca1199dda52aULL, 0x367c2dbd0baa4b18ULL, 0xf492992a6f33d4c7ULL, 0x6a5ec0a17fa00cf9ULL, 0x75f1b09cc51001a2ULL, 0x1f38d9524ea2c82bULL, 0xcce2fe22caeb51aeULL };
static const lmp_limb_t b18[] = { 0x6fce3aadb2a5e059ULL, 0xb12daf88cb544c3aULL, 0xd028cadcf94f504ULL, 0x3576f8aa08fab925ULL, 0xe89f1274a5e53df2ULL, 0xbe57c9317acf934fULL, 0xe8b7ace4ae19b22aULL, 0xce47f25d88075ULL };
static const lmp_limb_t q18[] = { 0xfe4ULL };
static const lmp_limb_t r18[] = { 0x78df5222a3e52a02ULL, 0x67d1705292525491ULL, 0x79f8b3c670a4cc89ULL, 0x5e0b2b33bf0dc404ULL, 0xea9d7b0550d3b020ULL, 0xcaf89a593a8f1fd5ULL, 0x18817773be4f98f2ULL, 0x409ad66920008ULL };


static const lmp_limb_t a19[] = { 0xbfd3f529ef12f5efULL, 0x89b42043085035d0ULL, 0x5fb4e13ed8936857ULL, 0x8f77c32f87cb35f8ULL, 0xe117efe8661a434cULL, 0xb66d82ffa60d158fULL, 0x827b4cdfbd39076dULL, 0x13465ad4e9aa8752ULL };
static const lmp_limb_t b19[] = { 0x2e416669bb8e3e5cULL, 0xf7de75a350e307c1ULL, 0xce0433189af41abaULL, 0xd9d082c2f7484c09ULL, 0xe6db8302afeb9f1ULL, 0x4e6c7365c33ae612ULL, 0x18676465788634a6ULL, 0xa09370376878bULL };
static const lmp_limb_t q19[] = { 0x1ebULL };
static const lmp_limb_t r19[] = { 0x864885f35415b7bULL, 0x22088006e4e0564dULL, 0x3da6e10da66423beULL, 0xcc8cf73f40215f2aULL, 0x34a7ab85ef8ba06fULL, 0x4c6c2ed23415d0eeULL, 0xb42dbe4193d20c75ULL, 0x6ae5145548f8aULL };

static const lmp_limb_t a20[] = { 0xfa1ff51c4271d66dULL, 0xfd8799adb80184a4ULL, 0x20deebff8fdbc1faULL, 0xf6450238eb9bc99eULL, 0x5cd64b22090846edULL, 0xfb4e9a94cd66c112ULL, 0xe6df67e02ad4179dULL, 0x2e7768bcd84844adULL };
static const lmp_limb_t b20[] = { 0x39509fc63d0f5485ULL, 0xb3afe2e942856305ULL, 0xf824b640d4efbe58ULL, 0xcb72c8b736e4c784ULL, 0x495525ae61811e1aULL, 0x2f57792a13e87d5bULL, 0x147a472fb8f0d5fULL, 0xa0aa40bf44c0eULL };
static const lmp_limb_t q20[] = { 0x4a0ULL };
static const lmp_limb_t r20[] = { 0xe53d0041db8aef4dULL, 0xf00e22da0f178c7bULL, 0x77140026bb0b67bbULL, 0x364b2db098102a2ULL, 0x3308049f13dd0b01ULL, 0x6be37f8ba22fadfULL, 0xfb86d414b5363f63ULL, 0x632058e6883e7ULL };

static const DivmodCase abs_divmod_cases[] = {
  { "7/3",     a1, b1, q1, r1, 1, 1, 1, 1 },
  { "5/9",     a2, b2, q2, r2, 1, 1, 0, 1 },
  { "(2^64*1+...)/...", a3, b3, q3, r3, 2, 1, 1, 1 },
  { "4limb a<b", a4, b4, q4, r4, 4, 4, 0, 4 },
  { "4limb/1limb 2^256-1 / 2", a5, b5, q5, r5, 4, 1, 4, 1 },
  { "a=b+1", a6, b6, q6, r6, 4, 4, 1, 1 },
  { "a=0", a7, b7, q7, r7, 0, 4, 0, 0 },
  { "a=b", a8, b8, q8, r8, 4, 4, 1, 0 },
  { "a=b * 2^319 + 2", a9, b9, q9, r9, 4, 1, 4, 1 },
  {"a < b", a10, b10, q10, r10, 4, 8, 0, 4},

  {"random_case_11", a11, b11, q11, r11, 8, 4, 4, 4},
  {"random_case_12", a12, b12, q12, r12, 8, 4, 5, 4},
  {"random_case_13", a13, b13, q13, r13, 8, 4, 4, 4},
  {"random_case_14", a14, b14, q14, r14, 8, 4, 4, 4},
  {"random_case_15", a15, b15, q15, r15, 8, 4, 4, 4},
  {"random_case_16", a16, b16, q16, r16, 8, 8, 1, 8},
  {"random_case_17", a17, b17, q17, r17, 8, 8, 1, 8},
  {"random_case_18", a18, b18, q18, r18, 8, 8, 1, 8},
  {"random_case_19", a19, b19, q19, r19, 8, 8, 1, 8},
  {"random_case_20", a20, b20, q20, r20, 8, 8, 1, 8}

};

START_TEST(test_abs_divmod)
{
  run_abs_divmod_case(&abs_divmod_cases[_i]);
}
END_TEST

Suite *abs_divmod_suite()
{
  Suite *s = suite_create("abs_divmod");
  TCase *tc = tcase_create("abs_divmod");

  constexpr int n = sizeof(abs_divmod_cases)/sizeof(DivmodCase);
  tcase_add_loop_test(tc, test_abs_divmod, 0, n);

  suite_add_tcase(s, tc);
  return s;
}
