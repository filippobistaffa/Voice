#!/usr/bin/env bash

red='\033[0;31m'			# Red
nc='\033[0m'				# No color
re='^[0-9]+$'				# Regular expression to detect natural numbers

usage() { echo -e "Usage: $0 -i <filename> [-o <filename>]\n-i\tInput graph filename\n-o\tOutputs solution to file (optional)\n-c\tEnable CSV output (optional)" 1>&2; exit 1; }

while getopts ":i:o:c" o; do
	case "${o}" in
	i)
		i=${OPTARG}
		if [ ! -f "$i" ]
		then
			echo -e "${red}Input file \"$i\" not found!${nc}\n" 1>&2; 
			usage
		fi
		;;
	o)
		out=${OPTARG}
		touch $out 2> /dev/null
		rc=$?
		if [[ $rc != 0 ]]
		then
			echo -e "${red}Unable to create $out${nc}" 1>&2; 
			exit
		else
			rm $out
		fi
		;;
	c)
		c=1
		;;
	\?)
		echo -e "${red}-$OPTARG is not a valid option!${nc}\n" 1>&2; 
		usage
		;;
	esac
done
shift $((OPTIND-1))

if [ -z "${i}" ]; then
	echo -e "${red}Required parameter -i missing${nc}\n" 1>&2; 
	usage
fi

td=`mktemp`
java -cp .:* TD "$i" $td
ntd=`sed '1q;d' $td`

tmp=`mktemp`

e=`cat "$i" | wc -l`
n=`grep -v " " "$i" | wc -l`
e=$(( $e - $n ))
echo "#define E $e" > $tmp
echo "#define N $n" >> $tmp
echo "#define NTD $ntd" >> $tmp

if [ ! -z "${c}" ]
then
	echo "#define CSV" >> $tmp
fi

if [ ! -z $out ]
then
	echo "#define SOL \"$out\"" >> $tmp
fi

if [ ! -f instance.h ]
then
	mv $tmp "instance.h"
else
	md5a=`md5sum instance.h | cut -d\  -f 1`
	md5b=`md5sum $tmp | cut -d\  -f 1`

	if [ $md5a != $md5b ]
	then
		mv $tmp "instance.h"
	else
		rm $tmp
	fi
fi

make -j

if [[ $? == 0 ]]
then
	bin=$0
	bin=${bin%???}
	$bin $i $td
fi

rm $td
