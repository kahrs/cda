#include "geom.h"
#include "thing.h"

String::String(char *ss)
{
	int i = 0;
	char *p = ss;
	do i++; while (*p++); p--;
	while (i > 1 && *--p == ' ') {		/* strip trailing blanks */
		i--;
		*p = 0;
	}
	p = s = new char[i];
	do *p = *ss++; while (*p++);
}

String *String::append(char *ss)
{
	int i = 0;
	char *p,*q;
	char *t;
	p = s;
	do i++; while (*p++);
	p = ss;
	do i++; while (*p++);
	t = new char[i-1];
	for (p = t, q = s; *p++ = *q++;);
	for (p--, q = ss; *p++ = *q++;);
	delete s;
	s = t;
	return this;
}

int
String::eq(Thing *p)
{
	char *a,*b;
	String *t = (String *) p;
	for (a=s, b=t->s; *a == *b; a++, b++)
		if (*a == 0)
			return 1;
	return 0;
}

void Vector::clear()
{
	int i;
	for (i = 0; i < n; i++)
		delete a[i];
	n = 0;
}

Thing *Vector::lookup(Thing *s)
{
	int i;
	Thing *t;
	for (i = 0; i < n; i++)
		if ((t = a[i])->eq(s))
			return t;
	return 0;
}

Thing *Vector::append(Thing *t)
{
	if (n >= N) {
		Thing **b = new Thing*[N*=4];
		int i;
		for (i = 0; i < n; i++)
			b[i] = a[i];
		delete a;
		a = b;
	}
	return a[n++] = t;
}

Thing *Vector::remove(int i)
{
	Thing *t;
	if (i < n) {
		t = a[i];
		if (n > 0)
			a[i] = a[n-1];
		n--;
		return t;
	}
	return 0;
}

void Vector::exchg(int i, int j)
{
	Thing *t;
	t = a[i];
	a[i] = a[j];
	a[j] = t;
}

void Vector::put(FILE *ouf)
{
	int i;
	for (i = 0; i < n; i++)
		a[i]->put(ouf);
}

Rectangle Vector::mbb(Rectangle r)
{
	int i;
	for (i = 0; i < n; i++)
		r = a[i]->mbb(r);
	return r;
}

void Vector::apply(int *arg, void (Thing::*f)(int *))
{
	int i;
	for (i = 0; i < n; i++)
		(a[i]->*f)(arg);
}

