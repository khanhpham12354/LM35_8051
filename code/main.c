#include <REGX52.H>
// khai bao chan ket noi led 7 doan
#define LED7SEG 		  P0
#define L7S1 			  P2_4
#define L7S2			  P2_5
#define L7S3			  P2_6
#define L7S4			  P2_7
//Khai bao chan giao tiep ADC0808
#define ADC0808_kq        P3
#define ADC0808_A0        P1_0
#define ADC0808_A1        P1_1
#define ADC0808_A2        P1_2
#define ADC0808_ALE       P1_3
#define ADC0808_START     P2_3
#define ADC0808_EOC       P2_2
#define ADC0808_OE        P2_1
#define ADC0808_CLK       P2_0
// khai bao ma led 7 doan
unsigned char So[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; // mã led 7 doan
unsigned char tach[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
void  Fn_Delay (unsigned int _vrui_Time){
         while(_vrui_Time --);
}
bit x = 0;
/*Read kq from ADC0808*/
unsigned char ADC0808_Read(unsigned char channel){
	unsigned char kq;
	unsigned char t1,t2,t3,t4,t5,t6,t7,t8,dat;
	ADC0808_A0 = channel & 0x01;
	ADC0808_A1 = channel & 0x02;
	ADC0808_A2 = channel & 0x04;
	ADC0808_ALE = 1;
	ADC0808_START = 1;
	ADC0808_ALE = 0;
	ADC0808_START = 0;
	while(ADC0808_EOC);
	while(!ADC0808_EOC);
	ADC0808_OE = 1;
	kq = ADC0808_kq;

	t1 = kq & tach[0];
	t1 = t1<<7;

	t2 = kq & tach[1];
	t2 = t2<<5;

	t3 = kq & tach[2];
	t3 = t3<<3;

	t4 = kq & tach[3];
	t4 = t4<<1;

	t5 = kq & tach[4];
	t5 = t5>>1;

	t6 = kq & tach[5];
	t6 = t6>>3;

	t7 = kq & tach[6];
	t7 = t7>>5;

	t8 = kq & tach[7];
	t8 = t8>>7;
	dat = t1|t2|t3|t4|t5|t6|t7|t8;
	ADC0808_OE = 0;
	return dat;
}
//ctr phuc vu ngat tao xung clock cho ADC0808
void INT_Timer0()interrupt 1{
     ADC0808_CLK=~ADC0808_CLK; //Dao bit
}
int main(){
	unsigned char Data,j;
	TMOD=0x02; //Timer0 8bit tu nap lai
	TH0=TL0=236;//Tao ngat 20us
	TR0=1;//Khoi dong timer0
    ET0=1;//Ngat timer0
    EA=1;//Cho phep ngat cuc bo
	L7S1 = 1;	 // tat tat ca cac led
	L7S2 = 1;
	L7S3 = 1;
	L7S4 = 1;
    while(1){		
		Data = ADC0808_Read(0);// doc gia tri nhiet do

		for (j = 0; j < 200; j++){
			P0 = So[(Data*2)/10];
			L7S1 = 0; Fn_Delay(100); L7S1 = 1;
			P0 = So[(Data*2)%10];
			L7S2 = 0; Fn_Delay(100); L7S2 = 1;
			P0 = 0x9C;
			L7S3 = 0; Fn_Delay(100); L7S3 = 1;
			P0 = 0xC6;
			L7S4 = 0; Fn_Delay(100); L7S4 = 1;
		}// hien thi gia tri len led 7 doan 4 so

		Fn_Delay(500);
	}
    return 0;
}
