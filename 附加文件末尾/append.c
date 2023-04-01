/*
	2023��4��1�� 13:33:33
	��һ���ļ����ӵ���һ���ļ���ĩβ
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ����һ���ļ������� ��СΪ 4096*/
#define BUFSIZE 4096

/* �����ֽ�*/
#define SLEN 81

/* ���������ļ�*/
/* ��һ��Դ�ļ� �� �ڶ��������ļ�*/
void append(FILE* source, FILE* dest);					

/*��ȡ�ļ����ַ�*/
char* s_gets(char* st, int n);


int main() {
	
	FILE* fa = NULL , * fs = NULL ;
	
	int files = 0;					// ���ӵ��ļ�����
	char file_app[SLEN] = "";			//Ŀ���ļ���
	char file_src[SLEN] = "";			//Դ�ļ�
	int ch;

	/* ��ȡҪ�򿪵��ļ��������û�оʹ���һ�� �������ʧ�ܻ��ߴ���ʧ���˳�����*/
	puts("enter name of destination file:");
	s_gets(file_app, SLEN);
	errno_t err;
	err = fopen_s(&fa, file_app, "a+");
	if (err != 0) {
		fprintf(stdout, "can't open \"%s\",file\n", file_app);
		exit(EXIT_FAILURE);
	}
	
	/* ���������� */
	if (setvbuf(fa, NULL, _IOFBF, BUFSIZE) != 0) {
		fputs("Can't create output buffer\n", stdout);
		exit(EXIT_FAILURE);
	}

	/* �������� ��������Ҫ�򿪵��ļ����Ѿ��ɹ�*/

	puts("enter name of first source file (empty lin to quit:)");
	/*��ȡ�ڶ������֣�������ǿ��ַ�����*/
	while (s_gets(file_src,SLEN) && file_src[0] != '\0')
	{
		/*�ж������ļ������Ƿ�һ�������һ�����������*/
		if (strcmp(file_src, file_app) == 0) {
			fputs("can't append file to itself\n", stderr);
		}
		/* ����Դ�ļ������,ʧ�ܳ��򷵻�*/
		else if ((err = fopen_s(&fs, file_src, "r")) != 0) {
			fprintf(stdout, "can't open \"%s\",file\n", file_src);
			exit(EXIT_FAILURE);
		}

		/* ׼��������� ���������������������*/
		else {
			if (setvbuf(fs, NULL, _IOFBF, BUFSIZE) != 0) {
				fputs("Can't create output buffer\n", stdout);
				continue;
			}

			/* ��� �������������ļ����Ѿ�����*/
			append(fs, fa);
			/* fs ��Դ�ļ���fa��Ŀ���ļ� ��fs�ļ����ݣ�׷�ӵ�fa�ļ�*/
			/*��ɺ�ر��ļ���*/
			if (ferror(fs) != 0) {
				fprintf(stderr,"error in reading file \"%s \"\n" , file_src);
			}
			if (ferror(fa) != 0) {
				fprintf(stderr, "error in reading file \"%s \"\n", file_app);
			}

			fclose(fs);
			files++;
			printf("File %s appended.\n", file_src);
			puts("next file (empty line to quit:)");
		}
	}
	printf("done appinded. %d files appended.\n", files);
	rewind(fa);
	printf("%s contents:\n", file_app);
	while ((ch = getc(fa)) != EOF)
	{
		putchar(ch);
	}

	puts("\ndone displaying.\n");
	fclose(fa);
	

	return 0;
}

/* ���������ļ�*/
/* ��һ��Դ�ļ� �� �ڶ��������ļ�*/
void append(FILE* source, FILE* dest) {
	size_t bytes;
	static char temp [BUFSIZE];					//���仺����
	//��Ŀ���ļ���ȡ�������� ��ȡ�ɹ����ٸ���д����ٸ�
	while ((bytes = fread(temp,sizeof (char),BUFSIZE,source)) > 0 )
	{
		fwrite(temp, sizeof(char), bytes, dest);
	}
}

/*��ȡ�ļ����ַ�*/
char* s_gets(char* st, int n){
	char* ret_val;
	char* find;

	//fgets()���������ȡ�ɹ����ص�һ�������ĵ�ַ
	ret_val = fgets(st,n, stdin);

	if (ret_val) {
		find = strchr(st, '\n');
		if (find) {
			*find = '\0';
		}
		else {
			while (getchar() != '\n')
			{
				continue;
			}
		}
	}
	return ret_val;
}
