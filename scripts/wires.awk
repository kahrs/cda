/^\.c/ {prev = $0; next}
/^\./ {next}
{sig[$1]++; chip[$1] = prev}
END	{
	for (s in sig) { printf("%s %d\n", s, sig[s]-1); }
	}
