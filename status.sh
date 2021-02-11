max=$(head -1 report.md|cut -d ' ' -f 3|cut -d '/' -f 2)
total=$(head -1 report.md|cut -d ' ' -f 3|cut -d '/' -f 1)
echo $total
echo $max
if ! [ "$GITHUB_ACTOR" = "hikmatfarhat" ]
then
gh pr comment 1 -b "Total: $total / $max "
fi

if [ "$total" -lt "$max" ]
then
	exit 1
fi

