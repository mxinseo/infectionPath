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
 
//환자 정보 구조체 생성  
typedef struct ifs_ele {
	int pIndex;  //환자 번호
	int age;  //환자 나이	
	int time;  //감염 확인 시점	
	place_t placeHist[N_HISTORY];  //감염 직전 이동경로
} ifs_ele_t;

 
//환자 정보를 생성하는 함수 
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]){
	ifs_ele_t *ptr;
	 
	ptr = (ifs_ele_t *)malloc(1*sizeof(ifs_ele_t));
	ptr->pIndex = index; 
	ptr->age = age;
	ptr->time = detected_time;
	
	int i;
	for(i=0; i<N_HISTORY; i++){
		ptr->placeHist[i] = history_place[i];
	}
	
	return ptr;
}

//장소 인덱스(정수)를 장소 이름(문자열)으로 반환하는 함수  
char* ifctele_getPlaceName(int placeIndex){
	
	return countryName[placeIndex]; 
}


//구조체 내부 변수에 접근할 수 있는 함수들  

//1. 특정 시점 방문 장소 접근  
int ifctele_getHistPlaceIndex(void* obj, int index){
	ifs_ele_t *ptr = (ifs_ele_t *)obj; 
	
    return ptr->placeHist[index];  //ptr가 가리키는 구조체의 index번째 방문 장소 반환  
	}

//2. 환자 나이 정보 접근  
int ifctele_getAge(void* obj){
	ifs_ele_t *ptr = (ifs_ele_t *)obj; 
    
	return ptr -> age; //ptr가 가리키는 구조체의 나이 멤버 변수 반환
 }

//3. 환자 감염 확인 일자 접근  
unsigned int ifctele_getinfestedTime(void* obj){
	ifs_ele_t* ptr = (ifs_ele_t*)obj;
    
    return ptr->time; //포인터로 멤버에 접근
}


//환자 정보 출력 함수  
void ifctele_printElement(void* obj){
	ifs_ele_t* ptr = (ifs_ele_t *)obj;
	
	printf("--------------------------------------\n");
    printf("Patient Index : %d\n", ptr->pIndex);
    printf("Patient Age : %d\n", ptr->age);
    printf("Detected time : %d\n", ptr->time);
    
    int i;
    int place;
    printf("Path History : ");
    for (i = 0; i < N_HISTORY; i++) {
    	place = ifctele_getHistPlaceIndex(ptr, i);
    	if(i==N_HISTORY-1)
    		printf("%s(%d)", ifctele_getPlaceName(place), ptr->time-(N_HISTORY-i)+1);
        else 
			printf("%s(%d) -> ", ifctele_getPlaceName(place), ptr->time-(N_HISTORY-i)+1);
    }
	printf("\n--------------------------------------\n");
}


