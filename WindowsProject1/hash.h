#include"uthash.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef int Status;
//վ��������ݽṹ
typedef struct node{
    int isTransfer;
    char whose_where[50];
    int index;
}node;

typedef struct hash_struct{
    char name[20];                    /* key */
    node data;
    UT_hash_handle hh;                /* makes this structure hashable */
}hash_struct;

/* �������ܣ���hash������ӽ��
 * ��ڲ�����stations��hash��station_name��keyֵ��data��������
 * ���ڲ�����ָ����ӵĽ��Ĵ洢��ַ��ָ�� */
hash_struct *add_station(hash_struct **stations,char* station_name,node data);

/* �������ܣ���hash������keyΪstation_name�Ľ��
 * ��ڲ�����stations��hash��station_name��keyֵ
 * ���ڲ�����ָ���ҵ��Ľ��Ĵ洢��ַ��ָ�룬��û���ҵ����򷵻�-1 */
hash_struct *find_station(hash_struct *stations,char* station_name);

/* �������ܣ�ɾ��hash���е�һ����ϣ���
 * ��ڲ�����stations��hash��station��ָ��ɾ������ָ�� */
void delete_station(hash_struct **stations,hash_struct *station);

#ifdef __cplusplus
}
#endif
