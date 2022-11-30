//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

//N_PLACE = 40, MAX_PLACENAME = 100
char countryName[N_PLACE+1][MAX_PLACENAME] =  
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};
 
 
typedef struct ifs_ele{
	//환자 번호
	int pIndex; 
	//환자 나이  
	int age;
	//감염 확인 시점
	int time;
	//감염 직전 이동경로
	place_t placeHist[N_HISTORY];
} ifs_ele_t;


static ifs_ele_t ifsarray[20];
static int ifs_cnt;


//환자 정보 생성하는 함수  
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]){
	//ifsarray 배열의 ifs_cnt 번째 요소에 입력 내용 저장
	ifsarray[ifs_cnt].pIndex = index; 
	ifsarray[ifs_cnt].age = age;
	ifsarray[ifs_cnt].time = detected_time;
	int i;
	for(i=0; i<N_HISTORY; i++){
		ifsarray[ifs_cnt].placeHist[i] = history_place[i];
	}
	
	ifs_cnt++;
	
	return (void*)&ifsarray[ifs_cnt-1];
}


//환자 정보를 얻는 함수들 
int ifctele_getHistPlaceIndex(void* obj, int index){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj; 
	
	return (void*)&ifsarray[index].placeHist; //포인터로 멤버에 접근
}

int ifctele_getAge(void* obj){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj; 
	
	return (void*)&ifsarray[ifs_cnt].age; //포인터로 멤버에 접근
}

unsigned int ifctele_getinfestedTime(void* obj){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj; 
	
	return (void*)&ifsarray[ifs_cnt].time; //포인터로 멤버에 접근
}

char* ifctele_getPlaceName(int placeIndex){
	
	return (void*)&countryName[placeIndex][MAX_PLACENAME]; //포인터로 멤버에 접근	
}


void ifctele_printElement(void* obj){
	ifs_ele_t* strPtr = (ifs_ele_t *)obj;
	
	//print element
}


