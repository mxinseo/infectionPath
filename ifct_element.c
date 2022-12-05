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
 
//ȯ�� ���� ����ü ����  
typedef struct ifs_ele{
	//ȯ�� ��ȣ
	int pIndex; 
	//ȯ�� ����  
	int age;
	//���� Ȯ�� ����
	int time;
	//���� ���� �̵����
	place_t placeHist[N_HISTORY];
} ifs_ele_t;


static ifs_ele_t ifsarray[20];
static int ifs_cnt;

 
//ȯ�� ���� �����ϴ� �Լ�   
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]){
	//ifsarray �迭�� ifs_cnt ��° ��ҿ� �Է� ���� ���� 
	ifsarray[ifs_cnt].pIndex = index; 
	ifsarray[ifs_cnt].age = age;
	ifsarray[ifs_cnt].time = detected_time;
	int i;
	for(i=0; i<N_HISTORY; i++){
		ifsarray[ifs_cnt].placeHist[i] = history_place[i];
	}
	
	ifs_cnt++;
	
	return (void*)&ifsarray[ifs_cnt];
}


//����ü ���� ������ ������ �� �ִ� �Լ� ����

//1. ȯ�� �̵� ��� ����  
int ifctele_getHistPlaceIndex(void* obj, int index){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj; 
    (int*)strPtr->placeHist;
    return &strPtr;  //�����ͷ� ����� ����
}

//2. ȯ�� ���� ���� ����  
int ifctele_getAge(void* obj){
	ifs_ele_t *strPtr = (ifs_ele_t *)obj; 
    (int*)strPtr -> age;
	return &strPtr; //strPtr�� ����Ű�� ����ü�� ���� ��� ���� ��ȯ
}


//3. ȯ�� ���� Ȯ�� ���� ����  
unsigned int ifctele_getinfestedTime(void* obj){
	ifs_ele_t* strPtr = (ifs_ele_t*)obj;
    (int*)strPtr->time;
    return &strPtr; //�����ͷ� ����� ����
}

//4. ���� �̸� ����  
char* ifctele_getPlaceName(int placeIndex){
	
	return (void*)&countryName[placeIndex][MAX_PLACENAME]; //�����ͷ� ����� ����	
}

//ȯ�� ���� ��� �Լ�  
void ifctele_printElement(void* obj){
	ifs_ele_t* strPtr = (ifs_ele_t *)obj;
	
	int i;
    int j;
    for (i = 0; i < ifs_cnt; i++) {
        printf("Patient Index : %d\n", strPtr->pIndex);
        printf("Age : %d\n", strPtr->age);
        printf("Time : %d\n", strPtr->time);
        for (j = 0; j < N_HISTORY; j++) {
            printf("%d \n", strPtr->placeHist[j]);
        }
	}
}


