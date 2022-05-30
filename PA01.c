// **********************
// PROGRAMMING ASSIGNMENT 1
// Sara Clarin: netID sclarin2
// *********************** 
#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<time.h>

int main( int argc, const char* argv[] ) {


// SETUP: choose p, q, compute modulus n = pq
//  read in variables from file 
    mpz_t  c_p, d_p, p_p, q_p, n_p, m_p,  n, phi_n, e, c, d, m;
    mpz_inits( p_p, q_p, c_p, d_p, n_p,m_p,  m, n, phi_n, e, c, d, NULL);
    char buf[BUFSIZ];
    int nread, i = 0, count = 0;
    
    int fd;
    fd = open( argv[1], O_RDWR);
    if (fd < 0) {
        printf("Couldn't open file");
        return EXIT_FAILURE;
    }

     nread = read(fd, buf, BUFSIZ);
     const char * token = strtok(buf, ",");   
     mpz_set_str( m, token, 10);     // m

     token = strtok( NULL, ",");
     mpz_set_str( c_p, token, 10);  // c"
     
     token = strtok( NULL, ",");
     mpz_set_str( d_p, token, 10);  // d"

     token = strtok( NULL, ",");
     mpz_set_str( p_p, token, 10);  //p"
     token = strtok( NULL, ",");
     mpz_set_str( q_p, token, 10);  // q"
    
    close(fd);
    
    // find prime number p, q
    
    mpz_t P,Q, primep_1, primeq_1,gcd ;   
    mpz_inits(  primep_1, primeq_1,gcd , NULL);
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui( state, (unsigned long) time(NULL));   
     
    mpz_init2( P, 1100);
    mpz_init2(Q, 1100);
   
    mpz_urandomb( P, state, 1000);          // P,Q random select
    mpz_urandomb( Q, state, 1000);          // P,Q random select
  
    mpz_nextprime( P, P);   
    mpz_nextprime( Q, Q);
     
// UNCOMMENT TO MANUALLY CHECK P, Q
   // printf("\nRandom P: "); mpz_out_str(stdout, 10, P);
   // printf("\nRandom Q: "); mpz_out_str(stdout, 10, Q);

    if ( (mpz_sizeinbase(P, 2) < 500) | !mpz_probab_prime_p(P, 10) ) {
       mpz_nextprime( P, P);
       printf("P is too small or not prime" );
        }

    mpz_mul( n, P, Q);                    // n = p * q
    
     if (mpz_sizeinbase(n,2) < 1000) {
        printf("N is too small" );
	printf("\n n = "); mpz_out_str(stdout, 10, n);
	}
// KEY GENERATION
        
    // Phi(n) = ( p-1)(q-1)
    mpz_sub_ui ( primep_1, P, 1); 
    mpz_sub_ui ( primeq_1, Q, 1);
    mpz_mul( phi_n, primep_1, primeq_1);

    // randomly choose e-- 1 < e < phi_n, coprime to phi_n 
    bool found = false;
	    
    while ( !found ) {
        mpz_urandomm( e, state, phi_n); 
        mpz_gcd( gcd, e, phi_n);
	    
        if ( mpz_cmp_d(gcd, 1) == 0 ){
            found = true;
        }   
      }
    // compute d, such that d = e^-1 mod phi_n 
    
    if ( !mpz_invert( d, e, phi_n)) {
        printf("inverse of d mod phi_n does not exist");
    }

// ENCRYPTION: c = m^e mod n

    mpz_powm( c, m,e, n);   

// DECRYPTION  C"^d" mod n" = m"
        
    mpz_mul( n_p, p_p, q_p);  
    mpz_powm( m_p, c_p, d_p, n_p);

// write results into a file
    FILE *outfile = fopen(argv[2], "w" );
    if (outfile < 0){
        printf("couldn't open writefile");
        return EXIT_FAILURE;
        }
    
    gmp_fprintf( outfile, "%Zd,%Zd,%Zd,%Zd,%Zd", c, e, d, n, m_p);
    fclose(outfile );

    mpz_clears( m, c_p, d_p, p_p, q_p, c, e, d, n, m_p, phi_n,P, Q, gcd, primep_1, primeq_1, (mpz_ptr) NULL);  
    return 0;
}
