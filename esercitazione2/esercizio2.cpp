#include <iostream>
#include <cmath>

int main(void) {
	static const int N = 10;
	double arr[N]={10.1, 11.11, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 99.9};
	double min = std::min(arr[0],arr[1]);
	double max = std::max(arr[0],arr[1]);
	double sum = arr[0]+arr[1];
	for(int i=1;i<=N-2;i++){
		double x = std::min(arr[i],arr[i+1]);
		double y = std::max(arr[i],arr[i+1]);
		if(x<min) {
			min=x;
		}
		if(y>max) {
			max=y;
		}
		sum = sum + arr[i+1];
	}
	double media = sum/N;
	std::cout << "Il minimo è " << min << "\n";
	std::cout << "Il massimo è " << max << "\n";
	std::cout << "La media è " << media << "\n";

	double dev = 0;
	for(int j=0;j<N;j++) {
		dev = dev + pow((arr[j]-media),2)/N;
	}
	std::cout << "La deviazione standard è " << std::sqrt(dev) << "\n";
	return 0;

}