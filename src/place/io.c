/* Copyright (c) 1992 AT&T - All rights reserved. */

char	RX[] = "rx";
RXPATH[] = "/bin/rx";

void
bootterm(char *machine, char **argv, char **end)
{
	int ph2t[2], pt2h[2];

	if(machine){
		dup(remotefd0, 0);
		dup(remotefd1, 1);
		close(remotefd0);
		close(remotefd1);
		argv[0] = "placeterm";
		*end = 0;
		exec(placeterm, argv);
		fprint(2, "can't exec: ");
		perror(placeterm);
		_exits("damn");
	}
	if(pipe(ph2t)==-1 || pipe(pt2h)==-1)
		panic("pipe");
	switch(fork()){
	case 0:
		dup(ph2t[0], 0);
		dup(pt2h[1], 1);
		close(ph2t[0]);
		close(ph2t[1]);
		close(pt2h[0]);
		close(pt2h[1]);
		argv[0] = "placeterm";
		*end = 0;
		exec(placeterm, argv);
		fprint(2, "can't exec: ");
		perror(placeterm);
		_exits("damn");
	case -1:
		panic("can't fork placeterm");
	}
	dup(pt2h[0], 0);
	dup(ph2t[1], 1);
	close(ph2t[0]);
	close(ph2t[1]);
	close(pt2h[0]);
	close(pt2h[1]);
}

void
connectto(char *machine)
{
	int p1[2], p2[2];

	if(pipe(p1)<0 || pipe(p2)<0){
		dprint("can't pipe\n");
		exits("pipe");
	}
	remotefd0 = p1[0];
	remotefd1 = p2[1];
	switch(fork()){
	case 0:
		dup(p2[0], 0);
		dup(p1[1], 1);
		close(p1[0]);
		close(p1[1]);
		close(p2[0]);
		close(p2[1]);
		execl(RXPATH, RX, machine, rplacename, "-R", (char*)0);
		dprint("can't exec %s\n", RXPATH);
		exits("exec");

	case -1:
		dprint("can't fork\n");
		exits("fork");
	}
	close(p1[1]);
	close(p2[0]);
}

void
startup(char *machine, int Rflag, char **argv, char **end)
{
	if(machine)
		connectto(machine);
	if(!Rflag)
		bootterm(machine, argv, end);
	downloaded = 1;
	outTs(Hversion, VERSION);
}
