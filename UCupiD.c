/* Miguel Alba Student ID: 913049129
 * This program intakes txt files
 * then based on user input returns 
 * txt file that contains the best 
 * matches for a given user.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student{

	char sid[10];
	int hieght, wieght, age, political;
	char name[20];
	char sex, orientation;
	char major_hobby[20];
	char minor_hobby[20];
	struct student *next;

};
struct student *destructor(struct student *student_profile)
{
	struct student *final, *initial;

	if(!student_profile)
		return NULL;

	if(!student_profile->next){
		free(student_profile);
		return NULL;
	}

	initial = student_profile;
	final = student_profile->next;

	while( final->next != NULL ){
		initial = final;
		final = final->next;
	}

	initial->next = NULL;
	free(final);

	return student_profile;
}

void get_match( struct student *student_profile, struct student *user, FILE *fout)
{
	struct student *std = student_profile;

	fprintf(fout, "Matches for user %s (%s): \n", user->sid, user->name);

	while(std->next != NULL){
		if( user->orientation == std->sex && user->sex == std->orientation && (strncmp(std->sid, user->sid, 10))){ //check sexual/sid
			if(((user->age-10) <= std->age && std->age <= (user->age+10))
				&&((user->political-2) <= std->political && std->political <= (user->political+2))){//social
				fprintf(fout,"- %s (%s)\n", std->sid, std->name);
				std = std->next;
			}			
			else if(!strncmp(std->major_hobby, user->major_hobby, 20) && !strncmp(std->minor_hobby, user->minor_hobby, 20)){//personal
				fprintf(fout,"- %s (%s)\n", std->sid, std->name);
				std = std->next;
			} 
			else if(((std->wieght <= (user->wieght*.1) + user->wieght) && std->wieght >= abs(((user->wieght*.1) - user->wieght)))&& 
				((std->hieght <= (user->hieght*.1) + user->hieght) && std->hieght >= abs(((user->hieght*.1) - user->hieght)))){
				fprintf(fout,"- %s (%s)\n", std->sid, std->name);
				std = std->next;
			} 
			else{
				std = std->next;				
			}
				
		} else{
				std = std->next;
		}
	}
}

struct student *find_student(struct student *student_profile, char* sid)
{
	struct student *s = student_profile;

	while (s && strncmp(s->sid, sid, 10))
		s = s->next;
	if(!s){
		printf("User with SID %s not found\n", sid);
		exit(1);
	}
	return s;
}

struct student *get_student_data( struct student *student_profile, FILE *fin ) 
{

	char buffer[256];
	struct student *stud, *last;
	memset(&buffer, 0, 256);	

	while( fgets(buffer, 256, fin) ){
		if( buffer[strlen(buffer) - 1] == '\n'){
			stud = malloc(sizeof(struct student));
			sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%d,%d,%[^,],%[^,],%d,%d\n", stud->sid, stud->name, &stud->sex, &stud->orientation,
					 &stud->age, &stud->political, stud->major_hobby, stud->minor_hobby, &stud->hieght, &stud->wieght);
			stud->next = NULL;
			
			if (!student_profile){
				student_profile = stud;
				
			}else{

				last = student_profile;

				while( last->next != NULL){
					last = last->next;
				}
				last->next = stud;
			}
		}
		memset(&buffer, 0, 256);
	}	
	
	return student_profile;
	
}

int main(int argc, char* argv[256])
{
	
	if(argc < 3){
		printf("Usage: ./UCupiD db_file SID\n");
		return 1;
	}

	FILE *fin, *fout;

	fin = fopen(argv[1], "r");
	
	char filename[21] = "match_";
	strncat(filename, argv[2], 10);
	strcat(filename,".txt");
	
	fout = fopen(filename, "w");

	if(!fin || !fout) {
		printf("Error: cannot open %s\n", argv[1]);
		return 1;
	}



	struct student *student_profile = NULL;
	student_profile = get_student_data(student_profile, fin);

	struct student *user = find_student(student_profile, argv[2]);
	get_match(student_profile, user, fout);
	destructor(student_profile);

	fclose(fin);	
	fclose(fout);

	return 0;
}
