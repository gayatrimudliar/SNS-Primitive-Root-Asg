#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;

#define MAX_FAC 10000

long int count_prime_fact(mpz_t num){
   	mpz_t i,n;
	mpz_init(i);
	mpz_set_ui(i,2);
	mpz_init(n);
	mpz_set(n,num);
	long int counter=0;
	while(mpz_cmp_ui(n,1)>0){

		if(mpz_divisible_p(n,i) != 0)
		{
			while(mpz_divisible_p(n,i) != 0){
				mpz_tdiv_q(n,n,i);
			}
			counter++;
		}
		mpz_add_ui(i,i,1);
	}
	return counter;
}

int prime_factorization(mpz_t *ret,mpz_t num){
   	mpz_t i,n;
	mpz_init(i);
	mpz_set_ui(i,2);
	mpz_init(n);
	mpz_set(n,num);
	int counter=0;
	while(mpz_cmp_ui(n,1)>0){

		if(mpz_divisible_p(n,i) != 0)
		{
			while(mpz_divisible_p(n,i) != 0){
				mpz_tdiv_q(n,n,i);
			}
			mpz_init(ret[counter]);
			mpz_set(ret[counter],i);
			counter++;
		}
		mpz_add_ui(i,i,1);
	}
	return counter;
}

bool totient(mpz_t phi_n,mpz_t n){
	bool flag=true;
	long int ct=count_prime_fact(n);
	mpz_t *facts = (mpz_t*)malloc(sizeof(mpz_t)*ct);
	for(int i=0;i<ct;i++)
	{
		mpz_init(facts[i]);
	}
	long int rc = prime_factorization(facts,n);
	int i=0;
	// primitive root exists iff n is either 1,2,4 or of the form p^k or 2*p^k where p is an odd prime
	if(mpz_cmp_ui(n,1)!=0 && mpz_cmp_ui(n,2)!=0 && mpz_cmp_ui(n,4)!=0)
	{
		if(mpz_cmp_ui(facts[i],2)==0)
		{
			i++;
		}
		if(mpz_divisible_ui_p(n,4) != 0 || ct==0|| i!=ct-1)
			flag=false;
	}
	
	//calculate phi_n
	mpz_set(phi_n,n);
	for(long int i=0;i<rc;i++){
		mpz_div(phi_n,phi_n,facts[i]);
	}
	for(long int i=0;i<rc;i++){
		mpz_t term1;
		mpz_init(term1);
		mpz_sub_ui(term1,facts[i],1);
		mpz_mul(phi_n,phi_n,term1);
	}
	free(facts);
	return flag;
}

bool isOne(mpz_t a, mpz_t phi_n,mpz_t p, mpz_t n){
	mpz_t times,rem;
	mpz_init(rem);
	mpz_init(times);
	mpz_div(times,phi_n,p);
	mpz_powm(rem,a,times,n);
    if(mpz_cmp_ui(rem,1)==0) return true;
    return false;
}
int main(){
	char inputStr[1024];
	mpz_t n;
  	int flag;
	printf ("Enter n: ");
 	scanf("%1023s" , inputStr);
	
	/* 1. Initialize the number */
	mpz_init(n);
	mpz_set_ui(n,0);
	
	flag = mpz_set_str(n,inputStr, 10);
  	assert (flag == 0);  /* If flag is not 0 then the operation failed */

	mpz_t a;	
	/* 1. Initialize the number */
	mpz_init(a);
	mpz_set_ui(a,1);
	mpz_t phi_n;
	mpz_init(phi_n);
	
	mpz_t phi_phi_n;
	mpz_init(phi_phi_n);
	
	bool primitive_root_exist =totient(phi_n,n);
	
	if(primitive_root_exist==false){
		cout<<"Primitive root does'nt exist"<<endl;
	}
	else
	{
		totient(phi_phi_n,phi_n);
	
		gmp_printf("Primitive roots of %Zd : \n",n);
		mpz_t primitive_root_cnt;
		mpz_init(primitive_root_cnt);
		mpz_set_ui(primitive_root_cnt,0);

		// prime factors of phi(n)
		long int ct=count_prime_fact(phi_n);
		mpz_t *v = (mpz_t*)malloc(sizeof(mpz_t)*ct);
		long int rc = prime_factorization(v,phi_n);
		printf("No. of Prime factors  %ld ",ct);
		gmp_printf ("of %Zd\n",phi_n);
		while(mpz_cmp(n,a)>0)
		{
			mpz_t gcd;
			mpz_init(gcd);
			mpz_gcd(gcd,n,a);
			if(mpz_cmp_ui(gcd,1)==0)
			{
				//checking a^(phi(n)/p(j))mod(n) != 1 ? for all
				bool flag=true;
				for(long int i=0;i<rc && flag;i++){
					flag &= (!isOne(a, phi_n,v[i], n));
				}
				if(flag)
				{
					mpz_add_ui(primitive_root_cnt,primitive_root_cnt,1);
					gmp_printf ("%Zd, ",a);
					if(mpz_cmp(primitive_root_cnt,phi_phi_n)==0){
						break;
					}
				}
			}
			mpz_add_ui(a,a,1);
		}
		free(v);
		cout<<endl;
		gmp_printf ("Count of primitive roots: %Zd, phi(phi(n)): %Zd",primitive_root_cnt,phi_phi_n);
		cout<<endl;
	}
	return 0;
}
