func swap(x)
{
	if (index(x, "/"))
	{
		split(x, sx, "/");
		return(sx[2] "/" sx[1]);
	}
	else	return(x);
}
/^\.B/	{ print $0; }
/^\.A/	{ print ".A",swap($2),swap($3),swap($4),swap($5); }
/^\.P/	{ print ".P",swap($2),$4,$3,swap($5),$6; }
/^\.v /	{ print $0; }
/^\.vd/	{ print $0; }
/^\.vb/	{ print ".vb",$2,swap($3),$4,$5,swap($6); }
/^\.R/	{ print ".R",swap($2),$4,$3,$5; }
