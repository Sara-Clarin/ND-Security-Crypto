//**************
// PROGRAMMING ASSIGNMENT 2
// Sara Clarin: netID sclarin2
//*******

#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<time.h>
#include<unistd.h>
#include<stdbool.h>
#include<fcntl.h>
#include<stdbool.h>
#include<sys/types.h>


int main( int argc, const char* argv[]) {
	mpz_t g, Q, P, p_1, x, h, r, m, res1, res2, res3, pow_one, pow_two, c1, c2, c3, gcd;
	mpz_inits(g, Q, gcd,  P, p_1, x, h, r,m, res1, res2, res3, pow_one, pow_two, c1, c2, c3, NULL);
	char buf[BUFSIZ];
	int nread, i = 0;

	if (argc < 2) {
		printf("not enough arguments");
		return EXIT_FAILURE;
	}
	int fd;
	fd = open (argv[1], O_RDWR);
	if (fd < 0) {
		printf("couldn't open file");
		return EXIT_FAILURE;
	}
	
// input m 
	nread = read(fd, buf, BUFSIZ);
	close(fd);

	mpz_set_str( m, buf, 10);
	
	// test input:
	printf("message: "); mpz_out_str(stdout, 10,  m); printf("\n");

// SETUP: choose a cyclic group and generator g 
	// CHOOSING SPECIAL PRIME #Q	
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui( state, (unsigned long) time(NULL));


	mpz_urandomb( P, state, 2050);  
	mpz_nextprime( P, P);
	
	while( mpz_sizeinbase(P, 2) < 2000) {  // choose a P at least 2000 bits
		mpz_nextprime( P, P);
	}

	printf("finding a prime p!");

/*	while( true ) {
		mpz_nextprime( P, P);
		mpz_sub_ui( p_1, P, 1);
		mpz_fdiv_q_ui( Q, p_1, 2);

		if (mpz_probab_prime_p( Q, 30) != 0) {
			break;
		}
	}
	*/
	mpz_set_str( P, "63299413021708786830619699262199031648256710296364939522951602016025333368092787043793201183309770446312773903557286425664219974330327271093391802821795507760493081515938617065454663621310038238901719236944620528113698674554726710560927526745162417057484163780212801756191536029980946722490128769618672677182016404682368896787240013394670803253042773277765696529186601047855052105418775255431872329654600035750702550715975223774729610975694258705490958545417017261845927118662932926902005733538073776108973671956733232888959495866163682965581299889142927503743848373781028153905054275473917898258937243", 10);

	
	mpz_sub_ui( p_1, P, 1); 
	mpz_fdiv_q_ui( Q, p_1, 2);
	
	mpz_fdiv_q_ui( pow_one, p_1, 2);
	mpz_fdiv_q( pow_two, p_1, Q);
	
	/* Find the generator of the group zp*  */
	while ( true ){
		mpz_urandomm( g, state, P);
		
	//	mpz_gcd( gcd, P, g);		// ensure element in Zp*
	//	if ( mpz_cmp_ui(gcd, 1) != 0){
	//		continue;
	//	}
		

		mpz_powm( res1, g, pow_one, P);
		mpz_powm( res2, g, pow_two, P);
	
		if (mpz_cmp_ui(res1, 1) != 0   && mpz_cmp_ui(res2, 1) != 0){
			break;
		}
	}


	mpz_powm_ui(g, g, 2, P);

// KEYGEN: assign x, the private key, compute h = g^x
	mpz_set_str(x,"1234567890123456789012345678901234567890" , 10 );
	mpz_powm( h, g, x, P);
 
// ENCRYPTION/OUTPUT  c1 = g^r,  c2 = m * h^r
	FILE * outfile = fopen( argv[2], "w+");
	if (outfile < 0){
		printf("Couldn't open output file: re-run program with \n ./program input.txt outfile.txt\n");
		return EXIT_FAILURE;
	}

	for ( i = 0; i < 3; i++){
		mpz_urandomm( r, state, Q);
		mpz_nextprime( r, r);
		mpz_powm( c1, g, r, P);
		mpz_powm( c2, h, r, P);
		mpz_mul( c2, m, c2);

	//	gmp_fprintf( stdout, "%Zd,%Zd,%Zd\n", c1, c2, P);
		gmp_fprintf( outfile, "%Zd,%Zd,%Zd\n", c1, c2, P);
	}
	
	fclose(outfile);
	mpz_clears( g, Q, gcd, P,p_1, x, h, r,m, res1, res2, res3, pow_one, pow_two, c1, c2, c3, (mpz_ptr) NULL);
	return 0;

}	
