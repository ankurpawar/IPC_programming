/**@brief Program to start a different program  
 *
 *Program uses system() to invoke another program
 *That converts lower case to upper case.
 *
 */

#include<stdio.h>
#include<stdlib.h>

int main() 
{
	int ret_val;
	ret_val = system("./lowcase_upcase");
	printf("ret_val = %d\n",ret_val);
	return EXIT_SUCCESS;
}
