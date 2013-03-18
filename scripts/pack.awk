BEGIN		{ mode = 0; }
/^\.c/		{ chip[$2] = $3; part[$3] = $2; }
/^\.f/		{ mode = 1; }
/^\.t[ \t]/	{ if (mode)
		    {
			if ($3 == "=")
			{
				if (package[$4] == "")
					package[$4] = $2;
				else
					{
					package[$2] = package[$4];
					package[$4] = $2;
					}
			}
			else
			{
				head = $2;
				while (package[head] != "")
				{
					newhead = package[head];
					package[head] = $3;
					head = newhead;
				}
				package[head] = $3;
			}
		    }
		}
END	{
	    for (c in chip)
		{
		printf("%s\t", c);
		if (length(c) < 8) printf("\t");
		printf("%s\t", chip[c]);
		if (length(chip[c]) < 8) printf("\t");
		printf("%s\n", package[chip[c]]);
		}
	}
