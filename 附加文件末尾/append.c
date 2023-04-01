/*
	2023年4月1日 13:33:33
	把一个文件附加到另一个文件的末尾
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 设置一个文件缓冲区 大小为 4096*/
#define BUFSIZE 4096

/* 设置字节*/
#define SLEN 81

/* 传入两个文件*/
/* 第一个源文件 ， 第二个附加文件*/
void append(FILE* source, FILE* dest);					

/*读取文件的字符*/
char* s_gets(char* st, int n);


int main() {
	
	FILE* fa = NULL , * fs = NULL ;
	
	int files = 0;					// 附加的文件数量
	char file_app[SLEN] = "";			//目标文件名
	char file_src[SLEN] = "";			//源文件
	int ch;

	/* 获取要打开的文件名，如果没有就创建一个 ，如果打开失败或者创建失败退出程序*/
	puts("enter name of destination file:");
	s_gets(file_app, SLEN);
	errno_t err;
	err = fopen_s(&fa, file_app, "a+");
	if (err != 0) {
		fprintf(stdout, "can't open \"%s\",file\n", file_app);
		exit(EXIT_FAILURE);
	}
	
	/* 创建缓冲区 */
	if (setvbuf(fa, NULL, _IOFBF, BUFSIZE) != 0) {
		fputs("Can't create output buffer\n", stdout);
		exit(EXIT_FAILURE);
	}

	/* 程序到这里 缓冲区和要打开的文件都已经成功*/

	puts("enter name of first source file (empty lin to quit:)");
	/*读取第二个名字，如果不是空字符并且*/
	while (s_gets(file_src,SLEN) && file_src[0] != '\0')
	{
		/*判断两个文件名字是否一样，如果一样将不能添加*/
		if (strcmp(file_src, file_app) == 0) {
			fputs("can't append file to itself\n", stderr);
		}
		/* 创建源文件来输出,失败程序返回*/
		else if ((err = fopen_s(&fs, file_src, "r")) != 0) {
			fprintf(stdout, "can't open \"%s\",file\n", file_src);
			exit(EXIT_FAILURE);
		}

		/* 准备工作完成 进入真正处理问题的流程*/
		else {
			if (setvbuf(fs, NULL, _IOFBF, BUFSIZE) != 0) {
				fputs("Can't create output buffer\n", stdout);
				continue;
			}

			/* 如果 两个缓冲区和文件都已经打开了*/
			append(fs, fa);
			/* fs 是源文件，fa是目标文件 把fs文件内容，追加到fa文件*/
			/*完成后关闭文件流*/
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

/* 传入两个文件*/
/* 第一个源文件 ， 第二个附加文件*/
void append(FILE* source, FILE* dest) {
	size_t bytes;
	static char temp [BUFSIZE];					//分配缓冲区
	//从目标文件读取到缓冲区 读取成功多少个就写入多少个
	while ((bytes = fread(temp,sizeof (char),BUFSIZE,source)) > 0 )
	{
		fwrite(temp, sizeof(char), bytes, dest);
	}
}

/*读取文件的字符*/
char* s_gets(char* st, int n){
	char* ret_val;
	char* find;

	//fgets()函数如果读取成功返回第一个参数的地址
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
