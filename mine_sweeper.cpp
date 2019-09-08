#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int click_result; //点击后游戏是否继续
int arr[11][11][3];//0是否被揭示(0否，1是)， 1摆放情况（0没雷，9雷）
int extend[11][11];//延伸时为了不走回头路标记是否已经走过（0没走过，1走过）
int orientation[9][4] = {{0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1},
						{1, -1}, {1, 0}, {1, 1}}; //[1-8]八个方向[0-1]x偏移,y偏移 

void init_reveal();
void set_mines();
void print_put_situation();
void print_put_situation_test();
void set_number();
void print_player_view();
int click();
void extend_click(int, int, bool);
void refresh_extend();
void init();
bool check_is_win();
void print_answer();
void welcome();
void game();   

int main(){
	game();
	return 0;
}

//游戏主体
void game(){
	char again = 'y';
	welcome();
	
	while(again == 'y' || again == 'Y'){
		init();
		do{
			//print_put_situation_test();
			print_player_view();
			cout<<endl;
			refresh_extend();
			click_result = click();
			if(check_is_win()){
				break; 
			} 
		}while(click_result != 1);
		
		if (click_result == 1){
			cout<<"---------------Game Over---------------!"<<endl;
			cout<<endl;
			print_answer();
		}else {
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"--Congratulations!You win!:)--"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<"------------------------------"<<endl;
			cout<<endl;
		}
		cout<<"Wanna play pationate game again?(y/n)"<<endl;
		cin>>again;
		cout<<endl;
	}
	
	cout<<"---------------BYE---------------"<<endl; 
}

//欢迎词
void welcome(){
	cout<<"------------------------------"<<endl;
	cout<<"---Welcome to mine sweeper!---"<<endl;
	cout<<"------------------------------"<<endl;
	cout<<"------Made By Shane Yee-------"<<endl;
	cout<<"------------------------------"<<endl;
	cout<<endl;
}

//检查是否胜利，条件是要么揭示，要么是雷
bool check_is_win(){
	bool temp = true;
	for (int i = 1; i <= 9; i++){
		for (int j = 1; j <= 9; j++){
			if (!(arr[i][j][1] == 9 || arr[i][j][0] == 1)){
				temp = false;
				break; 	
			}
		}
	}
	
	return temp;
}

void init(){
	init_reveal();
	set_mines(); 
	set_number();
	//print_put_situation();
	click_result = 0;
}

//从点击处向外延伸查看能否一起被揭示 
//bool参数是判断是否没雷没数字
//因为没雷没数字的地方要向外延伸至有数字处
void extend_click(int x, int y, bool is_blank){
	extend[x][y] = 1;
	for (int i = 1; i <= 8; i++){
		int offset_x = x;
		int offset_y = y;
		offset_x += orientation[i][0];
		offset_y += orientation[i][1];
		
		//（测试）延伸到了哪里
		//cout<<offset_x<<" "<<offset_y<<endl;
		if (offset_x < 1 || offset_x > 9 || offset_y < 1 || offset_y > 9
			|| extend[offset_x][offset_y] == 1){
			continue;
		}else {
			//数字延展数字或者延展到地雷是不允许的
			if (arr[offset_x][offset_y][1] == 9 ||arr[offset_x][offset_y][1]
			>= 1 && arr[offset_x][offset_y][1] <= 8 && is_blank == false){
				return;
			}else if(arr[offset_x][offset_y][1] == 0){	//数字延展空白
				arr[offset_x][offset_y][0] = 1;
				extend_click(offset_x, offset_y, true);
			}else {	//空白延展数字
				arr[offset_x][offset_y][0] = 1;
				extend_click(offset_x, offset_y, false);
			}
		}
	}
	
	return;
}

//点击处理 返回1表示踩到雷 返回0表示继续游戏
int click(){
	int x, y;
	
	while(true){
		cout<<"Please input where to click.(Format:x y)"<<endl;
		cin>>x>>y;
		cout<<endl;
		if (arr[x][y][0] == 1){
			cout<<"This grid has been clicked, please choose another one."<<endl;
			cout<<endl;
		}else {
			break;
		}
	}
	
	if (arr[x][y][1] == 9){
		return 1;
	}
	arr[x][y][0] = 1;
	
	if (arr[x][y][1] == 0){
		extend_click(x, y, true);
	}else {
		extend_click(x, y, false);
	}
	
	return 0;
}

//打印玩家视角
void print_player_view(){
	cout<<"--1-2-3-4-5-6-7-8-9"<<endl;
	for (int i = 1; i <= 9; i++){
		cout<<i<<"|";
		
		for (int j = 1; j <= 9; j++){
			if (arr[i][j][0] == 0){
				cout<<"* "; 		
			}else {
				if (arr[i][j][1] == 0){
					cout<<"- ";
				}else {
					cout<<arr[i][j][1]<<" ";
				}
			}
			
			if (j == 9){
				cout<<endl;
			}
		}
		
	}
}

//设置数字
void set_number(){
	for (int i = 1; i <= 9; i++){
		for (int j = 1; j <= 9; j++){
			int mines_number = 0;
			//是雷不用试探
			if (arr[i][j][1] == 9){
				continue; 
			}
			//向四周试探是否有雷
			for (int k = 1; k <= 8; k++){
				int x = i;
				int y = j;
				x += orientation[k][0];
				y += orientation[k][1];
				if (x < 1 || x > 9 || y < 1 || y > 9){
					continue;
				} else{
					if (arr[x][y][1] == 9){
						mines_number++;
					}
				}
			}
			
			if (mines_number == 0){
				continue;
			}else {
				arr[i][j][1] = mines_number;
			}
		}
	}
}

//失败后把答案输出
void print_answer(){
	cout<<"-------ANSWER------"<<endl;
	print_put_situation();
	cout<<"-------ANSWER------"<<endl;
	cout<<endl;
}

//打印摆放情况 (测试用)
void print_put_situation_test(){
	cout<<"--------TEST-------"<<endl;
	print_put_situation();
	cout<<"--------TEST-------"<<endl;
	cout<<endl;
}

//打印摆放情况
void print_put_situation(){
	cout<<"--1-2-3-4-5-6-7-8-9"<<endl;
	for (int i = 1; i <= 9; i++){
		cout<<i<<"|";
		
		for (int j = 1; j <= 9; j++){
			if (arr[i][j][1] == 9){
				cout<<"* ";
			}else if(arr[i][j][1] == 0){
				cout<<"- ";
			}else {
				cout<<arr[i][j][1]<<" ";
			}
			
			if (j == 9){
				cout<<endl;
			}
		}
	}
}

//随机投放地雷
void set_mines(){
	srand((unsigned)time(NULL));
	int mines_number = 0; 
	
	while(mines_number < 10){
		int rand_number_x = rand() % 9 + 1;
		int rand_number_y = rand() % 9 + 1;
		
		if (arr[rand_number_x][rand_number_y][1] != 9){
			arr[rand_number_x][rand_number_y][1] = 9;
			mines_number++;
		}
	}
}

//设置全部未揭示
void init_reveal(){
	for (int i = 1; i <= 9; i++){
		for (int j = 1; j <= 9; j++){
			arr[i][j][0] = 0;
		}
	}
}

//重置extend数组
void refresh_extend(){
	for (int i = 1; i <= 9; i++){
		for (int j = 1; j <= 9; j++){
			extend[i][j] = 0;
		}
	}
}
