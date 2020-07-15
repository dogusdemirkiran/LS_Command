#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>


void  ls_command(char *fn)
{
	DIR *d;
	struct stat buf;
	struct dirent *de;

	struct passwd *pwd;
	struct group *grp;
	int exists;
	int total_size;

	struct tm *tmp;
	time_t t_mtime;
	char DATE[20];

	int dict=0;
	int f=0;
	int sl=0;
	int hf=0;


	d = opendir(fn);

	if(d==NULL)
	{
		perror("prsize");
		exit(1);
	}


	for(de=readdir(d); de!=NULL ; de=readdir(d))
	{

		exists=stat(de->d_name,&buf);

		if(exists<0)
		{
			fprintf(stderr,"NONE: %s\n",de->d_name);
		}

		else
		{

			if(de->d_name[0] == '.')
			{
				hf++;
			}

			else
			{
				if(S_ISDIR(buf.st_mode))
				{
					dict++;
					printf("d");
				}

				if(S_ISREG(buf.st_mode))
				{
					f++;
					printf("-");
				}

				if(S_ISLNK(buf.st_mode))
				{
					sl++;
					printf("l");
				}
			}


			printf((buf.st_mode & S_IRUSR) ? "r" : "-");
			printf((buf.st_mode & S_IWUSR) ? "w" : "-");
			printf((buf.st_mode & S_IXUSR) ? "x" : "-");

			printf((buf.st_mode & S_IRGRP) ? "r" : "-");
			printf((buf.st_mode & S_IWGRP) ? "w" : "-");
    			printf((buf.st_mode & S_IXGRP) ? "x" : "-");

			printf((buf.st_mode & S_IROTH) ? "r" : "-");
			printf((buf.st_mode & S_IWOTH) ? "w" : "-");
   			printf((buf.st_mode & S_IXOTH) ? "x" : "-");
			printf("\t");



			printf("%ld\t", buf.st_nlink);

			pwd = getpwuid(buf.st_uid);
			printf("%s\t",pwd->pw_name);
			grp = getgrgid(buf.st_gid);
			printf("%s\t",grp->gr_name);

			printf("%ld\t",buf.st_size);

			tmp = localtime(&buf.st_mtime);
			strftime(DATE, sizeof(DATE), "%b\t%d\t%I:%M%p\t", tmp);
			printf("%s\t", DATE);
			printf("%s\t",de->d_name);

			if(de->d_name[0] == '.')
			{
				printf("+hf->%d\n",hf);
			}
			else
			{
				if(S_ISDIR(buf.st_mode))
				{
					printf("+d->%d\n",dict);
				}
				
				if(S_ISLNK(buf.st_mode))
				{
					printf("*sl->%d\n",sl);
				}

				if(S_ISREG(buf.st_mode))
				{
					printf("+f->%d\n",f);
				}
			}
		}

	}

closedir(d);
}



int main()
{
	printf("\n");
	ls_command(".");
}
 
