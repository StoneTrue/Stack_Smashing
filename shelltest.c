// Simple C program to illustrate stack overflows & code injection
//
// Usage - enter password when running program, e.g: ./shelltest XXXXXXXX where XXXXXX is the password.
// Compile options:  to see the overflow, compile with: gcc -m32 -g -z execstack -o shelltest shelltest.c
// (-m32 to get 32 bit program on 64 bit OS)
//  In this case, if ./shelltest AAAAAAAAAAAAAAAAAAAAAAAAAAAAA entered as password, character byte will overwrite boolean for auth_flag in stack and grant access.
//  Note that if not enough "A's" entered,there will be no overflow.  If too many "A'" entered, accesss will be granted but there will also be a segmentation fault.
// Compile option:  to have gcc protect you from yourself, compile with "gcc -g -fstack-protector-all auth_overlow1.c"
// In this case, if AAAAAAAAAAAAAAAAAAAAA entered as password, will return "*** stack smashing detected ***: ./no_smash terminated
//Segmentation fault
//  Alternatively, uncomment the lines "Rob's Stack Canary" to enable your own stack canary without the compiler adding it.
// For real fun, enter the below (unfortunately this will only work in gdb debugger):
// ./shelltest $(perl -e 'print "\x90"x20,"\x90\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x11\x59\xb0\x04\xb3\x01\xb2\x11\xcd\x80\xb0\x01\xb3\x01\xfe\xcb\xcd\x80\xe8\xea\xff\xff\xff\x49\x95\x68\x61\x76\x65\x95\x79\x6f\x75\x95\x6e\x6f\x77\x21\x21\x21\x21\x21","\x0c\xd3\xff\xff"x10')

// Using gcc version 4.9.2 (Debian 4.9.2-10)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_authentication (char *password)

{
	int auth_flag = 0;
	// int no_smash = 145;			// This is the stack canary value.  Works with hard coded, which is a bad practice.  Better to randomize it each 							// time.  However, gcc pushes stack_can before password_buffer, so stack canary can be overwritten.
	char password_buffer[64];		// Order of these declarations is important; auth_flag is declared first, so pushes first;
	// int stack_can = no_smash;		// If declared in the opposite order (password_buffer before auth_flag), no overflow possible into auth_flag.
	
	strcpy (password_buffer,password);

	if (strcmp(password_buffer, "brillig") == 0)
		auth_flag = 1;
	
	if (strcmp(password_buffer, "outgrabe") == 0)
		auth_flag = 1;
	
	// Rob's Stack Canary!
	// Uncomment the below lines to enable a Rob-written stack canary.
	//
	//if (no_smash != 145)		// Note hard coded stack value.
	//{
	//	auth_flag = 0;
	//	printf("\nYour Jedi mind tricks will not work on me, boy!\n");
	//}
	//
	// Rob's Stack Canary ends here

	return auth_flag;

}

int main (int argc, char *argv[])

{

	if (argc < 2)
	
	{
		printf("Usage: %s <password>\n", argv[0]);		// Fusses at you for not entering an argument & closes
		exit(0);
	}

	if (check_authentication (argv[1]))

	{
		printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
		printf("	Access Granted.\n");
		printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	}
	else
	{	
		printf("\nAccess Denied. :(\n");
	}
	
	if (argc >100)
	{	
		printf("\nYou found the easter egg!!!\n");
	}
}
