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
typedef struct ifs_ele {
	int pIndex;  //ȯ�� ��ȣ
	int age;  //ȯ�� ����	
	int time;  //���� Ȯ�� ����	
	place_t placeHist[N_HISTORY];  //���� ���� �̵����
} ifs_ele_t;

 
//ȯ�� ���� �����ϴ� �Լ� 
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


//����ü ���� ������ ������ �� �ִ� �Լ� ����

//1. ȯ�� �̵� ��� ����  
int ifctele_getHistPlaceIndex(void* obj, int index){
	ifs_ele_t *ptr = (ifs_ele_t *)obj; 
	
    return ptr->placeHist[index];  //�����ͷ� ����� ����
}

//2. ȯ�� ���� ���� ����  
int ifctele_getAge(void* obj){
	ifs_ele_t *ptr = (ifs_ele_t *)obj; 
    
	return ptr -> age; //strPtr�� ����Ű�� ����ü�� ���� ��� ���� ��ȯ
}

//3. ȯ�� ���� Ȯ�� ���� ����  
unsigned int ifctele_getinfestedTime(void* obj){
	ifs_ele_t* ptr = (ifs_ele_t*)obj;
    
    return ptr->time; //�����ͷ� ����� ����
}

//4. ���� �̸� ����  
char* ifctele_getPlaceName(int placeIndex){
	
	return countryName[placeIndex]; 
}

//ȯ�� ���� ��� �Լ�  
void ifctele_printElement(void* obj){
	ifs_ele_t* ptr = (ifs_ele_t *)obj;
	
	printf("--------------------------------------\n");
    printf("Patient Index : %d\n", ptr->pIndex);
    printf("Age : %d\n", ptr->age);
    printf("Time : %d\n", ptr->time);
    
    int i;
    for (i = 0; i < N_HISTORY-1; i++) {
        printf("%d Days ago : %d \n", N_HISTORY-1-i, ptr->placeHist[i]);
    }
    printf("Detected place : %d\n", ptr->placeHist[i]);
	printf("--------------------------------------\n");
}


