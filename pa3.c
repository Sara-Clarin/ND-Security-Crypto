#include<stdio.h>
#include "pbc.h"
#include<stdlib.h>
#include<string.h>
//#define m1 "CSE 40622 is really fun!!"
int main(int argc, char* argv[]){
/* SIGN the input message m using sk; output sigma, g1 */
// hardcode inputs

pairing_t pairing;
char param[3000] = "type a\nq

8780710799663312522437781984754049815806883199414208211028653399266475
6308802229570786251794226622214231558587695823174592777133673174813249
25129998224791\nh
1201601226489114607938882136674053420480295440125131182291961513104720
7289359704531102844802183906537786776\nr
730750818665451621361119245571504901405976559617\nexp2 159\nexp1
107\nsign1 1\nsign0 1\n";

if ( pairing_init_set_buf( pairing, param, sizeof(param))){
pbc_die("pairing initialization failed");
}

else if (!pairing_is_symmetric(pairing)){
pbc_die("pairing not symmetric");
}
int b;

element_t g1, h, pk, sk, pk_p, sigma_p, sigma, temp1, temp2,
h2;

element_init_Zr(sk, pairing);
element_init_G1( g1, pairing);
element_init_G1( h, pairing);
element_init_G1( h2, pairing);
element_init_G1( pk, pairing);
element_init_G1( sigma, pairing);
element_init_G1( pk_p, pairing);
element_init_G1(sigma_p, pairing);
element_set_str(g1,

"[12097087162247825318067859395596484057225810823744501789886476880005
2646883835974022381185646096762005501518255250246268860792244966342702
7116607731602349,
3620944025207005131182150321199719937725711840975921369561598094745584
2752504362806853726887172677818130420282435119504311897486466775773389
18633409797434]", 10);

char m1[BUFSIZ] = "CSE 40622 is really fun!!";
char m2[BUFSIZ] = "OMG! CSE 40622 is so fun!!!!";

element_set_str( sk,"124082650188479568800315864596935264695441418483"
,10);

element_set_str( sigma_p,

"[61428709185668426649485660058444461835295569060671097309733440512304
4060376542080472029202138815109165195694288330756629839854624512147328
4358836910701814,
2327537991882555510086515472694392091098806284878382963258616736336371
2557202873186599711989624967200649371954147562484027242240165973078142
04647956085167] ", 10);

element_set_str(pk_p,

"[63631079210295664116406114259048744587515501935709313590102659254703
4078984320304799555501681666657029426519710263096402898533774810185090
1210499297919565,
4908033973002147664980673680854365370917916942221276602869639006930465
6483725838477498792237573196473764303651626190132917378058233653420377
39189570000000] " , 10);
// initialize parameters
// key generation: pk = g1^x, sk = x
// element_random(sk);
element_pow_zn( pk, g1, sk);
// signing: signma = h^x
element_from_hash(h, m1, strlen(m1));
element_pow_zn( sigma, h, sk);
element_printf("%B,%B\n", sigma,g1);
/* VERIFY if m' is valid for given sigma'; output b=0 if invalid, b=1
if valid */
// use pairing_apply

element_init_GT( temp1, pairing);
element_init_GT( temp2, pairing);
element_from_hash(h2, m2, strlen(m2));
element_pairing( temp1, h2, pk_p);
element_pairing( temp2, sigma_p, g1);
//pairing_apply( temp2, h, pk_p , pairing);
if ( element_cmp(temp1, temp2)){

b = 0;

}
else{

b = 1;

}
printf("%d\n", b);
//pairing_clear(pairing);
return 0;
}
