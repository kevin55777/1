#include<iostream>
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define MAX 100
#define PAVEMENT_TIME 0
using namespace std;
int Position=0;//车在便道上的位置 
int count=0;//统计便道上的车辆数做数组下标
int waitTime=0;//记录车子在便道上停留的时间
int EnterTime=0;//记录车子刚进入便道的时间



//栈
typedef struct {
	int LicensePlate[100]; //车牌号，最多100个 
	int arriveTime[100];//到达时间，对应各个车 
	int Place;//停车场车位位置 
	int Capacity;//容量
} Stack;

//队列
typedef struct 	QueneNode {
	int LicensePlate;//车牌号 
	int arriveTime;//到达时间 
	//为了更方便，position放在这里 
	int position;//车在便道上的位置 
	struct QueneNode *next;
} QueneNode,*queue;
typedef struct {
	queue front;
	queue rear;
} Queue;

//初始化栈
void initStack(Stack &S,int Capacity) {
	S.Place=-1;
	S.Capacity=Capacity;
}

//检测栈是否能被清空
bool emptyStack(Stack &S) {
	if(S.Place==-1) {
	return false;
	} else {
		return true;
	}
}
//检测栈是否能被填满
bool fullStack(Stack &S) {
	if(S.Place==S.Capacity-1) {
		return false;
	} else {
		return true;
	}
}

//入栈
void push(Stack &S,int LicensePlate,int arriveTime) {
	S.Place++;
	S.LicensePlate[S.Place]=LicensePlate;
	S.arriveTime[S.Place]=arriveTime;
}
//出栈
void pop(Stack &S,int &LicensePlate,int &arriveTime) {
	LicensePlate=S.LicensePlate[S.Place];
	arriveTime=S.arriveTime[S.Place];
	S.Place--;
}

//初始化队列
void initQueue(Queue &Q) {
	Q.front=Q.rear=(queue)malloc(sizeof(QueneNode));//队头指针、队尾指针指向同一空结点
	if(!Q.front) {
		exit(-1);//内存分配失败，退出
	}
	Q.front->next=NULL;
}

//检测队列是否能被清空
bool emptyQueue(Queue &Q) {
	if(Q.front==Q.rear) {
		return false;
	} else {
		return true;
	}
}

//入队列
void inQueue(Queue &Q,int LicensePlate,int arriveTime) {
	queue p;
	p=(queue)malloc(sizeof(QueneNode));//分配一个结点空间
	p->LicensePlate=LicensePlate;
	p->arriveTime=arriveTime;
	p->next=NULL;
	Q.rear->next=p;//队尾指针指向p
	Q.rear=p;//队尾指针后移一位
	Position++;
}
//出队列
void outQueue(Queue &Q,int &LicensePlate,int &arriveTime) {
	queue p;
	p=(queue)malloc(sizeof(QueneNode));//分配一个结点空间
	p=Q.front->next;//指定一个新的结点指向队头指针指向的结点
	
	LicensePlate=p->LicensePlate;//返回结点数据值
	arriveTime=p->arriveTime;
	Q.front->next=p->next;//将p指向的下一数据给队头指针，令下一数据前移到队头
	if(Q.rear==p) {
		Q.rear=Q.front;//使队尾指针回到初始位置
	}
	free(p);//释放p所指结点空间
	Position--;//车在便道的位置 向前移动 
}

//0.0判断停车场内是否已有车辆，避免重复输入相同车牌号的车 
bool judgeParking(Stack S,char state,int LicensePlate){
	int i=S.Place;
	if(state!='A'){
		return true;//只用判断 A(输入) 情况 
	}
	while(i!=-1) {
		if(LicensePlate==S.LicensePlate[i]){
			cout<<"停车场里已经有该车，请重新进入输入界面！\n";
			return false;
		}
		i--;//逐个遍历
	}
	return true;
}

//0.1判断便道上是否已有车辆，避免重复输入相同车牌号的车 
bool judgePavement(Queue Q,char state,int LicensePlate){
	if(state!='A'){
		return true;//只用判断 A(输入) 情况 
	}
	queue p=Q.front->next;//指定一个新的结点指向队头指针指向的结点
	while(p!=NULL) {
		if(LicensePlate==p->LicensePlate){
			cout<<"便道上已经有该车，请重新进入输入界面！\n";
			return false;
		}
		p=p->next;//逐个遍历
	}
	return true;
}

//1.车辆 进/出 停车场
void CarAccess(Stack &S,Stack &LS,Queue &Q,int parkingFee,int pavementFee,int time[]) {
	
	int LicensePlate,arriveTime;	
	char state;//状态描述
	
	cout<<"请输入车的状态（A进站/D出站），车牌号(整数) 和 时间整数（进站时间/出站时间）：\n输入 E 0 0 结束输入\n\n";
	cin>>state>>LicensePlate>>arriveTime;
	while((judgeParking(S,state,LicensePlate)) && (judgePavement(Q,state,LicensePlate))&&state!='E') {
			int leaveTime=arriveTime;
			if(state=='A') {
				//停车场是否为空
				if(fullStack(S)) {
					push(S,LicensePlate,arriveTime);//车进入停车场
					cout<<"车牌号是 "<<LicensePlate<<" 的车停在停车场的位置是 "<<S.Place+1<<endl;
				} else {
				
					//记录车子 刚进入便道 的时间
					EnterTime=arriveTime;
					//记录便道车牌号
					time[count]=LicensePlate; 
					count++;
					//便道不计费，因此初始默认数值为 常数 0
					inQueue(Q,LicensePlate,arriveTime);//停车场非空，停在便道上
					cout<<"车牌号是 "<<LicensePlate<<" 的车在便道的位置是 "<<Position<<endl;
				}
			} else if(state=='D') {
				//查询该车
				bool yes=false;
				bool is=false;
				//无该车
				for(int i=0; i<=S.Place; i++) {
					if(S.LicensePlate[i]==LicensePlate) {
						is=true;
						break;
					}
				}
	
				if(!is) {
					cout<<"停车场无车牌号为 "<<LicensePlate<<" 的车！"<<endl;
				}
	
				for(int j=S.Place; j>=0; j--) {
					if(!is) {
						break;//停车场内无车，跳出循环
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
					//找到了该车
					pop(S,LicensePlate,arriveTime);//该车离开停车场
					
					for(int i=0;i<=count;i++){
						
						if(LicensePlate==time[i]){
							//这里约定是 0 时进入的 
							cout<<"车牌号是 "<<LicensePlate<<" 的车离开便道，停留时间是："<<arriveTime-EnterTime<<
						    "小时，共花费了 "<<(pavementFee*(arriveTime-EnterTime))<<" 元。"<<endl;
						    
							cout<<"车牌号是 "<<LicensePlate<<" 的车离开停车场，停留时间是："<<(leaveTime-arriveTime)<<
						    "小时，共花费了 "<<parkingFee*(leaveTime-arriveTime)<<" 元。"<<endl;
						    
						    to=false;
						    break;
						}
					}
					if(to){
						cout<<"车牌号是 "<<LicensePlate<<" 的车未在便道停留，消费0元\n"; 
		
						cout<<"车牌号是 "<<LicensePlate<<" 的车离开停车场，停留时间是："<<(leaveTime-arriveTime)<<
					    "小时，共花费了 "<<parkingFee*(leaveTime-arriveTime)<<" 元。"<<endl;  
					}
					
					

					//将临时栈里的车放进停车场
					while(emptyStack(LS)) {
						pop(LS,LicensePlate,arriveTime);
						push(S,LicensePlate,arriveTime);
					}
					//如果便道上有车，开进停车场
					if(emptyQueue(Q)) {
						outQueue(Q,LicensePlate,arriveTime);
						push(S,LicensePlate,leaveTime);
					}
				}
			}
			cin>>state>>LicensePlate>>arriveTime;
	}
}

//2.显示停车场基本信息
void ShowParkMessage(int size,int fee) {
	cout<<"\n 名称：临时停车场"
	    <<"\n 车位数："<<size
	    <<"\n 收费情况："<<fee<<"元/时"<<endl;
}

//3.查询当前停车场内车辆信息
void SelectCarMessage(Stack S) {
	int i=S.Place;
	bool have=true;
	
	if(i==-1){
		have=false;
		cout<<"\n停车场内无车辆存在！\n";
	}
	
	cout<<"---------------------------------------------------------------------\n"
		<<"  状态(A/D)    |    车牌号     |     所在车位     |     进入时间     \n";
		
	while((i!=-1) && have) {
		cout<<"      A        |      "<<S.LicensePlate[i]<<"        |         "<<(i+1)<<"        |         "<<S.arriveTime[i]<<endl;
		i--;//逐个遍历
	}
	
	cout<<"---------------------------------------------------------------------\n\n";
}

//4.查看便道车辆信息
void DetourLaneCarMessage(Queue Q) {
	queue p=Q.front->next;//指定一个新的结点指向队头指针指向的结点
	bool have=true;
	int pos=1;
	
	if(p==NULL){
		have=false;
		cout<<"\n便道内无车辆存在！\n";
	}
	
	cout<<"-----------------------------------------------------\n"
		<<"    车牌号     |      便道位置    |      进入时间    \n";
		
	while((p!=NULL) && have) {
		cout<<"      "<<p->LicensePlate<<"        |         "<<pos++<<"        |         "<<p->arriveTime<<endl;
		p=p->next;//后移遍历
	}
	
	cout<<"-----------------------------------------------------\n\n";
}

//5.清空停车场车辆
void CleanParkCar(Stack &S) {
	while(emptyStack(S)) {
		S.Place--;
	}
	cout<<"停车场车辆已清空！\n";
}

//6.查看停车场车位余量
void ViewMargin(Stack S,int size) {
	if(size==0){
		//若管理员没有设置容量  或  剩余量已经为 0，则输出0 
		cout<<"停车场车位剩余 0 个\n\n";
	}else{
		//不为 0 时计算
		//size-S.top-1 ---> -1 是因为 S.top从0开始计数
		cout<<"停车场车位剩余 "<<(size-S.Place-1)<<" 个\n\n"; 
	} 
}

//7.清空便道车辆
void CleanDetourLane(Queue &Q) {
	while(emptyQueue(Q)) {
		queue p;
		p=(queue)malloc(sizeof(QueneNode));//分配一个结点空间
		p=Q.front->next;//指定一个新的结点指向队头指针指向的结点
		Q.front->next=p->next;//将p指向的下一数据给队头指针，令下一数据前移到队头
		if(Q.rear==p) {
			Q.rear=Q.front;//使队尾指针回到初始位置
		}
		delete(p);//释放p所指结点空间
	}
	cout<<"便道车辆已清空！\n";
}

//8.停止停车场开放
void CloseOpen(Stack &S,Queue &Q) {
	CleanParkCar(S);//清空停车场
	CleanDetourLane(Q);//清空便道
	cout<<"停车场内已无任何车辆......\n停车场已关闭！\n";
}

//9.菜单说明 
void FunctionMenu(){
	cout<<"\n\n"        
	<<"                                *** 欢迎您进入 简洁版停车场管理系统功能界面 ***\n"              
	<<"                                          @@输入序号选择您需要的功能：\n"
	<<"                                0.设置 停车场车位数、收费情况 + 便道收费情况\n"                 
	<<"                                1.车辆 进/出 停车场              2.显示停车场基本信息\n" 
	<<"                                3.查询当前停车场内车辆信息       4.查看便道车辆信息\n" 
	<<"                                5.清空停车场车辆                 6.查看停车场车位余量\n" 
	<<"                                7.清空便道车辆                   8.清屏，保留菜单说明\n"  
	<<"                                9、停止停车场开放"; 

} 

//10.若管理员选择，则调用函数即可
void InstallMessage(int &parkingSize,int &parkingFee,int &pavementFee){
	char ch;
	cout<<"请输入停车场最大容量：";
	cin>>parkingSize;
	cout<<"请输入车在'停车场'每小时的费用（元/时）：";
	cin>>parkingFee;
	//是否涉及便道收费问题 
	cout<<"\n请选择车在'便道'上停留是否收费[ Y 或 N ]：";
	cin>>ch;
	if(ch=='y'|| ch=='Y'){
		cout<<"请输入车在'便道'每小时的费用（元/时）：";
		cin>>pavementFee;
	}else{
		pavementFee=0;//这里不涉及收费仍然给 0，为了后面显示"信息"都全面 
	}

}

//11.功能界面
void OpenfunctionWindow() { 
	//定义数据信息 
	int pavementFee=0;//便道费用 ，默认为 0 
	int parkingSize=0;//停车场容量 ，默认为 0 
	int parkingFee=0;//停车场费用 ，默认为 0
	int time[parkingSize];
	
	//定义停车场、便道 
	Stack S,LS;//S指停车场，LS指临时栈
	Queue Q;//队列（便道）
	
	//初始情况下默认一切均为 0 (这个时候管理员还没有初始化） 
	initStack(S,parkingSize);
	initStack(LS,parkingSize);
	
	//初始化便道-------------> 因为 便道 使用链队列自动分配空间，不涉及大小，所以不影响 便道 初始化 
	initQueue(Q);
	
	bool open=true;//开关 
	int SELECT;//选择 
	
	FunctionMenu();//调用"功能菜单"函数
	
	while(true) {
		cout<<"\n请选择所需功能：\n";
		cin>>SELECT;
		switch(SELECT) {
			case 0:{
				InstallMessage(parkingSize,parkingFee,pavementFee);//调用函数
				//初始化停车场
				//这个时候 管理员 进行了初始化，所以重新对停车场 赋值 
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
				system("cls");//清屏
				FunctionMenu();//调用"功能菜单"函数
				break;
			}
		}
		if(SELECT==9) {
			CloseOpen(S,Q);
			break;
		}
	}
}


//12.管理员登陆系统 
void AdministratorLogin(){
	int managerName=1;//账号 
	int password=1;//密码
	bool BUTTON=false;//选择开关 
	int TempVar1,TempVar2;//两个临时存储信息的变量，用于存储账号密码并进行比较 
	cout<<"           <--------------------------------------------------------------------------------------->\n"; 
	cout<<"                            ‘Welcome to the parking lot management system’ \n"                         ;
	cout<<"                           <@Please follow the instructions to log in and use>\n"                         ;
	cout<<"                                            请输入用户账号："; 
	//判断账号是否正确 
	while(true){
		cin>>TempVar1;//存储到临时变量1中
		if(TempVar1==managerName){
			break;
		}else{
			cout<<"                                        ！账号错误，请重新输入：";
		}
	}
	cout<<"                                            请输入用户密码："; 
	//判断密码是否正确 
	while(true){
		cin>>TempVar2;//存储到临时变量2中
		if(TempVar2==password){
			BUTTON=true;//验证通过：登陆到管理界面 
			break;
		}else{
			cout<<"                                         !密码错误，请重新输入：";
		}
	}
	if(BUTTON){
		OpenfunctionWindow();//通过函数调用功能窗口
	}
} 



//主函数 
int main() {
	AdministratorLogin();//调用管理员登陆从而打开系统 
}


