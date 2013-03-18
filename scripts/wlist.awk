/^\.f/ {next}
/^\.q/ {next}
/^\.t/ {next}
/^\.c/ {chip = $2; part = $3; next}
/^\.o/ {chip = $2; part = $3; next}
{print $1"	"chip"	"part"	"$2}
