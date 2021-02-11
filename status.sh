max=$(head -1 report.md|cut -d ' ' -f 3|cut -d '/' -f 2)
total=$(head -1 report.md|cut -d ' ' -f 3|cut -d '/' -f 1)
echo $total
echo $max
echo "ACTOR = $GITHUB_ACTOR "

echo "RUN NUMBER= $GITHUB_RUN_NUMBER"
if [ "$GITHUB_RUN_NUMBER" -gt 4 ]
then
gh pr comment 1 -b "Total: $total / $max "
fi

if [ "$GITHUB_ACTOR" = "github-classroom" ]
then
gh pr comment 1 -b "Total: $total / $max "
fi


if [ "$total" -lt "$max" ]
then
	exit 1
fi

