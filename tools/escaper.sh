#! /bin/bash
while true; do
    read -r -p "Enter the bash command: " line
	echo -n $line > tools/data
	< tools/data jq -aRs . | sed 's/^.\(.*\).$/\1/'
	exit;
done
