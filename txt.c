#include <stdio.h>
#include<stdlib.h>
#include<math.h>

#define NUM 10      //change to 100 when used

int main(void)
{
	FILE* file = fopen("10.691m8h_20171127_gps.txt", "r");
	char line[256];   //save a line
	char str[20];    //save the second list
	int i = 0, j, k;
	//define two parameters in the kalman filter
	float x_last;
	float p_last;
	//used for calculate mean & std
	double mean,std,sum = 0,sum_square = 0;

	if (file == NULL)
	{
		printf("There is no file.");
		getchar();
		return 0;
	}
	
	while (fgets(line, sizeof(line), file)) { //fgets read file by lines, ended by while circle at the end of the file
		sscanf(line, "%*s %s", str);//sscanf analysis the data by lines, pass the first line(see the usage of sscanf)
		float y;           //get and save the distance value
		y = atof(str);     //change to float
		float a[NUM];
		if (i < NUM) {
			a[i] = y;
			sum = sum + a[i];
			sum_square = sum_square + y*y;   //the sum of the square
			mean = sum / NUM;       //the last one is needed
			std = sqrt((sum_square - 2 * sum*mean + NUM*mean*mean) / (NUM - 1));
			//printf("%f  %f\n", mean, std);

			printf("a[%d] = %f\n",i,a[i]);
			FILE *fpWrite = fopen("data.txt", "a+");
				fprintf(fpWrite, "%f\n", a[i]);
			fclose(fpWrite);
		//initializing two parameters in the kalman filter
		x_last = a[NUM - 1];
		p_last = 1.0;
		}
		else {
			//update the a[NUM], add a number in the end and wipe the first number
			for (j=0; j<NUM-1; j++)
				a[j] = a[j+1];
			//3 sigma criterion(use 5 sigma usually)
			if(fabs(y - mean) < 5 * std){
				a[NUM-1] = y;
			}
			else {
				printf("%f is gross error.\n", y);
				a[NUM - 1] = a[NUM - 2]; //last data of the series replace the error one(thinking about mean)
			}
			//update the mean & std
			sum = 0;
			sum_square = 0;
			for (k=0; k<NUM; k++){
				sum = sum + a[k];
				sum_square = sum_square + a[k]*a[k];   //the sum of the square
			}
			mean = sum / NUM;
			std = sqrt((sum_square - 2 * sum*mean + NUM*mean*mean) / (NUM - 1));
			//printf("mean=%f,std=%f\n", mean, std);

			//kalman filter
			float Q = 1e-4;   //process variance
			float R = 0.5;    //estimate of measurement variance, change to see effect
			float kg;
			float x_mid;
			float x_now;
			float p_mid;
			float p_now;
			//time update 
			x_mid = x_last;      //x_mid=x(k|k-1), x_last=x(k-1|k-1), X(k|k-1) = AX(k-1|k-1) + BU(k) + W(k), A=1, BU(k) = 0
			p_mid = p_last + Q;  //p_mid=p(k|k-1), p_last=p(k-1|k-1), P(k|k-1) = AP(k-1|k-1)A + Q(k), A=1
			//measurement update
			kg = p_mid / (p_mid + R); //Kg(k)=P(k|k-1)H / [HP(k|k-1)H + R], H=1
			x_now = x_mid + kg*(a[NUM-1] - x_mid); //X(k|k) = X(k|k-1) + Kg(k)[Z(k) - HX(k|k-1)], H=1, Z(k)=a[NUM-1]
			p_now = (1 - kg)*p_mid;   //P(k|k) = (1 - Kg(k)H)P(k|k-1), H=1
			//next round parameter update
			x_last = x_now;
			p_last = p_now;

			printf("a'[%d] = %f\n",i,x_now);    //the output value
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