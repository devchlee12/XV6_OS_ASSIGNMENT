#include "types.h"
#include "stat.h"
#include "user.h"

int line;
char buf[512];
int file_size_count(int fd);
char* my_strcat(char *src, char *dest,int n);
void htac(int fd, char *filename);
int line_count(char *arr);
int max_len_count(char *arr);

void htac(int fd, char *filename)
{
	int n;
	int file_size;
	char *file_data_all;
	int line_cnt;
	char **file_data_line;
	int max_len;
	int i,j,k;
	int line_cnt_temp;
	int fd2;

	//파일 크기를 알아냅니다.
	file_size = file_size_count(fd);
	//파일의 모든 값을 읽어서 file_data_all변수에 char형으로 넣기 위해 파일 크기에 널문자 까지 포함한 크기를 동적할당합니다.
	file_data_all = (char *)malloc(sizeof(char) * (file_size + 1));
	//만든 배열의 첫 요소에 널문자를 집어넣어서 빈 문자열으로 초기화 합니다.
	file_data_all[0] = 0;

	//이미 위에서 파일디스크립터의 오프셋이 맨 뒤로 갔으므로 다시 파일 디스크립터를 얻습니다.
	fd2 = open(filename, 0);
	//파일의 모든 데이터를 배열에 담습니다.
	while ((n = read(fd2, buf, sizeof(buf))) > 0)
	{
		my_strcat(buf, file_data_all, n);
	}
	close(fd2);

	//배열의 라인 개수를 센다.
	line_cnt = line_count(file_data_all);

	//메모리를 해제하기 위해 line_cnt 변수를 복제해놓는다.
	line_cnt_temp = line_cnt;

	//이차원 배열 만들기.
	file_data_line = (char **)malloc(sizeof(char *) * line_cnt);
	
	//최대 행 길이를 센다.
	max_len = max_len_count(file_data_all);
	
	//이차원 배열 할당
	for(int i = 0;i<line_cnt;i++)
	{
		file_data_line[i] = (char *)malloc(sizeof(char) * max_len);
	}

	//이차원 배열에 한 행씩 넣는다.
	i = 0;
	j = 0;
	k = 0;
	while (file_data_all[i])
	{
		if (file_data_all[i] == '\n')
		{
			file_data_line[j][k] = 0;
			k = 0;
			j++;
		}
		else
		{
			file_data_line[j][k] = file_data_all[i];
			k++;
		}
		i++;
	}
	file_data_line[j][k] = 0;

	//뒤에서부터 한 행 씩 읽는다.
	while (line > 0 && line_cnt > 0)
	{
		i = 0;
		while (file_data_line[line_cnt -1][i])
		{
			write(1, &file_data_line[line_cnt - 1][i], 1);
			i++;
		}
		write(1, "\n", 1);
		line--;
		line_cnt--;
	}

	//메모리 누수를 방지하기 위해 메모리를 해제한다.
	for(int i = 0;i<line_cnt_temp;i++)
	{
		free(file_data_line[i]);
	}
	free(file_data_line);
}

//행의 최대 길이 세기
int max_len_count(char *arr)
{
	int i = 0;
	int max_len = 0;
	int temp_len = 0;
	
	while(arr[i])
	{
		if (arr[i] == '\n')
		{
			if (max_len < temp_len)
				max_len = temp_len;
			temp_len = -1;
		}
		temp_len++;
		i++;
	}
	if (max_len < temp_len)
		max_len = temp_len;

	return (max_len + 1);
}

//배열안에 몇 라인이 있는지 센다.
int line_count(char *arr)
{
	int i = 0;
	int line_cnt = 0;
	
	while(arr[i])
	{
		if (arr[i] == '\n')
		{
			line_cnt++;
		}
		i++;
	}

	return (line_cnt + 1);
}

//나만의 strncat 구현 - n개를 뒤에 붙여넣는 함수
char* my_strcat(char *src, char *dest, int n)
{
	int i = 0;
	int j = 0;

	while(dest[i])
		i++;
	
	while(j < n)
	{
		dest[i] = src[j];
		i++;
		j++;
	}

	dest[i] = 0;
	return dest;
}

//파일의 크기(바이트 수)세기
int file_size_count(int fd)
{
	int n,size = 0;
	char size_buf[1024];

	while((n = read(fd, size_buf, sizeof(size_buf))) > 0)
	{
		size += n;
	}
	return size;
}
//메인 함수
int main(int argc, char *argv[])
{
	int fd, i;

	//인자가 들어오지 않으면 오류
	if (argc <= 1)
	{
		printf(1, "htac: not enough arguments\n");
		exit();
	}

	//첫 인자를 line변수로
	line = atoi(argv[1]);

	//뒤에서부터 파일 하나씩 읽어서 출력
	for (i = argc - 1; i >= 2; i--)
	{
		if((fd = open(argv[i], 0)) < 0)
		{
			printf(1, "htac: cannot open %s\n", argv[i]);
			exit();
		}
		htac(fd, argv[i]);
		close(fd);
	}
	exit();
}
