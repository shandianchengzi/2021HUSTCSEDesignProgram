#include"head.h"
#include"readData.h"

/* 默认拥挤度 */
int crowded[linenum][timepart] = { 40, 80, 40, 40, 40, 75, 40, 40, 40, 
								   50, 50, 50, 50, 50, 50, 50, 50, 50, 
								   40, 80, 40, 40, 40, 75, 40, 40, 40, 
								   50, 50, 50, 50, 50, 50, 50, 50, 50, 
								   15, 15, 15, 15, 15, 15, 15, 65, 15, 
								   15, 15, 15, 15, 15, 15, 15, 65, 15, 
								   20, 20, 20, 65, 20, 20, 20, 20, 20};

lineInfo* Init_lineInfo() {
	lineInfo* l;
	l = (lineInfo*)malloc(sizeof(lineInfo) * linenum);
	if (!l)
		exit(1);
	return l;
}

void Copy_Default_lineInfo(lineInfo* L)
{
	for (int i = 0;i < linenum;i++)
	{
		for (int j = 0;j < timepart;j++)
		{
			(L[i]).crowded[j] = crowded[i][j];
		}
	}
}

Status read_Data(FILE* fp, hash_struct** stations, lineInfo* lineDetail) {
	node test;
	char name[20];
	int index = 0;
	for (int k = 0; k < linenum; k++)
	{
		//读入一条线路的信息
		fscanf_s(fp, "%d", &lineDetail[k].lineIndex);
		fscanf_s(fp, "%*[^0-9]s");
		fscanf_s(fp, "%d", &lineDetail[k].lineFull);
		fscanf_s(fp, "%*[^0-9]s");
		fscanf_s(fp, "%f", &lineDetail[k].lineLength);
		fscanf_s(fp, "%*[^0-9]s");
		fscanf_s(fp, "%d", &lineDetail[k].lineCount);
		lineDetail[k].lineAverage = lineDetail[k].lineLength / lineDetail[k].lineCount;
		fscanf_s(fp, "%*s");//跳过本行其余内容

		//读入本线路的站点信息，并初始化站点hash表stations
		int j = 1, isOut = 1;
		while (isOut)
		{
			fscanf_s(fp, "%*d");
			fscanf_s(fp, "%[^,(;]s", name, 20);
			test.isTransfer = 0;
			//判断分割符号，以确定是否是换乘结点
			char split = fgetc(fp);
			if (split == '(')
			{
				fscanf_s(fp, "%*[^)]s");
				test.isTransfer = 1;
				fgetc(fp);
				split = fgetc(fp);
				if (split == ';')
					isOut = 0;
			}
			else if (split == ';')
				isOut = 0;

			strcpy_s(test.whose_where, 20, "");
			//若没有找到名字为name的结点，则添加该结点；否则仅对whose_where操作
			struct hash_struct* sss = find_station(*stations, name);
			if (!sss)
			{
				sss = add_station(stations, name, test);
				sss->data.index = index;
				index++;
			}
			else
			{
				int numnow = strlen(sss->data.whose_where);
				sss->data.whose_where[numnow - 1] = ',';
			}
			char where[5];
			_itoa_s(j, where, 5, 10);
			strcpy_s(lineDetail[k].lineStations[j], sizeof(char) * strlen(name) + 1, name);//初始化线路的站点信息
			//初始化站点信息
			test.whose_where[0] = '0' + k;
			test.whose_where[1] = '.';
			test.whose_where[2] = 0;
			strcat_s(test.whose_where, 20, where);
			strcat_s(test.whose_where, 20, ";");
			strcat_s(sss->data.whose_where, 20, test.whose_where);
			if (isOut) {
				fscanf_s(fp, "%*[^0-9]s");
				j++;
			}
			else j = 0;
		}
	}

	Copy_Default_lineInfo(lineDetail);
	rewind(fp);
	return 0;
}
