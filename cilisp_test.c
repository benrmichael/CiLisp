#include "clt.h"

// Variables used throughout
FILE *fp;
char 	buffer[BUFFER_LEN];
char 	exe[256], sys_path[64];
char 	*test_file;
char 	warn_reg[] = ".*(WARNING|operands|arguments|not|missing).*";
char 	nan_reg[] = ".*Double\\s+:\\s+[+-]?nan.*"; 
char	inf_reg[] = ".*Double\\s+:\\s+[+-]?inf.*";
char 	rand_reg[] = ".*Double\\s+:\\s+0\\.[0-9]*.*";
char 	rand_sum_reg[] = ".*Double\\s+:\\s+[0-9]\\.[0-9]*.*";
char 	read_reg[] = ".*read.*[+-]?[0-9]*[\\.]?[0-9]*.*";
int		print_table_flag = 1;
int		warn_flag = 0;

const char prompt[] =
    "\n"
    "\tOption                Description\n"
    "\t--------------------  ---------------------------------------------------\n"
    "\tTest a new file       Test a new file by entering the abbreviation (e.g., add for add.cilisp)\n"
    "\tChange file path      Enter \"path\" to prompt for a new filepath\n"
    "\tList                  Enter \"list\" to show all files in the current test path (runs ls)\n"
    "\tAll                   Enter \"all\" to test all files from task 1 (MUST HAVE CORRECT PATH SETUP)\n"
    "\tHelp                  Enter \"help\" to display this information again\n"
    "\tExit                  Enter \"quit\" to exit\n";

int main(int argc, char **argv){
	if(argc != 2){
		error(1, " need an input file to test! Example : cilisp_test INPUTS/task_1/add.cilisp");
	}
	char *filePathCopy = strdup(argv[1]);
    // Use dirname to get the directory path
    char *dirPath = dirname(filePathCopy);
	strcpy(sys_path, dirPath);
	strcat(sys_path, "/");
	free(filePathCopy);

	char loop;
	int multipleTests = 0;

	// Validates the input and sets up the exe file and test_file var
	setupFileVar(argv[1], NULL, 0);
	
	printf("Would you like to test multiple files (y/n)? ");
	scanf(" %c", &loop);
	if(loop == 'y' || loop == 'Y') multipleTests = 1;

	if(!multipleTests){ 
		printf("No worries, running single test...\n\n");
		runTest(argv[1]);
	}
	else runMultipleTest(argv[1]);

	printf("\nThanks for testing :)\n\n");

	return EXIT_SUCCESS;
	
}

void runMultipleTest(char *filename){
	char command[64], newfile[64];
	int passed, loop = 0;
	file_t test;

	//printf("\nAwesome! First off, please enter the path to your test files (ex. \"INPUTS/task_1/\" NOTE: The slashes are important!): ");
	//scanf(" %s", sys_path);
	printf("\nThanks. Your initial file will be tested, then you will be prompted for a command. The options are:\n%s", prompt);
	clear();
	printf("\nTo start, hit enter!");
	fgets(buffer, BUFFER_LEN, stdin);

	while(1){
		if(loop == 0) {
			runTest(filename);
			loop++;
			continue;
		}
		printf("Enter in a command (current path = \"%s\"): ", sys_path);
		scanf(" %s", command);
		if(strcmp(command, "help") == 0){
			printf("The commands are:\n");
			printf(prompt);
		} else if(strcmp(command, "quit") == 0){
			break;
		} else if(strcmp(command, "path") == 0){
			printf("Enter in new file path: ");
			scanf(" %s", sys_path);
		} else if(strcmp(command, "list") == 0){
			char ls[64];
			strcpy(ls, "");
			strcat(ls, "ls ");
			strcat(ls, sys_path);
			system(ls);
		} else if(strcmp(command, "all") == 0){
			int index = 0;
			print_table_flag = 0;
			while (index < 18){
				strcpy(newfile, "");
				strcpy(newfile, test_f[index]);
				setupFileVar(newfile, sys_path, 1);
				runTest(newfile);
				index++;
			}
			print_table_flag = 1;
		} else {
			strcpy(newfile, command);
			strcat(newfile, ".cilisp");
			setupFileVar(newfile, sys_path, 1);
			runTest(newfile);
		}
	}
}

char *genReg(num_t type, double value){
    char regex[64];
    if(!type){
        snprintf(regex, 63, ".*Integer\\s+:\\s+%0.0lf.*", value);
    } else {
        snprintf(regex, 63, ".*Double\\s+:\\s+%lf.*", value);
    }

    return strdup(regex);
}

void runTest(char *filename){
	file_t test = determineTestFile(filename);
	if(test == -1) {
		error(0, "Unexpected test file entered!");
		return;
	} 
	printf("\nTesting %s...", filename);

	// Run on the file in the pipeline
	if((fp = popen(exe, "r")) == NULL){
		perror("Failed to create fork. Terminating...");
		error(1, "pipline creation failed!");
	}

	// Go to the right test function
	int passed = testFile(test);
	if(passed) printf(GREEN "You passed all the tests! Great work!\n" RESET_COLOR);
	else printf(RED "Not all tests passed! %s%s", RESET_COLOR, "keep trying! You got this!\n");

	// Close the pipeline and exit
	pclose(fp);
}

int testFile(file_t t) {
	warn_flag = 0;
    if(print_table_flag) printTableHeader();
    switch (t) {
        case ADD: return test(t, 8, warn_reg, genReg(INT, 0), genReg(INT, 1), genReg(DOUBLE, 1), genReg(INT, 15), genReg(INT, -3), genReg(DOUBLE, -3), genReg(DOUBLE, 0));
        case ABS: return test(t, 10, genReg(INT, 1), genReg(DOUBLE, 1.2), genReg(INT, 3), genReg(INT, 0), genReg(DOUBLE, 0), genReg(DOUBLE, 1.4), warn_reg, nan_reg, warn_reg, genReg(INT, 1));
        case CBRT: return test(t, 13, warn_reg, nan_reg, genReg(DOUBLE, 0.0), genReg(DOUBLE, 0.0), genReg(DOUBLE, -1.0), genReg(DOUBLE, -1.0), genReg(DOUBLE, 1.0), genReg(DOUBLE, 1.0), genReg(DOUBLE, 3.0), genReg(DOUBLE, 3.0), genReg(DOUBLE, 1.587401), warn_reg, genReg(DOUBLE, 1.0));
        case COMP: return test(t, 11, genReg(DOUBLE, 1.0), genReg(DOUBLE, 1.414214), genReg(DOUBLE, 3.0), genReg(DOUBLE, 9.0), genReg(DOUBLE, 1.0), genReg(INT, 14), genReg(DOUBLE, -14.0), genReg(DOUBLE, 22.990681), genReg(DOUBLE, 10.0), genReg(INT, 28), genReg(DOUBLE, 28.0));
        case DIV: return test(t, 18, warn_reg, nan_reg, warn_reg, nan_reg, warn_reg, nan_reg, genReg(INT, 0), genReg(DOUBLE, 0.5), genReg(INT, 2), genReg(DOUBLE, 2), genReg(DOUBLE, 2.5), genReg(DOUBLE, -5), warn_reg, genReg(INT, 0), warn_reg, genReg(INT, 0), warn_reg, genReg(DOUBLE, 2.5));
        case EXP: return test(t, 8, warn_reg, nan_reg, genReg(DOUBLE, 2.718282), genReg(DOUBLE, 0.367879), genReg(DOUBLE, 148.413159), genReg(DOUBLE, 0.135335), warn_reg, genReg(DOUBLE, 2.718282));
        case EXP2: return test(t, 11, warn_reg, nan_reg, genReg(INT, 2), genReg(DOUBLE, 2.0), genReg(INT, 1), genReg(DOUBLE, 1.0), genReg(DOUBLE, 1.414214), genReg(DOUBLE, 0.25), genReg(DOUBLE, 1048576.0), warn_reg, genReg(INT, 2));
        case HYPOT: return test(t, 13, warn_reg, genReg(DOUBLE, 0.0), genReg(DOUBLE, 1.0), genReg(DOUBLE, 1.0), genReg(DOUBLE, 5.0), genReg(DOUBLE, 5.0), genReg(DOUBLE, 50.0), genReg(DOUBLE, 9.0), genReg(DOUBLE, 9.0), genReg(DOUBLE, 22.561028), genReg(DOUBLE, 8.660254), genReg(DOUBLE, 8.660254), genReg(DOUBLE, 13.0));
        case LOG: return test(t, 12, warn_reg, nan_reg, genReg(DOUBLE, 0.0), inf_reg, nan_reg, inf_reg, nan_reg, genReg(DOUBLE, 1.0), genReg(DOUBLE, 3.0), genReg(DOUBLE, 3.0), warn_reg, genReg(DOUBLE, 0.0));
        case MAX: return test(t, 8, warn_reg, nan_reg, genReg(INT, 1), genReg(INT, -1), genReg(DOUBLE, 1.0), genReg(DOUBLE, 232311.121), genReg(INT, 9), genReg(DOUBLE, 25.0));
        case MIN: return test(t, 7, warn_reg, nan_reg, genReg(INT, 1), genReg(DOUBLE, 0.0), genReg(INT, 0), genReg(INT, -5), genReg(DOUBLE, -12.0));
        case MULT: return test(t, 8, warn_reg, genReg(INT, 1), genReg(INT, 1), genReg(DOUBLE, 1.0), genReg(INT, -1), genReg(DOUBLE, 1.0), genReg(INT, 120), genReg(DOUBLE, -120.0));
        case NEG: return test(t, 8, genReg(INT, -5), genReg(DOUBLE, -5.5), genReg(DOUBLE, 5.0), genReg(INT, 5), warn_reg, nan_reg, warn_reg, genReg(INT, -1));
        case NUMBER: return test(t, 13, genReg(INT, 0), genReg(DOUBLE, 0.0), genReg(INT, 1), genReg(DOUBLE, 1.0), genReg(DOUBLE, 0.0), genReg(DOUBLE, 0.5), genReg(INT, 0), genReg(DOUBLE, 10.55), genReg(DOUBLE, -12.87), genReg(DOUBLE, -12.0), genReg(INT, -12), warn_reg, genReg(INT, 34));
        case POW: return test(t, 19, warn_reg, nan_reg, warn_reg, nan_reg, warn_reg, nan_reg, genReg(INT, 1), genReg(DOUBLE, 1.0), genReg(INT, 2), genReg(DOUBLE, 2.1), nan_reg, genReg(INT, 1), genReg(DOUBLE, 1.0), genReg(DOUBLE, 1.0), genReg(INT, 27), genReg(DOUBLE, 27.0), genReg(DOUBLE, 3.0), warn_reg, genReg(INT, 1));
        case REM: return test(t, 19, warn_reg, nan_reg, warn_reg, nan_reg, warn_reg, nan_reg, warn_reg, nan_reg, genReg(INT, 1), genReg(INT, 0), genReg(DOUBLE, 0.5), genReg(INT, 2), genReg(INT, 4), genReg(DOUBLE, 4.0), genReg(DOUBLE, 4.0), warn_reg, genReg(INT, 1), warn_reg, genReg(INT, 2));
        case SQRT: return test(t, 13, warn_reg, nan_reg, genReg(DOUBLE, 1.0), genReg(DOUBLE, 1.0), genReg(DOUBLE, 0.0), genReg(DOUBLE, 0.0), nan_reg, nan_reg, genReg(DOUBLE, 2.0), genReg(DOUBLE, 13.038405), genReg(DOUBLE, 1.414214), warn_reg, genReg(DOUBLE, 1.0));
        case SUB: return test(t, 15, warn_reg, nan_reg, warn_reg, nan_reg, warn_reg, nan_reg, genReg(INT, -1), genReg(INT, 1), genReg(INT, 3), genReg(DOUBLE, 1.0), genReg(DOUBLE, 3.0), genReg(DOUBLE, 0.0), genReg(DOUBLE, 1.0), warn_reg, genReg(INT, -1));
        case TASK2: return test(t, 20, warn_reg, nan_reg, genReg(INT, 1), warn_reg, genReg(INT, 1),genReg(INT, 2), genReg(INT, 3), genReg(INT, 5), warn_reg, nan_reg, genReg(INT ,1), warn_reg, nan_reg, genReg(INT, 1), genReg(INT, 1), genReg(INT, 6), genReg(DOUBLE, 4.242641), genReg(DOUBLE, 5.398346), genReg(DOUBLE, 17.414214), genReg(INT, 4));
		case TASK3: return test(t, 4, warn_reg, genReg(INT, 2), genReg(DOUBLE, 30), genReg(DOUBLE, 9));
		case TASK4: return test(t, 56, warn_reg, nan_reg, genReg(INT, 1), genReg(INT, 1), warn_reg, genReg(INT, 1), genReg(INT, 1), genReg(INT, 2), genReg(INT, 3), rand_reg, rand_reg, rand_reg, rand_reg, rand_reg, rand_reg, rand_reg, rand_reg, rand_reg, rand_reg, read_reg, genReg(INT, 1), read_reg, warn_reg, nan_reg, read_reg, warn_reg, nan_reg, read_reg, genReg(DOUBLE, -5.5), read_reg, read_reg, genReg(INT, 0), genReg(INT, 1), genReg(INT, 1), genReg(INT, 0), genReg(INT, 0), genReg(INT, 1), genReg(INT, 0), genReg(INT, 0), genReg(INT, 1), genReg(INT, 0), genReg(INT, 1), genReg(INT, 6), genReg(INT, 5), genReg(INT, 5), genReg(INT, 6), read_reg, genReg(DOUBLE, -17.2), genReg(DOUBLE, -17.2), read_reg, genReg(INT, 127), genReg(INT, 127), genReg(DOUBLE, 219.6), rand_reg, rand_reg, rand_reg);
		case TASK5: return test(t, 11, warn_reg, genReg(INT, 2), warn_reg, genReg(INT, 2), genReg(DOUBLE, 22), genReg(INT, 9), genReg(INT, 6), rand_reg, rand_reg, rand_sum_reg, genReg(DOUBLE, 0.5));
		default: return 0;
    }
}

int checkForMatch(char *reg, char *str, int *ln, int i_flag, file_t test_f){
    regex_t regex;
	int r = regcomp(&regex, reg, REG_EXTENDED | REG_ICASE);
	if(r) {
		error(0, "Could not compile regex pattern -> %s", reg);
		return 0;
	}

	r = regexec(&regex, str, 0, NULL, 0);
	if(r) {
		if(print_table_flag) printCase(test_f, str, (*ln), 0);
		if(i_flag) {
			(*ln)++;
		}
        regfree(&regex);
		return 0;
	} else {
		if(print_table_flag) printCase(test_f, str, (*ln), 1);
        regfree(&regex);
		return 1;
	}
}

void printCase(file_t ft, char *given, int n, int passed){
	int pad = 0;
	if(warn_flag) pad = 1;

	size_t len = strlen(given);
	if(given[len - 1] == '\n'){
		given[len - 1] = '\0';
	}
	if(given[0] == '>'){
		for (int i = 0; given[i] != '\0'; ++i) {
            given[i] = given[i + 1];
        }
	}
	if(given[0] == ' '){
		for (int i = 0; given[i] != '\0'; ++i) {
            given[i] = given[i + 1];
        }
	}
	//const char *colorCode = passed ? GREEN : RED;
	const char *delimeter = "^";
	char *string = getExpectedValue(ft, n);
	if(string == NULL) return;
    char *expected = strtok(string, delimeter);
	if(expected == NULL) return;
    char *input = strdup(expected);
    expected = strtok(NULL, delimeter);
	if(expected == NULL) return;
	expected = (strstr(expected, "WARNING") != NULL) ? "WARNING statement used" : expected;
	if(strstr(given, "WARNING") != NULL){
		warn_flag = 1;
		given = "WARNING statement used";
	} else warn_flag = 0;

	const char *colorCode = (passed) ? GREEN : RED;
	if(!pad) printf("| %-15.15s | %-25.25s | %s%-25.25s | %s%-15.15s%s |\n", input, expected, RESET_COLOR, given, colorCode, (passed) ? "Passed" : "Failed", RESET_COLOR);
	else printf("| %-15.15s | %-25.25s | %s%-25.25s     | %s%-15.15s%s |\n", input, expected, RESET_COLOR, given, colorCode, (passed) ? "Passed" : "Failed", RESET_COLOR);
	printf("+ %s + %s + %s + %s +\n", "---------------", "-------------------------", "-------------------------", "---------------");
    free(input);
    free(string);
}

void printTableHeader() {
    printf("\n+ %s + %s + %s + %s +\n", "---------------", "-------------------------", "-------------------------", "---------------");
    printf("| %-15s | %-25s | %-25s | %-15s |\n", "Input", "Expected", "Found", "Status");
    printf("+ %s + %s + %s + %s +\n", "---------------", "-------------------------", "-------------------------", "---------------");
}

char *getExpectedValue(file_t t, int n){
	switch (t){
	case ADD: 	return strdup(add_case_expected[n]);
	case ABS:  	return strdup(abs_case_expected[n]);
	case CBRT: 	return strdup(cbrt_case_expected[n]);
	case COMP:	return strdup(comp_case_expected[n]);
	case DIV: 	return strdup(div_case_expected[n]);
	case EXP:	return strdup(exp_case_expected[n]);
	case EXP2:	return strdup(exp2_case_expected[n]);
	case HYPOT:	return strdup(hypot_case_expected[n]);
	case LOG:	return strdup(log_case_expected[n]);
	case MAX:	return strdup(max_case_expected[n]);
	case MIN:	return strdup(min_case_expected[n]);
	case MULT: 	return strdup(mult_case_expected[n]);
	case NEG:	return strdup(neg_case_expected[n]);
	case NUMBER:return strdup(literal_case_expected[n]);
	case POW:	return strdup(pow_case_expected[n]);
	case REM:	return strdup(remainder_case_expected[n]);
	case SQRT:	return strdup(sqrt_case_expected[n]);
	case SUB:	return strdup(sub_case_expected[n]);
	case TASK2: return strdup(task_2_expected[n]);
	case TASK3: return strdup(task_3_expected[n]);
	case TASK4: return strdup(task_4_expected[n]);
	case TASK5: return strdup(task_5_expected[n]);
	default: 	return "";
	}
}

file_t determineTestFile(char *filename){
	int i = 0;
	char *name = basename(filename);
    while (test_f[i][0] != '\0')
    {
        if (strcmp(test_f[i], name) == 0)
        {
            return i;
        }
        i++;
    }

	return -1;
}

void setupFileVar(char *fname, char *path, int flag){
	strcpy(exe, "");
	if(strstr(fname, "task_4.cilisp") != NULL){
		snprintf(exe, sizeof(exe), "./cilisp %s%s %s%s", flag ? path : "", fname, sys_path, "task_4_read_target.txt");
		return;
	}
	snprintf(exe, 128, "./cilisp < %s%s", flag ? path : "", fname);
}

void error(int e, char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 255, format, args);

    printf(RED "\nERROR: " RESET_COLOR);
	printf("%s\n", buffer);
    fflush(stdout);

    va_end(args);
    if(e) exit(1);
}

int test(file_t ft, int n, ...){
	va_list args;
	va_start(args, n);

	int reg, ln = 0, numtests = n, numright = 0;

	while(fgets(buffer, BUFFER_LEN, fp) != NULL){
		if(strcmp(buffer, "\n") == 0) continue;
		if(ft == TASK4){
			if(buffer[0] == '>') continue;
		}

		char *reg_pat;
		if(ln >= n) reg_pat = ".*";
		else reg_pat = va_arg(args, char*);

		int flag = (strcmp(reg_pat, warn_reg) == 0);
		numright += checkForMatch(reg_pat, buffer, &ln, flag, ft);
        if(!(flag || (strcmp(reg_pat, nan_reg) == 0) || (strcmp(reg_pat, inf_reg) == 0) || (strcmp(reg_pat, read_reg) == 0) || (strcmp(reg_pat, rand_reg) == 0) || (strcmp(reg_pat, rand_sum_reg) == 0) || (strcmp(reg_pat, ".*")) == 0)){
            printf("Freeing : %s\n", reg_pat);
            free(reg_pat);
        }
		++ln;
	}

	return numright >= numtests;
}

void clear ()
{
    while ( getchar() != '\n' );
}
