remove the line starts with x
grep -v '^X(' input.txt > output.txt

To highlight both IP addresses in red and the specified keyword in green. and to print 100 previous lines as well
grep -B 100 "keyword" input.txt | sed -E 's/([0-9]{1,3}\.){3}[0-9]{1,3}/\x1b[31m&\x1b[0m/g; s/(keyword)/\x1b[32m\1\x1b[0m/g'
