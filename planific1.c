#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct plan{
	int timp;
	char nume[21];
	int prioritate;
	int toruning ; 
	int towaiting;
	struct plan*next;
}plan;

void init(plan **proces)
{	
	*proces = NULL;
}

void push(plan **proces, char *num, int tp, int pr,int torun, int towait)
{
	plan *actual;
	actual = (plan*)malloc(sizeof(plan));
	strcpy(actual->nume,num);
	actual->timp = tp;
	actual->prioritate = pr;
	actual->toruning = torun;
	actual->towaiting = towait;
	actual->next = NULL;

	if((*proces) == NULL)
	{
		(*proces) = actual;
	}
	else
	{
		actual->next = *proces;
		(*proces) = actual;
	}
}

plan* pop(plan **proces)
{
	plan *actual, *aux, *urmator;
	actual = *proces;

	if(actual == NULL)
	{
		return NULL;
	}	
	urmator = (*proces) -> next;

	if(urmator==NULL)
	{
		aux = actual;
		(*proces)=NULL;
		return aux;
	}

	while(urmator->next != NULL)
	{
		actual = urmator;
		urmator = urmator->next;
	}

	aux = urmator;
	actual->next = NULL;

	return aux;
}

plan* search(plan **proces,char *nume)
{
	plan *actual, *aux, *urmator;
	actual = (*proces);

	if(strcmp(actual->nume,nume)==0)
	{	
		aux = actual;
		(*proces) = (*proces)->next;
		return aux;
	}

	urmator = (*proces) -> next;

	if(strcmp(urmator->nume,nume)==0)
	{
		aux = urmator;
		actual->next = urmator->next;
		return aux;
	}

	while(urmator->next != NULL && strcmp(urmator->nume,nume)!=0)
	{
		actual = urmator;
		urmator =urmator->next;
	}
	
	aux = urmator;
	actual->next = urmator->next;
	return aux;

}

plan* search_time(plan *ready)
{
	plan *aux;
	aux = ready;

	while(ready!=NULL)
	{
		if(ready->timp <= aux->timp)
			aux = ready;
		ready = ready -> next;
	}

	printf("Nodul cautat este %s\n",aux->nume );
	return aux;
}

plan* search_priority(plan* ready)
{
	plan *aux;
	aux = ready;

	while(ready != NULL)
	{
		if(ready->prioritate >= aux ->prioritate)
			aux = ready;
		ready = ready -> next;
	}
	return aux;
}

void show(plan *proces)
{

	if(proces ==NULL)
		printf("UPS\n");

	while(proces!=NULL)
	{
		printf("%s %d ",proces->nume,proces->timp); //pune backspace la sf liniei
		proces = proces->next;
	}

}

void fcsf(FILE *fisier, FILE *iesire)
{
	char comanda[100];
	plan *running;
	plan *ready;
	plan *waiting;
	plan *aux;
	int timp;
	int prioritate;
	int torun =0;
	int towait= 0;
	char nume[21];

	init(&running);
	init(&ready);
	init(&waiting);

	while(fscanf(fisier,"%s",comanda) != EOF )
	{
		
		if(strcmp(comanda,"a")==0) 
		{
			//printf("s a citit ADD\n" );
			fscanf(fisier,"%s",nume);
			fscanf(fisier,"%d",&timp);
			fscanf(fisier,"%d",&prioritate);
			push(&ready, nume, timp, prioritate,torun,towait);
				
			//printf("s a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready !=NULL)
							running=pop(&ready);
					}
				}
			}
		}
		
		if(strcmp(comanda,"t")==0)
		{	
			//printf("s a citit comanda TICK EXPLICIT\n");
			if(running == NULL)
			{
				if(ready != NULL)
				{
					running = pop(&ready);	
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
			
					if(running -> timp == 0)
					{
						init(&running);
						if(ready !=NULL)
							running=pop(&ready);
					}
				} 	
			}
		}
	
		if(strcmp(comanda,"s")==0)
		{
			//printf("s a citit comanda SHOW\n");
			if(running == NULL)
			{
				fprintf(iesire, "\n");
			}
			else
			if(running !=NULL && running->timp !=0 )
			{	
				fprintf(iesire, "%s %d\n", running -> nume,running->timp);
			}
		}
	
		if(strcmp(comanda,"w")==0)
		{	
			//printf("s a citit comanda WAITING\n");
			push(&waiting,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
			init(&running);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready !=NULL)
							running=pop(&ready);
					}
				}
			}
			
		}
	
		if(strcmp(comanda,"e")==0)
		{
			//printf("s a citit comanda EVENT\n");
			fscanf(fisier,"%s",nume);
			aux = search(&waiting,nume);
			push(&ready,aux->nume,aux->timp,aux->prioritate,aux->toruning,aux->towaiting);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready != NULL)
							running=pop(&ready);
					}
				}
			}
		}

	 	if(strcmp(comanda,"ma")==0)
	 	{
	 		//printf("s a citit MULTIPLE ADD\n" );
	 		fscanf(fisier,"%s",nume);
	 		while((strcmp(nume,"a")!=0) && strcmp(nume,"s")!=0 && strcmp(nume,"t")!=0 && strcmp(nume,"w")!=0 && strcmp(nume,"e")!=0  )
	 		{
				fscanf(fisier,"%d",&timp);
				fscanf(fisier,"%d",&prioritate);
				push(&ready, nume, timp, prioritate,torun,towait);
				fscanf(fisier,"%s",nume);
	 		}

	 		if(strcmp(nume,"ma")==0)
	 			fseek(fisier,-2,SEEK_CUR);
	 		else
	 			fseek(fisier,-1,SEEK_CUR);

	 		//printf("s a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{	
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready != NULL)
							running=pop(&ready);
					}
				}
			}
	 	}
	}
	
}


void SJF(FILE *fisier, FILE *iesire)
{
	char comanda[100];
	plan *running;
	plan *ready;
	plan *waiting;
	plan *aux;
	plan *elem;
	int timp;
	int prioritate;
	int torun =0;
	int towait= 0;
	char nume[21];

	init(&running);
	init(&ready);
	init(&waiting);

	while(fscanf(fisier,"%s",comanda) != EOF )
	{
		
		if(strcmp(comanda,"a")==0) 
		{
			//printf("s a citit ADD\n" );
			fscanf(fisier,"%s",nume);
			fscanf(fisier,"%d",&timp);
			fscanf(fisier,"%d",&prioritate);
			push(&ready, nume, timp, prioritate,torun,towait);
				
			//printf("s a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					elem = search_time(ready);
					running = search(&ready,elem->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready != NULL)
						{
							elem = search_time(ready);
							running = search(&ready,elem->nume);
						}
					}
				}
			}
		}
		
		if(strcmp(comanda,"t")==0)
		{	
			//printf("s a citit comanda TICK EXPLICIT\n");
			if(running == NULL)
			{
				if(ready != NULL)
				{
					elem = search_time(ready);
					running = search(&ready,elem->nume);		
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
				
					if(running -> timp == 0)
					{	
						init(&running);
						if(ready!= NULL)
						{
							elem = search_time(ready);
							running = search(&ready,elem->nume);
						}
					}
				} 	
			}
		}
	
		if(strcmp(comanda,"s")==0)
		{
			//printf("s a citit comanda SHOW\n");
			if(running == NULL)
			{
				fprintf(iesire, "\n");
			}
			else
				if(running !=NULL && running->timp !=0 )
				{	
					fprintf(iesire, "%s %d\n", running -> nume,running->timp);
				}
		}
	
		if(strcmp(comanda,"w")==0)
		{	
			//printf("s a citit comanda WAITING\n");
			push(&waiting,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
			//printf("s a primit tick IMPLICIT cred\n");
			init(&running);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					elem = search_time(ready);
					running = search(&ready,elem->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready != NULL)
						{
							elem = search_time(ready);
							running = search(&ready,elem->nume);
						}
					}
				}
			}
		}
	
		if(strcmp(comanda,"e")==0)
		{
			//printf("s a citit comanda EVENT\n");
			fscanf(fisier,"%s",nume);
			aux = search(&waiting,nume);
			push(&ready,aux->nume,aux->timp,aux->prioritate,aux->toruning,aux->towaiting);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					elem = search_time(ready);
					running = search(&ready,elem->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready != NULL)
						{
							elem = search_time(ready);
							running = search(&ready,elem->nume);
						}
					}
				}
			}
		}

	 	if(strcmp(comanda,"ma")==0)
	 	{
	 		printf("s a citit MULTIPLE ADD\n" );
	 		fscanf(fisier,"%s",nume);
	 		while((strcmp(nume,"a")!=0) && strcmp(nume,"s")!=0 && strcmp(nume,"t")!=0 && strcmp(nume,"w")!=0 && strcmp(nume,"e")!=0  )
	 		{
				fscanf(fisier,"%d",&timp);
				fscanf(fisier,"%d",&prioritate);
				push(&ready, nume, timp, prioritate,torun,towait);
				fscanf(fisier,"%s",nume);
	 		}

			if(strcmp(nume,"ma")==0)
	 			fseek(fisier,-2,SEEK_CUR);
	 		else
	 			fseek(fisier,-1,SEEK_CUR);

	 		//printf("s a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{	
					elem = search_time(ready);
					running = search(&ready,elem->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{	
					running->timp--;
				
					if(running->timp == 0)
					{
						init(&running);
						if(ready != NULL)
						{	
							elem = search_time(ready);
							running = search(&ready,elem->nume);
						}
					}
				}
			}
	 	}
	}	
}


void RR(FILE *fisier, FILE *iesire)
{
	char comanda[100];
	plan *running;
	plan *ready;
	plan *waiting;
	plan *aux;
	int timp;
	int prioritate;
	int torun =0;
	int towait= 0;
	char nume[21];

	init(&running);
	init(&ready);
	init(&waiting);
	int cuanta_stabilita;
	int contor;

	fscanf(fisier,"%d",&cuanta_stabilita);

	while(fscanf(fisier,"%s",comanda) != EOF )
	{
		if(strcmp(comanda,"a")==0) 
		{
			//printf("s a citit ADD\n" );
			fscanf(fisier,"%s",nume);
			fscanf(fisier,"%d",&timp);
			fscanf(fisier,"%d",&prioritate);
			push(&ready, nume, timp, prioritate,torun,towait);
				
			//printf("s a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					contor = 0;
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
							running = pop(&ready);
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
								running = pop(&ready);
						}
					}
				}
			}
		}
		
		if(strcmp(comanda,"t")==0)
		{	
			//printf("\ns a citit comanda TICK EXPLICIT\n");
			if(running == NULL)
			{
				if(ready != NULL)
				{
					contor = 0;
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;
					
					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
							running = pop(&ready);
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
								running = pop(&ready);
						}
					}
				} 	
			}
		}
	
		if(strcmp(comanda,"s")==0)
		{
			//printf("s a citit comanda SHOW\n");
			if(running == NULL)
			{
				fprintf(iesire, "\n");
			}
			else
			if(running !=NULL && running->timp !=0 )
			{	
				fprintf(iesire, "%s %d\n", running -> nume,running->timp);
			}
		}
	
		if(strcmp(comanda,"w")==0)
		{	
			//printf("s a citit comanda WAITING\n");
			push(&waiting,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
			//printf("s a primit tick IMPLICIT cred\n");
			init(&running);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					contor = 0;
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
							running = pop(&ready);
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
								running = pop(&ready);
						}
					}
				}
			}
			
		}
	
		if(strcmp(comanda,"e")==0)
		{
			//printf("s a citit comanda EVENT\n");
			fscanf(fisier,"%s",nume);
			aux = search(&waiting,nume);
			push(&ready,aux->nume,aux->timp,aux->prioritate,aux->toruning,aux->towaiting);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					contor = 0;
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
							running = pop(&ready);
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
								running = pop(&ready);
						}
					}
				}
			}
		}

	 	if(strcmp(comanda,"ma")==0)
	 	{
	 		//printf("s a citit MULTIPLE ADD\n" );
	 		fscanf(fisier,"%s",nume);
	 		while((strcmp(nume,"a")!=0) && strcmp(nume,"s")!=0 && strcmp(nume,"t")!=0 && strcmp(nume,"w")!=0 && strcmp(nume,"e")!=0 && strcmp(nume,"ma")!=0 )
	 		{
				fscanf(fisier,"%d",&timp);
				fscanf(fisier,"%d",&prioritate);
				push(&ready, nume, timp, prioritate,torun,towait);
				fscanf(fisier,"%s",nume);
	 		}

	 		if(strcmp(nume,"ma")==0)
	 			fseek(fisier,-2,SEEK_CUR);
	 		else
	 			fseek(fisier,-1,SEEK_CUR);

	 		//printf("\ns a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					contor = 0;
					running = pop(&ready);
				}
			}
			else
			{
				if(running->timp != 0)
				{	
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
							running = pop(&ready);
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
								running = pop(&ready);
						}
					}
				}
			}
	 	}
	}	
}

void PP(FILE* fisier, FILE* iesire)
{
	char comanda[100];
	plan *running;
	plan *ready;
	plan *waiting;
	plan *aux;
	plan *temp;
	int timp;
	int prioritate;
	int torun =0;
	int towait= 0;
	char nume[21];

	init(&running);
	init(&ready);
	init(&waiting);
	int cuanta_stabilita;
	int contor;
	int prior;

	fscanf(fisier,"%d",&cuanta_stabilita);
	fscanf(fisier,"%d",&prior);

	while(fscanf(fisier,"%s",comanda) != EOF )
	{
		
		if(strcmp(comanda,"a")==0) 
		{
			//printf("s a citit ADD\n" );
			fscanf(fisier,"%s",nume);
			fscanf(fisier,"%d",&timp);
			fscanf(fisier,"%d",&prioritate);
			push(&ready, nume, timp, prioritate,torun,towait);
				
			//printf("s a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);
				}
			}
			else
			{	
				if(running->timp != 0)
				{
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}	
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}	
						}
						else
						{
							if(ready != NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}				
			}
		}
		
		if(strcmp(comanda,"t")==0)
		{	
			//printf("\ns a citit comanda TICK EXPLICIT\n");
			if(running == NULL)
			{
				if(ready != NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);	
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;
					
					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}
						}
						else
						{	
							if(ready != NULL)
							{	
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}
			} 			
		}	
	
		if(strcmp(comanda,"s")==0)
		{
			//printf("s a citit comanda SHOW\n");
			if(running == NULL)
			{
				fprintf(iesire, "\n");
			}
			else
			if(running !=NULL && running->timp !=0 )
			{	
				fprintf(iesire, "%s %d\n", running -> nume,running->timp);
			}
		}
	
		if(strcmp(comanda,"w")==0)
		{	
			//printf("s a citit comanda WAITING\n");
			push(&waiting,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
			//printf("s a primit tick IMPLICIT cred\n");
			init(&running);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{	
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
							//printf("runing %s\n",running->nume );
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
								//printf("runing %s\n",running->nume );
							}
						}
						else
						{	
							if(ready !=NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
									//printf("runing %s\n",running->nume );
								}
							}
						}
					}
				}
			}
		}
			
	
		if(strcmp(comanda,"e")==0)
		{
			//printf("s a citit comanda EVENT\n");
			fscanf(fisier,"%s",nume);
			aux = search(&waiting,nume);
			push(&ready,aux->nume,aux->timp,aux->prioritate,aux->toruning,aux->towaiting);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}
						}
						else
						{
							if(ready != NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}
			}
		}
				

	 	if(strcmp(comanda,"ma")==0)
	 	{
	 		//printf("s a citit MULTIPLE ADD\n" );
	 		fscanf(fisier,"%s",nume);
	 		while((strcmp(nume,"a")!=0) && strcmp(nume,"s")!=0 && strcmp(nume,"t")!=0 && strcmp(nume,"w")!=0 && strcmp(nume,"e")!=0 && strcmp(nume,"ma")!=0 )
	 		{
				fscanf(fisier,"%d",&timp);
				fscanf(fisier,"%d",&prioritate);
				push(&ready, nume, timp, prioritate,torun,towait);
				fscanf(fisier,"%s",nume);
				
	 		}

	 		if(strcmp(nume,"ma")==0)
	 			fseek(fisier,-2,SEEK_CUR);
	 		else
	 			fseek(fisier,-1,SEEK_CUR);

	 		//printf("\ns a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{	
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}	
						}
						else
						{
							if(ready!= NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}
			}
		}									
	}	
}

void bonus(FILE* fisier, FILE* iesire)
{
	char comanda[100];
	plan *running;
	plan *ready;
	plan *waiting;
	plan *aux;
	plan *temp;
	int timp;
	int prioritate;
	char nume[21];

	init(&running);
	init(&ready);
	init(&waiting);
	int cuanta_stabilita;
	int contor;
	int prior;
	int towait=0,torun=0;

	fscanf(fisier,"%d",&cuanta_stabilita);
	fscanf(fisier,"%d",&prior);

	while(fscanf(fisier,"%s",comanda) != EOF )
	{
		
		if(strcmp(comanda,"a")==0) 
		{
			//printf("s a citit ADD\n" );
			fscanf(fisier,"%s",nume);
			fscanf(fisier,"%d",&timp);
			fscanf(fisier,"%d",&prioritate);
			push(&ready, nume, timp, prioritate,torun,towait);
				
			//printf("s a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);
				}
			}
			else
			{	
				if(running->timp != 0)
				{
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}	
					}
					else
					{
						if(contor == cuanta_stabilita)
						{	
							running->toruning++;
							if(running->toruning == 2)
							{
								running->prioritate --;
								running->toruning = 0;
							}
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}	
						}
						else
						{
							if(ready != NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{	
									running->toruning++;
									if(running->toruning == 2)
									{
										running->prioritate --;
										running->toruning = 0;
									}
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}				
			}
		}
		
		if(strcmp(comanda,"t")==0)
		{	
			//printf("\ns a citit comanda TICK EXPLICIT\n");
			if(running == NULL)
			{
				if(ready != NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);	
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;
					
					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{	
							running->toruning++;
							if(running->toruning == 2)
							{
								running->prioritate --;
								running->toruning = 0;
							}

							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}
						}
						else
						{	
							if(ready != NULL)
							{	
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{	
									running->toruning++;
									if(running->toruning == 2)
									{
										running->prioritate --;
										running->toruning = 0;
									}
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}
			} 		
		}	
	
		if(strcmp(comanda,"s")==0)
		{
			//printf("s a citit comanda SHOW\n");
			if(running == NULL)
			{
				fprintf(iesire, "\n");
			}
			else
				if(running !=NULL && running->timp !=0 )
			{	
				fprintf(iesire, "%s %d\n", running -> nume,running->timp);
			}
		}
	
		if(strcmp(comanda,"w")==0)
		{	
			//printf("s a citit comanda WAITING\n");
			running->towaiting++;
			if(running->towaiting == 2)
			{
				running->prioritate ++;
				running->towaiting =0;
			}
			push(&waiting,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
			init(&running);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{	
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{	
							running->toruning++;
							if(running->toruning == 2)
							{
								running->prioritate --;
								running->toruning = 0;
							}
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}
						}
						else
						{	
							if(ready !=NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{
									running->toruning++;
									if(running->toruning == 2)
									{
										running->prioritate --;
										running->toruning = 0;
									}
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}
			}
		}
			
	
		if(strcmp(comanda,"e")==0)
		{
			//printf("s a citit comanda EVENT\n");
			fscanf(fisier,"%s",nume);
			aux = search(&waiting,nume);
			push(&ready,aux->nume,aux->timp,aux->prioritate,aux->toruning,aux->towaiting);

			if(running == NULL)
			{
				if(ready !=NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);
				}
			}
			else
			{
				if(running->timp != 0)
				{
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							running->toruning++;
							if(running->toruning == 2)
							{
								running->prioritate --;
								running->toruning = 0;
							}
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}
						}
						else
						{
							if(ready != NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{	
									running->toruning++;
									if(running->toruning == 2)
									{
										running->prioritate --;
										running->toruning = 0;
									}
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}
			}
		}
				

	 	if(strcmp(comanda,"ma")==0)
	 	{
	 		printf("s a citit MULTIPLE ADD\n" );
	 		fscanf(fisier,"%s",nume);
	 		while((strcmp(nume,"a")!=0) && strcmp(nume,"s")!=0 && strcmp(nume,"t")!=0 && strcmp(nume,"w")!=0 && strcmp(nume,"e")!=0 && strcmp(nume,"ma")!=0 )
	 		{
				fscanf(fisier,"%d",&timp);
				fscanf(fisier,"%d",&prioritate);
				push(&ready, nume, timp, prioritate,torun,towait);
				fscanf(fisier,"%s",nume);
			}

	 		if(strcmp(nume,"ma")==0)
	 			fseek(fisier,-2,SEEK_CUR);
	 		else
	 			fseek(fisier,-1,SEEK_CUR);

	 		//printf("\ns a primit comanda TICK IMPLICIT\n");	
			if(running == NULL)
			{	
				if(ready != NULL)
				{
					contor = 0;
					temp = search_priority(ready);
					running = search(&ready,temp->nume);					
				}
			}
			else
			{
				if(running->timp != 0)
				{	
					running->timp--;
					contor++;

					if(running->timp == 0)
					{
						contor = 0;
						init(&running);
						if(ready != NULL)
						{
							temp = search_priority(ready);
							running = search(&ready,temp->nume);
						}
					}
					else
					{
						if(contor == cuanta_stabilita)
						{
							running->toruning++;
							if(running->toruning == 2)
							{
								running->prioritate --;
								running->toruning = 0;
							}
							push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
							init(&running);
							contor = 0;
							if(ready != NULL)
							{
								temp = search_priority(ready);
								running = search(&ready,temp->nume);
							}
						}
						else
						{
							if(ready!= NULL)
							{
								temp = search_priority(ready);
								if(temp->prioritate > running->prioritate)
								{	
									running->toruning++;
									if(running->toruning == 2)
									{
										running->prioritate --;
										running->toruning = 0;
									}
									push(&ready,running->nume,running->timp,running->prioritate,running->toruning,running->towaiting);
									init(&running);
									running = search(&ready,temp->nume);
								}
							}
						}
					}
				}
			}
		}							
	}	
}

int main(int argc, char **argv)
{
	if(argc == 1)
	{
		printf("[Eroare]: Nu s-au dat argumente de comanda.\n ");
		return 1;
	}

	if(argc == 2)
	{
		printf("Nu avem fisier de output\n");
		return 1;
	}

	FILE *fisier, *iesire;
	int tip;
	
	fisier = fopen(argv[1], "r");
	iesire = fopen(argv[2], "w");
	if(fisier == NULL)
	{
		printf("[Eroare]: Fisierul %s nu poate fi deschis.\n", argv[1]);
		return 0;
	}

	fscanf(fisier,"%d",&tip);
	char c;
	fscanf(fisier, "%c", &c);
	plan *ready, *waiting;
	init(&ready);
	init(&waiting);

	switch(tip)
	{
		case 1 :
			fcsf(fisier, iesire);
		break;

		case 2 :
			SJF(fisier, iesire);
		break;

		case 3 :
			RR(fisier, iesire);
		break;

		case 4 :
			PP(fisier, iesire);
		break;

		case 5:
			bonus(fisier, iesire);

	}

	fclose(fisier);
	fclose(iesire);
	return 0;

}
