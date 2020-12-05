/**
  ******************************************************************************
  * File Name          : mkfolder.c
  * Description        : ���������ļ��У�WINDOWSϵͳ�汾��
  *	��\tab(��ʾtab����Ϊ�㼶��ϵָʾ����
  *	���磬tof.txt��Ŀ¼���ı���������ʾʱ������ִ�г�������Ŀ¼�д�����
  *		һ��lib�ļ��У�
  *		lib�ļ�������A��B�����ļ��У�
  *		A�ļ�������AA��AB�����ļ��У�
  *		AB�ļ�����ABCһ�����ļ���
  	A
	\tabAA
	\tabAB
	\tab\tabABA
	B
  *	note				:
  *	--Ŀ¼����\tab��'��'���ո�ᱻ���ԣ�
  *	--ÿ�н�֧��һ���ļ�Ŀ¼��
  *	--�໥��Ӧ����Ŀ¼�㼶��ϵ��
  *	--#�ſ�ͷΪע���У�
  *	
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 FH.
  * All rights reserved.</center></h2>
  ******************************************************************************
  */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <direct.h>	//WINDOWSϵͳ֧��

#define BOOK_LIB_INDEX_FILE		"tof.txt"
#define BOOK_LIB_FOLDER_NAME	"lib"
#define LOB_FILE_NAME			"log.txt"

#define ERROR_		"ERROR: "
#define WARNING_	"WARNING: "
#define INFO_		"INFO: "

#define FILE_PATH_LENGTH	200
#define FILE_NAME_LENGTH	200

int pref_tab_getnum(char *s)
{
	int i = 0,num = 0;
	
	assert(s);
	while(*(s + i++) == '\t'){
		num++;
	}
	return num;
}

int tab_valid_verify(int cur,int pre)
{
	if(cur == (pre + 1) || cur == pre || cur == 0 || cur < pre){
		return 0;	//valid
	}else{
		return -1;	//invalid
	}
}

int folder_name_preprocess(char *s)
{
	int i = 0,j = 0,ret;
	char tmp;
	char buf[FILE_NAME_LENGTH];
	
	assert(s);
	
	while(tmp = *(s + i)){
		if(tmp == '\t' || tmp == ' ') {
			i++;
			continue;
		}else{
			ret = memcmp((s + i),"��",(sizeof("��") - 1));
			if(!ret){
				i += (sizeof("��") - 1);
				buf[j] = '_';
			}else{
				i++;
				buf[j] = tmp;
			}
			j++;
		}
	};
	buf[j] = '\0';
	strcpy(s,buf);
	return 0;
}

int main(int argc,char *argv[])
{
	int curtab_num = 0,pretab_num = 0;
	int ret = 0;
	int len,success_num = 0,fail_num = 0,sum = 0;
	FILE *fp = NULL;
	char buf[FILE_NAME_LENGTH],prebuf[FILE_NAME_LENGTH];
	char *cpath = NULL;
	
	(void)argc;
	(void)argv;
	
	cpath = getcwd(NULL,FILE_PATH_LENGTH);
	if(cpath == NULL){
		printf(ERROR_"can't get current path\n");
		ret = -1;
		goto err;
	}
	printf(INFO_"current path:%s\n",cpath);
	chdir(cpath);
	
	//open
	fp = fopen(BOOK_LIB_INDEX_FILE,"r");
	if(fp == NULL){
		printf(ERROR_"can't find the file\n");
		ret = -1;
		goto err;
	}
	
	mkdir(BOOK_LIB_FOLDER_NAME);
	printf(INFO_"make folder %s\n",BOOK_LIB_FOLDER_NAME);
	chdir(BOOK_LIB_FOLDER_NAME);
	printf(INFO_"enter into:%s\n",BOOK_LIB_FOLDER_NAME);
	while(fgets(buf,sizeof(buf),fp) != NULL)	//��һ��
	{
		sum++;
		printf(INFO_"read line number: %d\n",sum);
		len = strlen(buf);
		if((len == 0) || (*buf == '#')){	/*���л���ע����*/
			continue;
		}			
		buf[len-1] = '\0';  /*ȥ�����з�*/
		//printf(INFO_"%s %d \n",buf,len - 1);
		
		//��ȡ��ͷ��tab����
		curtab_num = pref_tab_getnum(buf);
		printf(WARNING_"cnum:%d\tpnum:%d \n",curtab_num,pretab_num);
		
		if(tab_valid_verify(curtab_num,pretab_num) < 0){
			printf(WARNING_"tab getting failure.\n");
			printf(INFO_"%s\n",buf);
			fail_num++;
			continue;
		}
		folder_name_preprocess(buf);
		
		if(curtab_num == 0){
			//������Ŀ¼			
			chdir(cpath);
			mkdir(BOOK_LIB_FOLDER_NAME);
			chdir(BOOK_LIB_FOLDER_NAME);
			printf(INFO_"enter into:%s\n",cpath);
		}else if(curtab_num > pretab_num){
			//������һ���������ļ���
			chdir(prebuf);
			printf(INFO_"enter into:%s\n",buf);
		}else if(curtab_num < pretab_num){
			while(curtab_num != pretab_num){	
				chdir("..");
				pretab_num--;
			}
			printf(INFO_"enter into:%s\n",cpath);
		}
		
		pretab_num = curtab_num;
		//�����ļ���
		mkdir(buf);
		strcpy(prebuf,buf);
		printf(INFO_"make folder %s ok\n",buf);
		success_num++;
	}
	
	printf(INFO_"make folders finish \n");
	printf(INFO_"folder sum:%d\tsuccess num:%d\tfail num:%d\n",sum,success_num,fail_num);
		
	ret = 0;
err:
	printf(INFO_"please click any key to exit\n");
	getch();
	return ret;
}

/************************ (C) COPYRIGHT FH *****END OF FILE****/
