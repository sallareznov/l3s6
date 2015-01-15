#ifndef _DO_H_
#define _DO_H_

enum operateur { AND, OR};
typedef enum operateur operateur;

extern int is_an_option(char *option);
extern int is_a_valid_option(char *option);
extern int option_present(char *option);
extern int nb_options(int argc, char *argv[]);
extern int build_options(int argc, char *argv[]);
extern int right_operation(int a, int b, operateur op);
extern int check_options_validity(void);
extern int check_command_validity(void);
extern void kill_all(void);
extern int final_value(char *argv[], int argc, operateur op);

#endif