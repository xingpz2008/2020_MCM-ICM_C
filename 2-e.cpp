#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cctype>
#define LOG "processlog.dat" 
#define RESLOG "res.txt"
#define WORDLEN 20
#define MAXWORD 500
class review {
private:
	char data[MAXWORD][WORDLEN];
	int wordcount;
public:
	char word[MAXWORD];
	review()
	{
		for (int i = 0; i < MAXWORD; i++)
		{
			word[i] = '\0';
		}
		for (int i = 0; i < MAXWORD; i++)
		{
			for (int j = 0; j < WORDLEN; j++)
			{
				data[i][j] = '\0';
			}
		}
		wordcount = 0;
	}
	int str2char()
	{
		int num = 0;
		int len = strlen(word);
		for (int i = 0; num < MAXWORD && num < len; num++)
		{
			if ((word[num] >= 64 && word[num] <= 90) ||(word[num]>=97&&word[num]<=122))
			{
				data[wordcount][i] = word[num];
				i++;
			}
			else
			{
				if (word[num] == ' ')
				{
					wordcount++;
					i = 0;
				}
			}
		}
		return num;
	}
	void ini()
	{
		for (int i = 0; i < MAXWORD; i++)
		{
			word[i] = '\0';
		}
		for (int i = 0; i < MAXWORD; i++)
		{
			for (int j = 0; j < WORDLEN; j++)
			{
				data[i][j] = '\0';
			}
		}
		wordcount = 0;
	}
	inline int numbck() { return wordcount; }
	char* datbck(int i) { return data[i]; }
};

class g_pool {
private:
	char word[WORDLEN];
	int num;
	FILE* log;
	int flag;
	int meaning;
public:
	g_pool()
	{
		flag = 0;
	}
	void inject(const char* ext, int x)
	{
		for (int i = 0; i < WORDLEN; i++)
		{
			word[i] = '\0';
		}
		strcpy(word, ext);
		num = 0;
		meaning = x;
		log = fopen(LOG, "a+");
		flag = 0;
	}
	void check(const char* input)
	{
		//FILE* res=fopen(RESLOG,"a");
		//int len = strlen(input);
		/*if (isalpha(word[len]))
		{
			//printf("%d\n", len);
			word[len] = '\0';
			fprintf(log, "[%d]enter processed with symbol %s at %d, flag=1\n", meaning, word, len);
		}*/
		//preprocess for \n
		if (!strcmp(input, word))
		{
			num++;
			fprintf(log, "[%d]success with %s, indicator added to %d\n", meaning, word, num);
			flag = 1;
		}
		else
		{
			fprintf(log, "[%d]not match with %s\n", meaning, word);
		}
	}
	~g_pool()
	{
		log = NULL;
	}
	inline void flag_reset()
	{
		flag = 0;
		fprintf(log, "flag reset\n");
	}
	inline int flagrtn()
	{
		return flag;
	}
	char* wdrtn()
	{
		return word;
	}
	inline int numrtn()
	{
		return num;
	}

};

const char* pool1[] = { "best","great","satisfied","love","perfect","perfectly","excellent","awesome","good","nice","nicely","fantastic","wonderful","amazing","happy","excited","favorite","adorable","comfortable" };

const char* pool2[] = { "worst","bad","dissatisfied","hate","disappointed","broken","terrible","awful","sad","annoying","annoyed" };

int main(void)
{
	FILE* fp;
	FILE* fplog = fopen(LOG, "w+");
	char str[10];
	int total;
	printf("input name then total lines:\n");
	scanf("%s %d", str, &total);
	fp = fopen(str, "r");
	int num = 1;
	review rw;
	g_pool good[19];
	g_pool bad[11];
	for (int i = 0; i < 19; i++)
	{
		good[i].inject(pool1[i], 1);
	}
	for (int i = 0; i < 11; i++)
	{
		bad[i].inject(pool2[i], 2);
	}
	while (!feof(fp))
	{
		rw.ini();
		fgets(rw.word, 500, fp);
		int tmp = rw.str2char();
		fprintf(fplog, "%d letters converted to the word at line %d\n", tmp, num);
		for (int i = 0; i < rw.numbck(); i++)
		{
			for (int j = 0; j < 19; j++)
			{
				if (!good[j].flagrtn())
				{
					good[j].check(rw.datbck(i));
				}
				else
				{
					fprintf(fplog, "flag config termintaed the process\n");
				}
			}
			for (int j = 0; j < 11; j++)
			{
				if (!bad[j].flagrtn())
				{
					bad[j].check(rw.datbck(i));
				}
				else
				{
					fprintf(fplog, "flag config termintaed the process\n");
				}
			}
		}
		for (int i = 0; i < 19; i++)
		{
			good[i].flag_reset();
		}
		for (int i = 0; i < 11; i++)
		{
			bad[i].flag_reset();
		}
		num++;
		printf("[INFO]%.6f Completed\n", (float)num / total);
	}
	printf("Completed.\n");
	fprintf(fplog, "[INFO]%d lines completed\n", num);
	FILE* reslog = fopen(RESLOG, "w+");
	for (int i = 0; i < 19; i++)
	{
		/*if (!i)
		{
			fprintf(reslog,"Res for good:\n");
		}*/
		fprintf(reslog, "%s\t%d\n", good[i].wdrtn(), good[i].numrtn());
	}
	for (int i = 0; i < 11; i++)
	{
		/*if (!i)
		{
			fprintf(reslog,"Res for bad:\n");
		}*/
		fprintf(reslog, "%s\t%d\n", bad[i].wdrtn(), bad[i].numrtn());
	}
	return 0;
}