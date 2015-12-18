/** @brief Program to convert lower case to upper case
 *
 *After conversion this program will pause for 2 sec
 *
 */

#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<stdlib.h>

int main()
{
	int ret_val;
	char str[] = "Hello TO the WOrld Of LinuX.Invoked FROM oTHer";
	int i;
	for(i = 0 ; *(str+i) ;i++)
	{
		if(isupper(str[i]))
			str[i] = tolower(str[i]);
	}
	printf("%s\n",str);
	sleep(2);
	return EXIT_SUCCESS;
}

