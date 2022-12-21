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
    
	//�պκ� 3���� ������ �б⸦ �����ϸ� placeHist[]�� ������� ���ʷ� �Է�
    while ( 3 == fscanf(fp, "%d %d %d", &pIndex, &age, &time) ) 
    {
        int i;
        for (i = 0; i < N_HISTORY; i++) {    
            fscanf(fp, "%d", &placeHist[i]); 
        } 
        
        ifct_element = ifctele_genElement(pIndex,age,time,placeHist); //����ü�� �����  
        
        ifctdb_addTail(ifct_element); //linked list�� ����   
    }
    
    //1-3. FILE pointer close
    fclose(fp);
    
    //menu_selection == 0�̱� ������ �ݺ� ����  
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
                
            case MENU_PATIENT:  //1. Ư�� ȯ�ڿ� ���� ���� ���
            	{
            		int patientIndex;
            		
					printf("Enter Patient Index : ");  //ȯ�� ��ȣ �Է� �ޱ�  
					scanf("%d", &patientIndex);
					
					ifctele_printElement(ifctdb_getData(patientIndex)); //�Է¹��� ȯ�� ���� ���  
				
                	break;
				}	
                
            case MENU_PLACE:  //2. Ư�� ��ҿ��� ������ Ȯ�ε� ȯ�� ���� ���� ���
                {
					char place[MAX_PLACENAME];
					
					printf("Enter place name : ");  //Ư�� ��� �̸� �Է� �ޱ�  
					scanf("%s", &place);
					
					int i;
					int j;
					int num = 0; //ȯ���� ��  
					
					for(i=0; i<ifctdb_len(); i++){
						ifct_element = ifctdb_getData(i);  //ȯ�� ���� �ҷ�����
						  
						if(strcmp(place, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1))) == 0){  
							num++;                                     //place�� ���� Ȯ�� ��Ұ� ������ num 1 ���� 
							ifctele_printElement(ifct_element);        //ȯ�� ���� ���
						}	
					}
					printf("\nThere are %d patients detected in %s\n", num, place);
				}	
                break;    
                
            case MENU_AGE:  //3. Ư�� ������ ���̿� �ش��ϴ� ȯ�� ���� ���� ���
                {
                	int minAge;
					int maxAge;
					
					printf("Enter minAge : "); //�ּ� ���̰� �Է�  
					scanf("%d", &minAge); 
					
					printf("Enter maxAge : "); //�ִ� ���̰� �Է� 
					scanf("%d", &maxAge);
					
					if(minAge <= maxAge){ //�Էµ� �ּҰ� < �Էµ� �ִ밪  
						int i;
						int num = 0; //ȯ���� ��  
					
						for(i=0; i<ifctdb_len(); i++){
							ifct_element = ifctdb_getData(i); //ȯ�� ���� �ҷ����� 
							int age = ifctele_getAge(ifct_element);
						
							if(age <= maxAge && age >= minAge){        // minAge <= age <= maxAge �� �ش��ϸ�
								num++;                                 //num 1 ����
								ifctele_printElement(ifct_element);    //ȯ�� ���� ���
							}	
						}
						printf("\nThere are %d patients between the ages of %d and %d.\n", num,  minAge, maxAge);
					}
					else  //�Էµ� �ּҰ� > �Էµ� �ִ밪�̸� ���� �޽��� ���  
						printf("\nThe minimum age is older than the maximum age! Please try again\n"); 
				}
                break;
                
            case MENU_TRACK:  //4. ���� ��� �� ���� ������ ����
                {
                	int patientIndex;  //���� ȯ��  
					int infester;      //������ 
					int firstInfester; //����������  
					
					int detected_time;  //���� ȯ���� ���� Ȯ�� ����
					int place[N_HISTORY];  //���� ȯ���� �湮 ��� 
					
					printf("Enter Patient Index : ");  //ȯ�� ��ȣ �Է� �ޱ�  
					scanf("%d", &patientIndex);
					
					
					while (patientIndex != -1){  //����ȯ�ڰ� �ִ� ���� 					
						ifct_element = ifctdb_getData(patientIndex); //ȯ�� ���� �ҷ�����  
					
						detected_time = ifctele_getinfestedTime(ifct_element);  //ȯ�� ���� ���� �ҷ����� 
											
						int i;
						for(i=0; i<N_HISTORY; i++){
							place[i] = ifctele_getHistPlaceIndex(ifct_element, i); // ȯ�� ���� ��� �ҷ����� 
						} 	

						infester = trackInfester(patientIndex, detected_time, place); // ���� ȯ���� ������ ���� 
						
						if (infester > -1 || infester != patientIndex) // �����ڰ� ������ �����ڿ� ���� ����, ��� ��� 
						{
							printf("--> patient %i is infected by %i (time : %i, place : %s)\n", patientIndex, infester, detected_time, ifctele_getPlaceName(place[0]));		
						}
	
						else   //�����ڰ� ������ ���� ȯ�ڸ� ���� �����ڷ� ���� 
						{
						    firstInfester = patientIndex;
						}
						
						if(infester == patientIndex) 
							break;  
						
						patientIndex = infester;
					}
					printf("���� ������ : %d", firstInfester);	
				}    
                break;
                
            default: //����ڰ� �޴� ������ �߸����� �� ���� �޽��� ���  
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    return 0;
}

// �� ȯ�ڿ� ���� �������� ������ �־����� ���� Ȯ��
int trackInfester(int patient_no, int *detected_time, int *place){   
	int i;
	int met_time;
	int met_time_min = N_PLACE; //���� �ð��� �ּҰ� = N_PLACE�� �ʱ�ȭ  
	int infester = -1;

	for (i=0; i<ifctdb_len(); i++)  //��� ȯ�ڿ� ���Ͽ� ���� 
	{
		met_time = isMet(patient_no, i);  
		if (met_time > 0) //�����ٸ�
		{
			if (met_time < met_time_min)   //���ݱ��� ȯ�� �� �����ð��� ���� �̸���  
			{
				infester = i;              //�����ڸ� i���� ȯ�ڷ� ������Ʈ  
				met_time_min = met_time;   //���� �ð��� �ּҰ� ������Ʈ  
			}
		}
	}
	return infester;  //���� �ð��� ���� �̸� �����ڸ� ��ȯ  
}

// �� ȯ�ڰ� ���� �ð��� �����ϴ� �Լ�  
int isMet(int patient_no, int patient_cmp){ //patient_no = ���� ȯ��,  patient_cmp = ���� ȯ��  
	int time; 
	int met_time = -1;       //�� ȯ�ڰ� �����ð�, �� ��������  -1 ��ȯ  
		
	int place;               //���� ȯ���� ��� ����  
	int place_cmp;           //�� ȯ���� ��� ���� 
	
	int detected_time;       //���� ȯ���� ���� Ȯ�� ����  
	int detected_time_cmp;   //�� ȯ���� ���� Ȯ�� ����  
	 
	void *ifct_element;      //���� ȯ���� ����ü ������ ���� ������ ����  
    void *ifct_element_cmp;  //�� ȯ���� ����ü ������ ���� ������ ����
    
    //ȯ�� ���� �ҷ�����  
    ifct_element = ifctdb_getData(patient_no);       
    ifct_element_cmp = ifctdb_getData(patient_cmp);      
    
    //ȯ�� ���� Ȯ�� ���� �ҷ�����  
    detected_time = ifctele_getinfestedTime(ifct_element);
    detected_time_cmp = ifctele_getinfestedTime(ifct_element_cmp);
    
    int index_cmp; //�� ȯ���� ��� index	  
    
	int i;
	for(i=2; i<N_HISTORY; i++){ 
		//time  = ���� ȯ���� i ��° �̵���� ����;
		time = detected_time - (N_HISTORY - i) + 1;
		place = ifctele_getHistPlaceIndex(ifct_element, i);
		
		//time �������� ���ȯ�� �̵���� ���ϱ� =  convertTimeToIndex �̿�;
		index_cmp = convertTimeToIndex(time, detected_time_cmp);
		place_cmp = ifctele_getHistPlaceIndex(ifct_element_cmp, index_cmp);
		
		if(place == place_cmp) // �� ��Ұ� ������ 
			met_time = time;
	}
	
	return met_time;
}


//���������� �˰� ���� ������ �Է� �޾�, �˰� ���� ������ index ��ȯ  
int convertTimeToIndex(int time, int infestedTime){
	int index = -1;
	
	if(time <= infestedTime && time > (infestedTime - N_HISTORY)){
		index = N_HISTORY - (infestedTime - time) -1;
	}
	
	return index;
} 

