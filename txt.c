#include <stdio.h>
#include<stdlib.h>
#include<math.h>

#define NUM 20

int main(void)
{
	FILE* file = fopen("4.571m8h_20171011_gps.txt", "r");
	char line[256];   //save a line
	char str[20];    //save the second list
	int i = 0, j;
	//define two parameters in kalman filter
	float x_last;
	float p_last;
	//double y;   //the value of the second list(changed to double first)
	//double mean,std,sum = 0,sum_square = 0;

	if (file == NULL)
	{
		printf("There is no file.");
		getchar();
		return 0;
	}
	
	while (fgets(line, sizeof(line), file)) { //fgets read file by lines, ended by while circle at the end of the file
		sscanf(line, "%*s %s", str);//sscanf analysis the data by lines, pass the first line(see the usage of sscanf)
		float y;
		y = atof(str);     //change to float
		float a[NUM];
		if (i < NUM) {   //加上粗大误差处理函数（条件改为i<+100）
			a[i] = y;
			printf("a[%d] = %f\n",i,a[i]);
			FILE *fpWrite = fopen("data.txt", "a+");
				fprintf(fpWrite, "%f\n", a[i]);
			fclose(fpWrite);
		//initializing two parameters in kalman filter
		x_last = a[NUM - 1];
		p_last = 1.0;
		}
		else {
			for (j=0;j<NUM-1;j++)   //加上
				a[j] = a[j+1];    //add a number in the end and wipe the first number
			a[NUM-1] = y;
			
			//kalman filter
			
			float Q = 1e-4;   //process variance
			float R = 0.5;    //estimate of measurement variance, change to see effect
			float kg;
			float x_mid;
			float x_now;
			float p_mid;
			float p_now;
			//time update 
			x_mid = x_last;      //x_mid=x(k|k-1), x_last=x(k-1|k-1), X(k|k-1) = AX(k-1|k-1) + BU(k) + W(k),A=1,BU(k) = 0
			p_mid = p_last + Q;  //p_mid=p(k|k-1), p_last=p(k-1|k-1), P(k|k-1) = AP(k-1|k-1)A + Q(k) ,A=1
			//measurement update
			kg = p_mid / (p_mid + R); //Kg(k)=P(k|k-1)H / [HP(k|k-1)H + R],H=1
			x_now = x_mid + kg*(a[NUM-1] - x_mid); //X(k|k) = X(k|k-1) + Kg(k)[Z(k) - HX(k|k-1)], H=1,Z(k)=a[NUM-1]
			p_now = (1 - kg)*p_mid;   //P(k|k) = (1 - Kg(k)H)P(k|k-1), H=1
			//next round parameter update
			x_last = x_now;
			p_last = p_now;

			printf("a'[%d] = %f\n",i,x_now);
			FILE *fpWrite = fopen("data.txt", "a+");
			fprintf(fpWrite, "%f\n", x_now);
			fclose(fpWrite);
		}
		i = i + 1;
		}

	fclose(file);
	getchar();     //keep waiting
	return 0;
}