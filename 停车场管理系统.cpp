#include<iostream>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define MAX 100
#define PAVEMENT_TIME 0
using namespace std;
int Position=0;//���ڱ���ϵ�λ�� 
int count=0;//ͳ�Ʊ���ϵĳ������������±�
int waitTime=0;//��¼�����ڱ����ͣ����ʱ��
int EnterTime=0;//��¼���Ӹս�������ʱ��



//ջ
typedef struct {
	int LicensePlate[100]; //���ƺţ����100�� 
	int arriveTime[100];//����ʱ�䣬��Ӧ������ 
	int Place;//ͣ������λλ�� 
	int Capacity;//����
} Stack;

//����
typedef struct 	QueneNode {
	int LicensePlate;//���ƺ� 
	int arriveTime;//����ʱ�� 
	//Ϊ�˸����㣬position�������� 
	int position;//���ڱ���ϵ�λ�� 
	struct QueneNode *next;
} QueneNode,*queue;
typedef struct {
	queue front;
	queue rear;
} Queue;

//��ʼ��ջ
void initStack(Stack &S,int Capacity) {
	S.Place=-1;
	S.Capacity=Capacity;
}

//���ջ�Ƿ��ܱ����
bool emptyStack(Stack &S) {
	if(S.Place==-1) {
	return false;
	} else {
		return true;
	}
}
//���ջ�Ƿ��ܱ�����
bool fullStack(Stack &S) {
	if(S.Place==S.Capacity-1) {
		return false;
	} else {
		return true;
	}
}

//��ջ
void push(Stack &S,int LicensePlate,int arriveTime) {
	S.Place++;
	S.LicensePlate[S.Place]=LicensePlate;
	S.arriveTime[S.Place]=arriveTime;
}
//��ջ
void pop(Stack &S,int &LicensePlate,int &arriveTime) {
	LicensePlate=S.LicensePlate[S.Place];
	arriveTime=S.arriveTime[S.Place];
	S.Place--;
}

//��ʼ������
void initQueue(Queue &Q) {
	Q.front=Q.rear=(queue)malloc(sizeof(QueneNode));//��ͷָ�롢��βָ��ָ��ͬһ�ս��
	if(!Q.front) {
		exit(-1);//�ڴ����ʧ�ܣ��˳�
	}
	Q.front->next=NULL;
}

//�������Ƿ��ܱ����
bool emptyQueue(Queue &Q) {
	if(Q.front==Q.rear) {
		return false;
	} else {
		return true;
	}
}

//�����
void inQueue(Queue &Q,int LicensePlate,int arriveTime) {
	queue p;
	p=(queue)malloc(sizeof(QueneNode));//����һ�����ռ�
	p->LicensePlate=LicensePlate;
	p->arriveTime=arriveTime;
	p->next=NULL;
	Q.rear->next=p;//��βָ��ָ��p
	Q.rear=p;//��βָ�����һλ
	Position++;
}
//������
void outQueue(Queue &Q,int &LicensePlate,int &arriveTime) {
	queue p;
	p=(queue)malloc(sizeof(QueneNode));//����һ�����ռ�
	p=Q.front->next;//ָ��һ���µĽ��ָ���ͷָ��ָ��Ľ��
	
	LicensePlate=p->LicensePlate;//���ؽ������ֵ
	arriveTime=p->arriveTime;
	Q.front->next=p->next;//��pָ�����һ���ݸ���ͷָ�룬����һ����ǰ�Ƶ���ͷ
	if(Q.rear==p) {
		Q.rear=Q.front;//ʹ��βָ��ص���ʼλ��
	}
	free(p);//�ͷ�p��ָ���ռ�
	Position--;//���ڱ����λ�� ��ǰ�ƶ� 
}

//0.0�ж�ͣ�������Ƿ����г����������ظ�������ͬ���ƺŵĳ� 
bool judgeParking(Stack S,char state,int LicensePlate){
	int i=S.Place;
	if(state!='A'){
		return true;//ֻ���ж� A(����) ��� 
	}
	while(i!=-1) {
		if(LicensePlate==S.LicensePlate[i]){
			cout<<"ͣ�������Ѿ��иó��������½���������棡\n";
			return false;
		}
		i--;//�������
	}
	return true;
}

//0.1�жϱ�����Ƿ����г����������ظ�������ͬ���ƺŵĳ� 
bool judgePavement(Queue Q,char state,int LicensePlate){
	if(state!='A'){
		return true;//ֻ���ж� A(����) ��� 
	}
	queue p=Q.front->next;//ָ��һ���µĽ��ָ���ͷָ��ָ��Ľ��
	while(p!=NULL) {
		if(LicensePlate==p->LicensePlate){
			cout<<"������Ѿ��иó��������½���������棡\n";
			return false;
		}
		p=p->next;//�������
	}
	return true;
}

//1.���� ��/�� ͣ����
void CarAccess(Stack &S,Stack &LS,Queue &Q,int parkingFee,int pavementFee,int time[]) {
	
	int LicensePlate,arriveTime;	
	char state;//״̬����
	
	cout<<"�����복��״̬��A��վ/D��վ�������ƺ�(����) �� ʱ����������վʱ��/��վʱ�䣩��\n���� E 0 0 ��������\n\n";
	cin>>state>>LicensePlate>>arriveTime;
	while((judgeParking(S,state,LicensePlate)) && (judgePavement(Q,state,LicensePlate))&&state!='E') {
			int leaveTime=arriveTime;
			if(state=='A') {
				//ͣ�����Ƿ�Ϊ��
				if(fullStack(S)) {
					push(S,LicensePlate,arriveTime);//������ͣ����
					cout<<"���ƺ��� "<<LicensePlate<<" �ĳ�ͣ��ͣ������λ���� "<<S.Place+1<<endl;
				} else {
				
					//��¼���� �ս����� ��ʱ��
					EnterTime=arriveTime;
					//��¼������ƺ�
					time[count]=LicensePlate; 
					count++;
					//������Ʒѣ���˳�ʼĬ����ֵΪ ���� 0
					inQueue(Q,LicensePlate,arriveTime);//ͣ�����ǿգ�ͣ�ڱ����
					cout<<"���ƺ��� "<<LicensePlate<<" �ĳ��ڱ����λ���� "<<Position<<endl;
				}
			} else if(state=='D') {
				//��ѯ�ó�
				bool yes=false;
				bool is=false;
				//�޸ó�
				for(int i=0; i<=S.Place; i++) {
					if(S.LicensePlate[i]==LicensePlate) {
						is=true;
						break;
					}
				}
	
				if(!is) {
					cout<<"ͣ�����޳��ƺ�Ϊ "<<LicensePlate<<" �ĳ���"<<endl;
				}
	
				for(int j=S.Place; j>=0; j--) {
					if(!is) {
						break;//ͣ�������޳�������ѭ��
					}
					if(S.LicensePlate[j]!=LicensePlate) {
						pop(S,LicensePlate,arriveTime);
						push(LS,LicensePlate,arriveTime);
						if(S.Place==0) {
							yes=true;
							break;
						}
					} else {
						yes=true;
						break;
					}
				}
	
				if(yes) {
					bool to=true; 
					//�ҵ��˸ó�
					pop(S,LicensePlate,arriveTime);//�ó��뿪ͣ����
					
					for(int i=0;i<=count;i++){
						
						if(LicensePlate==time[i]){
							//����Լ���� 0 ʱ����� 
							cout<<"���ƺ��� "<<LicensePlate<<" �ĳ��뿪�����ͣ��ʱ���ǣ�"<<arriveTime-EnterTime<<
						    "Сʱ���������� "<<(pavementFee*(arriveTime-EnterTime))<<" Ԫ��"<<endl;
						    
							cout<<"���ƺ��� "<<LicensePlate<<" �ĳ��뿪ͣ������ͣ��ʱ���ǣ�"<<(leaveTime-arriveTime)<<
						    "Сʱ���������� "<<parkingFee*(leaveTime-arriveTime)<<" Ԫ��"<<endl;
						    
						    to=false;
						    break;
						}
					}
					if(to){
						cout<<"���ƺ��� "<<LicensePlate<<" �ĳ�δ�ڱ��ͣ��������0Ԫ\n"; 
		
						cout<<"���ƺ��� "<<LicensePlate<<" �ĳ��뿪ͣ������ͣ��ʱ���ǣ�"<<(leaveTime-arriveTime)<<
					    "Сʱ���������� "<<parkingFee*(leaveTime-arriveTime)<<" Ԫ��"<<endl;  
					}
					
					

					//����ʱջ��ĳ��Ž�ͣ����
					while(emptyStack(LS)) {
						pop(LS,LicensePlate,arriveTime);
						push(S,LicensePlate,arriveTime);
					}
					//���������г�������ͣ����
					if(emptyQueue(Q)) {
						outQueue(Q,LicensePlate,arriveTime);
						push(S,LicensePlate,leaveTime);
					}
				}
			}
			cin>>state>>LicensePlate>>arriveTime;
	}
}

//2.��ʾͣ����������Ϣ
void ShowParkMessage(int size,int fee) {
	cout<<"\n ���ƣ���ʱͣ����"
	    <<"\n ��λ����"<<size
	    <<"\n �շ������"<<fee<<"Ԫ/ʱ"<<endl;
}

//3.��ѯ��ǰͣ�����ڳ�����Ϣ
void SelectCarMessage(Stack S) {
	int i=S.Place;
	bool have=true;
	
	if(i==-1){
		have=false;
		cout<<"\nͣ�������޳������ڣ�\n";
	}
	
	cout<<"---------------------------------------------------------------------\n"
		<<"  ״̬(A/D)    |    ���ƺ�     |     ���ڳ�λ     |     ����ʱ��     \n";
		
	while((i!=-1) && have) {
		cout<<"      A        |      "<<S.LicensePlate[i]<<"        |         "<<(i+1)<<"        |         "<<S.arriveTime[i]<<endl;
		i--;//�������
	}
	
	cout<<"---------------------------------------------------------------------\n\n";
}

//4.�鿴���������Ϣ
void DetourLaneCarMessage(Queue Q) {
	queue p=Q.front->next;//ָ��һ���µĽ��ָ���ͷָ��ָ��Ľ��
	bool have=true;
	int pos=1;
	
	if(p==NULL){
		have=false;
		cout<<"\n������޳������ڣ�\n";
	}
	
	cout<<"-----------------------------------------------------\n"
		<<"    ���ƺ�     |      ���λ��    |      ����ʱ��    \n";
		
	while((p!=NULL) && have) {
		cout<<"      "<<p->LicensePlate<<"        |         "<<pos++<<"        |         "<<p->arriveTime<<endl;
		p=p->next;//���Ʊ���
	}
	
	cout<<"-----------------------------------------------------\n\n";
}

//5.���ͣ��������
void CleanParkCar(Stack &S) {
	while(emptyStack(S)) {
		S.Place--;
	}
	cout<<"ͣ������������գ�\n";
}

//6.�鿴ͣ������λ����
void ViewMargin(Stack S,int size) {
	if(size==0){
		//������Աû����������  ��  ʣ�����Ѿ�Ϊ 0�������0 
		cout<<"ͣ������λʣ�� 0 ��\n\n";
	}else{
		//��Ϊ 0 ʱ����
		//size-S.top-1 ---> -1 ����Ϊ S.top��0��ʼ����
		cout<<"ͣ������λʣ�� "<<(size-S.Place-1)<<" ��\n\n"; 
	} 
}

//7.��ձ������
void CleanDetourLane(Queue &Q) {
	while(emptyQueue(Q)) {
		queue p;
		p=(queue)malloc(sizeof(QueneNode));//����һ�����ռ�
		p=Q.front->next;//ָ��һ���µĽ��ָ���ͷָ��ָ��Ľ��
		Q.front->next=p->next;//��pָ�����һ���ݸ���ͷָ�룬����һ����ǰ�Ƶ���ͷ
		if(Q.rear==p) {
			Q.rear=Q.front;//ʹ��βָ��ص���ʼλ��
		}
		delete(p);//�ͷ�p��ָ���ռ�
	}
	cout<<"�����������գ�\n";
}

//8.ֹͣͣ��������
void CloseOpen(Stack &S,Queue &Q) {
	CleanParkCar(S);//���ͣ����
	CleanDetourLane(Q);//��ձ��
	cout<<"ͣ�����������κγ���......\nͣ�����ѹرգ�\n";
}

//9.�˵�˵�� 
void FunctionMenu(){
	cout<<"\n\n"        
	<<"                                *** ��ӭ������ ����ͣ��������ϵͳ���ܽ��� ***\n"              
	<<"                                          @@�������ѡ������Ҫ�Ĺ��ܣ�\n"
	<<"                                0.���� ͣ������λ�����շ���� + ����շ����\n"                 
	<<"                                1.���� ��/�� ͣ����              2.��ʾͣ����������Ϣ\n" 
	<<"                                3.��ѯ��ǰͣ�����ڳ�����Ϣ       4.�鿴���������Ϣ\n" 
	<<"                                5.���ͣ��������                 6.�鿴ͣ������λ����\n" 
	<<"                                7.��ձ������                   8.�����������˵�˵��\n"  
	<<"                                9��ֹͣͣ��������"; 

} 

//10.������Աѡ������ú�������
void InstallMessage(int &parkingSize,int &parkingFee,int &pavementFee){
	char ch;
	cout<<"������ͣ�������������";
	cin>>parkingSize;
	cout<<"�����복��'ͣ����'ÿСʱ�ķ��ã�Ԫ/ʱ����";
	cin>>parkingFee;
	//�Ƿ��漰����շ����� 
	cout<<"\n��ѡ����'���'��ͣ���Ƿ��շ�[ Y �� N ]��";
	cin>>ch;
	if(ch=='y'|| ch=='Y'){
		cout<<"�����복��'���'ÿСʱ�ķ��ã�Ԫ/ʱ����";
		cin>>pavementFee;
	}else{
		pavementFee=0;//���ﲻ�漰�շ���Ȼ�� 0��Ϊ�˺�����ʾ"��Ϣ"��ȫ�� 
	}

}

//11.���ܽ���
void OpenfunctionWindow() { 
	//����������Ϣ 
	int pavementFee=0;//������� ��Ĭ��Ϊ 0 
	int parkingSize=0;//ͣ�������� ��Ĭ��Ϊ 0 
	int parkingFee=0;//ͣ�������� ��Ĭ��Ϊ 0
	int time[parkingSize];
	
	//����ͣ��������� 
	Stack S,LS;//Sָͣ������LSָ��ʱջ
	Queue Q;//���У������
	
	//��ʼ�����Ĭ��һ�о�Ϊ 0 (���ʱ�����Ա��û�г�ʼ���� 
	initStack(S,parkingSize);
	initStack(LS,parkingSize);
	
	//��ʼ�����-------------> ��Ϊ ��� ʹ���������Զ�����ռ䣬���漰��С�����Բ�Ӱ�� ��� ��ʼ�� 
	initQueue(Q);
	
	bool open=true;//���� 
	int SELECT;//ѡ�� 
	
	FunctionMenu();//����"���ܲ˵�"����
	
	while(true) {
		cout<<"\n��ѡ�����蹦�ܣ�\n";
		cin>>SELECT;
		switch(SELECT) {
			case 0:{
				InstallMessage(parkingSize,parkingFee,pavementFee);//���ú���
				//��ʼ��ͣ����
				//���ʱ�� ����Ա �����˳�ʼ�����������¶�ͣ���� ��ֵ 
				initStack(S,parkingSize);
				initStack(LS,parkingSize);
				break;
			}
			case 1: {
				
				CarAccess(S,LS,Q,parkingFee,pavementFee,time);
				
				break;
			}
			case 2: {
				ShowParkMessage(parkingSize,parkingFee);
				break;
			}
			case 3: {
				SelectCarMessage(S);
				break;
			}
			case 4: {
				DetourLaneCarMessage(Q);
				break;
			}
			case 5: {
				CleanParkCar(S);
				break;
			}
			case 6:{
				ViewMargin(S,parkingSize);
				break;
			}
			case 7:{
				CleanDetourLane(Q);
				break;
			}
			case 8:{
				system("cls");//����
				FunctionMenu();//����"���ܲ˵�"����
				break;
			}
		}
		if(SELECT==9) {
			CloseOpen(S,Q);
			break;
		}
	}
}


//12.����Ա��½ϵͳ 
void AdministratorLogin(){
	int managerName=1;//�˺� 
	int password=1;//����
	bool BUTTON=false;//ѡ�񿪹� 
	int TempVar1,TempVar2;//������ʱ�洢��Ϣ�ı��������ڴ洢�˺����벢���бȽ� 
	cout<<"           <--------------------------------------------------------------------------------------->\n"; 
	cout<<"                            ��Welcome to the parking lot management system�� \n"                         ;
	cout<<"                           <@Please follow the instructions to log in and use>\n"                         ;
	cout<<"                                            �������û��˺ţ�"; 
	//�ж��˺��Ƿ���ȷ 
	while(true){
		cin>>TempVar1;//�洢����ʱ����1��
		if(TempVar1==managerName){
			break;
		}else{
			cout<<"                                        ���˺Ŵ������������룺";
		}
	}
	cout<<"                                            �������û����룺"; 
	//�ж������Ƿ���ȷ 
	while(true){
		cin>>TempVar2;//�洢����ʱ����2��
		if(TempVar2==password){
			BUTTON=true;//��֤ͨ������½��������� 
			break;
		}else{
			cout<<"                                         !����������������룺";
		}
	}
	if(BUTTON){
		OpenfunctionWindow();//ͨ���������ù��ܴ���
	}
} 



//������ 
int main() {
	AdministratorLogin();//���ù���Ա��½�Ӷ���ϵͳ 
}


