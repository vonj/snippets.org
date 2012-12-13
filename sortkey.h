#ifndef SORTKEY_H_
#define SORTKEY_H_

/**************************************************************************
* @(#)sortkey.h
* @(#)      This file contains examples of two sort order tables:
* @(#)            CaseMatch & CaseIgnore
* @(#)      These tables are used for sorting, comparing and case converting
* @(#)      using non-ASCII charactersets
*
*  Each table containes 256 elements. The sort value of a charater is
*  defined as the value in the characters position in the table.
*
*  EXAMPLE:
*     The character ‚ is to be sorted as an ordinary e
*     The ‚ has the ASCII value 130 (82h). On position 130 in the
*     table you will find an e.
*
*     -         |130      |140   <- ASCII decimal value
*     -       €‚ƒ„…†‡ˆ‰Š‹Œ  <- Standard ASCII sort sequence
*            "Cyea{a}ceeeiii[]E  <- Local sort sequence
*
*
*  Please note the C syntax using \ in combination with ' " and \ :
*           \", \', \\
*
*
*=========================================================================
*
*  Danish/Norwegan sort order:
*     special mapping:
*               =   y        š     =  Y
*         w      =   v        W     =  V
*         ‘ „    =   {        ’    =  [
*         › ”    =   |         ”   =  \\
*         †      =   }             =  ]
*
*     diacritical charaters to ordinary characters:
*         ‚ Š ˆ ‰  = e
*           … ƒ    = a   (note „ = ‘ sorted as { )
*         ¡  Œ ‹  = i
*         ¢ • “    = o   (note ” = › sorted as } )
*         £ — –    = u   (note  = y)
*
*  Sorting cent and yen characters might sound strange to you, but the
*  Norwegan and danish character set contains tree extra characters:
*
*     ‘ ’  [î:] a combination of a & e
*     ›   [íÁ:] (o slash like í ) a combination of o & e
*     †   [†:] a sort of double a
*
*  These characters follow A-Z in order ’, ,  and a-z in order ‘, ›, †
*  I swedish the characters „ & ” are used instead of ‘ and ›. Their sort
*  order is A-Z, , , ™
*
*
*
*=========================================================================
*
* The following examples of other sort orders are picked up from:
*        TDE, the Thomson-Davis Editor
*                 Version 2.10
*              November 13, 1992
*                 Frank Davis
*
* Frank Davis claimes, that Pierre Jelenc is the creator of these examples.
*
*
* Standard ASCII sort sequence:

!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_'abcdefghijkl
mnopqrstuvwxyz{|}~€‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œŸ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸
¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ

*
*  English/French/Esperanto sort order:  map accents to unaccented
*
***************************     ******
* CaseMatch

!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_'abcdefghijkl
mnopqrstuvwxyz{|}~CueaaaaceeeiiiAAEaAooouuyOU›œŸaiounN

*
* CaseIgnore

!"#$%&'()*+,-./0123456789:;<=>?@abcdefghijklmnopqrstuvwxyz[\]^_'abcdefghijkl
mnopqrstuvwxyz{|}~cueaaaaceeeiiiaaeaaooouuyou›œŸaiounn

*
*
*  Swedish/Finnish sort order:  map  = y, w = v, and the accents
*
* CaseMatch

!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVVXYZ[\]^_'abcdefghijkl
mnopqrstuvvxyz{|}~CyeaaaaceeeiiiAAEaAooouuyOY›œŸaiounN

*
* CaseIgnore

!"#$%&'()*+,-./0123456789:;<=>?@abcdefghijklmnopqrstuvvxyz[\]^_'abcdefghijkl
mnopqrstuvvxyz{|}~cyeaaaaceeeiiiaaeaaooouuyoy›œŸaiounn

*
*
*  German sort order:  slide everything down for „, ”, , á, and the accents
*  see asterisks for changes ====>                                   *
*     *           *             *   * *          *  *         *     *    **
*
* Standard ASCII sort sequence:

!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_'abcdefghijkl
mnopqrstuvwxyz{|}~€‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œŸ ¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸
¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ

*
* CaseMatch

!"#$%&'()*+,-./0123456789:;<=>?@ACDEFGHIJKLMNOPRSTUVWYZ[\]^_'abcdfghijklmnop
qrsuvwxz{}~€‚ƒ„…†D|ideddgiiimmmBAFdAsts{{€QXœŸ dms{rO§¨©ª«¬­®¯°±²³´µ¶·¸¹
º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáy

*  see asterisks for changes ====>                                   *
*     *           *             *   * *          *  *         *     *    **
*
* CaseIgnore

!"#$%&'()*+,-./0123456789:;<=>?@dfghijklmnopqrsuvwxz{}~€^_'abcdfghijklmnop
qrsuvwxz{}~€‚ƒ„…†g|ideddgiiimmmedidAsts{{€t|œŸ dms{rr§¨©ª«¬­®¯°±²³´µ¶·¸¹
º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáy

*  see asterisks for changes ====>                                   *
*     *           *             *   * *          *  *         *     *    **
*
*
******************************************************************************
*@(#)1993-06-10/Erik Bachmann
******************************************************************************/

const unsigned char *CaseMatch  =
/*
Standard ASCII sort sequence:
-        |00                 |05                 |10                 |15
-        \x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F
*/      "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
/*
-        |16             |20                 |25                 |30
-        \x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F
*/      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"

/*
-       |32       |40       |50       |60       |70
-        !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO
*/     " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO"
/*
-       |80       |90         |100      |110      |120
-       PQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~
*/     "PQRSTUVVXYZ[\\]^_\'abcdefghijklmnopqrstuvvxyz{|}~"
/*
-         |130      |140      |150      |160      |170
-       €‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜ ™š›œ Ÿ ¡¢£¤¥¦§¨©ª«¬­®¯
*/     "Cyea{a}ceeeiii[]E{[o|ouuy\\Y|œ\\ŸaiounN¦§¨©ª«¬­®¯"
/*
-           |180      |190      |200      |210      |220
-       °±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß
*/     "°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß"
/*
-             |230      |240      |250
-       àáâãäåæçèéêëìíîïğñòóôõö÷øùúûüış\xFF
*/     "àáâãäåæçèéêëìíîïğñòóôõö÷øùúûüış\xFF";


/*====================================================================*/

const unsigned char *CaseIgnore =
/*
Standard ASCII sort sequence:
-        |00                 |05                 |10                 |15
-        \x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F
*/      "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
/*
-        |16             |20                 |25                 |30
-        \x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F
*/      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"

/*
-       |32       |40       |50       |60       |70
-        !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO
*/     " !\"#$%&\'()*+,-./0123456789:;<=>?@abcdefghijklmno"
/*
-       |80       |90         |100      |110      |120
-       PQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~
*/     "pqrstuvvxyz[\\]^_\'abcdefghijklmnopqrstuvvxyz{|}~"
/*
-         |130      |140      |150      |160      |170
-       €‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜ ™š›œ Ÿ ¡¢£¤¥¦§¨©ª«¬­®¯
*/     "cyea{a}ceeeiii{}e{{o|ouuy|y|œ|Ÿaiounn¦§¨©ª«¬­®¯"
/*
-           |180      |190      |200      |210      |220
-       °±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß
*/     "°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß"
/*
-             |230      |240      |250
-       àáâãäåæçèéêëìíîïğñòóôõö÷øùúûüış\xFF
*/     "àáâãäåæçèéêëìíîïğñòóôõö÷øùúûüış\xFF";

/*====================================================================*/

const char *CaseUpper  =
/*
Standard ASCII sort sequence:
-        |00                 |05                 |10                 |15
-        \x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F
*/      "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
/*
-        |16             |20                 |25                 |30
-        \x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F
*/      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"

/*
-       |32       |40       |50       |60       |70
-        !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO
*/     " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO"
/*
-       |80       |90         |100      |110      |120
-       PQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~
*/     "PQRSTUVWXYZ[\\]^_\'ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~"
/*
-         |130      |140      |150    |160      |170
-       €‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œŸ ¡¢£¤¥¦§¨©ª«¬­®¯
*/     "€šÒ·€ÒÓÔØ×Ş’’â™ãêëY™šœŸµÖàé¥¥¦§¨©ª«¬­®¯"
/*
-           |180      |190      |200      |210      |220
-       °±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß
*/     "°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÇÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß"
/*
-             |230      |240      |250
-       àáâãäåæçèéêëìíîïğñòóôõö÷øùúûüış\xFF
*/     "àáâãååæçèéêëííîïğñòóôõö÷øùúûüış\xFF";


/*====================================================================*/


const char *CaseLower  =
/*
Standard ASCII sort sequence:
-        |00                 |05                 |10                 |15
-        \x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F
*/      "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
/*
-        |16             |20                 |25                 |30
-        \x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F
*/      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"

/*
-       |32       |40       |50       |60       |70
-        !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO
*/     " !\"#$%&\'()*+,-./0123456789:;<=>?@abcdefghijklmno"
/*
-       |80       |90         |100      |110      |120
-       PQRSTUVWXYZ[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~
*/     "pqrstuvwxyz[\\]^_\'abcdefghijklmnopqrstuvwxyz{|}~"
/*
-         |130      |140      |150    |160      |170
-       €‚ƒ„…†‡ˆ‰Š‹Œ‘’“”•–—˜™š›œŸ ¡¢£¤¥¦§¨©ª«¬­®¯
*/     "‡‚ƒ„…†‡ˆ‰Š‹Œ„†‚‘‘“”•–—˜”›œ›Ÿ ¡¢£¤¤¦§¨©ª«¬­®¯"
/*
-           |180      |190      |200      |210      |220
-       °±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß
*/     "°±²³´ ƒ…¸¹º»¼½¾¿ÀÁÂÃÄÅÆÆÈÉÊËÌÍÎÏĞÑˆ‰ŠÕ¡Œ‹ÙÚÛÜİß"
/*
-             |230      |240      |250
-       àáâãäåæçèéêëìíîïğñòóôõö÷øùúûüış\xFF
*/     "¢á“•ääæèè£–—ììîïğñòóôõö÷øùúûüış\xFF";

#endif /*  SORTKEY_H_ */
