#   awk script to change block asm statments into the single line
#   style that versions of Turbo C up to 2.01 can parse.
#   asm {
#      pushf
#      pop flags
#   }
#   /*START asm { */
#   asm   pushf
#   asm   pop flags
#   /*END } */
#
# If the asm block start or end lines contain comments you'll end up
# with a nested comment, other than that I believe comments are handled
# correctly. If you fix that or fix or improve anything else send me a
# copy via fidonet 1:140/12.9 or C_ECHO
#
# public domain code written by Ed Kowalski 22 aug 93, use freely.
# The comment code was written by Dan Kozak, check c_lines.awk in
# snippets. (so what's all the fuss about c++ and reusable code<g>)

{

if ($1 ~ /^\/\*/ && $NF ~ /\*\/$/) { print $0; next }
else if ($0 ~ /\/\*/ && $0 !~ /\*\//) { in_comment = 1 }
else if ($0 !~ /\/\*/ && $0 ~ /\*\//) { in_comment = 0 }
else if ($0 ~/[\t ]*}/ && asmblok) {
    asmblok = 0
    $0 = "/*END" $0 " */"
}
else if ($0 ~ /[\t ]*asm[\t ]+{/ || asmblok > 0 && !in_comment ) {
    if( asmblok )
        $0 = "asm" $0
    else  {
        asmblok = 1
        $0 = $0 = "/*START" $0 " */"
    }
}

{ print $0 }

}
