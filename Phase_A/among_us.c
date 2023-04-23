/***************************************************
 *                                                 *
 * file: among_us.c                                *
 *                                                 *
 * @Author  Antonios Peris                         *
 * @Version 20-10-2020                             *
 * @email   csdp1196@csd.uoc.gr                    *
 *                                                 *
 * @brief   Implementation of among_us.h           *
 *                                                 *
 ***************************************************
 */

#include "among_us.h"
#include <assert.h>
#include <stdbool.h>

 
struct Players *new_player(int pid, int is_alien);
struct Tasks *new_task(int tid, int difficulty);


int Total_Tasks = 0;


struct Players *new_player(int pid,int is_alien) {	
	struct Players *node;
	node = (struct Players*)malloc(sizeof(struct Players));
	assert(node != NULL);
	node->pid = pid;
	node->is_alien = is_alien;
	node->evidence = 0;
	node->next = NULL;
	node->prev = NULL;
	node->tasks_head = NULL;
	node->tasks_sentinel = NULL;
	return node;
}

struct Tasks *new_task(int tid, int difficulty) {
	struct Tasks *node;
	node = (struct Tasks*)malloc(sizeof(struct Tasks));
	assert(node != NULL);
	node->tid = tid;
	node->difficulty = difficulty;
	node->next = NULL;
	return node;
}


struct Players *find_player(int pid) {
	struct Players *curr = players_head->next;
	if (players_head == NULL) {
		return NULL;
	}
	while (curr != players_head) {
		if (curr->pid == pid) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

struct Tasks *find_task(struct Players *pl,int difficulty) {
	struct Tasks *prev;
	struct Tasks *curr = pl->tasks_head;
	struct Tasks *max=pl->tasks_head;
	if(pl->tasks_head == pl->tasks_sentinel) {
		return NULL;
	}
	
	while (curr != pl->tasks_sentinel) {
		prev = curr;
		if (curr->difficulty == difficulty) {
			
			if(curr->next == pl->tasks_sentinel) {
				curr = pl->tasks_head;
				while(curr != pl->tasks_sentinel) {
					if(curr->next == prev) {
						return curr;
					}
					curr=curr->next;
				}
			}
			return prev;
		}
		else {
			if (max->difficulty < curr->difficulty) {
				max = prev;
			}
		}
		
		curr = curr->next;
	}
	if(max->next == pl->tasks_sentinel) {
		curr = pl->tasks_head;
		while(curr != pl->tasks_sentinel) {
			if(curr->next == max) {
				return curr;
			}
			curr = curr->next;
		}
	}
	return max;
}


bool IsEmptyStack() {
	if (tasks_stack->head == NULL) {
		return true;
	}
	else
	{
		return false;
	}
}

struct Tasks *Top() {
	if(tasks_stack->head==NULL) return NULL;
	return tasks_stack->head;
}


void Push(struct Tasks *task) {
	tasks_stack->count++;
	task->next = tasks_stack->head;
	tasks_stack->head = task;
}

void Pop() {
	struct Tasks *tmp = tasks_stack->head;
	tasks_stack->head = tasks_stack->head->next;
	tasks_stack->count--;
	free(tmp);
}

void PrintStack() {
	struct Tasks *tmp = tasks_stack->head,*top_clone;
	if (IsEmptyStack()) {
		return ;
	}
	tmp = Top();
	top_clone = new_task(tmp->tid,tmp->difficulty);
	Pop();
	printf("<%d:%d>", top_clone->tid, top_clone->difficulty);
	PrintStack();
	Push(top_clone);
}

struct Players *find_min(int *min_tasks_number){
	int curr_pl_min_count;
	struct Tasks* curr_pl_min_tsk;
	struct Players* min_pl;	
	struct Players* curr_pl=players_head->next;	
	struct Tasks* min_tsk=curr_pl->tasks_head;
	int min_count=0;
	assert(curr_pl != players_head);
	assert(players_head != NULL);
	while(curr_pl != players_head) {
		if(curr_pl->is_alien == 0) {
			break;
		}
		curr_pl = curr_pl->next;
	}
	min_pl = curr_pl;
	min_tsk=curr_pl->tasks_head;
	while(min_tsk!=min_pl->tasks_sentinel){
		min_count++;
		min_tsk=min_tsk->next;
	}
	min_pl=curr_pl;
	curr_pl = curr_pl->next;

	while(curr_pl!=players_head){
		if(curr_pl->is_alien == 1) {
			curr_pl = curr_pl->next;
			continue;
		}
		curr_pl_min_count = 0;
		curr_pl_min_tsk = curr_pl->tasks_head;
		while(curr_pl_min_tsk != curr_pl->tasks_sentinel) {
			curr_pl_min_count++;
			curr_pl_min_tsk = curr_pl_min_tsk->next;
		}
		if(curr_pl_min_count < min_count) {
			min_count = curr_pl_min_count;
			min_pl = curr_pl;
		}
		curr_pl = curr_pl->next;
	}
	*min_tasks_number = min_count;
	return min_pl;
}

void merge(struct Players *pl1,struct Players *pl2){        
    struct Tasks *tsk1, *tsk2, *prev, *temp1;
    tsk1 = pl1->tasks_head;
    tsk2 = pl2->tasks_head;
    prev = tsk2;
	if(tsk1==pl1->tasks_sentinel)return;

	//if(tsk1==pl1->tasks_sentinel){

	//}
   
    while(tsk1!=pl1->tasks_sentinel && tsk2!=pl2->tasks_sentinel){
        if(tsk2->difficulty < tsk1->difficulty){
            prev=tsk2;
            tsk2=tsk2->next;
        }
        else{
            temp1=tsk1->next;
            if(tsk1==pl1->tasks_head){
                pl1->tasks_head=tsk1->next;
            }
            if(tsk2==pl2->tasks_head){
                tsk1->next=tsk2;
                pl2->tasks_head=tsk1;
                prev=pl2->tasks_head;
                tsk2=pl2->tasks_head;
            }
            else{
                tsk1->next=tsk2;
                prev->next=tsk1;
                prev=tsk1;
            }
            tsk1=temp1;
        }
    }
   
    if(tsk2 == pl2->tasks_sentinel && tsk1 != pl1->tasks_sentinel){
        assert(tsk1 == pl1->tasks_head);

        while(tsk1 != pl1->tasks_sentinel) {
            if(prev == pl2->tasks_sentinel) {
                temp1 = tsk1->next;
                pl2->tasks_head = tsk1;
                tsk1->next = pl2->tasks_sentinel;
                prev = tsk1;
                pl2->tasks_sentinel->next = tsk1;
                tsk1 = temp1;
                pl1->tasks_head = tsk1;
               
            }
            else {
                pl1->tasks_head = tsk1->next;
                tsk1->next = prev->next;
                if(prev->next == pl2->tasks_sentinel) {
                    pl2->tasks_sentinel->next = tsk1;
                }
                prev->next = tsk1;
                prev = tsk1;
                tsk1 = pl1->tasks_head;
            }
        }
	}
}

void delete(struct Players *pl){
	struct Players *curr_pl,*prev_pl;
	struct Tasks *curr_tsk,*prev_tsk;
	curr_tsk=pl->tasks_head;
	prev_tsk=curr_tsk;
	while(curr_tsk!=pl->tasks_sentinel){
		if(curr_tsk==pl->tasks_head){
			pl->tasks_head=curr_tsk->next;
			free(curr_tsk);
		}
		else{
			prev_tsk->next=curr_tsk->next;
			free(curr_tsk);
		}
		prev_tsk=curr_tsk;
		curr_tsk=curr_tsk->next;
	}

	if(players_head->next==players_head ||pl==NULL){
		return;
	}
	if(pl==players_head->next){
		players_head->next=pl->next;
	}
	if(pl->next!=players_head){
		pl->next->prev=pl->prev;
	}
	if(pl->prev!=players_head){
		pl->prev->next=pl->next;
	}
	free(pl);

	/*
	curr_pl=players_head->next;
	while(curr_pl!=players_head){
		if(curr_pl==players_head->next && curr_pl->pid==pl->pid){
			players_head->next=curr_pl->next;
			free(curr_pl);
			return;
		}
		if(curr_pl->pid==pl->pid){
			prev_pl->next=curr_pl->next;
			free(curr_pl);
			return;
		}
		prev_pl=curr_pl;
		curr_pl=curr_pl->next;
	}*/
	return;
}


struct Players *find_player_with_max_evidence() {
	struct Players *curr = players_head->next;
	struct Players *max_player=curr;
	if (players_head == NULL) {
		return NULL;
	}
	while (curr != players_head) {
		if (curr->evidence > max_player->evidence) {
			max_player=curr;
		}
		curr = curr->next;
	}
	return max_player;
}


struct Players *find_max(int *max_tasks_number){
	int curr_pl_max_count;
	struct Tasks* curr_pl_max_tsk;
	struct Players* max_pl;	
	struct Players* curr_pl=players_head->next;	
	struct Tasks* max_tsk=curr_pl->tasks_head;
	int max_count=0;
	assert(players_head != NULL);
	assert(curr_pl != players_head);

	while(curr_pl != players_head) {
		if(curr_pl->is_alien == 0) {
			break;
		}
		curr_pl = curr_pl->next;
	}
	max_pl = curr_pl;
	max_tsk=curr_pl->tasks_head;
	while(max_tsk!=max_pl->tasks_sentinel){
		max_count++;
		max_tsk=max_tsk->next;
	}
	max_pl=curr_pl;
	curr_pl = curr_pl->next;
	while(curr_pl!=players_head){
		if(curr_pl->is_alien == 1) {
			curr_pl = curr_pl->next;
			continue;
		}
		curr_pl_max_count = 0;
		curr_pl_max_tsk = curr_pl->tasks_head;
		while(curr_pl_max_tsk != curr_pl->tasks_sentinel) {
			curr_pl_max_count++;
			curr_pl_max_tsk = curr_pl_max_tsk->next;
		}
		if(curr_pl_max_count > max_count) {
			max_count = curr_pl_max_count;
			max_pl = curr_pl;
		}
		curr_pl = curr_pl->next;
	}
	*max_tasks_number = max_count;
	return max_pl;
}

struct Players *find_first_sabotage_player(int pid,int number_of_tasks){
	struct Players *curr=find_player(pid);
	struct Players *sabbotaged_player=curr;
	struct Players *temp;
	int players_left=number_of_tasks/2;
	while(players_left!=0){
		sabbotaged_player=sabbotaged_player->prev;
		players_left--;
	}
	temp = sabbotaged_player;
	while(sabbotaged_player->is_alien == 1) {
		sabbotaged_player = sabbotaged_player->next;
		if(temp == sabbotaged_player) {
			return NULL;
		}
	}
	return sabbotaged_player;
}


 void sorted_insert_task(struct Players * pl, struct Tasks* task){
	struct Tasks * prev,*curr;
	assert(pl->is_alien != 1);
	curr=pl->tasks_head;
	prev=curr;
	if(curr == pl->tasks_sentinel || curr->difficulty > task->difficulty) {
		task->next = pl->tasks_head;
		if(curr == pl->tasks_sentinel) {
			pl->tasks_sentinel->next = task;
		}
		pl->tasks_head = task;
		return;
	}
	while(curr!=pl->tasks_sentinel){
		if(curr->difficulty > task->difficulty){
			prev->next=task;
			task->next=curr;
			return;
		}
		prev=curr;
		curr=curr->next;
	}
	prev->next = task;
	task->next = pl->tasks_sentinel;
	pl->tasks_sentinel->next = task;
 
 }
 
 void _print(){
	 struct Players *curr_pl=players_head->next;
	 struct Tasks *curr_tsk;
	while (curr_pl != players_head) {
		printf("<Player-%d, %d> = ", curr_pl->pid,curr_pl->evidence);
		
		curr_tsk = curr_pl->tasks_head;
		while (curr_tsk != curr_pl->tasks_sentinel) {
			printf("<%d:%d>", curr_tsk->tid, curr_tsk->difficulty);
			if (curr_tsk != curr_pl->tasks_sentinel) {
				printf(", ");
			}
			curr_tsk = curr_tsk->next;
		}
		curr_pl=curr_pl->next; 
		printf("\n");
	}
	printf("\nDONE\n\n"); 
	return;
 }




/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize() {
	int i;
	players_head = new_player(-1,-1);
	players_head->evidence = -1;
	players_head->tasks_head = NULL;
	players_head->tasks_sentinel = NULL;
	players_head->next = players_head;
	players_head->prev = players_head;

	tasks_head = (struct Head_GL*)malloc(sizeof(struct Head_GL));
	tasks_head->head = NULL;
	for (i = 0; i < 3; i++) {
		tasks_head->tasks_count[i] = 0;

	}
	tasks_stack = (struct Head_Completed_Task_Stack*)malloc(sizeof(struct Head_Completed_Task_Stack));
	tasks_stack->count = 0;
	tasks_stack->head = NULL;;

    return 1;
}

/**
 * @brief Register player
 *
 * @param pid The player's id
 *
 * @param is_alien The variable that decides if he is an alien or not
 * @return 1 on success
 *         0 on failure
 */
int register_player(int pid,int is_alien) {

	struct Players *new_node = new_player(pid,is_alien);
	struct Tasks *sent;

	sent = (struct Tasks*)malloc(sizeof(struct Tasks));
	sent->tid = -1;
	sent->difficulty = 0;
	sent->next = new_node->tasks_sentinel;

	new_node->tasks_head = sent;
	new_node->tasks_sentinel = sent;

	assert(players_head != NULL);
	players_head->next->prev = new_node;
	new_node->prev = players_head;
	new_node->next = players_head->next;
	players_head->next = new_node;

	print_players();
    return 1;
}



/**
 * @brief Insert task in the general task list
 *
 * @param tid The task id
 * 
 * @param difficulty The difficulty of the task
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_task(int tid,int difficulty) {

	struct Tasks *curr_task, *prev_task, *new_node;
	new_node = new_task(tid, difficulty);
	Total_Tasks++;
	if (new_node->difficulty == 1) tasks_head->tasks_count[0]++;
	else if (new_node->difficulty == 2) tasks_head->tasks_count[1]++;
	else tasks_head->tasks_count[2]++;

	if (tasks_head->head == NULL) {
		tasks_head->head = new_node;
		print_tasks();
		return 1;
	}
	prev_task = tasks_head->head;
	curr_task = tasks_head->head;
	if (difficulty < tasks_head->head->difficulty) {
		new_node->next = tasks_head->head;
		tasks_head->head = new_node;
		print_tasks();
		return 1;
	}
	while (curr_task != NULL) {
		if (curr_task->difficulty > difficulty) {
			prev_task->next = new_node;
			new_node->next = curr_task;
			print_tasks();
			return 1;
		}
		prev_task = curr_task;
		curr_task = curr_task->next;
	}
	if (curr_task == NULL) {
		prev_task->next = new_node;
	}
	print_tasks(); 
    return 1;
}

/**
 * @brief Distribute tasks to the players
 * @return 1 on success
 *         0 on failure
 */
int distribute_tasks() {
	struct Tasks *tmp;
	struct Tasks *task = tasks_head->head;
	struct Players* player=players_head;
	assert(task != NULL);
	assert(player != NULL);

	while(task != NULL) {
		tmp = task->next;

		do {
			player = player->next;
		} while(player == players_head || player->is_alien == 1);

	/*tasks_head->head = tmp;*/
		if(player->tasks_head == player->tasks_sentinel) {
			player->tasks_head = task;
			task->next = player->tasks_sentinel;
			player->tasks_sentinel->next = task;
		}
		else {
			player->tasks_sentinel->next->next = task;
			task->next = player->tasks_sentinel;
			player->tasks_sentinel->next = task;
		}
		task = tmp;
	}

	tasks_head->head = NULL;
	print_double_list();
	return 1;
}

/**
 * @brief Implement Task
 *
 * @param pid The player's id
 *
 * @param difficulty The task's difficuly
 *
 * @return 1 on success
 *         0 on failure
 */
int implement_task(int pid, int difficulty) {
	struct Players *pl;
	struct Tasks *temp, *curr, *prev;
	if(players_head == NULL){
		print_double_list(); 
		return 1;
	}
	pl = find_player(pid);
	if(pl == NULL){
		print_double_list(); 
		return 1;
	}
	
	
	temp =find_task(pl,difficulty);
	
	
	if(temp == NULL) {
		print_double_list(); 
		return 1;
	}
	if (pl == NULL) {
		print_double_list(); 
		return 1;
	}
	curr=pl->tasks_head;
	prev=curr;
	
	while(curr!=temp){
		prev=curr;
		curr=curr->next;
	}
	
	

	if (curr == pl->tasks_head) { 
		pl->tasks_head = curr->next;
		if(curr->next == pl->tasks_sentinel) {
			pl->tasks_head = pl->tasks_sentinel;
		}
		Push(curr);
		print_double_list();
		return 1;
	}
	prev->next = curr->next;
	Push(curr);
	print_double_list(); 
	return 1;
}

/**
 * @brief Eject Player
 * 
 * @param pid The ejected player's id
 *
 * @return 1 on success
 *         0 on failure
 */
int eject_player(int pid) {
	int *min_tasks_number = (int *)malloc(sizeof(int));
	struct Players *ej_pl=find_player(pid);
	struct Players* min_pl=find_min(min_tasks_number);	
	if(ej_pl == NULL){
		print_double_list(); 
		return 1;
	}
	assert(ej_pl != NULL);
	assert(min_pl!=NULL);
	
	//printf("--------------%d\n",min_pl->pid);
	merge(ej_pl,min_pl);

	delete(ej_pl);
	print_double_list(); 
    return 1;
}

/**
 * @brief Witness Eject Player
 *
 * @param pid_a The alien's pid
 * 
 * @param pid The crewmate's pid
 * 
 * @param number_of_witnesses The number of witnesses
 *
 * @return 1 on success
 *         0 on failure
 */
int witness_eject(int pid, int pid_a, int number_of_witnesses){
	int *min_tasks_number = (int *)malloc(sizeof(int));
	struct Players *ej_pl=find_player(pid);
	struct Players *al=find_player(pid_a);
	struct Players* min_pl=find_min(min_tasks_number);	
	
	if(ej_pl == NULL){
		_print(); 
		return 1;
	}
	if(al->is_alien==0){
		_print(); 
		return 1;
	}
	if(ej_pl->is_alien==1){
		_print(); 
		return 1;
	}
	assert(ej_pl != NULL);
	assert(min_pl!=NULL);
	merge(ej_pl,min_pl);
	_print();
	printf("--------------%d\n",ej_pl->tasks_head->tid);
	//assert(1==0);
	delete(ej_pl);
	al->evidence+=number_of_witnesses;
	
	if (players_head == NULL) {
		_print(); 
		return 1;
	}
	_print();
	//assert(1==0);
    return 1;
	
}


/**
 * @brief Sabbotage
 *
 * @param pid The player's id
 *
 * @param number_of_tasks The number of tasks to be popped
 * 
 * @return 1 on success
 *         0 on failure
 */
int sabbotage(int pid,int number_of_tasks) {
	struct Players* curr_pl= find_first_sabotage_player(pid,number_of_tasks);
	struct Tasks *tmp,*curr;
	if(number_of_tasks>tasks_stack->count){
		print_double_list();

		return 1;
	}
	if(curr_pl==NULL){
		print_double_list();

		return 1;
	}
	while(number_of_tasks!=0){
		if(curr_pl->is_alien==0){
			tmp=Top();
			curr=new_task(tmp->tid,tmp->difficulty);
			Pop();
			sorted_insert_task(curr_pl,curr);
			number_of_tasks--;
		}
		curr_pl=curr_pl->next;
	}
	print_double_list();
    return 1;
}


/**
 * @brief Vote
 *
 * @param pid The player's id
 * 
 * @param vote_evidence The vote's evidence
 *
 * @return 1 on success
 *         0 on failure
 */
int vote(int pid, int vote_evidence) {
	int *min_tasks_number = (int *)malloc(sizeof(int));
	struct Players *sus=find_player(pid);
	struct Players *ej_pl;
	struct Players* min_pl=find_min(min_tasks_number);	
	if(sus==NULL){ 
		_print();
		return 1;
	}
	if(min_pl==NULL){
		_print();
		return 1;
	}
	
	sus->evidence+=vote_evidence;
	ej_pl=find_player_with_max_evidence();
	printf("-----------------%d\n",ej_pl->pid);
	if(ej_pl == NULL){
		_print();
		return 1;
	}
	merge(ej_pl,min_pl);
	delete(ej_pl);
	if (players_head == NULL) {
		_print();
		return 1;
	}
	_print();
    return 1;
}


/**
 * @brief Give Away Work
 *
 * @return 1 on success
 *         0 on failure
 */
 
int give_work() {
	int *max_tasks_number = (int *)malloc(sizeof(int));
	int *min_tasks_number = (int *)malloc(sizeof(int));
	struct Players *pmin=find_min(min_tasks_number) ,*pmax=find_max(max_tasks_number);
	struct Tasks *curr;
	int j=0;
	if(pmin->pid==pmax->pid){
		while(pmax->next->is_alien==1){
			if(pmin->pid==pmax->pid){
				print_double_list();
				return 1;
			}
			pmax=pmax->next;
		}
		pmax=pmax->next;
	}
	curr= pmax->tasks_head;
	if(*min_tasks_number==0 && *max_tasks_number==1){
		print_double_list();
		return 1;
	}
	while(j != *max_tasks_number/2||*max_tasks_number==1 ){

		pmax->tasks_head = curr->next;
		sorted_insert_task(pmin,curr);
		curr=pmax->tasks_head;
		j++;
		
	}		
	print_double_list();
	return 1;
}

/**
 * @brief Terminate
 *
 * @return 1 on success
 *         0 on failure
 */
int terminate() {
	struct Players *curr = players_head->next;
	int al = 0,cr=0;
	if (curr == NULL) return 0;

	while (curr != players_head) {
		if (curr->is_alien == 1) {
			al++;
		}
		else {
			cr++;
		}
		curr = curr->next;
	}	
	if (al > cr) {
		printf("\nAliens win\n");
	}
	else if (al == 0 || tasks_stack->count == Total_Tasks) {
		printf("\nCrewmates win\n");
	}
	printf("\nDONE\n\n");
    return 1;
}

/**
 * @brief Print Players
 *
 * @return 1 on success
 *         0 on failure
 */
int print_players() {
	struct Players *curr=players_head->next;

	if (curr == NULL) return 0;

	printf("\nPlayers = ");
	while (curr != players_head) {
		printf("<%d:%d>, ", curr->pid,curr->is_alien);
		if (curr != players_head) {
			printf(", ");
		}
		curr = curr->next;
	}
	printf("\nDONE\n\n");

    return 1;
}

/**
 * @brief Print Tasks
 *
 * @return 1 on success
 *         0 on failure
 */
int print_tasks() {
	struct Tasks *curr=tasks_head->head;
	if (curr == NULL) return 1;

	printf("\nList_Tasks = ");
	while (curr != NULL) {
		printf("<%d:%d>", curr->tid, curr->difficulty);
		if (curr != NULL) {
			printf(", ");
		}
		curr = curr->next;
	}

	printf("\nDONE\n\n");
    return 1;
}

/**
 * @brief Print Stack
 *
 * @return 1 on success
 *         0 on failure
 */
int print_stack() {
	printf("Stack_Tasks = ");
	PrintStack();
	printf("\nDONE\n\n");

    return 1;
}

/**
 * @brief Print Players & Task List
 *
 * @return 1 on success
 *         0 on failure
 */
int print_double_list() {
	int i = 1;
	struct Players *curr_pl = players_head->next;
	struct Tasks *curr_tsk;
	if (players_head == NULL) {
		return 0;
	}
	while (curr_pl != players_head) {
		printf("Player-%d = ", curr_pl->pid);
		
		curr_tsk = curr_pl->tasks_head;
		while (curr_tsk != curr_pl->tasks_sentinel) {
			printf("<%d:%d>", curr_tsk->tid, curr_tsk->difficulty);
			if (curr_tsk != curr_pl->tasks_sentinel) {
				printf(", ");
			}
			curr_tsk = curr_tsk->next;
		}
		curr_pl=curr_pl->next; 
		printf("\n");
		i++;
	}
	printf("\nDONE\n\n");
    return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */

int free_all(void) {
    return 1;
}
