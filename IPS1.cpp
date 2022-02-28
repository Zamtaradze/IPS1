// IPS1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include <locale.h>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>



using namespace std;
std::mutex g_pages_mutex;
using namespace std::chrono;

double function(double x)
{
    double y;
    y = 8 / (1 + pow(x, 2));
    return y;
}

double simpson_alg(double a, double b, int h)
{
    double width = (b - a) / h;

    double simpson_alg = 0;
    //#pragma loop(no_vector)
    //#pragma (hint_parallel(4))
    for (int i = 0; i < h; i++) {
        double x1 = a + i * width;
        double x2 = a + (i + 1) * width;

        simpson_alg += (x2 - x1) / 6.0 * (function(x1) + 4.0 * function(0.5 * (x1 + x2)) + function(x2));
    }

    return simpson_alg;

}

double no_vector_simpson_alg(double a, double b, int h)
{
    double width = (b - a) / h;

    double simpson_alg = 0;
    #pragma loop(no_vector)
    //#pragma (hint_parallel(4))
    for (int i = 0; i < h; i++) {
        double x1 = a + i * width;
        double x2 = a + (i + 1) * width;

        simpson_alg += (x2 - x1) / 6.0 * (function(x1) + 4.0 * function(0.5 * (x1 + x2)) + function(x2));
    }

    return simpson_alg;

}

double parallel_simpson_alg(double a, double b, int h)
{
    double width = (b - a) / h;

    double simpson_alg = 0;
    //#pragma loop(no_vector)
    #pragma (hint_parallel(4))
    for (int i = 0; i < h; i++) {
        double x1 = a + i * width;
        double x2 = a + (i + 1) * width;

        simpson_alg += (x2 - x1) / 6.0 * (function(x1) + 4.0 * function(0.5 * (x1 + x2)) + function(x2));
    }

    return simpson_alg;

}

void thread_simpson_alg(double a, double b,double width,  int h,int i,double& simpson_alg)
{
    std::lock_guard<std::mutex> guard(g_pages_mutex);
    for (;i < h; i++) 
    {
        double x1 = a + i * width;
        double x2 = a + (i + 1) * width;

        simpson_alg += (x2 - x1) / 6.0 * (function(x1) + 4.0 * function(0.5 * (x1 + x2)) + function(x2));
    }
}

int main()
{
    setlocale(LC_ALL, "Rus");

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    duration<double> duration;
    double a=0;
    double b=1;
    double simpson;
    cout.precision(30);

    
    cout << "default" << endl;
    //-------------
    t1 = high_resolution_clock::now();
    simpson = simpson_alg(a, b, 10);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    t1 = high_resolution_clock::now();
    simpson = simpson_alg(a, b, 100);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    t1 = high_resolution_clock::now();
    simpson = simpson_alg(a, b, 10000000);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    //-------------

    cout << " \n\n " << endl;
    cout << "no vector" << endl;
    //-------------
    t1 = high_resolution_clock::now();
    simpson = no_vector_simpson_alg(a, b, 10);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    t1 = high_resolution_clock::now();
    simpson = no_vector_simpson_alg(a, b, 100);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    t1 = high_resolution_clock::now();
    simpson = no_vector_simpson_alg(a, b, 10000000);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    //-------------

    cout<<" \n\n "<< endl;
    cout << "parallel" << endl;
    //-------------
    t1 = high_resolution_clock::now();
    simpson = parallel_simpson_alg(a, b, 10);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    t1 = high_resolution_clock::now();
    simpson = parallel_simpson_alg(a, b, 100);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;

    t1 = high_resolution_clock::now();
    simpson = parallel_simpson_alg(a, b, 10000000);
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson << " | Duration is " << duration.count() << "  seconds" << endl;
    //-------------
    cout << " \n\n " << endl;

    //-------------

    cout << "multithread\n" << endl;
    double simpson_alg;
    double width;
    int quan_thread;
    int h;
    int h_div_thread;
    int k0;
    int k1;
    
    //---------
    std::vector<thread> threads1;
    simpson_alg = 0;
    quan_thread = 2;
    h = 10;
    width = (b-a)/h;
    h_div_thread = h / quan_thread;
    k0 = 0;
    k1 = h_div_thread;

    t1 = high_resolution_clock::now();
    for (int i = quan_thread; i > 0; --i){
        
        threads1.emplace_back(thread_simpson_alg, a, b, width, k1, k0, ref(simpson_alg));
        
        k1 += h_div_thread;
        k0 += h_div_thread;
    } 
    for (auto& thread : threads1)
        thread.join();

    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson_alg << " | Duration is " << duration.count() << "  seconds" << "  for 2 threads" << endl;

    //---------

    std::vector<thread> threads2;
    simpson_alg = 0;
    quan_thread = 4;
    h = 100;
    width = (b - a) / h;
    h_div_thread = h / quan_thread;
    k0 = 0;
    k1 = h_div_thread;
    t1 = high_resolution_clock::now();
    for (int i = quan_thread; i > 0; --i) {

        threads2.emplace_back(thread_simpson_alg, a, b, width, k1, k0, ref(simpson_alg));

        k1 += h_div_thread;
        k0 += h_div_thread;
    }
    for (auto& thread : threads2)
        thread.join();
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson_alg << " | Duration is " << duration.count() << "  seconds" << "  for 4 threads" << endl;
    
    //---------

    std::vector<thread> threads3;
    simpson_alg = 0;
    quan_thread = 8;
    h = 10000000;
    width = (b - a) / h;
    h_div_thread = h / quan_thread;
    k0 = 0;
    k1 = h_div_thread;
    t1 = high_resolution_clock::now();
    for (int i = quan_thread; i > 0; --i) {

        threads3.emplace_back(thread_simpson_alg, a, b, width, k1, k0, ref(simpson_alg));

        k1 += h_div_thread;
        k0 += h_div_thread;
    }
    for (auto& thread : threads3)
        thread.join();
    t2 = high_resolution_clock::now();
    duration = (t2 - t1);
    cout << simpson_alg << " | Duration is " << duration.count() << "  seconds" << "  for 8 threads" << endl;
    //---------

    cout <<" \n 2 * pi = " << 2 * M_PI << endl;
    return 0;
}