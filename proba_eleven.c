/*
	proba_eleven.c
	Compute probabilities of Arrangements with 10 against 11

	Author : davfrv@gmail.com

	License : MIT License (see LICENSE file)

	Compilation : use any ansi c compiler

#compil# gcc -std=c99 -W -Wall -Werror -Wextra -pedantic -o a.exe THIS_FILE &&
#compil# echo compil ok 1>&2 &&
#compil# time a.exe > a.txt

*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME 10 /* Max size (number of letters) of names */
#define YES 1 /* Boolean */
#define NO  0 /* Boolean */

/* Memory allocation of n integers */
int* new_array(int n)
{
	int* array = (int*)malloc(n * sizeof(int));
	if(!array) {
		fprintf(stderr, "error : malloc()\n");
		exit(1);
	}
	return array;
}

/* Prints an array t of n integers */
void print_array(int* t, int n)
{
	for(int i=0 ; i<n ; i++) {
		fprintf(stdout, "%d", t[i]);
		if(i < n-1)
			fprintf(stdout, "\t");
	}
	fprintf(stdout, "\n");
}

/*
	Returns an array with the next permutation of n elements,
	or NULL if the loop is finished.
	For example, with n == 4 :
		at  1st call returns 0 1 2 3
		at  2nd call returns 0 1 3 2
		at  3rd call returns 0 2 1 3
		at  4th call returns 0 2 3 1
		at  5th call returns 0 3 1 2
		at  6th call returns 0 3 2 1
		at  7th call returns 1 0 2 3
		at  8th call returns 1 0 3 2
		at  9th call returns 1 2 0 3
		at 10th call returns 1 2 3 0
		at 11th call returns 1 3 0 2
		at 12th call returns 1 3 2 0
		at 13th call returns 2 0 1 3
		at 14th call returns 2 0 3 1
		at 15th call returns 2 1 0 3
		at 16th call returns 2 1 3 0
		at 17th call returns 2 3 0 1
		at 18th call returns 2 3 1 0
		at 19th call returns 3 0 1 2
		at 20th call returns 3 0 2 1
		at 21th call returns 3 1 0 2
		at 22th call returns 3 1 2 0
		at 23th call returns 3 2 0 1
		at 24th call returns 3 2 1 0
		at 25th call returns NULL
*/
int* next_permutation(int n)
{
	static int* tp = NULL; /* The array with the current permutation */
	if(tp == NULL) { /* First call for this loop */
		tp = new_array(n);
		int i;
		for(i=0 ; i<n ; i++)
			tp[i] = i;
	} else { /* Next call for this loop */
		int i, c, d, j, p;
		for(i = n-1 ; tp[i] < tp[i-1] ; i--) { /* Set i to the position of the first ordered number */
			if(i==1) { /* The last value has been reached */
				free(tp);
				tp = NULL;
				return tp;
			}
		}
		c = tp[i-1]; /* The last unordered number */
		d = tp[i]; /* The first ordered number */
		for(j=i+1 ; j<=n-1 ; j++)
			if((tp[j]>c) && (tp[j]<d))
				d = tp[j];
		tp[i-1] = d;
		for( j=n-1 ; i<=j ; i++, j-- ) {
			if(i<j) { /* Permutation tp[i] <--> tp[j] */
				p = tp[i];
				tp[i] = tp[j];
				tp[j] = p;
				if(tp[i] == d) tp[i] = c;
			}
			if(tp[j] == d) tp[j] = c;
		}
	}
	return tp;
}

/*
	Returns an array with the next arrangement of n-1 elements in n positions
	(one element is doubled),
	or NULL if the loop is finished.
	For example, with n == 4 :
		at  1st call returns 0 1 2 0
		at  2nd call returns 0 1 2 1
		at  3rd call returns 0 1 2 2
		at  4th call returns 0 1 0 2
		at  5th call returns 0 1 1 2
		at  6th call returns 0 2 1 0
		at  7th call returns 0 2 1 1
		at  8th call returns 0 2 1 2
		at  9th call returns 0 2 0 1
		at 10th call returns 0 2 2 1
		at 11th call returns 0 0 1 2
		at 12th call returns 0 0 2 1
		...
		at 25th call returns 2 0 1 0
		at 26th call returns 2 0 1 1
		at 27th call returns 2 0 1 2
		at 28th call returns 2 0 0 1
		at 29th call returns 2 0 2 1
		at 30th call returns 2 1 0 0
		at 31th call returns 2 1 0 1
		at 32th call returns 2 1 0 2
		at 33th call returns 2 1 1 0
		at 34th call returns 2 1 2 0
		at 35th call returns 2 2 0 1
		at 36th call returns 2 2 1 0
		at 37th call returns NULL
*/
int* next_arrangement(int n)
{
	static int k; /* The element currently in double */
	static int p; /* Position of the element currently in double */
	static int* ta = NULL; /* The array with the current arrangement */

	int skip = 1;
	while(skip) { /* Need to restart all if this arrangement has already been provided */
	skip = 0;

		if(ta == NULL) { /* First call for this loop */
			ta = new_array(n);
			k = n-2; /* The last possible value of k, to force the start a new permutation */
		}

		if(k == n-2) { /* First call for this permutation */
			int* tp = next_permutation(n); /* Go to next permutation */
			if(tp == NULL) { /* The loop is finished */
				free(ta);
				ta = NULL;
				return ta;
			}
			if(tp[0] == n-1) { /* Little optimization : all the end already provided */
				while(next_permutation(n))
					; /* Finish all permutations */
				free(ta);
				ta = NULL;
				return ta;
			}
			for(int i=0 ; i<n ; i++) { /* Initialize our arrangement using the permutation */
				ta[i] = tp[i]; /* Copy ta <- tp */
				if(ta[i] == n-1) p = i; /* p is the position of the n_th element, that will be used for the double */
			}
			k = 0;
		} else { /* Next arrangement for this permutation */
			k++;
		}

		ta[p] = k; /* Set the new arrangement, with a new double element */

		for(int i=p+1 ; i<n ; i++) /* Each arrangement has 2 occurrences, need to skip one of them */
			if(ta[i] == k)
				skip = 1;

	} /* End while skip */

	return ta;
}

void test_permutation()
{
	int i, n;
	int* tp;
	for(n=2 ; n<8 ; n++) {
		fprintf(stdout, "n = %d\n", n);
		for(i=0 ; (tp=next_permutation(n)) != NULL ; i++) {
			print_array(tp, n);
		}
		fprintf(stdout, "i = %d\n\n", i);
	}
}

void test_arrangement()
{
	int i, n;
	int* ta;
	for(n=2 ; n<8 ; n++) {
		fprintf(stdout, "n = %d\n", n);
		for(i=0 ; (ta=next_arrangement(n)) != NULL ; i++) {
			print_array(ta, n);
		}
		fprintf(stdout, "i = %d\n\n", i);
	}
}

void test_eleven()
{
	int i;
	int n = 11;
	int* ta;
	for(i=0 ; (ta=next_arrangement(n)) != NULL ; i++)
		;
	printf("i = %d\n", i);
}

/* Creates and init the array is_right[] */
int* init_is_right(int n_arr)
{
	int* is_right = new_array(n_arr);
	for(int i=0 ; i<n_arr ; i++)
		is_right[i] = 1; /* At the beginning; all arrangements are possible */
	return is_right;
}

/* Prints names of n-1 elements and n positions
	gender == 0 -> elements are boys, positions are girls
	gender == 1 -> elements are girls, positions are boys
*/
void print_names(int n, int gender, const char nelt[][MAX_NAME+1], const char npos[][MAX_NAME+1])
{
	int i;
	printf("The %s are :\n", (gender?"boys":"girls"));
	for(i=0 ; i<n ; i++)
		fprintf(stdout, "\tpos=%d\t%s\n", i, npos[i]);
	printf("\n");
	printf("The %s are :\n", (gender?"girls":"boys"));
	for(i=0 ; i<n-1 ; i++)
		fprintf(stdout, "\telt=%d\t%s\n", i, nelt[i]);
	printf("\n");
}

/* Prints the current state of is_right[]
	(This is for control purpose)
*/
void print_is_right(int n, int* is_right)
{
	int i;
	int* ta;
	printf("Current is_right[] :\n");
	for(i=0 ; (ta=next_arrangement(n)) != NULL ; i++) {
		fprintf(stdout, "%s\t", is_right[i] ? "yes" : "no ");
		print_array(ta, n);
	}
	printf("\n");
}

/* Prints probability of having each element to each position */
void print_proba_right(int n, int* is_right, int nb_right, const char nelt[][MAX_NAME+1], const char npos[][MAX_NAME+1])
{
	int i, pos, elt;
	const int nsq = n * (n-1); /* Number of couples (element,position) */

	/* Init proba_right[] */
	int* proba_right = new_array(nsq);
	for(i=0 ; i<nsq ; i++)
		proba_right[i] = 0;

	/* Fill proba_right[] */
	int* ta; /* One possible arrangement (size = n) */
	for(i=0 ; (ta=next_arrangement(n)) != NULL ; i++) {
		if(is_right[i] == 1) {
			for(pos=0 ; pos<n ; pos++) {
				elt = ta[pos];
				proba_right[elt*n+pos]++; /* ta[pos] is elt */
			}
		}
	}

	/* Prints proba_right[] */
	fprintf(stdout, "Number of possible arrangements : %d / %d\n", nb_right, i);
	fprintf(stdout, ".\t."); /* First header */
	for(pos=0 ; pos<n ; pos++)
		fprintf(stdout, "\tpos=%d", pos);
	fprintf(stdout, "\n");
	fprintf(stdout, ".\t."); /* Second header */
	for(pos=0 ; pos<n ; pos++)
		fprintf(stdout, "\t%s", npos[pos]);
	fprintf(stdout, "\n");
	for(elt=0 ; elt<n-1 ; elt++) { /* Content */
		fprintf(stdout, "elt=%d", elt);
		fprintf(stdout, "\t%s", nelt[elt]);
		for(pos=0 ; pos<n ; pos++) {
			if(nb_right == 0)
				fprintf(stdout, "\t X    ");
			else if(proba_right[elt*n+pos] == 0)
				fprintf(stdout, "\t  0%% ");
			else if(proba_right[elt*n+pos] == nb_right)
				fprintf(stdout, "\t100%% ");
			else
				fprintf(stdout, "\t%4.1f%%", 100*(double)proba_right[elt*n+pos]/nb_right);
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

	/* End */
	free(proba_right);
	fflush(stdout);
}

/* Updates the array is_right[] knowing that elt is associated (or not) to pos,
	and returns the new value for nb_right
	if couple == 1 then it's a couple
	if couple == 0 then it's not a couple
*/
int add_new_couple(int n, int* is_right, int couple, int elt, int pos, const char nelt[][MAX_NAME+1], const char npos[][MAX_NAME+1])
{
	int* ta;
	int nb_right = 0;
	fprintf(stdout, "%s %s in couple with %s\n",
		nelt[elt],
		couple ? "is" : "is not",
		npos[pos]);
	for(int i=0 ; (ta=next_arrangement(n)) != NULL ; i++)
		if(is_right[i] == 1) {
			if(
				(( couple) && (ta[pos] != elt)) ||
				((!couple) && (ta[pos] == elt))
				)
				is_right[i] = 0;
			else
				nb_right ++;
		}
	return nb_right;
}

/* Updates the array is_right[] knowing a ceremony;
	we have a set of n-1 couples, and the number of matches (the right couples),
	and returns the new value for nb_right
	a_elt[] has a size of n, but only n-1 positions are experimented (one position stays alone)
	for the alone position, a_elt[] is -1
*/
int add_ceremony(int n, int* is_right, const int* a_elt, int matches, const char nelt[][MAX_NAME+1], const char npos[][MAX_NAME+1])
{
	int i, j;
	int* ta;
	int nb_right = 0;
	fprintf(stdout, "A new ceremony :\n");
	for(i=0 ; i<n ; i++)
		if(a_elt[i] == -1)
			printf("(pos=%d) %s stays alone\n", i, npos[i]);
		else
			printf("(pos=%d,elt=%d) %s with %s\n", i, a_elt[i], npos[i], nelt[a_elt[i]]);
	printf("We have %d matches\n", matches);
	for(i=0 ; (ta=next_arrangement(n)) != NULL ; i++)
		if(is_right[i] == 1) {
			/* Count how many common couple between a_elt[] and ta[] */
			int nb_matches = 0;
			for(j=0 ; j<n ; j++)
				if(ta[j] == a_elt[j]) /* It also handles the case of a_elt[j] == -1 */
					nb_matches++;

			/* If same matches, then ta[] is a possible solution */
			if(nb_matches == matches)
				nb_right ++;
			else
				is_right[i] = 0;
		}
	return nb_right;
}

/* Updates the array is_right[] knowing that pos is the doublon
	and returns the new value for nb_right
*/
int add_the_doublon(int n, int* is_right, int pos, const char npos[][MAX_NAME+1])
{
	int i, j;
	int* ta;
	int nb_right = 0;
	fprintf(stdout, "We know who is the doublon : %s\n", npos[pos]);
	for(i=0 ; (ta=next_arrangement(n)) != NULL ; i++)
		if(is_right[i] == 1) {
			/* Count how many common couple between a_elt[] and ta[] */
			int doublon = NO;
			for(j=0 ; j<n ; j++)
				if((j != pos) && ta[j] == ta[pos])
					doublon = YES;

			/* If pos is the doublon, then ta[] is a possible solution */
			if(doublon == YES)
				nb_right ++;
			else
				is_right[i] = 0;
		}
	return nb_right;
}

/* Test of every functions,
	assuming that the answer is (0, 1, 2, 3, 4, 5, 5)
*/
void test_small()
{
	int elt, pos, couple, matches;

	const int n = 7;         /* Number of elements and positions; we have n-1 elements in n positions */
	const int n_arr = 15120; /* Number of possible arrangements; n_arr = 1*2*3*...*n * (n-1) / 2 */
	const char npos[7][MAX_NAME+1] = { "Ah", "Do", "Ve", "So", "Gi", "Co", "Ta" };
	const char nelt[6][MAX_NAME+1] = { "Ka", "Ke", "Ki", "Ko", "Ku", "Ky" };

	/* Init is_right[] */
	int* is_right = init_is_right(n_arr); /* For each arrangement (size = n_arr), says if this arrangement is possible (=1) or not (=0) */
	int nb_right = n_arr;
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_new_couple() */
	elt = 3;
	pos = 3;
	couple = YES;
	nb_right = add_new_couple(n, is_right, couple, elt, pos, nelt, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_new_couple() */
	elt = 5;
	pos = 1;
	couple = NO;
	nb_right = add_new_couple(n, is_right, couple, elt, pos, nelt, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_ceremony() */
	const int a_elt_1[7] = { 1, 5, 2, 3, -1, 0, 4 };
	matches = 2;
	nb_right = add_ceremony(n, is_right, a_elt_1, matches, nelt, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_ceremony() */
	const int a_elt_2[7] = { 0, 5, -1, 3, 4, 1, 2 };
	matches = 3;
	nb_right = add_ceremony(n, is_right, a_elt_2, matches, nelt, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_new_couple() */
	elt = 5;
	pos = 2;
	couple = NO;
	nb_right = add_new_couple(n, is_right, couple, elt, pos, nelt, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_the_doublon() */
	pos = 6;
	nb_right = add_the_doublon(n, is_right, pos, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_new_couple() */
	elt = 5;
	pos = 6;
	couple = YES;
	nb_right = add_new_couple(n, is_right, couple, elt, pos, nelt, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	/* add_ceremony() */
	const int a_elt_3[7] = { 0, 1, 2, 5, 4, 3, -1 };
	matches = 4;
	nb_right = add_ceremony(n, is_right, a_elt_3, matches, nelt, npos);
	print_proba_right(n, is_right, nb_right, nelt, npos);

	//print_is_right(n, is_right);
	free(is_right);
}

/* Some little macros to save place */
#define TITLE(a) fprintf(stdout, "%s\n\n", a); fprintf(stderr, "%s\n\n", a); fflush(stderr);
#define PPR      print_proba_right(n, is_right, nb_right, nelt, npos);

/* A real test, with the french's season 2 */
void test_season_2()
{
	const int n = 11;            /* Number of elements and positions; we have n-1 elements in n positions */
	const int n_arr = 199584000; /* Number of possible arrangements; n_arr = 1*2*3*...*n * (n-1) / 2 */
	const char npos[][MAX_NAME+1] = {
		"Daisie",      /* pos=0 */
		"Hilona",      /* pos=1 */
		"Ines",        /* pos=2 */
		"Jessica",     /* pos=3 */
		"Anastassia",  /* pos=4 */
		"Marilou",     /* pos=5 */
		"Mia",         /* pos=6 */
		"Noee",        /* pos=7 */
		"Stella",      /* pos=8 */
		"Celia",       /* pos=9 */
		"Melanie"      /* pos=10 */
	};
	const char nelt[][MAX_NAME+1] = {
		"Nicolas",     /* elt=0	*/
		"Selim",       /* elt=1	*/
		"Tony",        /* elt=2	*/
		"Tom",         /* elt=3	*/
		"Max",         /* elt=4	*/
		"Jonathan",    /* elt=5	*/
		"Sergio",      /* elt=6	*/
		"Lorenzo",     /* elt=7	*/
		"Jeremy",      /* elt=8	*/
		"Illan"        /* elt=9	*/
	};

	print_names(n, 0, nelt, npos);

	/* Init is_right[] */
	TITLE("Init")
	int* is_right = init_is_right(n_arr); /* For each arrangement (size = n_arr), says if this arrangement is possible (=1) or not (=0) */
	int nb_right = n_arr;
	PPR

	/* We already know who is the doublon */
	TITLE("The doublon")
	nb_right = add_the_doublon(n, is_right, /* pos = */ 10, npos);
	PPR

	TITLE("Week  1")
	nb_right = add_new_couple(n, is_right, NO , 6, 5, nelt, npos); PPR
	const int e01[] = {8, 9, 6, 2, 4, 1, 0, 5, 7, 3, -1}; nb_right = add_ceremony(n, is_right, e01, 1, nelt, npos); PPR

	TITLE("Week  2")
	nb_right = add_new_couple(n, is_right, YES, 4, 4, nelt, npos); PPR
	nb_right = add_new_couple(n, is_right, NO , 4, 10, nelt, npos); PPR
	const int e02[] = {1, 9, -1, 7, 4, 3, 6, 0, 8, 2, 5}; nb_right = add_ceremony(n, is_right, e02, 3, nelt, npos); PPR

	TITLE("Week  3")
	nb_right = add_new_couple(n, is_right, NO , 7, 3, nelt, npos); PPR
	const int e03[] = {0, 6, 5, -1, 4, 3, 1, 9, 8, 7, 2}; nb_right = add_ceremony(n, is_right, e03, 3, nelt, npos); PPR

	TITLE("Week  4")
	nb_right = add_new_couple(n, is_right, YES, 8, 8, nelt, npos); PPR
	nb_right = add_new_couple(n, is_right, NO , 8, 10, nelt, npos); PPR
	const int e04[] = {2, 6, 3, 0, 4, 5, 1, 9, 8, 7, -1}; nb_right = add_ceremony(n, is_right, e04, 3, nelt, npos); PPR

	TITLE("Week  5")
	nb_right = add_new_couple(n, is_right, NO , 1, 6, nelt, npos); PPR
	const int e05[] = {0, 3, 2, 1, 4, 9, 6, 5, 8, 7, -1}; nb_right = add_ceremony(n, is_right, e05, 5, nelt, npos); PPR

	TITLE("Week  6")
	nb_right = add_new_couple(n, is_right, NO , 3, 1, nelt, npos); PPR
	nb_right = add_new_couple(n, is_right, YES, 7, 7, nelt, npos); PPR
	const int e06[] = {0, 2, 3, -1, 4, 5, 1, 9, 8, 7, 6}; nb_right = add_ceremony(n, is_right, e06, 4, nelt, npos); PPR

	TITLE("Week  7")
	nb_right = add_new_couple(n, is_right, NO , 7, 10, nelt, npos); PPR
	const int e07[] = {1, 2, 3, 0, 4, 5, 6, 7, 8, 9, -1}; nb_right = add_ceremony(n, is_right, e07, 6, nelt, npos); PPR

	TITLE("Week  8")
	nb_right = add_new_couple(n, is_right, NO , 0, 3, nelt, npos); PPR
	const int e08[] = {0, 9, 1, 3, 4, 2, 6, 7, 8, -1, 5}; nb_right = add_ceremony(n, is_right, e08, 6, nelt, npos); PPR

	TITLE("Week  9")
	nb_right = add_new_couple(n, is_right, NO , 2, 10, nelt, npos); PPR
	const int e09[] = {0, -1, 3, 2, 4, 5, 6, 7, 8, 9, 1}; nb_right = add_ceremony(n, is_right, e09, 7, nelt, npos); PPR

	TITLE("Week 10")
	nb_right = add_new_couple(n, is_right, YES, 6, 6, nelt, npos); PPR
	nb_right = add_new_couple(n, is_right, NO , 6, 10, nelt, npos); PPR
	const int e10[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9}; nb_right = add_ceremony(n, is_right, e10, 11, nelt, npos); PPR

	free(is_right);
}

/* A real test, with the french's season 3 */
void test_season_3()
{
	const int n = 11;            /* Number of elements and positions; we have n-1 elements in n positions */
	const int n_arr = 199584000; /* Number of possible arrangements; n_arr = 1*2*3*...*n * (n-1) / 2 */
	const char npos[][MAX_NAME+1] = {
		"Antoine",  /* pos=0 */
		"Ariel",    /* pos=1 */
		"Hugo",     /* pos=2 */
		"Chine",    /* pos=3 */
		"Enzo",     /* pos=4 */
		"Kevin",    /* pos=5 */
		"Seb",      /* pos=6 */
		"Steevy",   /* pos=7 */
		"Stephane", /* pos=8 */
		"Ted",      /* pos=9 */
		"Thomas"    /* pos=10 */
	};
	const char nelt[][MAX_NAME+1] = {
		"Camille",  /* elt=0 */
		"Chani",    /* elt=1 */
		"Charlene", /* elt=2 */
		"Lena",     /* elt=3 */
		"Emma",     /* elt=4 */
		"Ines",     /* elt=5 */
		"Julie",    /* elt=6 */
		"Lorine",   /* elt=7 */
		"Marine",   /* elt=8 */
		"Mina"      /* elt=9 */
	};

	print_names(n, 1, nelt, npos);

	/* Init is_right[] */
	TITLE("Init")
	int* is_right = init_is_right(n_arr); /* For each arrangement (size = n_arr), says if this arrangement is possible (=1) or not (=0) */
	int nb_right = n_arr;
	PPR

	TITLE("Week  1")
	nb_right = add_new_couple(n, is_right, NO , 3, 2 , nelt, npos); PPR
	nb_right = add_new_couple(n, is_right, NO , 7, 7 , nelt, npos); PPR
	const int e01[] = {6, 5, 4, 3, 8, 9, 7, -1, 0, 2, 1}; nb_right = add_ceremony(n, is_right, e01, 1, nelt, npos); PPR

	TITLE("Week  2")
	nb_right = add_new_couple(n, is_right, NO , 1, 10, nelt, npos); PPR
	const int e02[] = {4, 3, 9, -1, 8, 0, 5, 6, 2, 1, 7}; nb_right = add_ceremony(n, is_right, e02, 1, nelt, npos); PPR

	TITLE("Week  3")
	nb_right = add_new_couple(n, is_right, YES, 4, 5 , nelt, npos); PPR
	nb_right = add_new_couple(n, is_right, YES, 4, 9 , nelt, npos); PPR
	const int e03[] = {6, 3, 5, 1, 8, 4, 9, 2, 0, 4, 7}; nb_right = add_ceremony(n, is_right, e03, 2+1, nelt, npos); PPR

	/* Remark : do not forget the doubled match => add_ceremony(... +1 ...) */

	TITLE("Week  4")
	nb_right = add_new_couple(n, is_right, NO , 8, 3 , nelt, npos); PPR
	const int e04[] = {5, 3, 2, 0, 1, 4, 6, 9, 8, 4, 7}; nb_right = add_ceremony(n, is_right, e04, 3+1, nelt, npos); PPR

	TITLE("Week  5")
	nb_right = add_new_couple(n, is_right, NO , 8, 4 , nelt, npos); PPR
	const int e05[] = {1, 3, 2, 9, 7, 4, 6, 5, 8, 4, 0}; nb_right = add_ceremony(n, is_right, e05, 2+1, nelt, npos); PPR

	TITLE("Week  6")
	nb_right = add_new_couple(n, is_right, NO , 8, 10, nelt, npos); PPR
	const int e06[] = {8, 3, 2, 1, 0, 4, 9, 7, 5, 4, 6}; nb_right = add_ceremony(n, is_right, e06, 2+1, nelt, npos); PPR

	TITLE("Week  7")
	nb_right = add_new_couple(n, is_right, NO , 9, 6 , nelt, npos); PPR
	const int e07[] = {9, 1, 8, 3, 6, 4, 5, 0, 2, 4, 7}; nb_right = add_ceremony(n, is_right, e07, 2+1, nelt, npos); PPR

	free(is_right);
}

int main()
{
	//test_permutation();
	//test_arrangement();
	//test_eleven();
	//test_small();
	//test_season_2();
	test_season_3();

	return 0;
}

