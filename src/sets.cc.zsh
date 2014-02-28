#!/bin/zsh

printf '#include "util.hh"\n\nnamespace euler {\n'

for s in $*; do
    printf 'extern std::list<problem const*> set%s;\n' $s
done

printf 'std::list<std::list<problem const*> const*> const sets'

if [[ ! -z $* ]]; then
    printf '{{'
    for s in $*; do
	printf '&set%s,' $s
    done
    printf '}}'
fi

printf ';\n}\n'
