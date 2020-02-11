#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "ctools.h"

#define ABORTSTRING "\\"
#define ABORTCHAR '\\'

#define MAXMATCHES 17

#define N_BASIC_FIELDS 8
#define OTHER -1

#ifdef DEBUG
  #define debug(msg) printf("DEBUG: %s,%d, %s \n", __FILE__,__LINE__,msg)
#else
  #define debug(msg)
#endif

typedef enum {

    R_NAME = 0, R_WORK_PHONE, R_HOME_PHONE, R_COMPANY, R_WORK_ADDRESS,
    R_HOME_ADDRESS, R_REMARKS, R_UPDATED
  } Basic_Field ;    
    
extern char *Field_Names[];  
  
/* A Rolodex entry */

typedef struct {

    char *basicfields[N_BASIC_FIELDS];
    int n_others;
    char **other_fields;

  } Rolo_Entry, *Ptr_Rolo_Entry;

int fl;
int fs;
int fu;
char * myuser;
int non_opt_count;
int myargc;
char ** myargv;  

#define get_basic_rolo_field(n,x) (((x) -> basicfields)[(n)]) //(Ptr_Rolo_Entry -> basicfields)[n]
#define get_n_others(x) ((x) -> n_others)  
#define get_other_field(n,x) (((x) -> other_fields)[n])
  
#define set_basic_rolo_field(n,x,s) (((x) -> basicfields[(n)]) = (s))
#define set_n_others(x,n) (((x) -> n_others) = (n))
#define incr_n_others(x) (((x) -> n_others)++)
#define set_other_field(n,x,s) ((((x) -> other_fields)[n]) = (s))

typedef struct link {

    Ptr_Rolo_Entry entry;
    int matched;
    struct link *prev;
    struct link *next;

  } Rolo_List, *Ptr_Rolo_List;


#define get_next_link(x) ((x) -> next)
#define get_prev_link(x) ((x) -> prev)
#define get_entry(x)     ((x) -> entry) //gives Ptr_Rolo_Entry
#define get_matched(x) ((x) -> matched)

#define set_next_link(x,y) (((x) -> next) = (y))
#define set_prev_link(x,y) (((x) -> prev) = (y))
#define set_entry(x,y) (((x) -> entry) = (y))
#define set_matched(x) (((x) -> matched) = 1)
#define unset_matched(x) (((x) -> matched) = 0);

extern Ptr_Rolo_List Begin_Rlist;
extern Ptr_Rolo_List End_Rlist;

#define MAXLINELEN 80
#define DIRPATHLEN 100

extern int changed;
extern int reorder_file;
extern int rololocked;

extern char *rolo_emalloc(int);
// extern char *malloc(int);
extern void * malloc(size_t);
extern Ptr_Rolo_List new_link_with_entry();
extern char *copystr(char *);
// extern int compare_links();
// extern char *timestring();
extern char *homedir(char *), *libdir(char *);
extern char *getenv(const char *);
extern char *ctime(const time_t *);
extern char *select_search_string();
extern int in_search_mode;

//rolo.h
extern char *timestring();
extern void user_interrupt();
extern void user_eof();
extern void roloexit(int);
extern void save_to_disk();
extern void save_and_exit(int);


//clear.c
extern int clearinit();
extern void clear_the_screen ();

//io.c
extern void write_rolo (FILE*, FILE*);
extern int read_rolodex(int);
extern void summarize_entry_list (Ptr_Rolo_List,char *);
extern void display_entry();
extern void cathelpfile (char * ,char *,int);
extern void any_char_to_continue ();
extern void display_entry_for_update (Ptr_Rolo_Entry);
extern void display_field_names ();

//menuaux.c
extern int rolo_menu_yes_no (char *,int ,int ,char *, char *);
extern int rolo_menu_data_help_or_abort (char *, char *, char *, char **);
extern int rolo_menu_number_help_or_abort (char *,int, int , int*);

//operations.c
extern int entry_action (Ptr_Rolo_List);
extern void display_list_of_entries (Ptr_Rolo_List);
extern void rolo_peruse_mode (Ptr_Rolo_List);
extern void rolo_add () ;

//options.c
extern void print_short();
extern void print_people();
extern void interactive_rolo();

//rlist.c
extern int rlength (Ptr_Rolo_List);
extern Ptr_Rolo_List new_link_with_entry();
extern void rolo_insert (Ptr_Rolo_List,int (*compare)());
extern void rolo_delete (Ptr_Rolo_List);
extern int compare_links (Ptr_Rolo_List,Ptr_Rolo_List);
extern void rolo_reorder(); 

//search.c
extern int select_field_to_search_by (int *,char ** );
extern int match_by_name_or_company (char *,int);
extern int match_link (Ptr_Rolo_List,int ,char*,int,char*,int);
extern int find_all_matches (int ,char *, char*, Ptr_Rolo_List *);
extern void rolo_search_mode (int ,char* ,char* );

//update.c
extern void rolo_update_mode(Ptr_Rolo_List);
extern Ptr_Rolo_Entry copy_entry (Ptr_Rolo_Entry);

//menu.c
extern int rgetline (FILE *,char *,int);

// extern char * non_option_arg_myparser(int);