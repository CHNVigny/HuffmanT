#include <iostream>
#include <string.h>
#include <stdio.h>
void strRev(char *s)
{
	char temp, *end = s + strlen(s) - 1;
	while (end > s)
	{
		temp = *s;
		*s = *end;
		*end = temp;
		--end;
		++s;
	}
}

int main()
{
	char a[3];
	a[0] = 'a';
	a[1] = 'b';
	a[2] = 'c';
	strRev(a);
	printf("%s", a);
	return 0;
}