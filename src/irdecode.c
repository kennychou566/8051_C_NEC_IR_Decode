/*紅外線接收程式,市面上的遙控器大部份都是NEC製的，按遙控器
在七段顯示器顯示相應的位址碼-指令碼（以16進製顯示）*/

//#include <reg51.h>
//#include <intrins.h>
#include <8052.h>
#include <lint.h>
#include <STC89xx.h>
SBIT(hwx,_P3,3);		//定義紅外接收腳

unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x40};
			//七段顯示器數字 0-9 a-f - 表
unsigned char l_tmpdate[8]={0,0,0x10,0,0};//顯示器資料

unsigned char l_all[66];		//定義66位數組變量來存儲接收的時間參數
void delay(void);//延時子函數
void display(unsigned char *lp,unsigned char lc);//數字的顯示函數；lp為指向數組的位址，lc為顯示的個數

void main(void)     //主函數
{
	EA=1;			//首先開啟總中斷
	EX1=1;  		//開啟外部中斷 1
	IT1=1;        // 設置成 下緣觸發模式
	while(1){		//循環顯示，接收都由中斷處理
		display(l_tmpdate,5);		
	}		

}
void display(unsigned char *lp,unsigned char lc)//顯示
{
	unsigned char i;		//定義變量
	P2=0;					//端2為輸出
	P1=P1&0xF8;				//將P1的前3位輸出0，對應138輸入腳，全0為第一位七段顯示器
	for(i=0;i<lc;i++){		//循環顯示
	P2=table[lp[i]];		//查表法得到要顯示數字的數碼段
	delay();				//延時
	P2=0;					//清除為0，準備顯示下位
	if(i==7)				//檢測顯示完8位否，完成直接退出，不讓P1再加1，否則進位影響到第四位資料
		break;	
	P1++;					//下一位七段顯示器
	}
}
void delay(void)								//空5個指令
{
	unsigned char i=13;
	while(i)
		i--;
}
void ir_decode(void) __interrupt(2)   //外部中斷 1 ，INT1（P3^3）連接紅外線接收IC資料腳
{
	unsigned char i,j,tmp;
	EX1=0;
//	hwx=1;
	j=33;			//傳送一組數包括引導碼1位，位址碼8位加反碼8位，指令碼8位加反碼8位，總共33位
	i=0;			//從第一維數組開始
	tmp=100;		//加八延時，
	while(tmp){
		tmp--;
	}
	if(hwx){		//然後再檢測紅線接收腳是有資料收，有繼續，沒有則退出
		EX1=1;
		return;
	}	
	while(j){		//循環接收33位資料，為何我們用到66位數組，我們可以不接收高電位時間常數，只接低電位常數就
				//可以判斷1或0了，在這裡我們都接收，為串調試軟體接收計算波形
				//還有一點要知道，接收波形是反向，在沒有接收時端為高電位
		tmp=0;
		l_all[i]=1;		//時間量從1開始
		while(!hwx){	//檢測高低電位的變化
			l_all[i]++;		//沒變繼續加1
			delay();		//加入延時，是因為我們採用8位二進來存儲，如果不加延時，時間量將計滿
			tmp++;			//此變量為防止干擾帶來的死循環
			if(tmp==250)
				break;
		}
		i++;
		tmp=0;
		l_all[i]=1;		//時間量從1開始
		while(hwx){		//檢測高低電位的變化
			l_all[i]++;	//沒變繼續加1	
			delay();		//同上
			tmp++;
			if(tmp==250)
				break;
		}	
		i++;		
		j--;
	}
	i=255;				//加入循環延時，抗干擾
	while(i){
	tmp=255;
	while(tmp){
		P2=0XF0;
		tmp--;
	}
	i--;			
	}
	tmp=0;
	for(i=3;i<19;i++,i++){		//處理位址位，對低電位時間資料的數理，這裡我們只處理位址碼和指令碼，引導碼和反碼我們就忽略
		tmp>>=1;				//右移一位，接收低電位在前
		if(l_all[i]>30)			//檢測低電位時間超過30就確認為1
			tmp+=0x80;
	}	
	l_tmpdate[0]=tmp/16;		//分開2位以16進制顯示
	l_tmpdate[1]=tmp%16;
	tmp=0;
	for(i=35;i<51;i++,i++){		//同上，這裡處理指令位
		tmp>>=1;
		if(l_all[i]>30)
			tmp+=0x80;
	}
	l_tmpdate[3]=tmp/16;
	l_tmpdate[4]=tmp%16;	
	EX1=1;	
}

