#include<bits/stdc++.h>
#include<gmp.h>
using namespace std;
/*******************************************************/
/*              Linked list                            */
/*******************************************************/
typedef struct node{
	mpz_t p;
	struct node* next;
}node_t;

node_t* newnode(mpz_t p)
{
	node_t *n = new node_t();
	mpz_init(n->p);
	mpz_set(n->p,p);
    n->next = NULL;
	return n;
}

void traverse_list(node_t *head)
{
    node_t *ptr = head;
    while(ptr!=NULL)
    {
        gmp_printf("%Zd, ",ptr->p);
        ptr=ptr->next;
    }
}

node_t* insert(node_t** head_ref,mpz_t p,node_t *last=NULL)   
{ 
    node_t* new_node = new node_t();      
	mpz_init(new_node->p);
	mpz_set(new_node->p,p);
    new_node->next = NULL;   
    if (*head_ref == NULL)   
    {   
        *head_ref = new_node;  
    }   
    else
    {
        if(last==NULL)
        {
            last=*head_ref;
            while (last->next != NULL)   
                last = last->next;   
        } 
        last->next = new_node;
    } 
    return new_node;
}

/*******************************************************/
/*      Function to calculate prime factors            */
/*******************************************************/
int prime_factorization(node_t **headref,mpz_t n){
   	mpz_t i,num;
	mpz_init(i);
	mpz_set_ui(i,2);
	mpz_init(num);
	mpz_set(num,n);
    node_t *ptr=NULL;
	int count=0;
	//check for all prime numbers and everytime reduce the num by dividing with that prime to reach the result faster
	while(mpz_cmp(num,i)>=0){
		
		if(mpz_divisible_p(num,i) != 0)
		{
			while(mpz_divisible_p(num,i) != 0)
			{
				mpz_div(num,num,i);
			}
            ptr = insert(headref,i,ptr);
			count++;
		}
		//if the number after division is prime itself we can skip i to num
		mpz_t term2;
		mpz_init(term2);
		mpz_sub_ui(term2,num,1);
		mpz_nextprime(term2,term2);
		if(mpz_cmp(term2,num)==0)
		{
			mpz_set(i,num);
		}
		else{
			//else increment i to the next prime
			mpz_t temp;
			mpz_init(temp);
			mpz_nextprime(temp,i);
			mpz_set(i,temp);
		}
	}
    return count;
}


/**********************************************************************************/
/* Calculates Euler Totient as well as check condition for having primitivte roots */
/**********************************************************************************/

void totient(node_t *head,mpz_t phi_n,mpz_t n){
	mpz_set(phi_n,n);
	mpz_t i,num;
	mpz_init(i);
	mpz_set_ui(i,2);
	mpz_init(num);
	mpz_set(num,n);
	
	//Calculating Eulers Totient 
	node_t *tmp = head;
    while(tmp!=NULL)
    {
		mpz_t term1;
		mpz_init(term1);
		mpz_div(phi_n,phi_n,tmp->p);
		mpz_sub_ui(term1,tmp->p,1);
		mpz_mul(phi_n,phi_n,term1);
        tmp=tmp->next;
    }
}

// checks whether a^(phi_n/p) mod n == 1 or not 
bool isOne(mpz_t a, mpz_t phi_n,mpz_t p, mpz_t n){
	mpz_t times,rem;
	mpz_init(rem);
	mpz_init(times);
	mpz_div(times,phi_n,p);
	mpz_powm(rem,a,times,n);
    if(mpz_cmp_ui(rem,1)==0) return true;
    return false;
}

//Driver function
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
	mpz_t phi_n;
	mpz_init(phi_n);

	//check primtive roots exist or not//
	node_t *head_n = NULL;
	int rc = prime_factorization(&head_n,n);
	node_t *ptr = head_n;

	/*The numbers which have primitive roots are 
	1. 1 or 2 or 4
			or
	2. in the form of odd_prime^k
			or
	3. in the form of 2 * ( odd_prime^k )*/
	if(mpz_cmp_ui(n,1)!=0 && mpz_cmp_ui(n,2)!=0 && mpz_cmp_ui(n,4)!=0)
	{
		if(mpz_cmp_ui(ptr->p,2)==0)
		{
			ptr=ptr->next;
		}
		if(mpz_divisible_ui_p(n,4) != 0 || ptr==NULL || ptr->next!=NULL)
		{
			cout<<"No primitve roots"<<endl;
			return 0;
		}
	}

	//Calculate Totient of n
	totient(head_n,phi_n,n);
	// prime factors of phi(n)
	node_t *head = NULL;
	rc = prime_factorization(&head,phi_n);

	//phi(phi(n)) to calculate no of primitive roots
	mpz_t phi_phi_n;
	mpz_init(phi_phi_n);
	totient(head,phi_phi_n,phi_n);
	gmp_printf("No of Primitive roots of %Zd are : %Zd",n,phi_phi_n);
	cout<<endl;
	//printing all the primitive roots
	mpz_t a;	
	mpz_init(a);
	mpz_set_ui(a,1);

    gmp_printf("Primitive roots of %Zd : ",n);
	//Checking from a=1 to n-1
	while(mpz_cmp(n,a)>0)         
	{
		mpz_t gcd;
		mpz_init(gcd);
		mpz_gcd(gcd,n,a);
		//Check if n and a are co-prime
		if(mpz_cmp_ui(gcd,1)==0)
		{
			
			bool flag=true;
            node_t *ptr = newnode(head->p);
            ptr->next = head->next;

			// a^(phi(n)/p(j))mod(n) != 1 ?? for all
			while(ptr!=NULL && flag)
			{
				flag &= (!isOne(a, phi_n,ptr->p, n));
                ptr=ptr->next;
			}

			//if exist print root
			if(flag)
			{
				gmp_printf ("%Zd, ",a);
			}
		}
		//incrment a
		mpz_add_ui(a,a,1);
	}
	cout<<endl;
	return 0;
}
