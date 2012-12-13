/*
**  Demonstration of PC line drawing characters by David Harmon
*/

#include <stdio.h>

main()
{
      /* compile and run this on a PC, and meditate on the results. */

      puts("ASCII     Decimal       Hex            ASCII Dec Hex\n"
      "-----   -----------   --------         ----- --- ---\n"
      "\332 \302 \277   218 194 191   da c2 bf           \304   196  c4\n"
      "\303 \305 \264   195 197 180   c3 c5 b4           \263   179  b3\n"
      "\300 \301 \331   192 193 217   c0 c1 d9           \315   205  cd\n"
      "                                         \272   186  ba\n"
      "\311 \313 \273   201 203 187   c9 cb bb\n"
      "\314 \316 \271   204 206 185   cc ce b9           \260   176  b0\n"
      "\310 \312 \274   200 202 188   c8 ca bc           \261   177  b1\n"
      "                                         \262   178  b2\n"
      "\326 \322 \267   214 210 183   d6 d2 b7           \333   219  db\n"
      "\307 \327 \266   199 215 182   c7 d7 b6\n"
      "\323 \320 \275   211 208 189   d3 d0 bd           \334   220  dc\n"
      "                                         \335   221  dd\n"
      "\325 \321 \270   213 209 184   d5 d1 b8           \336   222  de\n"
      "\306 \330 \265   198 216 181   c6 d8 b5           \337   223  df\n"
      "\324 \317 \276   212 207 190   d4 cf be\n"
      );

      return 0;
}
