#!/bin/zsh

printf '#include "util.hh"\n\n'

for s in $*; do
    printf '#include "set%s.hh"\n' $s
done

printf '\nusing namespace euler;\n\n'

printf 'std::list<std::list<problem const*> const*> const euler::sets'

if [[ ! -z $* ]]; then
    printf '{{'
    for s in $*; do
	printf '&set%s,' $s
    done
    printf '}}'
fi

printf ';\n'
