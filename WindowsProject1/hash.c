#include"head.h"
#include"hash.h"

hash_struct* add_station(hash_struct **stations,char* station_name,node data){
    hash_struct *s;
    //HASH_FIND_STR(stations,&station_name,s);
    s=(hash_struct *)malloc(sizeof *s);
    if(s!=NULL){
        strcpy_s(s->name, sizeof(char) * strlen(station_name)+1,station_name);
        HASH_ADD_STR(*stations,name,s);
        s->data=data;
        return s;
    }else exit(1);
}

hash_struct *find_station(hash_struct *stations,char* station_name) {
    hash_struct *s;
    s=(hash_struct *)malloc(sizeof *s);
    HASH_FIND_STR( stations, station_name,s );  /* s: output pointer */
    return s;
}

void delete_station(hash_struct **stations,hash_struct *station) {
    HASH_DEL(*stations, station);  /* station: pointer to delete */
    free(station);             /* optional; it's up to you! */
}

