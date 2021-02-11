max=$(head -1 report.md|cut -d ' ' -f 3|cut -d '/' -f 2)
total=$(head -1 report.md|cut -d ' ' -f 3|cut -d '/' -f 1)
echo $total
echo $max

gh pr comment 1 -b "Total: $total / $max "

if [ "$total" -lt "$max" ]
then
	exit 1
fi

