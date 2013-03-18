/^circle/	{ if ($4 == "4.3")
			{
			print "line ",$2-2,$3,$2+2,$3;
			print "line ",$2,$3-2,$2,$3+2;
			}
		else	print $0;
		}
!/^circle/	{ print $0; }
