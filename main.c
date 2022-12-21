//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int *detected_time, int *place);
int isMet(int patient_no, int patient_cmp);
int convertTimeToIndex(int time, int infestedTime);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY]; 
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    } 
    
    
    //1-2. loading each patient informations
    
	//앞부분 3개의 데이터 읽기를 성공하면 placeHist[]에 감염경로 차례로 입력
    while ( 3 == fscanf(fp, "%d %d %d", &pIndex, &age, &time) ) 
    {
        int i;
        for (i = 0; i < N_HISTORY; i++) {    
            fscanf(fp, "%d", &placeHist[i]); 
        } 
        
        ifct_element = ifctele_genElement(pIndex,age,time,placeHist); //구조체로 만들기  
        
        ifctdb_addTail(ifct_element); //linked list에 저장   
    }
    
    //1-3. FILE pointer close
    fclose(fp);
    
    //menu_selection == 0이기 전까지 반복 실행  
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:  //1. 특정 환자에 대한 정보 출력
            	{
            		int patientIndex;
            		
					printf("Enter Patient Index : ");  //환자 번호 입력 받기  
					scanf("%d", &patientIndex);
					
					ifctele_printElement(ifctdb_getData(patientIndex)); //입력받은 환자 정보 출력  
				
                	break;
				}	
                
            case MENU_PLACE:  //2. 특정 장소에서 감염이 확인된 환자 관련 정보 출력
                {
					char place[MAX_PLACENAME];
					
					printf("Enter place name : ");  //특정 장소 이름 입력 받기  
					scanf("%s", &place);
					
					int i;
					int j;
					int num = 0; //환자의 수  
					
					for(i=0; i<ifctdb_len(); i++){
						ifct_element = ifctdb_getData(i);  //환자 정보 불러오기
						  
						if(strcmp(place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1))) == 0){  
							num++;                                     //place와 감염 확인 장소가 같으면 num 1 증가 
							ifctele_printElement(ifct_element);        //환자 정보 출력
						}	
					}
					printf("\nThere are %d patients detected in %s\n", num, place);
				}	
                break;    
                
            case MENU_AGE:  //3. 특정 범위의 나이에 해당하는 환자 관련 정보 출력
                {
                	int minAge;
					int maxAge;
					
					printf("Enter minAge : "); //최소 나이값 입력  
					scanf("%d", &minAge); 
					
					printf("Enter maxAge : "); //최대 나이값 입력 
					scanf("%d", &maxAge);
					
					if(minAge <= maxAge){ //입력된 최소값 < 입력된 최대값  
						int i;
						int num = 0; //환자의 수  
					
						for(i=0; i<ifctdb_len(); i++){
							ifct_element = ifctdb_getData(i); //환자 정보 불러오기 
							int age = ifctele_getAge(ifct_element);
						
							if(age <= maxAge && age >= minAge){        // minAge <= age <= maxAge 에 해당하면
								num++;                                 //num 1 증가
								ifctele_printElement(ifct_element);    //환자 정보 출력
							}	
						}
						printf("\nThere are %d patients between the ages of %d and %d.\n", num,  minAge, maxAge);
					}
					else  //입력된 최소값 > 입력된 최대값이면 오류 메시지 출력  
						printf("\nThe minimum age is older than the maximum age! Please try again\n"); 
				}
                break;
                
            case MENU_TRACK:  //4. 감염 경로 및 최초 전파자 추적
                {
                	int patientIndex;  //현재 환자  
					int infester;      //전파자 
					int firstInfester; //최초전파자  
					
					int detected_time;  //현재 환자의 감염 확인 일자
					int place[N_HISTORY];  //현재 환자의 방문 장소 
					
					printf("Enter Patient Index : ");  //환자 번호 입력 받기  
					scanf("%d", &patientIndex);
					
					
					while (patientIndex != -1){  //현재환자가 있는 동안 					
						ifct_element = ifctdb_getData(patientIndex); //환자 정보 불러오기  
					
						detected_time = ifctele_getinfestedTime(ifct_element);  //환자 감염 시점 불러오기 
											
						int i;
						for(i=0; i<N_HISTORY; i++){
							place[i] = ifctele_getHistPlaceIndex(ifct_element, i); // 환자 감염 경로 불러오기 
						} 	

						infester = trackInfester(patientIndex, detected_time, place); // 현재 환자의 전파자 추적 
						
						if (infester > -1 || infester != patientIndex) // 전파자가 있으면 전파자와 감염 시점, 장소 출력 
						{
							printf("--> patient %i is infected by %i (time : %i, place : %s)\n", patientIndex, infester, detected_time, ifctele_getPlaceName(place[0]));		
						}
	
						else   //전파자가 없으면 현재 환자를 최초 전파자로 간주 
						{
						    firstInfester = patientIndex;
						}
						
						if(infester == patientIndex) 
							break;  
						
						patientIndex = infester;
					}
					printf("최초 감염자 : %d", firstInfester);	
				}    
                break;
                
            default: //사용자가 메뉴 선택을 잘못했을 때 오류 메시지 출력  
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    return 0;
}

// 각 환자에 대해 감염가능 시점에 있었는지 여부 확인
int trackInfester(int patient_no, int *detected_time, int *place){   
	int i;
	int met_time;
	int met_time_min = N_PLACE; //만난 시간의 최소값 = N_PLACE로 초기화  
	int infester = -1;

	for (i=0; i<ifctdb_len(); i++)  //모든 환자에 대하여 실행 
	{
		met_time = isMet(patient_no, i);  
		if (met_time > 0) //만났다면
		{
			if (met_time < met_time_min)   //지금까지 환자 중 만난시간이 가장 이르면  
			{
				infester = i;              //전파자를 i번쨰 환자로 업데이트  
				met_time_min = met_time;   //만난 시간의 최소값 업데이트  
			}
		}
	}
	return infester;  //만난 시간이 가장 이른 전파자를 반환  
}

// 두 환자가 만난 시간을 산출하는 함수  
int isMet(int patient_no, int patient_cmp){ //patient_no = 현재 환자,  patient_cmp = 비교할 환자  
	int time; 
	int met_time = -1;       //두 환자가 만난시간, 안 만났으면  -1 반환  
		
	int place;               //현재 환자의 장소 정보  
	int place_cmp;           //비교 환자의 장소 정보 
	
	int detected_time;       //현재 환자의 감염 확인 일자  
	int detected_time_cmp;   //비교 환자의 감염 확인 일자  
	 
	void *ifct_element;      //현재 환자의 구조체 접근을 위한 포인터 선언  
    void *ifct_element_cmp;  //비교 환자의 구조체 접근을 위한 포인터 선언
    
    //환자 정보 불러오기  
    ifct_element = ifctdb_getData(patient_no);       
    ifct_element_cmp = ifctdb_getData(patient_cmp);      
    
    //환자 감염 확인 일자 불러오기  
    detected_time = ifctele_getinfestedTime(ifct_element);
    detected_time_cmp = ifctele_getinfestedTime(ifct_element_cmp);
    
    int index_cmp; //비교 환자의 장소 index	  
    
	int i;
	for(i=2; i<N_HISTORY; i++){ 
		//time  = 현재 환자의 i 번째 이동장소 시점;
		time = detected_time - (N_HISTORY - i) + 1;
		place = ifctele_getHistPlaceIndex(ifct_element, i);
		
		//time 시점에서 대상환자 이동장소 구하기 =  convertTimeToIndex 이용;
		index_cmp = convertTimeToIndex(time, detected_time_cmp);
		place_cmp = ifctele_getHistPlaceIndex(ifct_element_cmp, index_cmp);
		
		if(place == place_cmp) // 두 장소가 같으면 
			met_time = time;
	}
	
	return met_time;
}


//감염시점과 알고 싶은 시점을 입력 받아, 알고 싶은 시점의 index 반환  
int convertTimeToIndex(int time, int infestedTime){
	int index = -1;
	
	if(time <= infestedTime && time > (infestedTime - N_HISTORY)){
		index = N_HISTORY - (infestedTime - time) -1;
	}
	
	return index;
} 

